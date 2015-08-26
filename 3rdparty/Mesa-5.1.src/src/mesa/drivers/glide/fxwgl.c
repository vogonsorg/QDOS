/*
 * Mesa 3-D graphics library
 * Version:  4.0
 *
 * Copyright (C) 1999-2001  Brian Paul   All Rights Reserved.
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
 *    Keith Whitwell
 *    Hiroshi Morii
 *    Daniel Borca
 */

/* fxwgl.c - Microsoft wgl functions emulation for
 *           3Dfx VooDoo/Mesa interface
 */


#ifdef _WIN32

#ifdef __cplusplus
extern "C"
{
#endif

#include <windows.h>
#define GL_GLEXT_PROTOTYPES
#include "GL/gl.h"
#include "GL/glext.h"

#ifdef __cplusplus
}
#endif

#include "GL/fxmesa.h"
#include "glheader.h"
#include "glapi.h"
#include "imports.h"
#include "fxdrv.h"

#define MAX_MESA_ATTRS  20

#if (_MSC_VER >= 1200)
#pragma warning( push )
#pragma warning( disable : 4273 )
#endif

struct __extensions__
{
   PROC proc;
   char *name;
};

struct __pixelformat__
{
   PIXELFORMATDESCRIPTOR pfd;
   GLint mesaAttr[MAX_MESA_ATTRS];
};

WINGDIAPI void GLAPIENTRY gl3DfxSetPaletteEXT(GLuint *);

struct __pixelformat__ pix[] = {
   /* 16bit RGB565 single buffer with depth */
   {
    {sizeof(PIXELFORMATDESCRIPTOR), 1,
     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
     PFD_TYPE_RGBA,
     16,
     5, 0, 6, 5, 5, 11, 0, 0,
     0, 0, 0, 0, 0,
     16,
     0,
     0,
     PFD_MAIN_PLANE,
     0, 0, 0, 0}
    ,
    {FXMESA_COLORDEPTH, 16,
     FXMESA_ALPHA_SIZE, 0,
     FXMESA_DEPTH_SIZE, 16,
     FXMESA_STENCIL_SIZE, 0,
     FXMESA_ACCUM_SIZE, 0,
     FXMESA_NONE}
   }
   ,
   /* 16bit RGB565 double buffer with depth */
   {
    {sizeof(PIXELFORMATDESCRIPTOR), 1,
     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
     PFD_DOUBLEBUFFER | PFD_SWAP_COPY,
     PFD_TYPE_RGBA,
     16,
     5, 0, 6, 5, 5, 11, 0, 0,
     0, 0, 0, 0, 0,
     16,
     0,
     0,
     PFD_MAIN_PLANE,
     0, 0, 0, 0}
    ,
    {FXMESA_COLORDEPTH, 16,
     FXMESA_DOUBLEBUFFER,
     FXMESA_ALPHA_SIZE, 0,
     FXMESA_DEPTH_SIZE, 16,
     FXMESA_STENCIL_SIZE, 0,
     FXMESA_ACCUM_SIZE, 0,
     FXMESA_NONE}
   }
   ,
   /* 16bit ARGB1555 single buffer with depth */
   {
    {sizeof(PIXELFORMATDESCRIPTOR), 1,
     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
     PFD_TYPE_RGBA,
     16,
     5, 0, 5, 5, 5, 10, 1, 15,
     0, 0, 0, 0, 0,
     16,
     0,
     0,
     PFD_MAIN_PLANE,
     0, 0, 0, 0}
    ,
    {FXMESA_COLORDEPTH, 15,
     FXMESA_ALPHA_SIZE, 1,
     FXMESA_DEPTH_SIZE, 16,
     FXMESA_STENCIL_SIZE, 0,
     FXMESA_ACCUM_SIZE, 0,
     FXMESA_NONE}
   }
   ,
   /* 16bit ARGB1555 double buffer with depth */
   {
    {sizeof(PIXELFORMATDESCRIPTOR), 1,
     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
     PFD_DOUBLEBUFFER | PFD_SWAP_COPY,
     PFD_TYPE_RGBA,
     16,
     5, 0, 5, 5, 5, 10, 1, 15,
     0, 0, 0, 0, 0,
     16,
     0,
     0,
     PFD_MAIN_PLANE,
     0, 0, 0, 0}
    ,
    {FXMESA_COLORDEPTH, 15,
     FXMESA_DOUBLEBUFFER,
     FXMESA_ALPHA_SIZE, 1,
     FXMESA_DEPTH_SIZE, 16,
     FXMESA_STENCIL_SIZE, 0,
     FXMESA_ACCUM_SIZE, 0,
     FXMESA_NONE}
   }
   ,
   /* 32bit ARGB8888 single buffer with depth */
   {
    {sizeof(PIXELFORMATDESCRIPTOR), 1,
     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
     PFD_TYPE_RGBA,
     32,
     8, 0, 8, 8, 8, 16, 8, 24,
     0, 0, 0, 0, 0,
     24,
     8,
     0,
     PFD_MAIN_PLANE,
     0, 0, 0, 0}
    ,
    {FXMESA_COLORDEPTH, 32,
     FXMESA_ALPHA_SIZE, 8,
     FXMESA_DEPTH_SIZE, 24,
     FXMESA_STENCIL_SIZE, 8,
     FXMESA_ACCUM_SIZE, 0,
     FXMESA_NONE}
   }
   ,
   /* 32bit ARGB8888 double buffer with depth */
   {
    {sizeof(PIXELFORMATDESCRIPTOR), 1,
     PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
     PFD_DOUBLEBUFFER | PFD_SWAP_COPY,
     PFD_TYPE_RGBA,
     32,
     8, 0, 8, 8, 8, 16, 8, 24,
     0, 0, 0, 0, 0,
     24,
     8,
     0,
     PFD_MAIN_PLANE,
     0, 0, 0, 0}
    ,
    {FXMESA_COLORDEPTH, 32,
     FXMESA_DOUBLEBUFFER,
     FXMESA_ALPHA_SIZE, 8,
     FXMESA_DEPTH_SIZE, 24,
     FXMESA_STENCIL_SIZE, 8,
     FXMESA_ACCUM_SIZE, 0,
     FXMESA_NONE}
   }
};
static int qt_pix = sizeof(pix) / sizeof(pix[0]);

static fxMesaContext ctx = NULL;
static WNDPROC hWNDOldProc;
static int curPFD = 0;
static HDC hDC;
static HWND hWND;

static GLboolean haveDualHead;

/* For the in-window-rendering hack */

#ifndef GR_CONTROL_RESIZE
/* Apparently GR_CONTROL_RESIZE can be ignored. OK? */
#define GR_CONTROL_RESIZE -1
#endif

static GLboolean gdiWindowHack;
static void *dibSurfacePtr;
static BITMAPINFO *dibBMI;
static HBITMAP dibHBM;
static HWND dibWnd;

static LRESULT APIENTRY 
__wglMonitor(HWND hwnd, UINT message, UINT wParam, LONG lParam)
 {
   long ret;			/* Now gives the resized window at the end to hWNDOldProc */

   if (ctx && hwnd == hWND) {
     switch (message) {
       case WM_PAINT:
       case WM_MOVE:
	   break;
       case WM_DISPLAYCHANGE:
       case WM_SIZE:
#if 0
       if (wParam != SIZE_MINIMIZED) {
         static int moving = 0;
         if (!moving) {
           if (!FX_grSstControl(GR_CONTROL_RESIZE)) {
             moving = 1;
             SetWindowPos(hwnd, 0, 0, 0, 300, 300, SWP_NOMOVE | SWP_NOZORDER);
             moving = 0;
             if (!FX_grSstControl(GR_CONTROL_RESIZE)) {
               /*MessageBox(0,_T("Error changing windowsize"),_T("fxMESA"),MB_OK); */
               PostMessage(hWND, WM_CLOSE, 0, 0);
		     }
		   }
	       /* Do the clipping in the glide library */
	       grClipWindow(0, 0, FX_grSstScreenWidth(), FX_grSstScreenHeight());
           /* And let the new size set in the context */
           fxMesaUpdateScreenSize(ctx);
	     }
	   }
#endif
       break;
       case WM_ACTIVATE:
       break;
       case WM_SHOWWINDOW:
	   break;
       case WM_SYSKEYDOWN:
       case WM_SYSCHAR:
	   break;
     }
   }

   /* Finaly call the hWNDOldProc, which handles the resize witch the
      now changed window sizes */
   ret = CallWindowProc(hWNDOldProc, hwnd, message, wParam, lParam);

   return (ret);
}

GLAPI BOOL GLAPIENTRY
wglCopyContext(HGLRC hglrcSrc, HGLRC hglrcDst, UINT mask)
{
   return (FALSE);
}

GLAPI HGLRC GLAPIENTRY
wglCreateContext(HDC hdc)
{
   HWND hWnd;
   WNDPROC oldProc;
   int error;

   if (ctx) {
      SetLastError(0);
      return (NULL);
   }

   if (!(hWnd = WindowFromDC(hdc))) {
      SetLastError(0);
      return (NULL);
   }

   if (curPFD == 0) {
      SetLastError(0);
      return (NULL);
   }

   if ((oldProc = (WNDPROC) GetWindowLong(hWnd, GWL_WNDPROC)) != __wglMonitor) {
      hWNDOldProc = oldProc;
      SetWindowLong(hWnd, GWL_WNDPROC, (LONG) __wglMonitor);
   }

#if FX_DEBUG
   freopen("MESA.LOG", "w", stderr);
#endif

   {
     RECT cliRect;
     ShowWindow(hWnd, SW_SHOWNORMAL);
     SetForegroundWindow(hWnd);
     Sleep(100); /* a hack for win95 */
     GetClientRect(hWnd, &cliRect);
     error = !(ctx = fxMesaCreateBestContext((GLuint) hWnd, cliRect.right, cliRect.bottom, pix[curPFD - 1].mesaAttr));
   }

   /*if (getenv("SST_DUALHEAD"))
      haveDualHead =
	 ((atoi(getenv("SST_DUALHEAD")) == 1) ? GL_TRUE : GL_FALSE);
   else
      haveDualHead = GL_FALSE;*/

   if (error) {
      SetLastError(0);
      return (NULL);
   }

   hDC = hdc;
   hWND = hWnd;

   /* Required by the OpenGL Optimizer 1.1 (is it a Optimizer bug ?) */
   wglMakeCurrent(hdc, (HGLRC) 1);

   return ((HGLRC) 1);
}

GLAPI HGLRC GLAPIENTRY
wglCreateLayerContext(HDC hdc, int iLayerPlane)
{
   SetLastError(0);
   return (NULL);
}

GLAPI BOOL GLAPIENTRY
wglDeleteContext(HGLRC hglrc)
{
   if (ctx && hglrc == (HGLRC) 1) {

      fxMesaDestroyContext(ctx);

      SetWindowLong(WindowFromDC(hDC), GWL_WNDPROC, (LONG) hWNDOldProc);

      ctx = NULL;
      hDC = 0;
      return (TRUE);
   }

   SetLastError(0);

   return (FALSE);
}

GLAPI HGLRC GLAPIENTRY
wglGetCurrentContext(VOID)
{
   if (ctx)
      return ((HGLRC) 1);

   SetLastError(0);
   return (NULL);
}

GLAPI HDC GLAPIENTRY
wglGetCurrentDC(VOID)
{
   if (ctx)
      return (hDC);

   SetLastError(0);
   return (NULL);
}

GLAPI BOOL GLAPIENTRY
wglSwapIntervalEXT (int interval)
{
 if (ctx == NULL) {
    return FALSE;
 }
 if (interval < 0) {
    interval = 0;
 } else if (interval > 3) {
    interval = 3;
 }
 ctx->swapInterval = interval;
 return TRUE;
}

GLAPI int GLAPIENTRY
wglGetSwapIntervalEXT (void)
{
 return (ctx == NULL) ? -1 : ctx->swapInterval;
}

GLAPI BOOL GLAPIENTRY
wglGetDeviceGammaRamp3DFX (HDC hdc, LPVOID arrays)
{
 return TRUE;
}

GLAPI BOOL GLAPIENTRY
wglSetDeviceGammaRamp3DFX (HDC hdc, LPVOID arrays)
{
 return TRUE;
}

GLAPI const char * GLAPIENTRY
wglGetExtensionsStringEXT (void)
{
 return "WGL_3DFX_gamma_control "
        "WGL_EXT_swap_control "
        "WGL_EXT_extensions_string WGL_ARB_extensions_string";
}

GLAPI const char * GLAPIENTRY
wglGetExtensionsStringARB (HDC hdc)
{
 return wglGetExtensionsStringEXT();
}

static struct {
       const char *name;
       PROC func;
} wgl_ext[] = {
       {"wglGetExtensionsStringARB", wglGetExtensionsStringARB},
       {"wglGetExtensionsStringEXT", wglGetExtensionsStringEXT},
       {"wglSwapIntervalEXT",        wglSwapIntervalEXT},
       {"wglGetSwapIntervalEXT",     wglGetSwapIntervalEXT},
       {"wglGetDeviceGammaRamp3DFX", wglGetDeviceGammaRamp3DFX},
       {"wglSetDeviceGammaRamp3DFX", wglSetDeviceGammaRamp3DFX},
       {NULL, NULL}
};

GLAPI PROC GLAPIENTRY
wglGetProcAddress(LPCSTR lpszProc)
{ 
   int i;
   PROC p = (PROC) _glapi_get_proc_address((const char *) lpszProc);

   if (p)
      return p;

   for (i = 0; wgl_ext[i].name; i++) {
       if (!strcmp(lpszProc, wgl_ext[i].name)) {
          return wgl_ext[i].func;
       }
   }

   SetLastError(0);
   return (NULL);
}

GLAPI BOOL GLAPIENTRY
wglMakeCurrent(HDC hdc, HGLRC hglrc)
{
   if ((hdc == NULL) && (hglrc == NULL))
      return (TRUE);

   if (!ctx || hglrc != (HGLRC) 1 || WindowFromDC(hdc) != hWND) {
      SetLastError(0);
      return (FALSE);
   }

   hDC = hdc;

   fxMesaMakeCurrent(ctx);

   return (TRUE);
}

GLAPI BOOL GLAPIENTRY
wglShareLists(HGLRC hglrc1, HGLRC hglrc2)
{
   if (!ctx || hglrc1 != (HGLRC) 1 || hglrc1 != hglrc2) {
      SetLastError(0);
      return (FALSE);
   }

   return (TRUE);
}

static BOOL
wglUseFontBitmaps_FX(HDC fontDevice, DWORD firstChar, DWORD numChars,
		     DWORD listBase)
{
   TEXTMETRIC metric;
   BITMAPINFO *dibInfo;
   HDC bitDevice;
   COLORREF tempColor;
   int i;

   GetTextMetrics(fontDevice, &metric);

   dibInfo = (BITMAPINFO *) calloc(sizeof(BITMAPINFO) + sizeof(RGBQUAD), 1);
   dibInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
   dibInfo->bmiHeader.biPlanes = 1;
   dibInfo->bmiHeader.biBitCount = 1;
   dibInfo->bmiHeader.biCompression = BI_RGB;

   bitDevice = CreateCompatibleDC(fontDevice);

   // Swap fore and back colors so the bitmap has the right polarity
   tempColor = GetBkColor(bitDevice);
   SetBkColor(bitDevice, GetTextColor(bitDevice));
   SetTextColor(bitDevice, tempColor);

   // Place chars based on base line
   SetTextAlign(bitDevice, TA_BASELINE);

   for (i = 0; i < (int)numChars; i++) {
      SIZE size;
      char curChar;
      int charWidth, charHeight, bmapWidth, bmapHeight, numBytes, res;
      HBITMAP bitObject;
      HGDIOBJ origBmap;
      unsigned char *bmap;

      curChar = (char)(i + firstChar); // [koolsmoky] explicit cast

      // Find how high/wide this character is
      GetTextExtentPoint32(bitDevice, &curChar, 1, &size);
 
      // Create the output bitmap
      charWidth = size.cx;
      charHeight = size.cy;
      bmapWidth = ((charWidth + 31) / 32) * 32;	// Round up to the next multiple of 32 bits
      bmapHeight = charHeight;
      bitObject = CreateCompatibleBitmap(bitDevice, bmapWidth, bmapHeight);
      //VERIFY(bitObject);

      // Assign the output bitmap to the device
      origBmap = SelectObject(bitDevice, bitObject);

      PatBlt(bitDevice, 0, 0, bmapWidth, bmapHeight, BLACKNESS);

      // Use our source font on the device
      SelectObject(bitDevice, GetCurrentObject(fontDevice, OBJ_FONT));

      // Draw the character
      TextOut(bitDevice, 0, metric.tmAscent, &curChar, 1);

      // Unselect our bmap object
      SelectObject(bitDevice, origBmap);

      // Convert the display dependant representation to a 1 bit deep DIB
      numBytes = (bmapWidth * bmapHeight) / 8;
      bmap = MALLOC(numBytes);
      dibInfo->bmiHeader.biWidth = bmapWidth;
      dibInfo->bmiHeader.biHeight = bmapHeight;
      res = GetDIBits(bitDevice, bitObject, 0, bmapHeight, bmap,
		      dibInfo, DIB_RGB_COLORS);

      // Create the GL object
      glNewList(i + listBase, GL_COMPILE);
      glBitmap(bmapWidth, bmapHeight, 0.0, metric.tmDescent,
	       charWidth, 0.0, bmap);
      glEndList();
      // CheckGL();

      // Destroy the bmap object
      DeleteObject(bitObject);

      // Deallocate the bitmap data
      FREE(bmap);
   }

   // Destroy the DC
   DeleteDC(bitDevice);

   FREE(dibInfo);

   return TRUE;
}

GLAPI BOOL GLAPIENTRY
wglUseFontBitmapsW(HDC hdc, DWORD first, DWORD count, DWORD listBase)
{
   return (FALSE);
}

GLAPI BOOL GLAPIENTRY
wglUseFontOutlinesA(HDC hdc, DWORD first, DWORD count,
		    DWORD listBase, FLOAT deviation,
		    FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf)
{
   SetLastError(0);
   return (FALSE);
}

GLAPI BOOL GLAPIENTRY
wglUseFontOutlinesW(HDC hdc, DWORD first, DWORD count,
		    DWORD listBase, FLOAT deviation,
		    FLOAT extrusion, int format, LPGLYPHMETRICSFLOAT lpgmf)
{
   SetLastError(0);
   return (FALSE);
}


GLAPI BOOL GLAPIENTRY
wglSwapLayerBuffers(HDC hdc, UINT fuPlanes)
{
   if (ctx && WindowFromDC(hdc) == hWND) {
      fxMesaSwapBuffers();

      return (TRUE);
   }

   SetLastError(0);
   return (FALSE);
}

GLAPI int GLAPIENTRY
wglChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR * ppfd)
{
   int i, best = -1, qt_valid_pix;

#if 0
  FILE *pix_file;
  pix_file = fopen("pix_log.txt", "a");
  if (pix_file) {
  fprintf(pix_file, "wglChoosePixelFormat\n");
  fprintf(pix_file, "nSize           = %d\n",ppfd->nSize);
  fprintf(pix_file, "nVersion        = %d\n",ppfd->nVersion);
  fprintf(pix_file, "dwFlags         = %d\n",ppfd->dwFlags);
  fprintf(pix_file, "iPixelType      = %d\n",ppfd->iPixelType);
  fprintf(pix_file, "cColorBits      = %d\n",ppfd->cColorBits);
  fprintf(pix_file, "cRedBits        = %d\n",ppfd->cRedBits);
  fprintf(pix_file, "cRedShift       = %d\n",ppfd->cRedShift);
  fprintf(pix_file, "cGreenBits      = %d\n",ppfd->cGreenBits);
  fprintf(pix_file, "cGreenShift     = %d\n",ppfd->cGreenShift);
  fprintf(pix_file, "cBlueBits       = %d\n",ppfd->cBlueBits);
  fprintf(pix_file, "cBlueShift      = %d\n",ppfd->cBlueShift);
  fprintf(pix_file, "cAlphaBits      = %d\n",ppfd->cAlphaBits);
  fprintf(pix_file, "cAlphaShift     = %d\n",ppfd->cAlphaShift);
  fprintf(pix_file, "cAccumBits      = %d\n",ppfd->cAccumBits);
  fprintf(pix_file, "cAccumRedBits   = %d\n",ppfd->cAccumRedBits);
  fprintf(pix_file, "cAccumGreenBits = %d\n",ppfd->cAccumGreenBits);
  fprintf(pix_file, "cAccumBlueBits  = %d\n",ppfd->cAccumBlueBits);
  fprintf(pix_file, "cAccumAlphaBits = %d\n",ppfd->cAccumAlphaBits);
  fprintf(pix_file, "cDepthBits      = %d\n",ppfd->cDepthBits);
  fprintf(pix_file, "cStencilBits    = %d\n",ppfd->cStencilBits);
  fprintf(pix_file, "cAuxBuffers     = %d\n",ppfd->cAuxBuffers);
  fprintf(pix_file, "iLayerType      = %d\n",ppfd->iLayerType);
  fprintf(pix_file, "bReserved       = %d\n",ppfd->bReserved);
  fprintf(pix_file, "dwLayerMask     = %d\n",ppfd->dwLayerMask);
  fprintf(pix_file, "dwVisibleMask   = %d\n",ppfd->dwVisibleMask);
  fprintf(pix_file, "dwDamageMask    = %d\n",ppfd->dwDamageMask);
  }
  fclose(pix_file);
#endif

#if 1 || QUAKE2
  if (ppfd->cColorBits == 24 && ppfd->cDepthBits == 32) {
     ppfd->cColorBits = 16;
     ppfd->cDepthBits = 16;
  }
#endif

   qt_valid_pix = qt_pix;

   if (ppfd->nSize != sizeof(PIXELFORMATDESCRIPTOR) || ppfd->nVersion != 1) {
      SetLastError(0);
      return (0);
   }

   for (i = 0; i < qt_valid_pix; i++) {
      if (ppfd->cColorBits > 0 && pix[i].pfd.cColorBits != ppfd->cColorBits) 
		  continue;

      if ((ppfd->dwFlags & PFD_DRAW_TO_WINDOW)
	  && !(pix[i].pfd.dwFlags & PFD_DRAW_TO_WINDOW)) continue;
      if ((ppfd->dwFlags & PFD_DRAW_TO_BITMAP)
	  && !(pix[i].pfd.dwFlags & PFD_DRAW_TO_BITMAP)) continue;
      if ((ppfd->dwFlags & PFD_SUPPORT_GDI)
	  && !(pix[i].pfd.dwFlags & PFD_SUPPORT_GDI)) continue;
      if ((ppfd->dwFlags & PFD_SUPPORT_OPENGL)
	  && !(pix[i].pfd.dwFlags & PFD_SUPPORT_OPENGL)) continue;
      if (!(ppfd->dwFlags & PFD_DOUBLEBUFFER_DONTCARE)
	  && ((ppfd->dwFlags & PFD_DOUBLEBUFFER) !=
	      (pix[i].pfd.dwFlags & PFD_DOUBLEBUFFER))) continue;
      if (!(ppfd->dwFlags & PFD_STEREO_DONTCARE)
	  && ((ppfd->dwFlags & PFD_STEREO) !=
	      (pix[i].pfd.dwFlags & PFD_STEREO))) continue;

      if (ppfd->cDepthBits > 0 && pix[i].pfd.cDepthBits == 0)
	 continue;		/* need depth buffer */

      if (ppfd->cAlphaBits > 0 && pix[i].pfd.cAlphaBits == 0)
	 continue;		/* need alpha buffer */

#if 0
      if ((ppfd->cColorBits == 32) && (ppfd->cStencilBits > 0 && pix[i].pfd.cStencilBits == 0))
	 continue;		/* need stencil */
#endif

      if (ppfd->iPixelType == pix[i].pfd.iPixelType) {
	 best = i + 1;
	 break;
      }
   }

   if (best == -1) {
      SetLastError(0);
      return (0);
   }

   return (best);
}

GLAPI int GLAPIENTRY
ChoosePixelFormat(HDC hdc, const PIXELFORMATDESCRIPTOR * ppfd)
{
  
   return wglChoosePixelFormat(hdc, ppfd);
}

GLAPI int GLAPIENTRY
wglDescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes,
		       LPPIXELFORMATDESCRIPTOR ppfd)
{
   int qt_valid_pix;

   qt_valid_pix = qt_pix;

   if (iPixelFormat < 1 || iPixelFormat > qt_valid_pix ||
       ((nBytes != sizeof(PIXELFORMATDESCRIPTOR)) && (nBytes != 0))) {
      SetLastError(0);
      return (0);
   }

   if (nBytes != 0)
      *ppfd = pix[iPixelFormat - 1].pfd;

   return (qt_valid_pix);
}

GLAPI int GLAPIENTRY
DescribePixelFormat(HDC hdc, int iPixelFormat, UINT nBytes,
		    LPPIXELFORMATDESCRIPTOR ppfd)
{
   return wglDescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd);
}

GLAPI int GLAPIENTRY
wglGetPixelFormat(HDC hdc)
{
   if (curPFD == 0) {
      SetLastError(0);
      return (0);
   }

   return (curPFD);
}

GLAPI int GLAPIENTRY
GetPixelFormat(HDC hdc)
{
   return wglGetPixelFormat(hdc);
}

GLAPI BOOL GLAPIENTRY
wglSetPixelFormat(HDC hdc, int iPixelFormat, const PIXELFORMATDESCRIPTOR * ppfd)
{
   int qt_valid_pix;

   qt_valid_pix = qt_pix;

   if (iPixelFormat < 1 || iPixelFormat > qt_valid_pix
       || ppfd->nSize != sizeof(PIXELFORMATDESCRIPTOR)) {
      SetLastError(0);
      return (FALSE);
   }
   curPFD = iPixelFormat;

   return (TRUE);
}

GLAPI BOOL GLAPIENTRY
wglSwapBuffers(HDC hdc)
{
   if (!ctx) {
      SetLastError(0);
      return (FALSE);
   }

   fxMesaSwapBuffers();

   return (TRUE);
}

GLAPI BOOL GLAPIENTRY
SetPixelFormat(HDC hdc, int iPixelFormat, const PIXELFORMATDESCRIPTOR * ppfd)
{
   return wglSetPixelFormat(hdc, iPixelFormat, ppfd);
}

GLAPI BOOL GLAPIENTRY
SwapBuffers(HDC hdc)
{
   return wglSwapBuffers(hdc);
}

static FIXED FixedFromDouble(double d)
{
  long l = (long) (d * 65536L);
  return *(FIXED *)&l;
}

/*
** This was yanked from windows/gdi/wgl.c
*/
GLAPI BOOL GLAPIENTRY
wglUseFontBitmapsA(HDC hdc, DWORD first, DWORD count, DWORD listBase)
{
  int i;
  GLuint font_list;
  DWORD size;
  GLYPHMETRICS gm;
  HANDLE hBits;
  LPSTR lpBits;
  MAT2 mat;
  int  success = TRUE;

  if (first<0)
     return FALSE;
  if (count<0)
     return FALSE;
  if (listBase<0)
     return FALSE;

  font_list = listBase;

  mat.eM11 = FixedFromDouble(1);
  mat.eM12 = FixedFromDouble(0);
  mat.eM21 = FixedFromDouble(0);
  mat.eM22 = FixedFromDouble(-1);

  memset(&gm,0,sizeof(gm));

  /*
  ** If we can't get the glyph outline, it may be because this is a fixed
  ** font.  Try processing it that way.
  */
  if( GetGlyphOutline(hdc, first, GGO_BITMAP, &gm, 0, NULL, &mat)
      == GDI_ERROR )
  {
    return wglUseFontBitmaps_FX( hdc, first, count, listBase );
  }

  /*
  ** Otherwise process all desired characters.
  */
  for (i = 0; i < count; i++)
  {
    DWORD err;

    glNewList( font_list+i, GL_COMPILE );

    /* allocate space for the bitmap/outline */
    size = GetGlyphOutline(hdc, first + i, GGO_BITMAP, &gm, 0, NULL, &mat);
    if (size == GDI_ERROR)
    {
      glEndList( );
      err = GetLastError();
      success = FALSE;
      continue;
    }

    hBits  = GlobalAlloc(GHND, size+1);
    lpBits = GlobalLock(hBits);

    err = GetGlyphOutline(hdc,                /* handle to device context */
                          first + i,          /* character to query */
                          GGO_BITMAP,         /* format of data to return */
                          &gm,                /* pointer to structure for metrics*/
                          size,               /* size of buffer for data */
                          lpBits,             /* pointer to buffer for data */
                          &mat                /* pointer to transformation */
                                              /* matrix structure */
                          );

    if (err == GDI_ERROR)
    {
      GlobalUnlock(hBits);
      GlobalFree(hBits);
      
      glEndList( );
      err = GetLastError();
      success = FALSE;
      continue;
    }

    glBitmap(gm.gmBlackBoxX,gm.gmBlackBoxY,
             -gm.gmptGlyphOrigin.x,
             gm.gmptGlyphOrigin.y,
             gm.gmCellIncX,gm.gmCellIncY,
             (const GLubyte * )lpBits);

    GlobalUnlock(hBits);
    GlobalFree(hBits);

    glEndList( );
  }

  return success;
}

GLAPI BOOL GLAPIENTRY
wglDescribeLayerPlane(HDC hdc, int iPixelFormat, int iLayerPlane,
                      UINT nBytes, LPLAYERPLANEDESCRIPTOR ppfd)
{
  SetLastError(0);
  return (FALSE);
}

GLAPI int GLAPIENTRY
wglGetLayerPaletteEntries(HDC hdc, int iLayerPlane, int iStart,
                          int cEntries, COLORREF *pcr)
{
  SetLastError(0);
  return (FALSE);
}

GLAPI BOOL GLAPIENTRY
wglRealizeLayerPalette(HDC hdc,int iLayerPlane,BOOL bRealize)
{
  SetLastError(0);
  return(FALSE);
}

GLAPI int GLAPIENTRY
wglSetLayerPaletteEntries(HDC hdc,int iLayerPlane, int iStart,
                          int cEntries, CONST COLORREF *pcr)
{
  SetLastError(0);
  return(FALSE);
}

#if (_MSC_VER >= 1200)
#pragma warning( pop )
#endif

#endif /* FX */
