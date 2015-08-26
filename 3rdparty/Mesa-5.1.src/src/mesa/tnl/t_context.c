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
 *
 * Authors:
 *    Keith Whitwell <keith@tungstengraphics.com>
 */


#include "api_arrayelt.h"
#include "glheader.h"
#include "imports.h"
#include "macros.h"
#include "mtypes.h"
#include "dlist.h"
#include "light.h"
#include "vtxfmt.h"

#include "t_context.h"
#include "t_array_api.h"
#include "t_vtx_api.h"
#include "t_save_api.h"
#include "t_pipeline.h"
#include "tnl.h"



void
_tnl_MakeCurrent( GLcontext *ctx,
		  GLframebuffer *drawBuffer,
		  GLframebuffer *readBuffer )
{
}


static void
install_driver_callbacks( GLcontext *ctx )
{
   ctx->Driver.NewList = _tnl_NewList;
   ctx->Driver.EndList = _tnl_EndList;
   ctx->Driver.FlushVertices = _tnl_FlushVertices;
   ctx->Driver.SaveFlushVertices = _tnl_SaveFlushVertices;
   ctx->Driver.MakeCurrent = _tnl_MakeCurrent;
   ctx->Driver.BeginCallList = _tnl_BeginCallList;
   ctx->Driver.EndCallList = _tnl_EndCallList;
}



GLboolean
_tnl_CreateContext( GLcontext *ctx )
{
   TNLcontext *tnl;

   /* Create the TNLcontext structure
    */
   ctx->swtnl_context = tnl = (TNLcontext *) CALLOC( sizeof(TNLcontext) );

   if (!tnl) {
      return GL_FALSE;
   }

   /* Initialize the VB.
    */
   tnl->vb.Size = ctx->Const.MaxArrayLockSize + MAX_CLIPPED_VERTICES;


   /* Initialize tnl state and tnl->vtxfmt.
    */
   _tnl_save_init( ctx );
   _tnl_array_init( ctx );
   _tnl_vtx_init( ctx );
   _tnl_install_pipeline( ctx, _tnl_default_pipeline );

   /* Initialize the arrayelt helper
    */
   if (!_ae_create_context( ctx ))
      return GL_FALSE;


   tnl->NeedNdcCoords = GL_TRUE;
   tnl->LoopbackDListCassettes = GL_FALSE;
   tnl->CalcDListNormalLengths = GL_TRUE;

   /* Hook our functions into exec and compile dispatch tables.
    */
   _mesa_install_exec_vtxfmt( ctx, &tnl->exec_vtxfmt );


   /* Set a few default values in the driver struct.
    */
   install_driver_callbacks(ctx);
   ctx->Driver.NeedFlush = 0;
   ctx->Driver.CurrentExecPrimitive = PRIM_OUTSIDE_BEGIN_END;
   ctx->Driver.CurrentSavePrimitive = PRIM_UNKNOWN;

   tnl->Driver.Render.PrimTabElts = _tnl_render_tab_elts;
   tnl->Driver.Render.PrimTabVerts = _tnl_render_tab_verts;
   tnl->Driver.NotifyMaterialChange = _mesa_validate_all_lighting_tables;
   
   return GL_TRUE;
}


void
_tnl_DestroyContext( GLcontext *ctx )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);

   _tnl_array_destroy( ctx );
   _tnl_vtx_destroy( ctx );
   _tnl_save_destroy( ctx );
   _tnl_destroy_pipeline( ctx );
   _ae_destroy_context( ctx );

   FREE(tnl);
   ctx->swtnl_context = 0;
}


void
_tnl_InvalidateState( GLcontext *ctx, GLuint new_state )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);

   if (new_state & _NEW_ARRAY) {
      tnl->pipeline.run_input_changes |= ctx->Array.NewState; /* overkill */
   }

   _ae_invalidate_state(ctx, new_state);

   tnl->pipeline.run_state_changes |= new_state;
   tnl->pipeline.build_state_changes |= (new_state &
					 tnl->pipeline.build_state_trigger);

   tnl->vtx.eval.new_state |= new_state;
}


void
_tnl_wakeup_exec( GLcontext *ctx )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);

   install_driver_callbacks(ctx);
   ctx->Driver.NeedFlush |= FLUSH_UPDATE_CURRENT;

   /* Hook our functions into exec and compile dispatch tables.
    */
   _mesa_install_exec_vtxfmt( ctx, &tnl->exec_vtxfmt );

   /* Call all appropriate driver callbacks to revive state.
    */
   _tnl_MakeCurrent( ctx, ctx->DrawBuffer, ctx->ReadBuffer );

   /* Assume we haven't been getting state updates either:
    */
   _tnl_InvalidateState( ctx, ~0 );
   tnl->pipeline.run_input_changes = ~0;

   if (ctx->Light.ColorMaterialEnabled) {
      _mesa_update_color_material( ctx, 
				   ctx->Current.Attrib[VERT_ATTRIB_COLOR0] );
   }
}


void
_tnl_wakeup_save_exec( GLcontext *ctx )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);

   _tnl_wakeup_exec( ctx );
   _mesa_install_save_vtxfmt( ctx, &tnl->save_vtxfmt );
}


/**
 * Drivers call this function to tell the TCL module whether or not
 * it wants Normalized Device Coords (NDC) computed.  I.e. whether
 * we should "Divide-by-W".  Software renders will want that.
 */
void
_tnl_need_projected_coords( GLcontext *ctx, GLboolean mode )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);
   if (tnl->NeedNdcCoords != mode) {
      tnl->NeedNdcCoords = mode;
      _tnl_InvalidateState( ctx, _NEW_PROJECTION );
   }
}

void
_tnl_need_dlist_loopback( GLcontext *ctx, GLboolean mode )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);
   tnl->LoopbackDListCassettes = mode;
}

void
_tnl_need_dlist_norm_lengths( GLcontext *ctx, GLboolean mode )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);
   tnl->CalcDListNormalLengths = mode;
}

void
_tnl_isolate_materials( GLcontext *ctx, GLboolean mode )
{
   TNLcontext *tnl = TNL_CONTEXT(ctx);
   tnl->IsolateMaterials = mode;
}
