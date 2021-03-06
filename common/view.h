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
// view.h

#ifndef __VIEW_H
#define __VIEW_H

extern	cvar_t	*crosshair;
extern	cvar_t	*crosshaircolor;

extern	cvar_t  *cl_crossx;
extern	cvar_t  *cl_crossy;

extern	cvar_t	*v_gamma;
extern	cvar_t	*lcd_x;
extern  cvar_t	*v_contentblend; /* FS: Fucking hate palette blends */

#ifdef QUAKEWORLD
extern  cvar_t	*net_showchat; /* FS: EZQ Chat */
extern  cvar_t	*net_showchatgfx; /* FS: EZQ Chat */
#endif

#ifdef GLQUAKE
extern float v_blend[4];
#endif

void V_Init (void);
void V_RenderView (void);
float V_CalcRoll (vec3_t angles, vec3_t velocity);
void V_UpdatePalette (void);

#endif // __VIEW_H
