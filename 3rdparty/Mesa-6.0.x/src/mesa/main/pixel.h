/**
 * \file pixel.h
 * Pixel operations.
 */

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


#ifndef PIXEL_H
#define PIXEL_H


#include "mtypes.h"


/** \name API functions */
/*@*/

extern void GLAPIENTRY
_mesa_GetPixelMapfv( GLenum map, GLfloat *values );

extern void GLAPIENTRY
_mesa_GetPixelMapuiv( GLenum map, GLuint *values );

extern void GLAPIENTRY
_mesa_GetPixelMapusv( GLenum map, GLushort *values );

extern void GLAPIENTRY
_mesa_PixelMapfv( GLenum map, GLsizei mapsize, const GLfloat *values );

extern void GLAPIENTRY
_mesa_PixelMapuiv(GLenum map, GLsizei mapsize, const GLuint *values );

extern void GLAPIENTRY
_mesa_PixelMapusv(GLenum map, GLsizei mapsize, const GLushort *values );

extern void GLAPIENTRY
_mesa_PixelStoref( GLenum pname, GLfloat param );


extern void GLAPIENTRY
_mesa_PixelStorei( GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_PixelTransferf( GLenum pname, GLfloat param );

extern void GLAPIENTRY
_mesa_PixelTransferi( GLenum pname, GLint param );

extern void GLAPIENTRY
_mesa_PixelZoom( GLfloat xfactor, GLfloat yfactor );

/*@}*/


/** \name Pixel processing functions */
/*@{*/

extern void
_mesa_scale_and_bias_rgba(const GLcontext *ctx, GLuint n, GLfloat rgba[][4],
                          GLfloat rScale, GLfloat gScale,
                          GLfloat bScale, GLfloat aScale,
                          GLfloat rBias, GLfloat gBias,
                          GLfloat bBias, GLfloat aBias);

extern void
_mesa_map_rgba(const GLcontext *ctx, GLuint n, GLfloat rgba[][4]);


extern void
_mesa_transform_rgba(const GLcontext *ctx, GLuint n, GLfloat rgba[][4]);


extern void
_mesa_lookup_rgba(const struct gl_color_table *table,
                  GLuint n, GLfloat rgba[][4]);


extern void
_mesa_shift_and_offset_ci(const GLcontext *ctx, GLuint n,
                          GLuint indexes[]);


extern void
_mesa_map_ci(const GLcontext *ctx, GLuint n, GLuint index[]);


extern void
_mesa_map_ci_to_rgba_chan(const GLcontext *ctx,
                          GLuint n, const GLuint index[],
                          GLchan rgba[][4]);


extern void
_mesa_map_ci_to_rgba(const GLcontext *ctx,
                     GLuint n, const GLuint index[], GLfloat rgba[][4]);


extern void
_mesa_map_ci8_to_rgba(const GLcontext *ctx,
                      GLuint n, const GLubyte index[],
                      GLchan rgba[][4]);


extern void
_mesa_shift_and_offset_stencil(const GLcontext *ctx, GLuint n,
                               GLstencil indexes[]);


extern void
_mesa_map_stencil(const GLcontext *ctx, GLuint n, GLstencil index[]);


extern void
_mesa_chan_to_float_span(const GLcontext *ctx, GLuint n,
                         CONST GLchan rgba[][4], GLfloat rgbaf[][4]);


extern void 
_mesa_update_pixel( GLcontext *ctx, GLuint newstate );

extern void 
_mesa_init_pixel( GLcontext * ctx );

/*@}*/

#endif
