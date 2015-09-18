/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <stdio.h>

#include "quakedef.h"
#include "sys_dxe.h"

#include <GL/dmesa.h>

#define WARP_WIDTH	320
#define WARP_HEIGHT	200

static DMesaVisual dv;
static DMesaContext dc;
static DMesaBuffer db;

// Gamma stuff
#define	USE_GAMMA_RAMPS			0
#define GAMMA_MAX	3.0

/* 3dfx gamma hacks: stuff are in fx_gamma.c
 * Note: gamma ramps crashes voodoo graphics */
#define	USE_3DFX_RAMPS			0
#if defined(USE_3DFXGAMMA)
#include "fx_gamma.h"
#endif

#if (USE_GAMMA_RAMPS) || (defined(USE_3DFXGAMMA) && (USE_3DFX_RAMPS))
static unsigned short	orig_ramps[3][256];
#endif

static qboolean	fx_gamma   = false;	// 3dfx-specific gamma control
static qboolean	gammaworks = false;	// whether hw-gamma works
static qboolean is_3dfx = false;

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];
unsigned char d_15to8table[65536];

cvar_t	*vid_mode;
cvar_t	*vid_wait;
cvar_t	*_vid_wait_override;

static int scr_width, scr_height;

/*-----------------------------------------------------------------------*/

int		texture_extension_number = 1;

float		gldepthmin, gldepthmax;

cvar_t	*gl_ztrick;
cvar_t	*r_ignorehwgamma;

const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
const char *gl_extensions;

static float vid_gamma = 1.0;

qboolean is8bit = false;
qboolean gl_mtexable = false;

/* FS: TODO: make a real video table */
static char currentVideoModeDesc[256];

/* FS: Fine control over the DMesa Context parameters.  Mostly for debugging and experimentation, but maybe someone has a reason to play with it. */
static int bpp = 16;
static int alphaBufferSize = 2;
static int depthBufferSize = 16;

void VID_MenuDraw (void);
void VID_MenuKey (int key);

/*-----------------------------------------------------------------------*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
}

void D_EndDirectRect (int x, int y, int width, int height)
{
}

/* FS: Moved here */
static void VID_CreateDMesaContext(int width, int height, int bpp)
{
	dv = DMesaCreateVisual((GLint)width, (GLint)height, bpp, 0, true, true, alphaBufferSize, depthBufferSize, 0, 0);
	if (!dv)
		Sys_Error("Unable to create 3DFX visual.\n");

	dc = DMesaCreateContext(dv, NULL);
	if (!dc)
		Sys_Error("Unable to create 3DFX context.\n");

	scr_width = width;
	scr_height = height;

	db = DMesaCreateBuffer(dv, 0,0,(GLint)width,(GLint)height);
	if (!db)
		Sys_Error("Unable to create 3DFX buffer.\n");
	DMesaMakeCurrent(dc, db);
}

#if !defined(USE_3DFXGAMMA)
static inline int Init_3dfxGammaCtrl (void)		{ return 0; }
static inline void Shutdown_3dfxGamma (void)		{ }
static inline int do3dfxGammaCtrl (float value)			{ return 0; }
static inline int glGetDeviceGammaRamp3DFX (void *arrays)	{ return 0; }
static inline int glSetDeviceGammaRamp3DFX (void *arrays)	{ return 0; }
static inline qboolean VID_Check3dfxGamma (void)	{ return false; }
#else
static qboolean VID_Check3dfxGamma (void)
{
	int		ret;

#if USE_3DFX_RAMPS /* not recommended for Voodoo1, currently crashes */
	ret = glGetDeviceGammaRamp3DFX(orig_ramps);
	if (ret != 0)
	{
		Con_SafePrintf ("Using 3dfx glide3 specific gamma ramps\n");
		return true;
	}
#else
	ret = Init_3dfxGammaCtrl();
	if (ret > 0)
	{
		Con_SafePrintf ("Using 3dfx glide%d gamma controls\n", ret);
		return true;
	}
#endif
	return false;
}
#endif	/* USE_3DFXGAMMA */

static void VID_InitGamma (void)
{
	gammaworks = fx_gamma = false;

	if(COM_CheckParm("-ignorehwgamma") || r_ignorehwgamma->intValue)
	{
		Con_SafePrintf("ignoring hardware gamma\n");
		return;
	}

	/* we don't have WGL_3DFX_gamma_control or an equivalent in dos. */
	/* Here is an evil hack abusing the exposed Glide symbols: */
	if (is_3dfx)
		fx_gamma = VID_Check3dfxGamma();

	if (!gammaworks && !fx_gamma)
		Con_SafePrintf("gamma adjustment not available\n");
}

static void VID_ShutdownGamma (void)
{
#if USE_3DFX_RAMPS
	if (fx_gamma) glSetDeviceGammaRamp3DFX(orig_ramps);
#else
/*	if (fx_gamma) do3dfxGammaCtrl(1);*/
#endif
	Shutdown_3dfxGamma();
}

static void VID_SetGamma (void)
{
#if (!USE_GAMMA_RAMPS) || (!USE_3DFX_RAMPS)
	float	value = (v_gamma->value > (1.0 / GAMMA_MAX)) ?
			(1.0 / v_gamma->value) : GAMMA_MAX;
#endif
#if USE_3DFX_RAMPS
	if (fx_gamma) glSetDeviceGammaRamp3DFX(ramps);
#else
	if (fx_gamma) do3dfxGammaCtrl(value);
#endif
}

void VID_ShiftPalette (unsigned char *palette)
{
	VID_SetGamma();
}

void	VID_SetPalette (unsigned char *palette)
{
	byte	*pal;
	unsigned r,g,b;
	unsigned v;
	int		r1,g1,b1;
	int		k;
	unsigned short i;
	unsigned	*table;
	int dist, bestdist;

//
// 8 8 8 encoding
//
	pal = palette;
	table = d_8to24table;
	for (i=0 ; i<256 ; i++)
	{
		r = pal[0];
		g = pal[1];
		b = pal[2];
		pal += 3;
		
		v = (255<<24) + (r<<0) + (g<<8) + (b<<16);
		*table++ = v;
	}
	d_8to24table[255] &= 0xffffff;	// 255 is transparent

	// JACK: 3D distance calcs - k is last closest, l is the distance.
	for (i=0; i < (1<<15); i++) {
		/* Maps
		000000000000000
		000000000011111 = Red  = 0x1F
		000001111100000 = Blue = 0x03E0
		111110000000000 = Grn  = 0x7C00
		*/
		r = ((i & 0x1F) << 3)+4;
		g = ((i & 0x03E0) >> 2)+4;
		b = ((i & 0x7C00) >> 7)+4;
		pal = (unsigned char *)d_8to24table;
		for (v=0,k=0,bestdist=10000*10000; v<256; v++,pal+=4) {
			r1 = (int)r - (int)pal[0];
			g1 = (int)g - (int)pal[1];
			b1 = (int)b - (int)pal[2];
			dist = (r1*r1)+(g1*g1)+(b1*b1);
			if (dist < bestdist) {
				k=v;
				bestdist = dist;
			}
		}
		d_15to8table[i]=k;
	}
}

void *qwglGetProcAddress(char *symbol)
{
	return DMesaGetProcAddress(symbol);
}

void CheckMultiTextureExtensions(void) 
{
	/* FS: Slow as shit if under 1280x1024, so explicitly require it to be set. */
	if (COM_CheckParm("-mtex"))
	{
		if (strstr(gl_extensions, "GL_ARB_multitexture"))
		{
			qglMTexCoord2fFunc = (void *) qwglGetProcAddress("glMultiTexCoord2fARB");
			qglSelectTextureFunc = (void *) qwglGetProcAddress("glActiveTextureARB");
			if (qglMTexCoord2fFunc && qglSelectTextureFunc)
			{
				Con_Printf("FOUND: ARB_multitexture\n");
				TEXTURE0 = GL_TEXTURE0_ARB;
				TEXTURE1 = GL_TEXTURE1_ARB;
				gl_mtexable = true;
			}
			else
				Con_Warning ("multitexture not supported (DMesaGetProcAddress failed)\n");
		}
		else
			Con_Warning ("multitexture not supported (extension not found)\n");
	}
}

/*
===============
GL_SetupState -- johnfitz

does all the stuff from GL_Init that needs to be done every time a new GL render context is created
GL_Init will still do the stuff that only needs to be done once
===============
*/
void GL_SetupState (void)
{
	glClearColor (0.15,0.15,0.15,0); //johnfitz -- originally 1,0,0,0
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.666);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel (GL_FLAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

/*
===============
GL_Init
===============
*/
void GL_Init (void)
{
	gl_vendor = (const char *)glGetString (GL_VENDOR);
	Con_Printf ("GL_VENDOR: %s\n", gl_vendor);
	gl_renderer = (const char *)glGetString (GL_RENDERER);
	Con_Printf ("GL_RENDERER: %s\n", gl_renderer);

	gl_version = (const char *)glGetString (GL_VERSION);
	Con_Printf ("GL_VERSION: %s\n", gl_version);
	gl_extensions = (const char *)glGetString (GL_EXTENSIONS);
	Con_Printf ("GL_EXTENSIONS: %s\n", gl_extensions);

	is_3dfx = false;
	if (!Q_strncasecmp((char *)gl_renderer, "3dfx", 4)	  ||
	    !Q_strncasecmp((char *)gl_renderer, "SAGE Glide", 10) ||
	    !Q_strncasecmp((char *)gl_renderer, "Glide ", 6)	  || /* possible with Mesa 3.x/4.x/5.0.x */
	    !Q_strncasecmp((char *)gl_renderer, "Mesa Glide", 10))
	{
	// This should hopefully detect Voodoo1 and Voodoo2
	// hardware and possibly Voodoo Rush.
	// Voodoo Banshee, Voodoo3 and later are hw-accelerated
	// by DRI in XFree86-4.x and should be: is_3dfx = false.
		Con_SafePrintf("3dfx Voodoo found\n");
		is_3dfx = true;
	}

	CheckMultiTextureExtensions ();

	GL_SetupState(); // johnfitz
}

/*
=================
GL_BeginRendering

=================
*/
void GL_BeginRendering (int *x, int *y, int *width, int *height)
{
	*x = *y = 0;
	*width = scr_width;
	*height = scr_height;
}

void GL_EndRendering (void)
{
	glFlush();
	DMesaSwapBuffers(db);
}

qboolean VID_Is8bit(void)
{
	return is8bit;
}
void (APIENTRY * qglColorTableEXT)( GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table );
void VID_Init8bitPalette() 
{
#if 0
/* FS: This now works in Mesa 5.1 but it looks rather silly from far distances.
 *     So, bye.  Here for historical purposes.
 */
	// Check for 8bit Extensions and initialize them.
	int i;

	if (COM_CheckParm("-no8bit"))
		return;

	if (strstr(gl_extensions, "GL_EXT_shared_texture_palette") &&
		(qglColorTableEXT = (void *)DMesaGetProcAddress("glColorTableEXT")) != NULL)
	{
		char thePalette[256*3];
		char *oldPalette, *newPalette;

		Con_SafePrintf("... Using GL_EXT_shared_texture_palette\n");
		glEnable( GL_SHARED_TEXTURE_PALETTE_EXT );
		oldPalette = (char *) d_8to24table; //d_8to24table3dfx;
		newPalette = thePalette;
		for (i=0;i<256;i++) {
			*newPalette++ = *oldPalette++;
			*newPalette++ = *oldPalette++;
			*newPalette++ = *oldPalette++;
			oldPalette++;
		}
		qglColorTableEXT(GL_SHARED_TEXTURE_PALETTE_EXT, GL_RGB, 256, GL_RGB, GL_UNSIGNED_BYTE, (void *) thePalette);
		is8bit = true;
	}
#endif
}

static void Check_Gamma (unsigned char *pal)
{
	float	f, inf;
	unsigned char	palette[768];
	int		i;

	vid_gamma = v_gamma->value;

	if ((i = COM_CheckParm("-gamma")) > 0)
		vid_gamma = Q_atof(com_argv[i+1]);

	for (i=0 ; i<768 ; i++)
	{
		f = pow ( (pal[i]+1)/256.0 , vid_gamma );
		inf = f*255 + 0.5;
		if (inf < 0)
			inf = 0;
		if (inf > 255)
			inf = 255;
		palette[i] = inf;
	}

	memcpy (pal, palette, sizeof(palette));
}

void VID_Init(unsigned char *palette)
{
	int i;
	char	gldir[MAX_OSPATH];
	int width = 640, height = 480;

	vid_mode = Cvar_Get("vid_mode","5", 0);
	vid_wait = Cvar_Get("vid_wait", "0", 0);;
	_vid_wait_override = Cvar_Get("_vid_wait_override", "0", 0);

	gl_ztrick = Cvar_Get("gl_ztrick", "1", 0);
	r_ignorehwgamma = Cvar_Get("r_ignorehwgamma", "0", CVAR_ARCHIVE);
	r_ignorehwgamma->description = "Skip testing for 3DFX Hardware Gamma capabilities";

	vid.maxwarpwidth = WARP_WIDTH;
	vid.maxwarpheight = WARP_HEIGHT;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));

// interpret command-line params
	if ((i = COM_CheckParm("-width")) != 0)
		width = atoi(com_argv[i+1]);
	if ((i = COM_CheckParm("-height")) != 0)
		height = atoi(com_argv[i+1]);

	if ((i = COM_CheckParm("-conwidth")) != 0)
		vid.conwidth = Q_atoi(com_argv[i+1]);
	else
		vid.conwidth = 640;

	if ((i = COM_CheckParm("-bpp")) != 0) /* FS: Force BPP */
	{
		int x = Q_atoi(com_argv[i+1]);
		if ((x == 15) || (x == 32))
			bpp = x;
	}
	if ((i = COM_CheckParm("-alphasize")) != 0) /* FS: Force alpha buffer size */
	{
		int x = Q_atoi(com_argv[i+1]);
		if(x)
		{
			Con_SafePrintf("\x02Warning: Alpha buffer size %i.  Default %i.\n", x, alphaBufferSize);
			alphaBufferSize = x;
		}
	}
	if ((i = COM_CheckParm("-depthsize")) != 0) /* FS: Force depth buffer size */
	{
		int x = Q_atoi(com_argv[i+1]);
		if(x)
		{
			Con_SafePrintf("\x02Warning: Depth buffer size %i.  Default %i.\n", x, depthBufferSize);
			depthBufferSize = x;
		}
	}

	vid.conwidth &= 0xfff8; // make it a multiple of eight

	if (vid.conwidth < 320)
		vid.conwidth = 320;

	// pick a conheight that matches with correct aspect
	vid.conheight = vid.conwidth*3 / 4;

	if ((i = COM_CheckParm("-conheight")) != 0)
		vid.conheight = Q_atoi(com_argv[i+1]);
	if (vid.conheight < 200)
		vid.conheight = 200;

	/* don't let fxMesa cheat multitexturing */
	putenv("FX_DONT_FAKE_MULTITEX=1");

	VID_CreateDMesaContext(width, height, bpp); /* FS: Setup DMesa contexts */

	if (vid.conheight > height)
		vid.conheight = height;
	if (vid.conwidth > width)
		vid.conwidth = width;
	vid.width = vid.conwidth;
	vid.height = vid.conheight;

	vid.aspect = ((float)vid.height / (float)vid.width) *
				(320.0 / 240.0);
	vid.numpages = 2;

	GL_Init();

	sprintf (gldir, "%s/glquake", com_gamedir);
	Sys_mkdir (gldir);

	VID_InitGamma();
	Check_Gamma(palette);
	VID_SetPalette(palette);

	// Check for 3DFX Extensions and initialize them.
	VID_Init8bitPalette();

	vid_menudrawfn = VID_MenuDraw;
	vid_menukeyfn = VID_MenuKey;

	Com_sprintf(currentVideoModeDesc, sizeof(currentVideoModeDesc), "%dx%dx%d", width, height, bpp);
	Con_SafePrintf ("Video mode %s initialized.\n", currentVideoModeDesc);

	vid.recalc_refdef = 1;				// force a surface cache flush
}

void VID_Shutdown(void)
{
	VID_ShutdownGamma();
	if (db)
	{
		DMesaDestroyBuffer(db);
		db = NULL;
	}
	if (dc)
	{
		DMesaDestroyContext(dc);
		dc = NULL;
	}
	if (dv)
	{
		DMesaDestroyVisual(dv);
		dv = NULL;
	}
}


//========================================================
// Video menu stuff
//========================================================

/* FS: TODO: Grab the GR_RESOLUTION list from earlier to show common resolutions */
extern void M_Menu_Options_f (void);
extern void M_Print (int cx, int cy, char *str);
extern void M_PrintWhite (int cx, int cy, char *str);
extern void M_DrawCharacter (int cx, int line, int num);
extern void M_DrawTransPic (int x, int y, qpic_t *pic);
extern void M_DrawPic (int x, int y, qpic_t *pic);

#if 0
static int	vid_line, vid_wmodes;

typedef struct
{
	int		modenum;
	char	*desc;
	int		iscur;
} modedesc_t;
#endif

#define MAX_COLUMN_SIZE		9
#define MODE_AREA_HEIGHT	(MAX_COLUMN_SIZE + 2)
#define MAX_MODEDESCS		(MAX_COLUMN_SIZE*3)

#if 0
static modedesc_t	modedescs[MAX_MODEDESCS];
#endif

/*
================
VID_MenuDraw
================
*/
void VID_MenuDraw (void)
{
	qpic_t		*p;
#if 0
	char		*ptr;
	int			lnummodes, i, j, k, column, row, dup, dupmode;
	char		temp[100];
	vmode_t		*pv;
#endif

	p = Draw_CachePic ("gfx/vidmodes.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);

#if 0
	vid_wmodes = 0;
	lnummodes = VID_NumModes ();
	
	for (i=1 ; (i<lnummodes) && (vid_wmodes < MAX_MODEDESCS) ; i++)
	{
		ptr = VID_GetModeDescription (i);
		pv = VID_GetModePtr (i);

		k = vid_wmodes;

		modedescs[k].modenum = i;
		modedescs[k].desc = ptr;
		modedescs[k].iscur = 0;

		if (i == vid_modenum)
			modedescs[k].iscur = 1;

		vid_wmodes++;

	}

	if (vid_wmodes > 0)
	{
		M_Print (2*8, 36+0*8, "Fullscreen Modes (WIDTHxHEIGHTxBPP)");

		column = 8;
		row = 36+2*8;

		for (i=0 ; i<vid_wmodes ; i++)
		{
			if (modedescs[i].iscur)
				M_PrintWhite (column, row, modedescs[i].desc);
			else
				M_Print (column, row, modedescs[i].desc);

			column += 13*8;

			if ((i % VID_ROW_SIZE) == (VID_ROW_SIZE - 1))
			{
				column = 8;
				row += 8;
			}
		}
	}
#else
	M_PrintWhite (8, 36+2*8, currentVideoModeDesc);
	M_Print (3*8, 36 + MODE_AREA_HEIGHT * 8 + 8*2,
			 "Video modes must be set from the");
	M_Print (3*8, 36 + MODE_AREA_HEIGHT * 8 + 8*3,
			 "command line with -width <width>");
	M_Print (3*8, 36 + MODE_AREA_HEIGHT * 8 + 8*4,
			 "and -bpp <bits-per-pixel>");
#endif
}


/*
================
VID_MenuKey
================
*/
void VID_MenuKey (int key)
{
	switch (key)
	{
	case K_ESCAPE:
		S_LocalSound ("misc/menu1.wav");
		M_Menu_Options_f ();
		break;

	default:
		break;
	}
}

void VID_UnlockBuffer() {}
void VID_LockBuffer() {}
