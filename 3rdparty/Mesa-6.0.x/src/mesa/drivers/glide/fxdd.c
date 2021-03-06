/*
 * Mesa 3-D graphics library
 * Version:  5.1
 *
 * Copyright (C) 1999-2003  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/* Authors:
 *    David Bucciarelli
 *    Brian Paul
 *    Daryll Strauss
 *    Keith Whitwell
 *    Daniel Borca
 *    Hiroshi Morii
 */

/* fxdd.c - 3Dfx VooDoo Mesa device driver functions */


#ifdef HAVE_CONFIG_H
#include "conf.h"
#endif

#if defined(FX)

#include "image.h"
#include "mtypes.h"
#include "fxdrv.h"
#include "enums.h"
#include "extensions.h"
#include "macros.h"
#include "texstore.h"
#include "teximage.h"
#include "swrast/swrast.h"
#include "swrast/s_context.h"
#include "swrast_setup/swrast_setup.h"
#include "tnl/tnl.h"
#include "tnl/t_context.h"
#include "tnl/t_pipeline.h"
#include "array_cache/acache.h"



/* lookup table for scaling 4 bit colors up to 8 bits */
GLuint FX_rgb_scale_4[16] = {
   0,   17,  34,  51,  68,  85,  102, 119,
   136, 153, 170, 187, 204, 221, 238, 255
};

/* lookup table for scaling 5 bit colors up to 8 bits */
GLuint FX_rgb_scale_5[32] = {
   0,   8,   16,  25,  33,  41,  49,  58,
   66,  74,  82,  90,  99,  107, 115, 123,
   132, 140, 148, 156, 165, 173, 181, 189,
   197, 206, 214, 222, 230, 239, 247, 255
};

/* lookup table for scaling 6 bit colors up to 8 bits */
GLuint FX_rgb_scale_6[64] = {
   0,   4,   8,   12,  16,  20,  24,  28,
   32,  36,  40,  45,  49,  53,  57,  61,
   65,  69,  73,  77,  81,  85,  89,  93,
   97,  101, 105, 109, 113, 117, 121, 125,
   130, 134, 138, 142, 146, 150, 154, 158,
   162, 166, 170, 174, 178, 182, 186, 190,
   194, 198, 202, 206, 210, 215, 219, 223,
   227, 231, 235, 239, 243, 247, 251, 255
};


/*
 * Disable color by masking out R, G, B, A
 */
static void fxDisableColor (fxMesaContext fxMesa)
{
 if (fxMesa->colDepth != 16) {
    /* 32bpp mode or 15bpp mode */
    fxMesa->Glide.grColorMaskExt(FXFALSE, FXFALSE, FXFALSE, FXFALSE);
 } else {
    /* 16 bpp mode */
    grColorMask(FXFALSE, FXFALSE);
 }
}


/**********************************************************************/
/*****                 Miscellaneous functions                    *****/
/**********************************************************************/

/* Return buffer size information */
static void
fxDDBufferSize(GLframebuffer *buffer, GLuint *width, GLuint *height)
{
   GET_CURRENT_CONTEXT(ctx);
   if (ctx && FX_CONTEXT(ctx)) {
      fxMesaContext fxMesa = FX_CONTEXT(ctx);

      if (TDFX_DEBUG & VERBOSE_DRIVER) {
         fprintf(stderr, "fxDDBufferSize(...)\n");
      }

      *width = fxMesa->width;
      *height = fxMesa->height;
   }
}


/* Implements glClearColor() */
static void
fxDDClearColor(GLcontext * ctx, const GLfloat color[4])
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);
   GLubyte col[4];

   if (TDFX_DEBUG & VERBOSE_DRIVER) {
      fprintf(stderr, "fxDDClearColor(%f, %f, %f, %f)\n",
	              color[0], color[1], color[2], color[3]);
   }

   CLAMPED_FLOAT_TO_UBYTE(col[0], color[0]);
   CLAMPED_FLOAT_TO_UBYTE(col[1], color[1]);
   CLAMPED_FLOAT_TO_UBYTE(col[2], color[2]);
   CLAMPED_FLOAT_TO_UBYTE(col[3], color[3]);

   fxMesa->clearC = FXCOLOR4(col);
   fxMesa->clearA = col[3];
}


/* Clear the color and/or depth buffers */
static void fxDDClear( GLcontext *ctx,
			GLbitfield mask, GLboolean all,
			GLint x, GLint y, GLint width, GLint height )
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);
   GLbitfield softwareMask = mask & (DD_ACCUM_BIT);
   const GLuint stencil_size = fxMesa->haveHwStencil ? ctx->Visual.stencilBits : 0;
   const FxU32 clearD = (FxU32) (((1 << ctx->Visual.depthBits) - 1) * ctx->Depth.Clear);
   const FxU8 clearS = (FxU8) (ctx->Stencil.Clear & 0xff);

   /* [dBorca] Hack alert:
    * if we set Mesa for 32bit depth, we'll get
    * clearD == 0
    * due to 32bit integer overflow!
    */

   if ( TDFX_DEBUG & MESA_VERBOSE ) {
      fprintf( stderr, "fxDDClear( %d, %d, %d, %d )\n",
	               (int) x, (int) y, (int) width, (int) height );
   }

   /* Need this check to respond to glScissor and clipping updates */
   /* should also take care of FX_NEW_COLOR_MASK, FX_NEW_STENCIL, depth? */
   if (fxMesa->new_state & FX_NEW_SCISSOR) {
      fxSetupScissor(ctx);
      fxMesa->new_state &= ~FX_NEW_SCISSOR;
   }

   /* we can't clear accum buffers */
   mask &= ~(DD_ACCUM_BIT);

   /*
    * As per GL spec, stencil masking should be obeyed when clearing
    */
   if (mask & DD_STENCIL_BIT) {
      if (!fxMesa->haveHwStencil || fxMesa->unitsState.stencilWriteMask != 0xff) {
         /* Napalm seems to have trouble with stencil write masks != 0xff */
         /* do stencil clear in software */
         softwareMask |= DD_STENCIL_BIT;
         mask &= ~(DD_STENCIL_BIT);
      }
   }

   /*
    * As per GL spec, color masking should be obeyed when clearing
    */
   if (ctx->Visual.greenBits != 8 && ctx->Visual.greenBits != 5) {
      /* can only do color masking if running in 24/32bpp on Napalm */
      if (ctx->Color.ColorMask[RCOMP] != ctx->Color.ColorMask[GCOMP] ||
          ctx->Color.ColorMask[GCOMP] != ctx->Color.ColorMask[BCOMP]) {
         softwareMask |= (mask & (DD_FRONT_LEFT_BIT | DD_BACK_LEFT_BIT));
         mask &= ~(DD_FRONT_LEFT_BIT | DD_BACK_LEFT_BIT);
      }
   }

   if (fxMesa->haveHwStencil) {
      /*
       * If we want to clear stencil, it must be enabled
       * in the HW, even if the stencil test is not enabled
       * in the OGL state.
       */
      BEGIN_BOARD_LOCK();
      if (mask & DD_STENCIL_BIT) {
	 fxMesa->Glide.grStencilMaskExt(0xff /*fxMesa->unitsState.stencilWriteMask*/);
	 /* set stencil ref value = desired clear value */
	 fxMesa->Glide.grStencilFuncExt(GR_CMP_ALWAYS, clearS, 0xff);
	 fxMesa->Glide.grStencilOpExt(GR_STENCILOP_REPLACE,
                                   GR_STENCILOP_REPLACE, GR_STENCILOP_REPLACE);
	 grEnable(GR_STENCIL_MODE_EXT);
      }
      else {
	 grDisable(GR_STENCIL_MODE_EXT);
      }
      END_BOARD_LOCK();
   }

   /*
    * This may be ugly, but it's needed in order to work around a number
    * of Glide bugs.
    */
   BEGIN_CLIP_LOOP();
   {
      /*
       * This could probably be done fancier but doing each possible case
       * explicitly is less error prone.
       */
      switch (mask & ~DD_STENCIL_BIT) {
      case DD_BACK_LEFT_BIT | DD_DEPTH_BIT:
	 /* back buffer & depth */
	 /* FX_grColorMaskv_NoLock(ctx, true4); */ /* work around Voodoo3 bug */
	 grDepthMask(FXTRUE);
	 grRenderBuffer(GR_BUFFER_BACKBUFFER);
	 if (stencil_size > 0) {
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
         }
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 if (!fxMesa->unitsState.depthTestEnabled) {
            grDepthMask(FXFALSE);
	 }
	 break;
      case DD_FRONT_LEFT_BIT | DD_DEPTH_BIT:
	 /* XXX it appears that the depth buffer isn't cleared when
	  * glRenderBuffer(GR_BUFFER_FRONTBUFFER) is set.
	  * This is a work-around/
	  */
	 /* clear depth */
	 grDepthMask(FXTRUE);
	 grRenderBuffer(GR_BUFFER_BACKBUFFER);
         fxDisableColor(fxMesa);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 /* clear front */
	 fxSetupColorMask(ctx);
	 grRenderBuffer(GR_BUFFER_FRONTBUFFER);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 if (!fxMesa->unitsState.depthTestEnabled) {
            grDepthMask(FXFALSE);
	 }
	 break;
      case DD_BACK_LEFT_BIT:
	 /* back buffer only */
	 grDepthMask(FXFALSE);
	 grRenderBuffer(GR_BUFFER_BACKBUFFER);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 if (fxMesa->unitsState.depthTestEnabled) {
            grDepthMask(FXTRUE);
	 }
	 break;
      case DD_FRONT_LEFT_BIT:
	 /* front buffer only */
	 grDepthMask(FXFALSE);
	 grRenderBuffer(GR_BUFFER_FRONTBUFFER);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 if (fxMesa->unitsState.depthTestEnabled) {
            grDepthMask(FXTRUE);
	 }
	 break;
      case DD_FRONT_LEFT_BIT | DD_BACK_LEFT_BIT:
	 /* front and back */
	 grDepthMask(FXFALSE);
	 grRenderBuffer(GR_BUFFER_BACKBUFFER);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 grRenderBuffer(GR_BUFFER_FRONTBUFFER);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 if (fxMesa->unitsState.depthTestEnabled) {
            grDepthMask(FXTRUE);
	 }
	 break;
      case DD_FRONT_LEFT_BIT | DD_BACK_LEFT_BIT | DD_DEPTH_BIT:
	 /* clear front */
	 grDepthMask(FXFALSE);
	 grRenderBuffer(GR_BUFFER_FRONTBUFFER);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 /* clear back and depth */
	 grDepthMask(FXTRUE);
	 grRenderBuffer(GR_BUFFER_BACKBUFFER);
         if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 if (!fxMesa->unitsState.depthTestEnabled) {
            grDepthMask(FXFALSE);
	 }
	 break;
      case DD_DEPTH_BIT:
	 /* just the depth buffer */
	 grRenderBuffer(GR_BUFFER_BACKBUFFER);
         fxDisableColor(fxMesa);
	 grDepthMask(FXTRUE);
	 if (stencil_size > 0)
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
	 else
            grBufferClear(fxMesa->clearC,
                          fxMesa->clearA,
                          clearD);
	 fxSetupColorMask(ctx);
	 if (ctx->Color._DrawDestMask & FRONT_LEFT_BIT) {
            grRenderBuffer(GR_BUFFER_FRONTBUFFER);
         }
	 if (!fxMesa->unitsState.depthTestEnabled) {
	    grDepthMask(FXFALSE);
         }
	 break;
      default:
         /* clear no color buffers or depth buffer but might clear stencil */
	 if (stencil_size > 0 && (mask & DD_STENCIL_BIT)) {
            /* XXX need this RenderBuffer call to work around Glide bug */
            grRenderBuffer(GR_BUFFER_BACKBUFFER);
            grDepthMask(FXFALSE);
            fxDisableColor(fxMesa);
            fxMesa->Glide.grBufferClearExt(fxMesa->clearC,
                                           fxMesa->clearA,
                                           clearD, clearS);
            if (fxMesa->unitsState.depthTestEnabled) {
               grDepthMask(FXTRUE);
            }
            fxSetupColorMask(ctx);
            if (ctx->Color._DrawDestMask & FRONT_LEFT_BIT) {
               grRenderBuffer(GR_BUFFER_FRONTBUFFER);
            }
         }
      }
   }
   END_CLIP_LOOP();

   if (fxMesa->haveHwStencil && (mask & DD_STENCIL_BIT)) {
      /* We changed the stencil state above.  Signal that we need to
       * upload it again.
       */
      fxMesa->new_state |= FX_NEW_STENCIL;
   }

   if (softwareMask)
      _swrast_Clear( ctx, softwareMask, all, x, y, width, height );
}


/* Set the buffer used for drawing */
/* XXX support for separate read/draw buffers hasn't been tested */
static void
fxDDSetDrawBuffer(GLcontext * ctx, GLenum mode)
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);

   if (TDFX_DEBUG & VERBOSE_DRIVER) {
      fprintf(stderr, "fxDDSetDrawBuffer(%x)\n", (int)mode);
   }

   if (mode == GL_FRONT_LEFT) {
      fxMesa->currentFB = GR_BUFFER_FRONTBUFFER;
      grRenderBuffer(fxMesa->currentFB);
   }
   else if (mode == GL_BACK_LEFT) {
      fxMesa->currentFB = GR_BUFFER_BACKBUFFER;
      grRenderBuffer(fxMesa->currentFB);
   }
   else if (mode == GL_NONE) {
      fxDisableColor(fxMesa);
   }
   else {
      /* we'll need a software fallback */
      /* XXX not implemented */
   }

   /* update s/w fallback state */
   _swrast_DrawBuffer(ctx, mode);
}


static void
fxDDDrawBitmap2 (GLcontext *ctx, GLint px, GLint py,
		 GLsizei width, GLsizei height,
		 const struct gl_pixelstore_attrib *unpack,
		 const GLubyte *bitmap)
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);
   SWcontext *swrast = SWRAST_CONTEXT(ctx);
   GrLfbInfo_t info;
   GrLfbWriteMode_t mode;
   FxU16 color;
   const struct gl_pixelstore_attrib *finalUnpack;
   struct gl_pixelstore_attrib scissoredUnpack;

   /* check if there's any raster operations enabled which we can't handle */
   if ((swrast->_RasterMask & (ALPHATEST_BIT |
			      /*BLEND_BIT |*/   /* blending ok, through pixpipe */
			      DEPTH_BIT |       /* could be done with RGB:DEPTH */
			      FOG_BIT |         /* could be done with RGB:DEPTH */
			      LOGIC_OP_BIT |
			      /*CLIP_BIT |*/    /* clipping ok, below */
			      STENCIL_BIT |
			      /*MASKING_BIT |*/ /* masking ok, test follows */
			      ALPHABUF_BIT |    /* nope! see 565 span kludge */
			      MULTI_DRAW_BIT |
			      OCCLUSION_BIT |   /* nope! at least not yet */
			      TEXTURE_BIT |
			      FRAGPROG_BIT))
       ||
       ((swrast->_RasterMask & MASKING_BIT) /*&& (ctx->Visual.greenBits != 8)*/ && (ctx->Visual.greenBits != 5))
      ) {
      _swrast_Bitmap(ctx, px, py, width, height, unpack, bitmap);
      return;
   }

   /* make sure the pixelpipe is configured correctly */
   fxSetupFXUnits(ctx);

   if (ctx->Scissor.Enabled) {
      /* This is a bit tricky, but by carefully adjusting the px, py,
       * width, height, skipPixels and skipRows values we can do
       * scissoring without special code in the rendering loop.
       */

      /* we'll construct a new pixelstore struct */
      finalUnpack = &scissoredUnpack;
      scissoredUnpack = *unpack;
      if (scissoredUnpack.RowLength == 0)
	 scissoredUnpack.RowLength = width;

      /* clip left */
      if (px < ctx->Scissor.X) {
	 scissoredUnpack.SkipPixels += (ctx->Scissor.X - px);
	 width -= (ctx->Scissor.X - px);
	 px = ctx->Scissor.X;
      }
      /* clip right */
      if (px + width >= ctx->Scissor.X + ctx->Scissor.Width) {
	 width -= (px + width - (ctx->Scissor.X + ctx->Scissor.Width));
      }
      /* clip bottom */
      if (py < ctx->Scissor.Y) {
	 scissoredUnpack.SkipRows += (ctx->Scissor.Y - py);
	 height -= (ctx->Scissor.Y - py);
	 py = ctx->Scissor.Y;
      }
      /* clip top */
      if (py + height >= ctx->Scissor.Y + ctx->Scissor.Height) {
	 height -= (py + height - (ctx->Scissor.Y + ctx->Scissor.Height));
      }

      if (width <= 0 || height <= 0)
	 return;
   }
   else {
      finalUnpack = unpack;
   }

   /* compute pixel value */
   {
      GLint r = (GLint) (ctx->Current.RasterColor[RCOMP] * 255.0f);
      GLint g = (GLint) (ctx->Current.RasterColor[GCOMP] * 255.0f);
      GLint b = (GLint) (ctx->Current.RasterColor[BCOMP] * 255.0f);
      GLint a = (GLint) (ctx->Current.RasterColor[ACOMP] * 255.0f);
      if (fxMesa->colDepth == 15) {
         color = TDFXPACKCOLOR1555(b, g, r, a);
         mode = GR_LFBWRITEMODE_1555;
      } else {
         color = fxMesa->bgrOrder ? TDFXPACKCOLOR565(r, g, b) : TDFXPACKCOLOR565(b, g, r);
         mode = GR_LFBWRITEMODE_565;
      }
   }

   info.size = sizeof(info);
   if (!grLfbLock(GR_LFB_WRITE_ONLY,
		  fxMesa->currentFB,
		  mode,
		  GR_ORIGIN_UPPER_LEFT, FXTRUE, &info)) {
      _swrast_Bitmap(ctx, px, py, width, height, unpack, bitmap);
      return;
   }

   {
      const GLint winX = 0;
      const GLint winY = fxMesa->height - 1;
      /* The dest stride depends on the hardware and whether we're drawing
       * to the front or back buffer.  This compile-time test seems to do
       * the job for now.
       */
      const GLint dstStride = info.strideInBytes / 2;	/* stride in GLushorts */

      GLint row;
      /* compute dest address of bottom-left pixel in bitmap */
      GLushort *dst = (GLushort *) info.lfbPtr
	 + (winY - py) * dstStride + (winX + px);

      for (row = 0; row < height; row++) {
	 const GLubyte *src =
	    (const GLubyte *) _mesa_image_address(finalUnpack,
						  bitmap, width, height,
						  GL_COLOR_INDEX, GL_BITMAP,
						  0, row, 0);
	 if (finalUnpack->LsbFirst) {
	    /* least significan bit first */
	    GLubyte mask = 1U << (finalUnpack->SkipPixels & 0x7);
	    GLint col;
	    for (col = 0; col < width; col++) {
	       if (*src & mask) {
		  dst[col] = color;
	       }
	       if (mask == 128U) {
		  src++;
		  mask = 1U;
	       }
	       else {
		  mask = mask << 1;
	       }
	    }
	    if (mask != 1)
	       src++;
	 }
	 else {
	    /* most significan bit first */
	    GLubyte mask = 128U >> (finalUnpack->SkipPixels & 0x7);
	    GLint col;
	    for (col = 0; col < width; col++) {
	       if (*src & mask) {
		  dst[col] = color;
	       }
	       if (mask == 1U) {
		  src++;
		  mask = 128U;
	       }
	       else {
		  mask = mask >> 1;
	       }
	    }
	    if (mask != 128)
	       src++;
	 }
	 dst -= dstStride;
      }
   }

   grLfbUnlock(GR_LFB_WRITE_ONLY, fxMesa->currentFB);
}

static void
fxDDDrawBitmap4 (GLcontext *ctx, GLint px, GLint py,
		 GLsizei width, GLsizei height,
		 const struct gl_pixelstore_attrib *unpack,
		 const GLubyte *bitmap)
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);
   SWcontext *swrast = SWRAST_CONTEXT(ctx);
   GrLfbInfo_t info;
   FxU32 color;
   const struct gl_pixelstore_attrib *finalUnpack;
   struct gl_pixelstore_attrib scissoredUnpack;

   /* check if there's any raster operations enabled which we can't handle */
   if ((swrast->_RasterMask & (ALPHATEST_BIT |
			      /*BLEND_BIT |*/   /* blending ok, through pixpipe */
			      DEPTH_BIT |       /* could be done with RGB:DEPTH */
			      FOG_BIT |         /* could be done with RGB:DEPTH */
			      LOGIC_OP_BIT |
			      /*CLIP_BIT |*/    /* clipping ok, below */
			      STENCIL_BIT |
			      /*MASKING_BIT |*/ /* masking ok, we're in 32bpp */
			      /*ALPHABUF_BIT |*//* alpha ok, we're in 32bpp */
			      MULTI_DRAW_BIT |
			      OCCLUSION_BIT |   /* nope! at least not yet */
			      TEXTURE_BIT |
			      FRAGPROG_BIT))
      ) {
      _swrast_Bitmap(ctx, px, py, width, height, unpack, bitmap);
      return;
   }

   /* make sure the pixelpipe is configured correctly */
   fxSetupFXUnits(ctx);

   if (ctx->Scissor.Enabled) {
      /* This is a bit tricky, but by carefully adjusting the px, py,
       * width, height, skipPixels and skipRows values we can do
       * scissoring without special code in the rendering loop.
       */

      /* we'll construct a new pixelstore struct */
      finalUnpack = &scissoredUnpack;
      scissoredUnpack = *unpack;
      if (scissoredUnpack.RowLength == 0)
	 scissoredUnpack.RowLength = width;

      /* clip left */
      if (px < ctx->Scissor.X) {
	 scissoredUnpack.SkipPixels += (ctx->Scissor.X - px);
	 width -= (ctx->Scissor.X - px);
	 px = ctx->Scissor.X;
      }
      /* clip right */
      if (px + width >= ctx->Scissor.X + ctx->Scissor.Width) {
	 width -= (px + width - (ctx->Scissor.X + ctx->Scissor.Width));
      }
      /* clip bottom */
      if (py < ctx->Scissor.Y) {
	 scissoredUnpack.SkipRows += (ctx->Scissor.Y - py);
	 height -= (ctx->Scissor.Y - py);
	 py = ctx->Scissor.Y;
      }
      /* clip top */
      if (py + height >= ctx->Scissor.Y + ctx->Scissor.Height) {
	 height -= (py + height - (ctx->Scissor.Y + ctx->Scissor.Height));
      }

      if (width <= 0 || height <= 0)
	 return;
   }
   else {
      finalUnpack = unpack;
   }

   /* compute pixel value */
   {
      GLint r = (GLint) (ctx->Current.RasterColor[RCOMP] * 255.0f);
      GLint g = (GLint) (ctx->Current.RasterColor[GCOMP] * 255.0f);
      GLint b = (GLint) (ctx->Current.RasterColor[BCOMP] * 255.0f);
      GLint a = (GLint) (ctx->Current.RasterColor[ACOMP] * 255.0f);
      color = TDFXPACKCOLOR8888(b, g, r, a);
   }

   info.size = sizeof(info);
   if (!grLfbLock(GR_LFB_WRITE_ONLY,
		  fxMesa->currentFB,
		  GR_LFBWRITEMODE_8888,
		  GR_ORIGIN_UPPER_LEFT, FXTRUE, &info)) {
      _swrast_Bitmap(ctx, px, py, width, height, unpack, bitmap);
      return;
   }

   {
      const GLint winX = 0;
      const GLint winY = fxMesa->height - 1;
      /* The dest stride depends on the hardware and whether we're drawing
       * to the front or back buffer.  This compile-time test seems to do
       * the job for now.
       */
      const GLint dstStride = info.strideInBytes / 4;	/* stride in GLuints */

      GLint row;
      /* compute dest address of bottom-left pixel in bitmap */
      GLuint *dst = (GLuint *) info.lfbPtr
	 + (winY - py) * dstStride + (winX + px);

      for (row = 0; row < height; row++) {
	 const GLubyte *src =
	    (const GLubyte *) _mesa_image_address(finalUnpack,
						  bitmap, width, height,
						  GL_COLOR_INDEX, GL_BITMAP,
						  0, row, 0);
	 if (finalUnpack->LsbFirst) {
	    /* least significan bit first */
	    GLubyte mask = 1U << (finalUnpack->SkipPixels & 0x7);
	    GLint col;
	    for (col = 0; col < width; col++) {
	       if (*src & mask) {
		  dst[col] = color;
	       }
	       if (mask == 128U) {
		  src++;
		  mask = 1U;
	       }
	       else {
		  mask = mask << 1;
	       }
	    }
	    if (mask != 1)
	       src++;
	 }
	 else {
	    /* most significan bit first */
	    GLubyte mask = 128U >> (finalUnpack->SkipPixels & 0x7);
	    GLint col;
	    for (col = 0; col < width; col++) {
	       if (*src & mask) {
		  dst[col] = color;
	       }
	       if (mask == 1U) {
		  src++;
		  mask = 128U;
	       }
	       else {
		  mask = mask >> 1;
	       }
	    }
	    if (mask != 128)
	       src++;
	 }
	 dst -= dstStride;
      }
   }

   grLfbUnlock(GR_LFB_WRITE_ONLY, fxMesa->currentFB);
}


static void
fxDDReadPixels565 (GLcontext * ctx,
		   GLint x, GLint y,
		   GLsizei width, GLsizei height,
		   GLenum format, GLenum type,
		   const struct gl_pixelstore_attrib *packing,
		   GLvoid *dstImage)
{
   if (ctx->_ImageTransferState/* & (IMAGE_SCALE_BIAS_BIT|IMAGE_MAP_COLOR_BIT)*/) {
      _swrast_ReadPixels(ctx, x, y, width, height, format, type,
			 packing, dstImage);
      return;
   }
   else {
      fxMesaContext fxMesa = FX_CONTEXT(ctx);
      GrLfbInfo_t info;

      BEGIN_BOARD_LOCK();
      info.size = sizeof(info);
      if (grLfbLock(GR_LFB_READ_ONLY,
		    fxMesa->currentFB,
		    GR_LFBWRITEMODE_ANY,
		    GR_ORIGIN_UPPER_LEFT, FXFALSE, &info)) {
	 const GLint winX = 0;
	 const GLint winY = fxMesa->height - 1;
	 const GLint srcStride = info.strideInBytes / 2;	/* stride in GLushorts */
	 const GLushort *src = (const GLushort *) info.lfbPtr
	    + (winY - y) * srcStride + (winX + x);
	 GLubyte *dst = (GLubyte *) _mesa_image_address(packing, dstImage,
							width, height, format,
							type, 0, 0, 0);
	 GLint dstStride =
	    _mesa_image_row_stride(packing, width, format, type);

	 if (format == GL_RGB && type == GL_UNSIGNED_BYTE) {
	    /* convert 5R6G5B into 8R8G8B */
	    GLint row, col;
	    const GLint halfWidth = width >> 1;
	    const GLint extraPixel = (width & 1);
	    for (row = 0; row < height; row++) {
	       GLubyte *d = dst;
	       for (col = 0; col < halfWidth; col++) {
		  const GLuint pixel = ((const GLuint *) src)[col];
                  *d++ = FX_rgb_scale_5[(pixel >> 11) & 0x1f];
                  *d++ = FX_rgb_scale_6[(pixel >> 5)  & 0x3f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 27) & 0x1f];
                  *d++ = FX_rgb_scale_6[(pixel >> 21) & 0x3f];
                  *d++ = FX_rgb_scale_5[(pixel >> 16) & 0x1f];
	       }
	       if (extraPixel) {
		  GLushort pixel = src[width - 1];
                  *d++ = FX_rgb_scale_5[(pixel >> 11) & 0x1f];
                  *d++ = FX_rgb_scale_6[(pixel >> 5)  & 0x3f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
	       }
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) {
	    /* convert 5R6G5B into 8R8G8B8A */
	    GLint row, col;
	    const GLint halfWidth = width >> 1;
	    const GLint extraPixel = (width & 1);
	    for (row = 0; row < height; row++) {
	       GLubyte *d = dst;
	       for (col = 0; col < halfWidth; col++) {
		  const GLuint pixel = ((const GLuint *) src)[col];
                  *d++ = FX_rgb_scale_5[(pixel >> 11) & 0x1f];
                  *d++ = FX_rgb_scale_6[(pixel >> 5)  & 0x3f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
		  *d++ = 255;
                  *d++ = FX_rgb_scale_5[(pixel >> 27) & 0x1f];
                  *d++ = FX_rgb_scale_6[(pixel >> 21) & 0x3f];
                  *d++ = FX_rgb_scale_5[(pixel >> 16) & 0x1f];
		  *d++ = 255;
	       }
	       if (extraPixel) {
		  const GLushort pixel = src[width - 1];
                  *d++ = FX_rgb_scale_5[(pixel >> 11) & 0x1f];
                  *d++ = FX_rgb_scale_6[(pixel >> 5)  & 0x3f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
		  *d++ = 255;
	       }
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else if (format == GL_RGB && type == GL_UNSIGNED_SHORT_5_6_5) {
	    /* directly memcpy 5R6G5B pixels into client's buffer */
	    const GLint widthInBytes = width * 2;
	    GLint row;
	    for (row = 0; row < height; row++) {
	       MEMCPY(dst, src, widthInBytes);
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else {
	    grLfbUnlock(GR_LFB_READ_ONLY, fxMesa->currentFB);
	    END_BOARD_LOCK();
	    _swrast_ReadPixels(ctx, x, y, width, height, format, type,
			       packing, dstImage);
	    return;
	 }

	 grLfbUnlock(GR_LFB_READ_ONLY, fxMesa->currentFB);
      }
      END_BOARD_LOCK();
   }
}

static void
fxDDReadPixels555 (GLcontext * ctx,
		   GLint x, GLint y,
		   GLsizei width, GLsizei height,
		   GLenum format, GLenum type,
		   const struct gl_pixelstore_attrib *packing,
		   GLvoid *dstImage)
{
   if (ctx->_ImageTransferState/* & (IMAGE_SCALE_BIAS_BIT|IMAGE_MAP_COLOR_BIT)*/) {
      _swrast_ReadPixels(ctx, x, y, width, height, format, type,
			 packing, dstImage);
      return;
   }
   else {
      fxMesaContext fxMesa = FX_CONTEXT(ctx);
      GrLfbInfo_t info;

      BEGIN_BOARD_LOCK();
      info.size = sizeof(info);
      if (grLfbLock(GR_LFB_READ_ONLY,
		    fxMesa->currentFB,
		    GR_LFBWRITEMODE_ANY,
		    GR_ORIGIN_UPPER_LEFT, FXFALSE, &info)) {
	 const GLint winX = 0;
	 const GLint winY = fxMesa->height - 1;
	 const GLint srcStride = info.strideInBytes / 2;	/* stride in GLushorts */
	 const GLushort *src = (const GLushort *) info.lfbPtr
	    + (winY - y) * srcStride + (winX + x);
	 GLubyte *dst = (GLubyte *) _mesa_image_address(packing, dstImage,
							width, height, format,
							type, 0, 0, 0);
	 GLint dstStride =
	    _mesa_image_row_stride(packing, width, format, type);

	 if (format == GL_RGB && type == GL_UNSIGNED_BYTE) {
	    /* convert 5R5G5B into 8R8G8B */
	    GLint row, col;
	    const GLint halfWidth = width >> 1;
	    const GLint extraPixel = (width & 1);
	    for (row = 0; row < height; row++) {
	       GLubyte *d = dst;
	       for (col = 0; col < halfWidth; col++) {
		  const GLuint pixel = ((const GLuint *) src)[col];
                  *d++ = FX_rgb_scale_5[(pixel >> 10) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 5)  & 0x1f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 26) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 21) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 16) & 0x1f];
	       }
	       if (extraPixel) {
		  GLushort pixel = src[width - 1];
                  *d++ = FX_rgb_scale_5[(pixel >> 10) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 5)  & 0x1f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
	       }
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) {
	    /* convert 5R6G5B into 8R8G8B8A */
	    GLint row, col;
	    const GLint halfWidth = width >> 1;
	    const GLint extraPixel = (width & 1);
	    for (row = 0; row < height; row++) {
	       GLubyte *d = dst;
	       for (col = 0; col < halfWidth; col++) {
		  const GLuint pixel = ((const GLuint *) src)[col];
                  *d++ = FX_rgb_scale_5[(pixel >> 10) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >>  5) & 0x1f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
		  *d++ =  (pixel & 0x8000) ? 255 : 0;
                  *d++ = FX_rgb_scale_5[(pixel >> 26) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 21) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >> 16) & 0x1f];
		  *d++ =  (pixel & 0x80000000) ? 255 : 0;
	       }
	       if (extraPixel) {
		  const GLushort pixel = src[width - 1];
                  *d++ = FX_rgb_scale_5[(pixel >> 10) & 0x1f];
                  *d++ = FX_rgb_scale_5[(pixel >>  5) & 0x1f];
                  *d++ = FX_rgb_scale_5[ pixel        & 0x1f];
		  *d++ =  (pixel & 0x8000) ? 255 : 0;
	       }
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else if (format == GL_RGB && type == GL_UNSIGNED_SHORT_5_6_5) {
	    /* directly memcpy 5R5G5B pixels into client's buffer */
	    const GLint widthInBytes = width * 2;
	    GLint row;
	    for (row = 0; row < height; row++) {
	       MEMCPY(dst, src, widthInBytes);
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else {
	    grLfbUnlock(GR_LFB_READ_ONLY, fxMesa->currentFB);
	    END_BOARD_LOCK();
	    _swrast_ReadPixels(ctx, x, y, width, height, format, type,
			       packing, dstImage);
	    return;
	 }

	 grLfbUnlock(GR_LFB_READ_ONLY, fxMesa->currentFB);
      }
      END_BOARD_LOCK();
   }
}

static void
fxDDReadPixels8888 (GLcontext * ctx,
		    GLint x, GLint y,
		    GLsizei width, GLsizei height,
		    GLenum format, GLenum type,
		    const struct gl_pixelstore_attrib *packing,
		    GLvoid *dstImage)
{
   if (ctx->_ImageTransferState/* & (IMAGE_SCALE_BIAS_BIT|IMAGE_MAP_COLOR_BIT)*/) {
      _swrast_ReadPixels(ctx, x, y, width, height, format, type,
			 packing, dstImage);
      return;
   }
   else {
      fxMesaContext fxMesa = FX_CONTEXT(ctx);
      GrLfbInfo_t info;

      BEGIN_BOARD_LOCK();
      info.size = sizeof(info);
      if (grLfbLock(GR_LFB_READ_ONLY,
		    fxMesa->currentFB,
		    GR_LFBWRITEMODE_ANY,
		    GR_ORIGIN_UPPER_LEFT, FXFALSE, &info)) {
	 const GLint winX = 0;
	 const GLint winY = fxMesa->height - 1;
	 const GLint srcStride = info.strideInBytes / 4;	/* stride in GLuints */
	 const GLuint *src = (const GLuint *) info.lfbPtr
	    + (winY - y) * srcStride + (winX + x);
	 GLubyte *dst = (GLubyte *) _mesa_image_address(packing, dstImage,
							width, height, format,
							type, 0, 0, 0);
	 GLint dstStride =
	    _mesa_image_row_stride(packing, width, format, type);

	 if (format == GL_RGB && type == GL_UNSIGNED_BYTE) {
	    /* convert 8A8R8G8B into 8R8G8B */
	    GLint row, col;
	    for (row = 0; row < height; row++) {
	       GLubyte *d = dst;
	       for (col = 0; col < width; col++) {
		  const GLuint pixel = ((const GLuint *) src)[col];
                  *d++ = pixel >> 16;
                  *d++ = pixel >> 8;
                  *d++ = pixel;
	       }
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) {
	    /* 8A8R8G8B pixels into client's buffer */
	    GLint row, col;
	    for (row = 0; row < height; row++) {
	       GLubyte *d = dst;
	       for (col = 0; col < width; col++) {
		  const GLuint pixel = ((const GLuint *) src)[col];
                  *d++ = pixel >> 16;
                  *d++ = pixel >> 8;
                  *d++ = pixel;
                  *d++ = pixel >> 24;
	       }
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else if (format == GL_RGB && type == GL_UNSIGNED_SHORT_5_6_5) {
	    /* convert 8A8R8G8B into 5R6G5B */
	    GLint row, col;
	    for (row = 0; row < height; row++) {
	       GLushort *d = (GLushort *)dst;
	       for (col = 0; col < width; col++) {
		  const GLuint pixel = ((const GLuint *) src)[col];
                  *d++ = (((pixel >> 16) & 0xf8) << 8) |
                         (((pixel >>  8) & 0xfc) << 3) |
                          ((pixel        & 0xf8) >> 3);
	       }
	       dst += dstStride;
	       src -= srcStride;
	    }
	 }
	 else {
	    grLfbUnlock(GR_LFB_READ_ONLY, fxMesa->currentFB);
	    END_BOARD_LOCK();
	    _swrast_ReadPixels(ctx, x, y, width, height, format, type,
			       packing, dstImage);
	    return;
	 }

	 grLfbUnlock(GR_LFB_READ_ONLY, fxMesa->currentFB);
      }
      END_BOARD_LOCK();
   }
}


/* [dBorca] Hack alert:
 * not finished!!!
 * revise fallback tests and fix scissor; implement new formats
 * also write its siblings: 565 and 1555
 */
void
fxDDDrawPixels8888 (GLcontext * ctx, GLint x, GLint y,
                    GLsizei width, GLsizei height,
                    GLenum format, GLenum type,
                    const struct gl_pixelstore_attrib *unpack,
                    const GLvoid * pixels)
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);
   GrLfbInfo_t info;

   if (ctx->Pixel.ZoomX != 1.0F ||
       ctx->Pixel.ZoomY != 1.0F ||
       (ctx->_ImageTransferState & (IMAGE_SCALE_BIAS_BIT|
				    IMAGE_MAP_COLOR_BIT)) ||
       ctx->Color.AlphaEnabled ||
       ctx->Depth.Test ||
       ctx->Fog.Enabled ||
       ctx->Scissor.Enabled ||
       ctx->Stencil.Enabled ||
       !ctx->Color.ColorMask[0] ||
       !ctx->Color.ColorMask[1] ||
       !ctx->Color.ColorMask[2] ||
       !ctx->Color.ColorMask[3] ||
       ctx->Color.ColorLogicOpEnabled ||
       ctx->Texture._EnabledUnits ||
       ctx->Depth.OcclusionTest ||
       fxMesa->fallback)
   {
      _swrast_DrawPixels( ctx, x, y, width, height, format, type, 
			  unpack, pixels );
      return; 
   }

   /* lock early to make sure cliprects are right */
   BEGIN_BOARD_LOCK();

   /* make sure the pixelpipe is configured correctly */
   fxSetupFXUnits(ctx);

   /* look for clipmasks, giveup if region obscured */
#if 0
   if (ctx->Color.DrawBuffer == GL_FRONT) {
      if (!inClipRects_Region(fxMesa, scrX, scrY, width, height)) {
         END_BOARD_LOCK(fxMesa);
         _swrast_DrawPixels(ctx, x, y, width, height, format, type, unpack, pixels);
         return;
      }
   }
#endif

   info.size = sizeof(info);
   if (!grLfbLock(GR_LFB_WRITE_ONLY,
                  fxMesa->currentFB,
                  GR_LFBWRITEMODE_8888,
                  GR_ORIGIN_UPPER_LEFT, FXTRUE, &info)) {
      _swrast_DrawPixels(ctx, x, y, width, height, format, type, unpack, pixels);
      return;
   }

   {
      const GLint winX = 0;
      const GLint winY = fxMesa->height - 1;

      const GLint dstStride = info.strideInBytes / 4;	/* stride in GLuints */
      GLuint *dst = (GLuint *) info.lfbPtr + (winY - y) * dstStride + (winX + x);
      const GLubyte *src = (GLubyte *)_mesa_image_address(unpack, pixels,
							width, height, format,
							type, 0, 0, 0);
      const GLint srcStride = _mesa_image_row_stride(unpack, width, format, type);

      if (format == GL_RGBA && type == GL_UNSIGNED_BYTE) {
         /* directly memcpy 8A8R8G8B pixels to screen */
         const GLint widthInBytes = width * 4;
         GLint row;
         for (row = 0; row < height; row++) {
             MEMCPY(dst, src, widthInBytes);
             dst -= dstStride;
             src += srcStride;
         }
      }
      else {
         grLfbUnlock(GR_LFB_WRITE_ONLY, fxMesa->currentFB);
         END_BOARD_LOCK();
         _swrast_DrawPixels(ctx, x, y, width, height, format, type, unpack, pixels);
         return;
      }

   }

   grLfbUnlock(GR_LFB_WRITE_ONLY, fxMesa->currentFB);
   END_BOARD_LOCK();
}


static void
fxDDFinish(GLcontext * ctx)
{
   grFlush();
}





/* KW: Put the word Mesa in the render string because quakeworld
 * checks for this rather than doing a glGet(GL_MAX_TEXTURE_SIZE).
 * Why?
 */
static const GLubyte *
fxDDGetString(GLcontext * ctx, GLenum name)
{
 fxMesaContext fxMesa = FX_CONTEXT(ctx);

 switch (name) {
        case GL_RENDERER:
             return (GLubyte *)fxMesa->rendererString;
#if __WIN32__ /* hack to advertise vanilla extension names */
        case GL_EXTENSIONS:
             if (ctx->Extensions.String == NULL) {
                GLubyte *ext = _mesa_make_extension_string(ctx);
                if (ext != NULL) {
                   ctx->Extensions.String = _mesa_malloc(strlen((char *)ext) + 256);
                   if (ctx->Extensions.String != NULL) {
                      strcpy((char *)ctx->Extensions.String, (char *)ext);
                      /* put any additional extension names here */
#if 0
                      strcat((char *)ctx->Extensions.String, " 3DFX_set_global_palette");
#endif
#if __WIN32__
                      strcat((char *)ctx->Extensions.String, " WGL_3DFX_gamma_control");
                      strcat((char *)ctx->Extensions.String, " WGL_EXT_swap_control");
                      strcat((char *)ctx->Extensions.String, " WGL_EXT_extensions_string WGL_ARB_extensions_string");
#endif
                      /* put any additional extension names here */
                      _mesa_free(ext);
                   } else {
                      ctx->Extensions.String = ext;
                   }
                }
             }
             return ctx->Extensions.String;
#endif
        default:
             return NULL;
 }
}

static const struct tnl_pipeline_stage *fx_pipeline[] = {
   &_tnl_vertex_transform_stage,	/* TODO: Add the fastpath here */
   &_tnl_normal_transform_stage,
   &_tnl_lighting_stage,
   /*&_tnl_fog_coordinate_stage,*/	/* TODO: Omit fog stage ZZZ ZZZ ZZZ */
   &_tnl_texgen_stage,
   &_tnl_texture_transform_stage,
   &_tnl_point_attenuation_stage,
   &_tnl_render_stage,
   0,
};




int
fxDDInitFxMesaContext(fxMesaContext fxMesa)
{
/* int i;*/
   GLcontext *ctx = fxMesa->glCtx;

   FX_setupGrVertexLayout();

   fxMesa->color = 0xffffffff;
   fxMesa->clearC = 0;
   fxMesa->clearA = 0;

   fxMesa->stats.swapBuffer = 0;
   fxMesa->stats.reqTexUpload = 0;
   fxMesa->stats.texUpload = 0;
   fxMesa->stats.memTexUpload = 0;

   fxMesa->tmuSrc = FX_TMU_NONE;
   fxMesa->lastUnitsMode = FX_UM_NONE;
   fxTMInit(fxMesa);

   /* FX units setup */

   fxMesa->unitsState.alphaTestEnabled = GL_FALSE;
   fxMesa->unitsState.alphaTestFunc = GL_ALWAYS;
   fxMesa->unitsState.alphaTestRefValue = 0.0;

   fxMesa->unitsState.blendEnabled = GL_FALSE;
   fxMesa->unitsState.blendSrcFuncRGB = GR_BLEND_ONE;
   fxMesa->unitsState.blendDstFuncRGB = GR_BLEND_ZERO;
   fxMesa->unitsState.blendSrcFuncAlpha = GR_BLEND_ONE;
   fxMesa->unitsState.blendDstFuncAlpha = GR_BLEND_ZERO;
   fxMesa->unitsState.blendEq = GR_BLEND_OP_ADD;

   fxMesa->unitsState.depthTestEnabled = GL_FALSE;
   fxMesa->unitsState.depthMask = GL_TRUE;
   fxMesa->unitsState.depthTestFunc = GL_LESS;
   fxMesa->unitsState.depthBias = 0;

   fxMesa->unitsState.stencilWriteMask = 0xff;

   if (fxMesa->colDepth != 16) {
      /* 32bpp mode or 15bpp mode */
      fxMesa->Glide.grColorMaskExt(FXTRUE, FXTRUE, FXTRUE, fxMesa->haveHwAlpha);
   } else {
      /* 16 bpp mode */
      grColorMask(FXTRUE, fxMesa->haveHwAlpha);
   }

   fxMesa->currentFB = fxMesa->haveDoubleBuffer ? GR_BUFFER_BACKBUFFER : GR_BUFFER_FRONTBUFFER;
   grRenderBuffer(fxMesa->currentFB);

   fxMesa->state = MALLOC(FX_grGetInteger(GR_GLIDE_STATE_SIZE));
   fxMesa->fogTable = (GrFog_t *) MALLOC(FX_grGetInteger(GR_FOG_TABLE_ENTRIES) *
			     sizeof(GrFog_t));

   if (!fxMesa->state || !fxMesa->fogTable) {
      if (fxMesa->state)
	 FREE(fxMesa->state);
      if (fxMesa->fogTable)
	 FREE(fxMesa->fogTable);
      return 0;
   }

   if (fxMesa->haveZBuffer)
      grDepthBufferMode(GR_DEPTHBUFFER_ZBUFFER);

   if (!fxMesa->bgrOrder) {
      grLfbWriteColorFormat(GR_COLORFORMAT_ABGR);
   }

   fxMesa->textureAlign = FX_grGetInteger(GR_TEXTURE_ALIGN);
   /* [koolsmoky] */
   {
    char *env;
    int textureLevels = 0;
    int textureSize = FX_grGetInteger(GR_MAX_TEXTURE_SIZE);
    do {
        textureLevels++;
    } while ((textureSize >>= 0x1) & 0x7ff);
    ctx->Const.MaxTextureLevels = textureLevels;
#if 1||FX_RESCALE_BIG_TEXURES
    fxMesa->textureMaxLod = textureLevels - 1;
    if ((env = getenv("MESA_FX_MAXLOD")) != NULL) {
       int maxLevels = atoi(env) + 1;
       if ((maxLevels <= MAX_TEXTURE_LEVELS) && (maxLevels > textureLevels)) {
          ctx->Const.MaxTextureLevels = maxLevels;
       }
    }
#endif
   }
   ctx->Const.MaxTextureCoordUnits =
   ctx->Const.MaxTextureImageUnits = fxMesa->haveTwoTMUs ? 2 : 1;
   ctx->Const.MaxTextureUnits = MAX2(ctx->Const.MaxTextureImageUnits, ctx->Const.MaxTextureCoordUnits);
   
   fxMesa->new_state = _NEW_ALL;
   if (!fxMesa->haveHwStencil) {
      /* don't touch stencil if there is none */
      fxMesa->new_state &= ~FX_NEW_STENCIL;
   }

   /* Initialize the software rasterizer and helper modules.
    */
   _swrast_CreateContext(ctx);
   _ac_CreateContext(ctx);
   _tnl_CreateContext(ctx);
   _swsetup_CreateContext(ctx);

   /* Install customized pipeline */
   _tnl_destroy_pipeline(ctx);
   _tnl_install_pipeline(ctx, fx_pipeline);

   fxAllocVB(ctx);

   fxSetupDDPointers(ctx);
   fxDDInitTriFuncs(ctx);

   /* Tell the software rasterizer to use pixel fog always.
    */
   _swrast_allow_vertex_fog(ctx, GL_FALSE);
   _swrast_allow_pixel_fog(ctx, GL_TRUE);

   /* Tell tnl not to calculate or use vertex fog factors.  (Needed to
    * tell render stage not to clip fog coords).
    */
/*     _tnl_calculate_vertex_fog( ctx, GL_FALSE ); */

   fxDDInitExtensions(ctx);

#if 0
   /* [dBorca] Hack alert:
    * do we want dither? It just looks bad...
    */
   grEnable(GR_ALLOW_MIPMAP_DITHER);
   grTexNccTable(GR_NCCTABLE_NCC0); /* set this once... no multipass */
#endif
   grGlideGetState((GrState *) fxMesa->state);

   return 1;
}

/* Undo the above.
 */
void
fxDDDestroyFxMesaContext(fxMesaContext fxMesa)
{
   _swsetup_DestroyContext(fxMesa->glCtx);
   _tnl_DestroyContext(fxMesa->glCtx);
   _ac_DestroyContext(fxMesa->glCtx);
   _swrast_DestroyContext(fxMesa->glCtx);

   if (fxMesa->state)
      FREE(fxMesa->state);
   if (fxMesa->fogTable)
      FREE(fxMesa->fogTable);
   fxTMClose(fxMesa);
   fxFreeVB(fxMesa->glCtx);
}




void
fxDDInitExtensions(GLcontext * ctx)
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);

#if 1 /* multipass ColorSum stage */
   _mesa_enable_extension(ctx, "GL_EXT_secondary_color");
#endif

   _mesa_enable_extension(ctx, "GL_EXT_point_parameters");
   _mesa_enable_extension(ctx, "GL_EXT_paletted_texture");
   _mesa_enable_extension(ctx, "GL_EXT_texture_lod_bias");
   _mesa_enable_extension(ctx, "GL_EXT_shared_texture_palette");
   _mesa_enable_extension(ctx, "GL_EXT_blend_func_separate");

   if (fxMesa->haveTwoTMUs) {
      _mesa_enable_extension(ctx, "GL_EXT_texture_env_add");
      _mesa_enable_extension(ctx, "GL_ARB_multitexture");
   }

   if (fxMesa->haveHwStencil) {
      _mesa_enable_extension( ctx, "GL_EXT_stencil_wrap" );
   }

   /* [dBorca] Hack alert:
    * True texture compression can be done only on Napalm.
    * We will advertise, however, generic texture compression
    * on all Voodoo cards; the Mesa logic allows us to eventually
    * fallback to uncompressed. This will fix those dumb applications
    * which refuse to run w/o texture compression! We actually _can_
    * do texture compression for pre-Napalm cores, through NCC. But
    * NCC poses many issues:
    * 1) NCC w/o DITHER_ERR has poor quality and NCC w/ DITHER_ERR is
    *    damn slow!
    * 2) NCC compression cannot be used with multitexturing, because
    *    the decompression tables are not per TMU anymore (bear in mind
    *    that earlier Voodoos could handle 2 NCC tables for each TMU --
    *    just look for POINTCAST_PALETTE). As a last resort, we could
    *    fake NCC multitexturing through multipass rendering, but...
    *    ohwell, it's not worth the effort...
    *    This stand true for multitexturing palletized textures.
    * 3) since NCC is not an OpenGL standard (as opposed to FXT1), we
    *    would need to plug deeper into the core... First, we would need to
    *    bind NCC to GL_COMPRESSED_RGB[A]. Then, we would need to trick
    *    Mesa into reporting our texture as compressed. Last, we would need
    *    to stash the NCC decompression table into the mipmap data and adjust
    *    CompressedSize accordingly!
    */
   if (fxMesa->HaveTexus2) {
      _mesa_enable_extension(ctx, "GL_ARB_texture_compression");

      if (fxMesa->type >= GR_SSTTYPE_Voodoo4) {
         _mesa_enable_extension(ctx, "GL_3DFX_texture_compression_FXT1");
         _mesa_enable_extension(ctx, "GL_EXT_texture_compression_s3tc");
         _mesa_enable_extension(ctx, "GL_S3_s3tc");
      }
   }

   if (fxMesa->HaveCmbExt) {
      _mesa_enable_extension(ctx, "GL_EXT_texture_env_combine");
   }

   if (fxMesa->HavePixExt) {
      _mesa_enable_extension(ctx, "GL_EXT_blend_subtract");
   }

   if (fxMesa->HaveMirExt) {
      _mesa_enable_extension(ctx, "GL_ARB_texture_mirrored_repeat");
   }
}


/************************************************************************/
/************************************************************************/
/************************************************************************/

/* Check if the hardware supports the current context
 *
 * Performs similar work to fxDDChooseRenderState() - should be merged.
 */
GLuint
fx_check_IsInHardware(GLcontext * ctx)
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);

   if (ctx->RenderMode != GL_RENDER) {
      return FX_FALLBACK_RENDER_MODE;
   }

   if (ctx->Stencil.Enabled && !fxMesa->haveHwStencil) {
      return FX_FALLBACK_STENCIL;
   }

   if (ctx->Color._DrawDestMask != FRONT_LEFT_BIT && ctx->Color._DrawDestMask != BACK_LEFT_BIT) {
      return FX_FALLBACK_DRAW_BUFFER;
   }

   if (ctx->Color.BlendEnabled) {
      if (ctx->Color.BlendEquation != GL_FUNC_ADD_EXT) {
         if (!fxMesa->HavePixExt ||
             ((ctx->Color.BlendEquation != GL_FUNC_SUBTRACT_EXT) &&
              (ctx->Color.BlendEquation != GL_FUNC_REVERSE_SUBTRACT_EXT))) {
            return FX_FALLBACK_BLEND;
         }
      }
   }

   if (ctx->Color.ColorLogicOpEnabled && (ctx->Color.LogicOp != GL_COPY)) {
      return FX_FALLBACK_LOGICOP;
   }

#if 0 /* multipass ColorSum stage */
   if (ctx->Light.Model.ColorControl == GL_SEPARATE_SPECULAR_COLOR) {
      return FX_FALLBACK_SPECULAR;
   }
#endif

   if ((fxMesa->colDepth == 16) &&
       ((ctx->Color.ColorMask[RCOMP] != ctx->Color.ColorMask[GCOMP]) ||
        (ctx->Color.ColorMask[GCOMP] != ctx->Color.ColorMask[BCOMP]))) {
      return FX_FALLBACK_COLORMASK;
   }

   /* Unsupported texture/multitexture cases */

   if (fxMesa->haveTwoTMUs) {
      /* we can only do 2D textures */
      if (ctx->Texture.Unit[0]._ReallyEnabled & ~TEXTURE_2D_BIT)
	 return FX_FALLBACK_TEXTURE_1D_3D;
      if (ctx->Texture.Unit[1]._ReallyEnabled & ~TEXTURE_2D_BIT)
	 return FX_FALLBACK_TEXTURE_1D_3D;

      if (ctx->Texture.Unit[0]._ReallyEnabled & TEXTURE_2D_BIT) {
         if (fxMesa->type < GR_SSTTYPE_Voodoo2)
	 if (ctx->Texture.Unit[0].EnvMode == GL_BLEND &&
	     (ctx->Texture.Unit[1]._ReallyEnabled & TEXTURE_2D_BIT ||
	      ctx->Texture.Unit[0].EnvColor[0] != 0 ||
	      ctx->Texture.Unit[0].EnvColor[1] != 0 ||
	      ctx->Texture.Unit[0].EnvColor[2] != 0 ||
	      ctx->Texture.Unit[0].EnvColor[3] != 1)) {
	    return FX_FALLBACK_TEXTURE_ENV;
	 }
	 if (ctx->Texture.Unit[0]._Current->Image[0]->Border > 0)
	    return FX_FALLBACK_TEXTURE_BORDER;
      }

      if (ctx->Texture.Unit[1]._ReallyEnabled & TEXTURE_2D_BIT) {
         if (fxMesa->type < GR_SSTTYPE_Voodoo2)
	 if (ctx->Texture.Unit[1].EnvMode == GL_BLEND)
	    return FX_FALLBACK_TEXTURE_ENV;
	 if (ctx->Texture.Unit[1]._Current->Image[0]->Border > 0)
	    return FX_FALLBACK_TEXTURE_BORDER;
      }

      if (TDFX_DEBUG & (VERBOSE_DRIVER | VERBOSE_TEXTURE))
	 fprintf(stderr, "fx_check_IsInHardware: envmode is %s/%s\n",
		 _mesa_lookup_enum_by_nr(ctx->Texture.Unit[0].EnvMode),
		 _mesa_lookup_enum_by_nr(ctx->Texture.Unit[1].EnvMode));

      /* KW: This was wrong (I think) and I changed it... which doesn't mean
       * it is now correct...
       * BP: The old condition just seemed to test if both texture units
       * were enabled.  That's easy!
       */
      if (ctx->Texture._EnabledUnits == 0x3) {
	 /* Can't use multipass to blend a multitextured triangle - fall
	  * back to software.
	  */
	 if (!fxMesa->haveTwoTMUs && ctx->Color.BlendEnabled) {
	    return FX_FALLBACK_TEXTURE_MULTI;
	 }

	 if ((ctx->Texture.Unit[0].EnvMode != ctx->Texture.Unit[1].EnvMode) &&
	     (ctx->Texture.Unit[0].EnvMode != GL_MODULATE) &&
	     (ctx->Texture.Unit[0].EnvMode != GL_REPLACE)) {	/* q2, seems ok... */
	    if (TDFX_DEBUG & VERBOSE_DRIVER)
	       fprintf(stderr, "fx_check_IsInHardware: unsupported multitex env mode\n");
	    return FX_FALLBACK_TEXTURE_MULTI;
	 }
      }
   }
   else {
      /* we have just one texture unit */
      if (ctx->Texture._EnabledUnits > 0x1) {
	 return FX_FALLBACK_TEXTURE_MULTI;
      }

      if (fxMesa->type < GR_SSTTYPE_Voodoo2)
      if ((ctx->Texture.Unit[0]._ReallyEnabled & TEXTURE_2D_BIT) &&
	  (ctx->Texture.Unit[0].EnvMode == GL_BLEND)) {
	 return FX_FALLBACK_TEXTURE_ENV;
      }
   }

   return 0;
}



static void
fxDDUpdateDDPointers(GLcontext * ctx, GLuint new_state)
{
   /*   TNLcontext *tnl = TNL_CONTEXT(ctx);*/
   fxMesaContext fxMesa = FX_CONTEXT(ctx);

   if (TDFX_DEBUG & VERBOSE_DRIVER) {
      fprintf(stderr, "fxDDUpdateDDPointers(%08x)\n", new_state);
   }

   _swrast_InvalidateState(ctx, new_state);
   _ac_InvalidateState(ctx, new_state);
   _tnl_InvalidateState(ctx, new_state);
   _swsetup_InvalidateState(ctx, new_state);

   fxMesa->new_gl_state |= new_state;
}




void
fxSetupDDPointers(GLcontext * ctx)
{
   fxMesaContext fxMesa = FX_CONTEXT(ctx);
/* TNLcontext *tnl = TNL_CONTEXT(ctx);*/

   if (TDFX_DEBUG & VERBOSE_DRIVER) {
      fprintf(stderr, "fxSetupDDPointers()\n");
   }

   ctx->Driver.UpdateState = fxDDUpdateDDPointers;
   ctx->Driver.GetString = fxDDGetString;
   ctx->Driver.ClearIndex = NULL;
   ctx->Driver.ClearColor = fxDDClearColor;
   ctx->Driver.Clear = fxDDClear;
   ctx->Driver.DrawBuffer = fxDDSetDrawBuffer;
   ctx->Driver.GetBufferSize = fxDDBufferSize;
   ctx->Driver.Accum = _swrast_Accum;
   ctx->Driver.CopyPixels = _swrast_CopyPixels;
   ctx->Driver.DrawPixels = _swrast_DrawPixels;
   switch (fxMesa->colDepth) {
          case 15:
               ctx->Driver.ReadPixels = fxDDReadPixels555;
               ctx->Driver.Bitmap = fxDDDrawBitmap2;
               break;
          case 16:
               ctx->Driver.ReadPixels = fxDDReadPixels565;
               ctx->Driver.Bitmap = fxDDDrawBitmap2;
               break;
          case 32:
               ctx->Driver.DrawPixels = fxDDDrawPixels8888;
               ctx->Driver.ReadPixels = fxDDReadPixels8888;
               ctx->Driver.Bitmap = fxDDDrawBitmap4;
               break;
   }
   ctx->Driver.ResizeBuffers = _swrast_alloc_buffers;
   ctx->Driver.Finish = fxDDFinish;
   ctx->Driver.Flush = NULL;
   ctx->Driver.ChooseTextureFormat = fxDDChooseTextureFormat;
   ctx->Driver.TexImage1D = _mesa_store_teximage1d;
   ctx->Driver.TexImage2D = fxDDTexImage2D;
   ctx->Driver.TexImage3D = _mesa_store_teximage3d;
   ctx->Driver.TexSubImage1D = _mesa_store_texsubimage1d;
   ctx->Driver.TexSubImage2D = fxDDTexSubImage2D;
   ctx->Driver.TexSubImage3D = _mesa_store_texsubimage3d;
   ctx->Driver.CompressedTexImage1D = _mesa_store_compressed_teximage1d;
   ctx->Driver.CompressedTexImage2D = fxDDCompressedTexImage2D;
   ctx->Driver.CompressedTexImage3D = _mesa_store_compressed_teximage3d;
   ctx->Driver.CompressedTexSubImage1D = _mesa_store_compressed_texsubimage1d;
   ctx->Driver.CompressedTexSubImage2D = fxDDCompressedTexSubImage2D;
   ctx->Driver.CompressedTexSubImage3D = _mesa_store_compressed_texsubimage3d;
   ctx->Driver.IsCompressedFormat = fxDDIsCompressedFormat;
   ctx->Driver.CompressedTextureSize = fxDDCompressedTextureSize;
   ctx->Driver.CopyTexImage1D = _swrast_copy_teximage1d;
   ctx->Driver.CopyTexImage2D = _swrast_copy_teximage2d;
   ctx->Driver.CopyTexSubImage1D = _swrast_copy_texsubimage1d;
   ctx->Driver.CopyTexSubImage2D = _swrast_copy_texsubimage2d;
   ctx->Driver.CopyTexSubImage3D = _swrast_copy_texsubimage3d;
   ctx->Driver.TestProxyTexImage = _mesa_test_proxy_teximage;
   ctx->Driver.CopyColorTable = _swrast_CopyColorTable;
   ctx->Driver.CopyColorSubTable = _swrast_CopyColorSubTable;
   ctx->Driver.CopyConvolutionFilter1D = _swrast_CopyConvolutionFilter1D;
   ctx->Driver.CopyConvolutionFilter2D = _swrast_CopyConvolutionFilter2D;
   ctx->Driver.TexEnv = fxDDTexEnv;
   ctx->Driver.TexParameter = fxDDTexParam;
   ctx->Driver.BindTexture = fxDDTexBind;
   ctx->Driver.DeleteTexture = fxDDTexDel;
   ctx->Driver.IsTextureResident = fxDDIsTextureResident;
   ctx->Driver.UpdateTexturePalette = fxDDTexPalette;
   ctx->Driver.AlphaFunc = fxDDAlphaFunc;
   ctx->Driver.BlendFunc = fxDDBlendFunc;
   ctx->Driver.BlendFuncSeparate = fxDDBlendFuncSeparate;
   ctx->Driver.BlendEquation = fxDDBlendEquation;
   ctx->Driver.DepthFunc = fxDDDepthFunc;
   ctx->Driver.DepthMask = fxDDDepthMask;
   ctx->Driver.ColorMask = fxDDColorMask;
   ctx->Driver.Fogfv = fxDDFogfv;
   ctx->Driver.Scissor = fxDDScissor;
   ctx->Driver.FrontFace = fxDDFrontFace;
   ctx->Driver.CullFace = fxDDCullFace;
   ctx->Driver.ShadeModel = fxDDShadeModel;
   ctx->Driver.Enable = fxDDEnable;
   if (fxMesa->haveHwStencil) {
      ctx->Driver.StencilFunc	= fxDDStencilFunc;
      ctx->Driver.StencilMask	= fxDDStencilMask;
      ctx->Driver.StencilOp	= fxDDStencilOp;
   }

   fxSetupDDSpanPointers(ctx);
   fxDDUpdateDDPointers(ctx, ~0);
}


#else


/*
 * Need this to provide at least one external definition.
 */

extern int gl_fx_dummy_function_dd(void);
int
gl_fx_dummy_function_dd(void)
{
   return 0;
}

#endif /* FX */
