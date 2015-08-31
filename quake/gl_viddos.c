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

#include <dlfcn.h>

#include "quakedef.h"
#include "cfgfile.h"

#include "vid_dos.h"
#include <GL/dmesa.h>
#include "GL/fxmesa.h"

#define WARP_WIDTH	320
#define WARP_HEIGHT	200

static DMesaVisual dv;
static DMesaContext dc;
static DMesaBuffer db;

#define stringify(m) { #m, m }

unsigned short	d_8to16table[256];
unsigned	d_8to24table[256];
unsigned char d_15to8table[65536];

int num_shades=32;

int	d_con_indirect = 0;

cvar_t		vid_mode = {"vid_mode","5",false};
 
cvar_t		vid_wait = {"vid_wait", "0", false};
cvar_t		_vid_wait_override = {"_vid_wait_override", "0", false};

char	*framebuffer_ptr;


int scr_width, scr_height;

/*-----------------------------------------------------------------------*/

int		texture_extension_number = 1;

float		gldepthmin, gldepthmax;

cvar_t	gl_ztrick = {"gl_ztrick","1"};

const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
const char *gl_extensions;

static float vid_gamma = 1.0;

qboolean is8bit = false;
qboolean isPermedia = false;
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
void VID_CreateDMesaContext(int width, int height, int bpp)
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

void VID_Shutdown(void)
{
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

void VID_ShiftPalette(unsigned char *p)
{
//	VID_SetPalette(p);
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
#if 0 /* FS: TODO Use ARB multitexture */
	if (COM_CheckParm("-nomtex"))
		Con_Warning ("Mutitexture disabled at command line\n");
	else
	{
		if (strstr(gl_extensions, "GL_ARB_multitexture "))
		{
			qglMTexCoord2fSGIS = (void *) qwglGetProcAddress("glMTexCoord2fSGIS");
			qglSelectTextureSGIS = (void *) qwglGetProcAddress("glSelectTextureSGIS");
			if (qglMTexCoord2fSGIS && qglSelectTextureSGIS)
			{
				Con_Printf("Multitexture extensions found.\n");
				gl_mtexable = true;
			}
			else
				Con_Warning ("multitexture not supported (wglGetProcAddress failed)\n");
		}
	}
#endif
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


#define NUM_RESOLUTIONS 8

static int resolutions[NUM_RESOLUTIONS][3]={ 
	{320,240,  GR_RESOLUTION_320x240},
	{400,300,  GR_RESOLUTION_400x300},
	{512,384,  GR_RESOLUTION_512x384},
	{640,480,  GR_RESOLUTION_640x480},
	{800,600,  GR_RESOLUTION_800x600},
	{1024,768, GR_RESOLUTION_1024x768},
	{1280,1024,GR_RESOLUTION_1280x1024},
	{1600,1200,GR_RESOLUTION_1600x1200}
};

int findres(int *width, int *height)
{
	int i;

	for(i=0;i<NUM_RESOLUTIONS;i++)
		if((*width<=resolutions[i][0]) && (*height<=resolutions[i][1])) {
			*width = resolutions[i][0];
			*height = resolutions[i][1];
			return resolutions[i][2];
		}

	*width = 640;
	*height = 480;
	return GR_RESOLUTION_640x480;
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

	vid_gamma = v_gamma.value;

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
	char *read_vars[] = {
		"gamma",
	};
#define num_readvars	(int)(sizeof(read_vars) / sizeof(read_vars[0]))


	Cvar_RegisterVariable (&vid_mode);
	Cvar_RegisterVariable (&gl_ztrick);

	Cvar_RegisterVariable (&vid_wait);
	Cvar_RegisterVariable (&_vid_wait_override);

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

	// perform an early read of config.cfg -- sezero
	CFG_ReadCvars (read_vars, num_readvars);
	// check for command line overrides -- sezero
	CFG_ReadCvarOverrides (read_vars, num_readvars);

	GL_Init();

	sprintf (gldir, "%s/glquake", com_gamedir);
	Sys_mkdir (gldir);

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
