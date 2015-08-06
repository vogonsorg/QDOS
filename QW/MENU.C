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
#include "quakedef.h"
#include "winquake.h"

void (*vid_menudrawfn)(void);
void (*vid_menukeyfn)(int key);

enum {m_none, m_main, m_singleplayer, m_load, m_save, m_multiplayer, m_setup, m_net, m_options, m_video, m_keys, m_help, m_quit, m_serialconfig, m_modemconfig, m_lanconfig, m_gameoptions, m_search, m_slist, m_extended, m_gamespy} m_state;

void M_Menu_Main_f (void);
	void M_Menu_SinglePlayer_f (void);
		void M_Menu_Load_f (void);
		void M_Menu_Save_f (void);
	void M_Menu_MultiPlayer_f (void);
		void M_Menu_Gamespy_f (void); /* FS: Gamespy stuff */
		void M_Menu_Setup_f (void);
		void M_Menu_Net_f (void);
	void M_Menu_Options_f (void);
		void M_Menu_Keys_f (void);
		void M_Menu_Video_f (void);
	void M_Menu_Help_f (void);
	void M_Menu_Quit_f (void);
void M_Menu_SerialConfig_f (void);
	void M_Menu_ModemConfig_f (void);
void M_Menu_LanConfig_f (void);
void M_Menu_GameOptions_f (void);
void M_Menu_Search_f (void);
void M_Menu_ServerList_f (void);
void M_Menu_Extended_f (void); /* FS: Extended options unique to QDOS */

void M_Main_Draw (void);
	void M_SinglePlayer_Draw (void);
		void M_Load_Draw (void);
		void M_Save_Draw (void);
	void M_MultiPlayer_Draw (void);
		void M_Gamespy_Draw (void); /* FS: Gamespy stuff */
		void M_Setup_Draw (void);
		void M_Net_Draw (void);
	void M_Options_Draw (void);
		void M_Keys_Draw (void);
		void M_Video_Draw (void);
	void M_Help_Draw (void);
	void M_Quit_Draw (void);
void M_SerialConfig_Draw (void);
	void M_ModemConfig_Draw (void);
void M_LanConfig_Draw (void);
void M_GameOptions_Draw (void);
void M_Search_Draw (void);
void M_ServerList_Draw (void);
void M_Extended_Draw (void); /* FS: Extended options unique to QDOS */

void M_Main_Key (int key);
	void M_SinglePlayer_Key (int key);
		void M_Load_Key (int key);
		void M_Save_Key (int key);
	void M_MultiPlayer_Key (int key);
		void M_Gamespy_Key (int key); /* FS: Gamespy stuff */
		void M_Setup_Key (int key);
		void M_Net_Key (int key);
	void M_Options_Key (int key);
		void M_Keys_Key (int key);
		void M_Video_Key (int key);
	void M_Help_Key (int key);
	void M_Quit_Key (int key);
void M_SerialConfig_Key (int key);
	void M_ModemConfig_Key (int key);
void M_LanConfig_Key (int key);
void M_GameOptions_Key (int key);
void M_Search_Key (int key);
void M_ServerList_Key (int key);
void M_Extended_Key (int key); /* FS: Extended options unique to QDOS */
extern void snd_restart_f (void); /* FS: For extended options */
void M_Extended_Set_Sound_KHz (int dir, int khz); /* FS: Extended options unique to QDOS */

/* FS: Gravis Ultrasound stuff */
#ifdef _WIN32
int havegus = 0;
#else
extern int havegus;
#endif

qboolean	m_entersound;	   // play after drawing a frame, so caching
								// won't disrupt the sound
qboolean	m_recursiveDraw;

int		     m_return_state;
qboolean	m_return_onerror;
char	    m_return_reason [32];

#define StartingGame    (m_multiplayer_cursor == 1)
#define JoiningGame	     (m_multiplayer_cursor == 0)
#define SerialConfig    (m_net_cursor == 0)
#define DirectConfig    (m_net_cursor == 1)
#define IPXConfig	       (m_net_cursor == 2)
#define TCPIPConfig	     (m_net_cursor == 3)

void M_ConfigureNetSubsystem(void);

//=============================================================================
/* Support Routines */

/*
================
M_DrawCharacter

Draws one solid graphics character
================
*/
void M_DrawCharacter (int cx, int line, int num)
{
	Draw_Character ( cx + ((vid.width - 320)>>1), line, num);
}

void M_Print (int cx, int cy, char *str)
{
	while (*str)
	{
		M_DrawCharacter (cx, cy, (*str)+128);
		str++;
		cx += 8;
	}
}

void M_PrintWhite (int cx, int cy, char *str)
{
	while (*str)
	{
		M_DrawCharacter (cx, cy, *str);
		str++;
		cx += 8;
	}
}

void M_DrawTransPic (int x, int y, qpic_t *pic)
{
	Draw_TransPic (x + ((vid.width - 320)>>1), y, pic);
}

void M_DrawPic (int x, int y, qpic_t *pic)
{
	Draw_Pic (x + ((vid.width - 320)>>1), y, pic);
}

byte identityTable[256];
byte translationTable[256];

void M_BuildTranslationTable(int top, int bottom)
{
	int	     j;
	byte    *dest, *source;

	for (j = 0; j < 256; j++)
		identityTable[j] = j;
	dest = translationTable;
	source = identityTable;
	memcpy (dest, source, 256);

	if (top < 128)  // the artists made some backwards ranges.  sigh.
		memcpy (dest + TOP_RANGE, source + top, 16);
	else
		for (j=0 ; j<16 ; j++)
			dest[TOP_RANGE+j] = source[top+15-j];

	if (bottom < 128)
		memcpy (dest + BOTTOM_RANGE, source + bottom, 16);
	else
		for (j=0 ; j<16 ; j++)
			dest[BOTTOM_RANGE+j] = source[bottom+15-j];	     
}


void M_DrawTransPicTranslate (int x, int y, qpic_t *pic)
{
	Draw_TransPicTranslate (x + ((vid.width - 320)>>1), y, pic, translationTable);
}


void M_DrawTextBox (int x, int y, int width, int lines)
{
	qpic_t  *p;
	int	     cx, cy;
	int	     n;

	// draw left side
	cx = x;
	cy = y;
	p = Draw_CachePic ("gfx/box_tl.lmp");
	M_DrawTransPic (cx, cy, p);
	p = Draw_CachePic ("gfx/box_ml.lmp");
	for (n = 0; n < lines; n++)
	{
		cy += 8;
		M_DrawTransPic (cx, cy, p);
	}
	p = Draw_CachePic ("gfx/box_bl.lmp");
	M_DrawTransPic (cx, cy+8, p);

	// draw middle
	cx += 8;
	while (width > 0)
	{
		cy = y;
		p = Draw_CachePic ("gfx/box_tm.lmp");
		M_DrawTransPic (cx, cy, p);
		p = Draw_CachePic ("gfx/box_mm.lmp");
		for (n = 0; n < lines; n++)
		{
			cy += 8;
			if (n == 1)
				p = Draw_CachePic ("gfx/box_mm2.lmp");
			M_DrawTransPic (cx, cy, p);
		}
		p = Draw_CachePic ("gfx/box_bm.lmp");
		M_DrawTransPic (cx, cy+8, p);
		width -= 2;
		cx += 16;
	}

	// draw right side
	cy = y;
	p = Draw_CachePic ("gfx/box_tr.lmp");
	M_DrawTransPic (cx, cy, p);
	p = Draw_CachePic ("gfx/box_mr.lmp");
	for (n = 0; n < lines; n++)
	{
		cy += 8;
		M_DrawTransPic (cx, cy, p);
	}
	p = Draw_CachePic ("gfx/box_br.lmp");
	M_DrawTransPic (cx, cy+8, p);
}

//=============================================================================

int m_save_demonum;
		
/*
================
M_ToggleMenu_f
================
*/
void M_ToggleMenu_f (void)
{
	m_entersound = true;

	if (key_dest == key_menu)
	{
		if (m_state != m_main)
		{
			M_Menu_Main_f ();
			return;
		}

		key_dest = key_game;
		m_state = m_none;
		return;
	}
	if (key_dest == key_console)
	{
		Con_ToggleConsole_f ();
	}
	else
	{
		Cmd_ChatInfo(2); /* FS: EZQ Chat */
		M_Menu_Main_f ();
	}
}

		
//=============================================================================
/* MAIN MENU */

int     m_main_cursor;
#define MAIN_ITEMS      5


void M_Menu_Main_f (void)
{
	if (key_dest != key_menu)
	{
		m_save_demonum = cls.demonum;
		cls.demonum = -1;
	}

	key_dest = key_menu;
	m_state = m_main;
	m_entersound = true;
}
				

void M_Main_Draw (void)
{
	int	     f;
	qpic_t  *p;

	M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
	p = Draw_CachePic ("gfx/ttl_main.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);
	M_DrawTransPic (72, 32, Draw_CachePic ("gfx/mainmenu.lmp") );

	f = (int)(realtime * 10)%6;
	
	M_DrawTransPic (54, 32 + m_main_cursor * 20,Draw_CachePic( va("gfx/menudot%i.lmp", f+1 ) ) );
	//M_DrawTransPic (54, 32 + m_main_cursor * 20,Draw_CachePic( va("gfx/menudot1.lmp", f+1 ) ) );
}


void M_Main_Key (int key)
{
	switch (key)
	{
	case K_ESCAPE:
		key_dest = key_game;
		m_state = m_none;
		cls.demonum = m_save_demonum;
		if (cls.demonum != -1 && !cls.demoplayback && cls.state == ca_disconnected)
			CL_NextDemo ();
		Cmd_ChatInfo(0); /* FS: EZQ Chat */
		break;
		
	case K_DOWNARROW:
		S_LocalSound ("misc/menu1.wav");
		if (++m_main_cursor >= MAIN_ITEMS)
			m_main_cursor = 0;
		break;

	case K_UPARROW:
		S_LocalSound ("misc/menu1.wav");
		if (--m_main_cursor < 0)
			m_main_cursor = MAIN_ITEMS - 1;
		break;

/* FS: Menu shortcuts */
	case 's':
		M_Menu_SinglePlayer_f ();
		break;
	case 'm':
		M_Menu_MultiPlayer_f ();
		break;
	case 'o':
		M_Menu_Options_f ();
		break;
	case 'h':
		M_Menu_Help_f ();
		break;
	case 'q':
		M_Menu_Quit_f ();
		break;

	case K_ENTER:
		m_entersound = true;

		switch (m_main_cursor)
		{
		case 0:
			M_Menu_SinglePlayer_f ();
			break;

		case 1:
			M_Menu_MultiPlayer_f ();
			break;

		case 2:
			M_Menu_Options_f ();
			break;

		case 3:
			M_Menu_Help_f ();
			break;

		case 4:
			M_Menu_Quit_f ();
			break;
		}
	}
}


//=============================================================================
/* OPTIONS MENU */

#define OPTIONS_ITEMS   16

#define SLIDER_RANGE    10

int	     options_cursor;

void M_Menu_Options_f (void)
{
	key_dest = key_menu;
	m_state = m_options;
	m_entersound = true;
}


void M_AdjustSliders (int dir)
{
	S_LocalSound ("misc/menu3.wav");

	switch (options_cursor)
	{
	case 3: // screen size
		scr_viewsize.value += dir * 10;
		if (scr_viewsize.value < 30)
			scr_viewsize.value = 30;
		if (scr_viewsize.value > 120)
			scr_viewsize.value = 120;
		Cvar_SetValue ("viewsize", scr_viewsize.value);
		break;
	case 4: // gamma
		v_gamma.value -= dir * 0.05;
		if (v_gamma.value < 0.5)
			v_gamma.value = 0.5;
		if (v_gamma.value > 1.0)
			v_gamma.value = 1.0;
		Cvar_SetValue ("gamma", v_gamma.value);
		break;
	case 5: // mouse speed
		sensitivity.value += dir * 0.5;
		if (sensitivity.value < 1)
			sensitivity.value = 1;
		if (sensitivity.value > 50) /* FS: 11 is so 1997. */
			sensitivity.value = 50;
		Cvar_SetValue ("sensitivity", sensitivity.value);
		break;
	case 6: // music volume
		bgmvolume.value += dir * 0.1;
		if (bgmvolume.value < 0)
			bgmvolume.value = 0;
		if (bgmvolume.value > 1)
			bgmvolume.value = 1;
		Cvar_SetValue ("bgmvolume", bgmvolume.value);
		break;
	case 7: // sfx volume
		volume.value += dir * 0.1;
		if (volume.value < 0)
			volume.value = 0;
		if (volume.value > 1)
			volume.value = 1;
		Cvar_SetValue ("volume", volume.value);
		break;
		
	case 8: // allways run
		if (cl_forwardspeed.value > 200)
		{
			Cvar_SetValue ("cl_forwardspeed", 200);
			Cvar_SetValue ("cl_backspeed", 200);
			Cvar_SetValue ("cl_sidespeed", 200); /* FS */
		}
		else
		{
			Cvar_SetValue ("cl_forwardspeed", 400);
			Cvar_SetValue ("cl_backspeed", 400);
			Cvar_SetValue ("cl_sidespeed", 400); /* FS */
		}
		break;
	
	case 9: // invert mouse
		Cvar_SetValue ("m_pitch", -m_pitch.value);
		break;
	
	case 10:	// lookspring
		Cvar_SetValue ("lookspring", !lookspring.value);
		break;
	
	case 11:	// lookstrafe
		Cvar_SetValue ("lookstrafe", !lookstrafe.value);
		break;

	case 12:
		Cvar_SetValue ("cl_sbar", !cl_sbar.value);
		break;

	case 13:
		Cvar_SetValue ("cl_hudswap", !cl_hudswap.value);
		break;  // JASON

	case 14:
	case 15: /* FS: Extended options */
		break;
	}
}


void M_DrawSlider (int x, int y, float range)
{
	int     i;

	if (range < 0)
		range = 0;
	if (range > 1)
		range = 1;
	M_DrawCharacter (x-8, y, 128);
	for (i=0 ; i<SLIDER_RANGE ; i++)
		M_DrawCharacter (x + i*8, y, 129);
	M_DrawCharacter (x+i*8, y, 130);
	M_DrawCharacter (x + (SLIDER_RANGE-1)*8 * range, y, 131);
}

void M_DrawCheckbox (int x, int y, int on)
{
	if (on)
		M_Print (x, y, "on");
	else
		M_Print (x, y, "off");
}

void M_Options_Draw (void)
{
	float	   r;
	qpic_t  *p;
	
	M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
	p = Draw_CachePic ("gfx/p_option.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);
	
	M_Print (16, 32, "    Customize controls");
	M_Print (16, 40, "	 Go to console");
	M_Print (16, 48, "     Reset to defaults");

	M_Print (16, 56, "	   Screen size");
	r = (scr_viewsize.value - 30) / (120 - 30);
	M_DrawSlider (220, 56, r);

	M_Print (16, 64, "	    Brightness");
	r = (1.0 - v_gamma.value) / 0.5;
	M_DrawSlider (220, 64, r);

	M_Print (16, 72, "	   Mouse Speed");
	r = (sensitivity.value)/50; /* FS: Changed from 11 */
	M_DrawSlider (220, 72, r);

	M_Print (16, 80, "       CD Music Volume");
	r = bgmvolume.value;
	M_DrawSlider (220, 80, r);

	M_Print (16, 88, "	  Sound Volume");
	r = volume.value;
	M_DrawSlider (220, 88, r);

	M_Print (16, 96,  "	    Always Run");
	M_DrawCheckbox (220, 96, cl_forwardspeed.value > 200);

	M_Print (16, 104, "	  Invert Mouse");
	M_DrawCheckbox (220, 104, m_pitch.value < 0);

	M_Print (16, 112, "	    Lookspring");
	M_DrawCheckbox (220, 112, lookspring.value);

	M_Print (16, 120, "	    Lookstrafe");
	M_DrawCheckbox (220, 120, lookstrafe.value);

	M_Print (16, 128, "    Use old status bar");
	M_DrawCheckbox (220, 128, cl_sbar.value);

	M_Print (16, 136, "      HUD on left side");
	M_DrawCheckbox (220, 136, cl_hudswap.value);

	if (vid_menudrawfn)
		M_Print (16, 144, "	 Video Options");

	M_Print (16, 152, "      Extended Options"); /* FS: Extended options unique to QDOS */



// cursor
	M_DrawCharacter (200, 32 + options_cursor*8, 12+((int)(realtime*4)&1));
}


void M_Options_Key (int k)
{
	switch (k)
	{
	case K_ESCAPE:
		M_Menu_Main_f ();
		break;

	case 'v': /* FS */
	case 'V':
		M_Menu_Video_f();
		break;

	case 'e': /* FS */
	case 'E':
		M_Menu_Extended_f();
		break;

	case K_ENTER:
		m_entersound = true;
		switch (options_cursor)
		{
		case 0:
			M_Menu_Keys_f ();
			break;
		case 1:
			m_state = m_none;
			Con_ToggleConsole_f ();
			break;
		case 2:
			Cbuf_AddText ("exec default.cfg\n");
			break;
		case 14:
			M_Menu_Video_f ();
			break;
		case 15: /* FS: Extended options unique to QDOS */
			M_Menu_Extended_f ();
			break;
		default:
			M_AdjustSliders (1);
			break;
		}
		return;
	
	case K_UPARROW:
		S_LocalSound ("misc/menu1.wav");
		options_cursor--;
		if (options_cursor < 0)
			options_cursor = OPTIONS_ITEMS-1;
		break;

	case K_DOWNARROW:
		S_LocalSound ("misc/menu1.wav");
		options_cursor++;
		if (options_cursor >= OPTIONS_ITEMS)
			options_cursor = 0;
		break;  

	case K_LEFTARROW:
		M_AdjustSliders (-1);
		break;

	case K_RIGHTARROW:
		M_AdjustSliders (1);
		break;
	}

	/* FS: Needed to readjust and that Use Mouse shit was getting in the way. */
	if (options_cursor == 17 && vid_menudrawfn == NULL)
	{
		if (k == K_UPARROW)
			options_cursor = 16;
		else
			options_cursor = 0;
	}

	if ((options_cursor == 17)) 
	
	{
		if (k == K_UPARROW)
			options_cursor = 16;
		else
			options_cursor = 0;
	}
}


//=============================================================================
/* KEYS MENU */

char *bindnames[][2] =
{
{"+attack",	     "attack"},
{"impulse 10",	  "change weapon"},
{"+jump",		       "jump / swim up"},
{"+forward",	    "walk forward"},
{"+back",		       "backpedal"},
{"+left",		       "turn left"},
{"+right",		      "turn right"},
{"+speed",		      "run"},
{"+moveleft",	   "step left"},
{"+moveright",	  "step right"},
{"+strafe",	     "sidestep"},
{"+lookup",	     "look up"},
{"+lookdown",	   "look down"},
{"centerview",	  "center view"},
{"+mlook",		      "mouse look"},
{"+klook",		      "keyboard look"},
{"+moveup",		     "swim up"},
{"+movedown",	   "swim down"}
};

#define NUMCOMMANDS     (sizeof(bindnames)/sizeof(bindnames[0]))

int	     keys_cursor;
int	     bind_grab;

void M_Menu_Keys_f (void)
{
	key_dest = key_menu;
	m_state = m_keys;
	m_entersound = true;
}


void M_FindKeysForCommand (char *command, int *twokeys)
{
	int	     count;
	int	     j;
	int	     l;
	char    *b;

	twokeys[0] = twokeys[1] = -1;
	l = strlen(command);
	count = 0;

	for (j=0 ; j<256 ; j++)
	{
		b = keybindings[j];
		if (!b)
			continue;
		if (!strncmp (b, command, l) )
		{
			twokeys[count] = j;
			count++;
			if (count == 2)
				break;
		}
	}
}

void M_UnbindCommand (char *command)
{
	int	     j;
	int	     l;
	char    *b;

	l = strlen(command);

	for (j=0 ; j<256 ; j++)
	{
		b = keybindings[j];
		if (!b)
			continue;
		if (!strncmp (b, command, l) )
			Key_SetBinding (j, "");
	}
}


void M_Keys_Draw (void)
{
	int	     i; //, l;
	int	     keys[2];
	char    *name;
	int	     x, y;
	qpic_t  *p;

	p = Draw_CachePic ("gfx/ttl_cstm.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);

	if (bind_grab)
		M_Print (12, 32, "Press a key or button for this action");
	else
		M_Print (18, 32, "Enter to change, backspace to clear");
		
// search for known bindings
	for (i=0 ; i<NUMCOMMANDS ; i++)
	{
		y = 48 + 8*i;

		M_Print (16, y, bindnames[i][1]);

//		l = strlen (bindnames[i][0]);
		
		M_FindKeysForCommand (bindnames[i][0], keys);
		
		if (keys[0] == -1)
		{
			M_Print (140, y, "???");
		}
		else
		{
			name = Key_KeynumToString (keys[0]);
			M_Print (140, y, name);
			x = strlen(name) * 8;
			if (keys[1] != -1)
			{
				M_Print (140 + x + 8, y, "or");
				M_Print (140 + x + 32, y, Key_KeynumToString (keys[1]));
			}
		}
	}
	
	if (bind_grab)
		M_DrawCharacter (130, 48 + keys_cursor*8, '=');
	else
		M_DrawCharacter (130, 48 + keys_cursor*8, 12+((int)(realtime*4)&1));
}


void M_Keys_Key (int k)
{
	char    cmd[80];
	int	     keys[2];
	
	if (bind_grab)
	{       // defining a key
		S_LocalSound ("misc/menu1.wav");
		if (k == K_ESCAPE)
		{
			bind_grab = false;
		}
		else if (k != '`')
		{
			sprintf (cmd, "bind %s \"%s\"\n", Key_KeynumToString (k), bindnames[keys_cursor][0]);		   
			Cbuf_InsertText (cmd);
		}
		
		bind_grab = false;
		return;
	}
	
	switch (k)
	{
	case K_ESCAPE:
		M_Menu_Options_f ();
		break;

	case K_LEFTARROW:
	case K_UPARROW:
		S_LocalSound ("misc/menu1.wav");
		keys_cursor--;
		if (keys_cursor < 0)
			keys_cursor = NUMCOMMANDS-1;
		break;

	case K_DOWNARROW:
	case K_RIGHTARROW:
		S_LocalSound ("misc/menu1.wav");
		keys_cursor++;
		if (keys_cursor >= NUMCOMMANDS)
			keys_cursor = 0;
		break;

	case K_ENTER:	   // go into bind mode
		M_FindKeysForCommand (bindnames[keys_cursor][0], keys);
		S_LocalSound ("misc/menu2.wav");
		if (keys[1] != -1)
			M_UnbindCommand (bindnames[keys_cursor][0]);
		bind_grab = true;
		break;

	case K_BACKSPACE:	       // delete bindings
	case K_DEL:			     // delete bindings
		S_LocalSound ("misc/menu2.wav");
		M_UnbindCommand (bindnames[keys_cursor][0]);
		break;
	}
}

//=============================================================================
/* VIDEO MENU */

void M_Menu_Video_f (void)
{
	key_dest = key_menu;
	m_state = m_video;
	m_entersound = true;
}


void M_Video_Draw (void)
{
	(*vid_menudrawfn) ();
}


void M_Video_Key (int key)
{
	(*vid_menukeyfn) (key);
}

//=============================================================================
/* HELP MENU */

int	     help_page;
#define NUM_HELP_PAGES  6


void M_Menu_Help_f (void)
{
	key_dest = key_menu;
	m_state = m_help;
	m_entersound = true;
	help_page = 0;
}



void M_Help_Draw (void)
{
	M_DrawPic (0, 0, Draw_CachePic ( va("gfx/help%i.lmp", help_page)) );
}


void M_Help_Key (int key)
{
	switch (key)
	{
	case K_ESCAPE:
		M_Menu_Main_f ();
		break;
		
	case K_UPARROW:
	case K_RIGHTARROW:
		m_entersound = true;
		if (++help_page >= NUM_HELP_PAGES)
			help_page = 0;
		break;

	case K_DOWNARROW:
	case K_LEFTARROW:
		m_entersound = true;
		if (--help_page < 0)
			help_page = NUM_HELP_PAGES-1;
		break;
	}

}

//=============================================================================
/* QUIT MENU */

int	     msgNumber;
int	     m_quit_prevstate;
qboolean	wasInMenus;

char *quitMessage [] = 
{
/* .........1.........2.... */
  "  Are you gonna quit    ",
  "  this game just like   ",
  "   everything else?     ",
  "			",
 
  " Milord, methinks that  ",
  "   thou art a lowly     ",
  " quitter. Is this true? ",
  "			",

  " Do I need to bust your ",
  "  face open for trying  ",
  "	to quit?	",
  "			",

  " Man, I oughta smack you",
  "   for trying to quit!  ",
  "     Press Y to get     ",
  "      smacked out.      ",
 
  " Press Y to quit like a ",
  "   big loser in life.   ",
  "  Press N to stay proud ",
  "    and successful!     ",
 
  "   If you press Y to    ",
  "  quit, I will summon   ",
  "  Satan all over your   ",
  "      hard drive!       ",
 
  "  Um, Asmodeus dislikes ",
  " his children trying to ",
  " quit. Press Y to return",
  "   to your Tinkertoys.  ",
 
  "  If you quit now, I'll ",
  "  throw a blanket-party ",
  "   for you next time!   ",
  "			"
};

void M_Menu_Quit_f (void)
{
	if (m_state == m_quit)
		return;
	wasInMenus = (key_dest == key_menu);
	key_dest = key_menu;
	m_quit_prevstate = m_state;
	m_state = m_quit;
	m_entersound = true;
	msgNumber = rand()&7;
}


void M_Quit_Key (int key)
{
	switch (key)
	{
		case K_ESCAPE:
		case 'n':
		case 'N':
			if (wasInMenus)
			{
				m_state = m_quit_prevstate;
				m_entersound = true;
			}
			else
			{
				key_dest = key_game;
				m_state = m_none;
			}
			break;

		case K_ENTER: /* FS: You can press enter to default Y */
		case 'Y':
		case 'y':
			key_dest = key_console;
			CL_Disconnect ();
			Sys_Quit ();
			break;

		default:
			break;
	}
}

void M_Menu_SinglePlayer_f (void) {
	m_state = m_singleplayer;
}

void M_SinglePlayer_Draw (void) {
	qpic_t  *p;

	M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
//      M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
	p = Draw_CachePic ("gfx/ttl_sgl.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);
//      M_DrawTransPic (72, 32, Draw_CachePic ("gfx/sp_menu.lmp") );

	M_DrawTextBox (60, 10*8, 23, 4);	
	M_PrintWhite (92, 12*8, "QuakeWorld is for");
	M_PrintWhite (88, 13*8, "Internet play only");

}

void M_SinglePlayer_Key (key) {
	if (key == K_ESCAPE || key==K_ENTER)
		m_state = m_main;
}

void M_Menu_MultiPlayer_f (void) {
	m_state = m_multiplayer;
}

void M_MultiPlayer_Draw (void) {
	qpic_t  *p;

	M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
//      M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
	p = Draw_CachePic ("gfx/p_multi.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);
//      M_DrawTransPic (72, 32, Draw_CachePic ("gfx/sp_menu.lmp") );

	M_DrawTextBox (46, 8*8, 27, 9); 
	M_PrintWhite (72, 10*8, "If you want to find QW  ");
	M_PrintWhite (72, 11*8, "games, head on over to: ");
	     M_Print (72, 12*8, "   www.quakeworld.net   ");
	M_PrintWhite (72, 13*8, "	  or	    ");
	     M_Print (72, 14*8, "   www.quakespy.com     ");
	M_PrintWhite (72, 15*8, "For pointers on getting ");
	M_PrintWhite (72, 16*8, "	started!	");
}

void M_MultiPlayer_Key (key) {
	if (key == K_ESCAPE || key==K_ENTER)
		m_state = m_main;
}

void M_Quit_Draw (void)
{
#define VSTR(x) #x
#define VSTR2(x) VSTR(x)
	char *cmsg[] = {
//    0123456789012345678901234567890123456789
	"0	    QuakeWorld",
	"1    version " VSTR2(VERSION) " by id Software",
	"0Programming",
	"1 John Carmack    Michael Abrash",
	"1 John Cash       Christian Antkow",
	"0Additional Programming",
	"1 Dave 'Zoid' Kirsch",
	"1 Jack 'morbid' Mathews",
	"0Id Software is not responsible for",
    "0providing technical support for",
	"0QUAKEWORLD(tm). (c)1996 Id Software,",
	"0Inc.  All Rights Reserved.",
	"0QUAKEWORLD(tm) is a trademark of Id",
	"0Software, Inc.",
	"1NOTICE: THE COPYRIGHT AND TRADEMARK",
	"1NOTICES APPEARING  IN YOUR COPY OF",
	"1QUAKE(r) ARE NOT MODIFIED BY THE USE",
	"1OF QUAKEWORLD(tm) AND REMAIN IN FULL",
	"1FORCE.",
	"0NIN(r) is a registered trademark",
	"0licensed to Nothing Interactive, Inc.",
	"0All rights reserved. Press y to exit",
	NULL };
	char **p;
	int y;

	if (wasInMenus)
	{
		m_state = m_quit_prevstate;
		m_recursiveDraw = true;
		M_Draw ();
		m_state = m_quit;
	}
#if 1
	M_DrawTextBox (0, 0, 38, 23);
	y = 12;
	for (p = cmsg; *p; p++, y += 8) {
		if (**p == '0')
			M_PrintWhite (16, y, *p + 1);
		else
			M_Print (16, y, *p + 1);
	}
#else
	M_DrawTextBox (56, 76, 24, 4);
	M_Print (64, 84,  quitMessage[msgNumber*4+0]);
	M_Print (64, 92,  quitMessage[msgNumber*4+1]);
	M_Print (64, 100, quitMessage[msgNumber*4+2]);
	M_Print (64, 108, quitMessage[msgNumber*4+3]);
#endif
}



//=============================================================================
/* Menu Subsystem */


void M_Init (void)
{
	Cmd_AddCommand ("togglemenu", M_ToggleMenu_f);

	Cmd_AddCommand ("menu_main", M_Menu_Main_f);
	Cmd_AddCommand ("menu_options", M_Menu_Options_f);
	Cmd_AddCommand ("menu_keys", M_Menu_Keys_f);
	Cmd_AddCommand ("menu_video", M_Menu_Video_f);
	Cmd_AddCommand ("help", M_Menu_Help_f);
	Cmd_AddCommand ("menu_quit", M_Menu_Quit_f);
	Cmd_AddCommand ("menu_extended", M_Menu_Extended_f); /* FS: Extended options unique to QDOS */
}


void M_Draw (void)
{
	if (m_state == m_none || key_dest != key_menu)
		return;

	if (!m_recursiveDraw)
	{
		scr_copyeverything = 1;

		if (scr_con_current)
		{
			Draw_ConsoleBackground (vid.height);
			VID_UnlockBuffer ();
			S_ExtraUpdate ();
			VID_LockBuffer ();
		}
		else
			Draw_FadeScreen ();

		scr_fullupdate = 0;
	}
	else
	{
		m_recursiveDraw = false;
	}

	switch (m_state)
	{
	case m_none:
		break;

	case m_main:
		M_Main_Draw ();
		break;

	case m_singleplayer:
		M_SinglePlayer_Draw ();
		break;

	case m_load:
//	      M_Load_Draw ();
		break;

	case m_save:
//	      M_Save_Draw ();
		break;

	case m_multiplayer:
		M_MultiPlayer_Draw ();
		break;

	case m_setup:
//	      M_Setup_Draw ();
		break;

	case m_net:
//	      M_Net_Draw ();
		break;

	case m_options:
		M_Options_Draw ();
		break;

	case m_keys:
		M_Keys_Draw ();
		break;

	case m_video:
		M_Video_Draw ();
		break;

	case m_help:
		M_Help_Draw ();
		break;

	case m_quit:
		M_Quit_Draw ();
		break;

	case m_serialconfig:
//	      M_SerialConfig_Draw ();
		break;

	case m_modemconfig:
//	      M_ModemConfig_Draw ();
		break;

	case m_lanconfig:
//	      M_LanConfig_Draw ();
		break;

	case m_gameoptions:
//	      M_GameOptions_Draw ();
		break;

	case m_search:
//	      M_Search_Draw ();
		break;

	case m_slist:
//	      M_ServerList_Draw ();
		break;
	case m_extended:  /* FS: Extended options unique to QDOS */
		M_Extended_Draw ();
		break;	

	case m_gamespy: /* FS: Unfinished */
		break;
	}

	if (m_entersound)
	{
		S_LocalSound ("misc/menu2.wav");
		m_entersound = false;
	}

	VID_UnlockBuffer ();
	S_ExtraUpdate ();
	VID_LockBuffer ();
}

void M_Keydown (int key)
{
	switch (m_state)
	{
	case m_none:
		return;

	case m_main:
		M_Main_Key (key);
		return;

	case m_singleplayer:
		M_SinglePlayer_Key (key);
		return;

	case m_load:
//	      M_Load_Key (key);
		return;

	case m_save:
//	      M_Save_Key (key);
		return;

	case m_multiplayer:
		M_MultiPlayer_Key (key);
		return;

	case m_setup:
//	      M_Setup_Key (key);
		return;

	case m_net:
//	      M_Net_Key (key);
		return;

	case m_options:
		M_Options_Key (key);
		return;

	case m_keys:
		M_Keys_Key (key);
		return;

	case m_video:
		M_Video_Key (key);
		return;

	case m_help:
		M_Help_Key (key);
		return;

	case m_quit:
		M_Quit_Key (key);
		return;

	case m_serialconfig:
//	      M_SerialConfig_Key (key);
		return;

	case m_modemconfig:
//	      M_ModemConfig_Key (key);
		return;

	case m_lanconfig:
//	      M_LanConfig_Key (key);
		return;

	case m_gameoptions:
//	      M_GameOptions_Key (key);
		return;

	case m_search:
//	      M_Search_Key (key);
		break;

	case m_slist:
//	      M_ServerList_Key (key);
		return;
	case m_extended: /* FS: Extended options unique to QDOS */
		M_Extended_Key (key);
		return;

	case m_gamespy: /* FS: Unfinished */
		return;

	}
}

/* FS: Extended options unique to QDOS */
int extended_cursor;
#define EXTENDED_OPTIONS 11
extern cvar_t r_waterwarp;
extern cvar_t scr_fov;

void M_Menu_Extended_f(void)
{
	key_dest = key_menu;
	m_state = m_extended;
	m_entersound = true;

}

void M_Extended_Draw()
{
	float	r;
	qpic_t  *p;

	M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
	p = Draw_CachePic ("gfx/p_option.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);

	M_Print (16, 32, "     Chat Broadcasting");
	M_DrawCheckbox (220, 32, net_broadcast_chat.value);

	M_Print (16, 40, "     Chat Notification");
	M_DrawCheckbox (220, 40, net_showchat.value);

	M_Print (16, 48, "	 Chat Graphics");
	M_DrawCheckbox (220, 48, net_showchatgfx.value);

	M_Print (16, 56,  "      Content Blending");
	M_DrawCheckbox (220, 56, v_contentblend.value);

	M_Print (16, 64,  "     Unbindall Protect");
	M_DrawCheckbox (220, 64, cl_unbindall_protection.value);

	M_Print (16, 72,  "	   Show Uptime");
	M_DrawCheckbox (220, 72, show_uptime.value);

	M_Print (16, 80,  "	     Show Time");
	if (show_time.value < 1 )
		M_Print (220, 80, "off");
	else if (show_time.value == 1)
		M_Print (220, 80, "Military");
	else if (show_time.value >= 2)
		M_Print (220, 80, "AM/PM");

	M_Print (16, 88,  "	Show Framerate");
	M_DrawCheckbox (220, 88, show_fps.value);

	M_Print (16, 96,  "	Mouse Freelook");
	M_DrawCheckbox (220, 96, in_freelook.value);

	M_Print (16,104,  "       Water View-warp");
	M_DrawCheckbox (220, 104, r_waterwarp.value);

	M_Print (16, 112, "       Field of Vision");
	r = (scr_fov.value - 30) / (175 - 30);
	M_DrawSlider (220, 112, r);
	M_DrawCharacter (200, 32 + extended_cursor*8, 12+((int)(realtime*4)&1));

	M_Print (16, 120, "	    Sound Rate");
	if (s_khz.intValue <= 0)
		Cvar_SetValue("s_khz", 11025);
	M_Print (220, 120, s_khz.string);
}

void M_AdjustSliders_Extended (int dir)
{
	switch(extended_cursor)
	{
	case 0:
		Cvar_SetValue ("net_broadcast_chat", !net_broadcast_chat.value);
		break;
	case 1:
		Cvar_SetValue ("net_showchat", !net_showchat.value);
		break;
	case 2:
		Cvar_SetValue ("net_showchatgfx", !net_showchatgfx.value);
		break;
	case 3:
		Cvar_SetValue ("v_contentblend", !v_contentblend.value);
		break;
	case 4:
		Cvar_SetValue ("cl_unbindall_protection", !cl_unbindall_protection.value);
		break;
	case 5:
		Cvar_SetValue ("show_uptime", !show_uptime.value);
		break;
	case 6:
		if (show_time.value >= 2)
			Cvar_SetValue ("show_time", 0);
		else if (show_time.value <= 0)
			Cvar_SetValue ("show_time", 1);
		else if (show_time.value == 1)
			Cvar_SetValue ("show_time", 2);
		break;
	case 7:
		Cvar_SetValue ("show_fps", !show_fps.value);
		break;
	case 8:
		Cvar_SetValue ("in_freelook", !in_freelook.value);
		break;
	case 9:
		Cvar_SetValue ("r_waterwarp", !r_waterwarp.value);
		break;
	case 10:
		scr_fov.value += dir * 5;
		if (scr_fov.value < 0)
			scr_fov.value = 0;
		if (scr_fov.value > 170)
			scr_fov.value = 170;
		Cvar_SetValue ("fov", scr_fov.value);
		break;
	case 11:
		M_Extended_Set_Sound_KHz(dir, s_khz.intValue);
		break;
	default:
		break;
	}
	S_LocalSound ("misc/menu3.wav");
}

void M_Extended_Key(int k)
{
    switch (k)
	{
	case K_UPARROW:
		S_LocalSound ("misc/menu1.wav");
		extended_cursor--;
		if (extended_cursor < 0)
			extended_cursor = EXTENDED_OPTIONS;
		break;

	case K_DOWNARROW:
		S_LocalSound ("misc/menu1.wav");
		extended_cursor++;
		if (extended_cursor > EXTENDED_OPTIONS)
			extended_cursor = 0;
		break;

	case K_ESCAPE:
		M_Menu_Options_f ();
		break;

	case K_LEFTARROW:
		M_AdjustSliders_Extended(-1);
		break;

	case K_RIGHTARROW:
		M_AdjustSliders_Extended(1);
		break;

	case K_ENTER:
		m_entersound = true;

		switch (extended_cursor)
		{
		case 0:
			Cvar_SetValue ("net_broadcast_chat", !net_broadcast_chat.value);
			break;
		case 1:
			Cvar_SetValue ("net_showchat", !net_showchat.value);
			break;
		case 2:
			Cvar_SetValue ("net_showchatgfx", !net_showchatgfx.value);
			break;
		case 3:
			Cvar_SetValue ("v_contentblend", !v_contentblend.value);
			break;
		case 4:
			Cvar_SetValue ("cl_unbindall_protection", !cl_unbindall_protection.value);
			break;
		case 5:
			Cvar_SetValue ("show_uptime", !show_uptime.value);
			break;
		case 6:
			if (show_time.value >= 2)
				Cvar_SetValue ("show_time", 0);
			else if (show_time.value <= 0)
				Cvar_SetValue ("show_time", 1);
			else if (show_time.value == 1)
				Cvar_SetValue ("show_time", 2);
			break;
		case 7:
			Cvar_SetValue ("show_fps", !show_fps.value);
			break;
		case 8:
			Cvar_SetValue ("in_freelook", !in_freelook.value);
			break;
		case 9:
			Cvar_SetValue ("r_waterwarp", !r_waterwarp.value);
			break;
		case 10:
			M_AdjustSliders_Extended(1);
			break;
		case 11:
			M_AdjustSliders_Extended(1);
			break;
		default:
			break;
		}
	}
}

void M_Extended_Set_Sound_KHz (int dir, int khz)
{
	switch(khz)
	{
		case 48000:
			if (dir > 0)
				break;
			else
			{
				Cvar_SetValue("s_khz", 44100);
				Cbuf_AddText("snd_restart\n");
			}
			break;
		case 44100:
			if (dir > 0)
			{
				Cvar_SetValue("s_khz", 48000);
			}
			else
			{
				Cvar_SetValue("s_khz", 22050);
			}
			Cbuf_AddText("snd_restart\n");
			break;
		case 22050:
			if (dir > 0)
			{
				Cvar_SetValue("s_khz", 44100);
			}
			else
			{
				if(havegus)
					Cvar_SetValue("s_khz", 19293);
				else
					Cvar_SetValue("s_khz", 11025);
			}
			Cbuf_AddText("snd_restart\n");
			break;
		case 19293:
			if (dir > 0)
			{
				Cvar_SetValue("s_khz", 22050);
			}
			else
			{
				Cvar_SetValue("s_khz", 11025);
			}
			Cbuf_AddText("snd_restart\n");
			break;
		case 11025:
			if (dir > 0)
			{
				if(havegus)
					Cvar_SetValue("s_khz", 19293);
				else
					Cvar_SetValue("s_khz", 22050);
				Cbuf_AddText("snd_restart\n");
			}
			else
			{
				break;
			}
			break;
		default:
			if (dir > 0)
			{
				if(khz < 11025)
					Cvar_SetValue("s_khz", 11025);
				else if(khz > 11025 && khz < 22050)
				{
					if(havegus)
						Cvar_SetValue("s_khz", 19293);
					else
						Cvar_SetValue("s_khz", 22050);
				}
				else if(khz > 22050 && khz < 44100)
					Cvar_SetValue("s_khz", 44100);
				else if(khz > 48000)
					Cvar_SetValue("s_khz", 48000);
				Cbuf_AddText("snd_restart\n");
			}
			else
			{
				if(khz < 11025)
					Cvar_SetValue("s_khz", 11025);
				else if(khz > 11025 && khz < 22050)
				{
					if(havegus)
						Cvar_SetValue("s_khz", 19293);
					else
						Cvar_SetValue("s_khz", 11025);
				}
				else if(khz > 22050 && khz < 44100)
					Cvar_SetValue("s_khz", 22050);
				else if(khz > 48000)
					Cvar_SetValue("s_khz", 44100);
				Cbuf_AddText("snd_restart\n");
			}
	}
}

/* FS: Gamespy stuff */
#define	NO_SERVER_STRING	"<no server>"
#define MAX_GAMESPY_MENU_SERVERS MAX_SERVERS /* FS: Maximum number of servers to show in the browser */
static char gamespy_server_names[MAX_GAMESPY_MENU_SERVERS][80]; /* FS: GameSpy Browser */
static char gamespy_connect_string[MAX_GAMESPY_MENU_SERVERS][128]; /* FS: GameSpy Browser: Connect string */
static int	m_num_gamespy_servers;
static int	m_num_active_gamespy_servers;
static int	curPageScale;
static int	gspyCurPage;
static int	totalAllowedBrowserPages;
static void JoinGamespyServer_Redraw(int serverscale);

static void ConnectGamespyServerFunc(int key) /* FS: GameSpy Browser Connect Function */
{
	char	buffer[128];
	int		index;

	index = key + curPageScale;

	if ( stricmp( gamespy_server_names[index], NO_SERVER_STRING ) == 0 )
	{
		return;
	}

	if (index >= m_num_gamespy_servers)
	{
		return;
	}

	sprintf (buffer, "%s\n", gamespy_connect_string[index]);
	Cbuf_AddText (buffer);
//	M_ForceMenuOff ();
}

static void FormatGamespyList (void)
{
	int j;
	int skip = 0;

	m_num_gamespy_servers = 0;
	m_num_active_gamespy_servers = 0;

	for (j = 0; j< MAX_SERVERS; j++)
	{
		if (m_num_gamespy_servers == MAX_GAMESPY_MENU_SERVERS)
			break;

		if ((browserList[j].hostname[0] != 0)) /* FS: Only show populated servers first */
		{
			if(browserList[j].curPlayers > 0)
			{
				if(vid.height <= 300) /* FS: Special formatting for low res. */
				{
					char buffer[80];

					DG_strlcpy(gamespy_server_names[m_num_gamespy_servers], browserList[j].hostname, 20);

					if(Q_strlen(browserList[j].hostname) >= 20)
					{
						Q_strcat(gamespy_server_names[m_num_gamespy_servers], "...");
					}

					sprintf(buffer, " [%d] %d/%d", browserList[j].ping, browserList[j].curPlayers, browserList[j].maxPlayers);
					DG_strlcat(gamespy_server_names[m_num_gamespy_servers], buffer, sizeof(gamespy_server_names[m_num_gamespy_servers]));
				}
				else
				{
					sprintf(gamespy_server_names[m_num_gamespy_servers], "%s [%d] %d/%d", browserList[j].hostname, browserList[j].ping, browserList[j].curPlayers, browserList[j].maxPlayers);
				}
				sprintf(gamespy_connect_string[m_num_gamespy_servers], "connect %s:%d", browserList[j].ip, browserList[j].port);
				m_num_gamespy_servers++;
				m_num_active_gamespy_servers++;
			}
		}
		else
		{
			break;
		}
	}

	j = 0;

	for(j = 0; j< MAX_SERVERS; j++)
	{
		if (m_num_gamespy_servers == MAX_GAMESPY_MENU_SERVERS)
			break;

		if ((browserListAll[j].hostname[0] != 0))
		{
			if(vid.height <= 300) /* FS: Special formatting for low res. */
			{
				char buffer[80];

				DG_strlcpy(gamespy_server_names[m_num_gamespy_servers-skip], browserListAll[j].hostname, 20);

				if(Q_strlen(browserListAll[j].hostname) >= 20)
				{
					Q_strcat(gamespy_server_names[m_num_gamespy_servers-skip], "...");
				}

				sprintf(buffer, " [%d] %d/%d", browserListAll[j].ping, browserListAll[j].curPlayers, browserListAll[j].maxPlayers);
				DG_strlcat(gamespy_server_names[m_num_gamespy_servers-skip], buffer, sizeof(gamespy_server_names[m_num_gamespy_servers-skip]));
			}
			else
			{
				sprintf(gamespy_server_names[m_num_gamespy_servers-skip], "%s [%d] %d/%d", browserListAll[j].hostname, browserListAll[j].ping, browserListAll[j].curPlayers, browserListAll[j].maxPlayers);
			}
			sprintf(gamespy_connect_string[m_num_gamespy_servers-skip], "connect %s:%d", browserListAll[j].ip, browserListAll[j].port);
			m_num_gamespy_servers++;
		}
		else
		{
			skip++;
			continue;
		}
	}
}

static void SearchGamespyGames (void)
{
	int		i;

	m_num_gamespy_servers = 0;

	for (i=0 ; i<=MAX_GAMESPY_MENU_SERVERS ; i++)
	{
		strcpy (gamespy_server_names[i], NO_SERVER_STRING);
	}

	// send out info packets
	CL_PingNetServers_f ();
}

static int Get_Vidscale(void)
{
	/* FS: Special function for some what scaling of the server browser depending on video resolution height. */
	if (vid.height <= 240)
		return 18;
	if (vid.height <= 300)
		return 21;
	if (vid.height <= 400)
		return 25;
	if (vid.height <= 480)
		return 30;
	if (vid.height <= 800)
		return 36;
	if (vid.height > 800)
		return 38;
	/* FS: We must have some weirdo mode, so 20 should be OK. */
	return 18;
}

static void JoinGamespyServer_NextPageFunc (void *unused)
{
	int vidscale = Get_Vidscale();
	int serverscale = Get_Vidscale();

	if((gspyCurPage + 1) > totalAllowedBrowserPages)
	{
		return;
	}

	gspyCurPage++;
	serverscale = vidscale*gspyCurPage;

	JoinGamespyServer_Redraw(serverscale);
	curPageScale = serverscale;
}

static void JoinGamespyServer_PrevPageFunc (void *unused)
{
	int vidscale = Get_Vidscale();
	int serverscale = Get_Vidscale();

	if((gspyCurPage - 1) < 0)
	{
		return;
	}

	gspyCurPage--;
	serverscale = (vidscale*gspyCurPage);

	JoinGamespyServer_Redraw(serverscale);
	curPageScale = serverscale;
}

void M_Menu_Gamespy_f(void)
{
	key_dest = key_menu;
	m_state = m_gamespy;
	m_entersound = true;

}

void M_Gamespy_Draw()
{
	qpic_t  *p;

	M_DrawTransPic (16, 4, Draw_CachePic ("gfx/qplaque.lmp") );
	p = Draw_CachePic ("gfx/p_option.lmp");
	M_DrawPic ( (320-p->width)/2, 4, p);

	M_Print (16, 32, "     Query Server List");
//	M_DrawCheckbox (220, 32, net_broadcast_chat.value);
}

static void JoinGamespyServer_Redraw( int serverscale )
{
	int i, vidscale;
	qboolean didBreak = false;

#if 0
	s_joingamespyserver_menu.x = viddef.width * 0.50 - 120;
	s_joingamespyserver_menu.y = viddef.height * 0.50 - 118;
	s_joingamespyserver_menu.nitems = 0;
	s_joingamespyserver_menu.cursor = 0; /* FS: Set the cursor at the top */

	s_joingamespyserver_search_action.generic.type = MTYPE_ACTION;
	s_joingamespyserver_search_action.generic.name	= "Query server list";
	s_joingamespyserver_search_action.generic.flags	= QMF_LEFT_JUSTIFY;
	s_joingamespyserver_search_action.generic.x	= 0;
	s_joingamespyserver_search_action.generic.y	= 0;
	s_joingamespyserver_search_action.generic.callback = SearchGamespyGamesFunc;
	s_joingamespyserver_search_action.generic.statusbar = "search for servers";

	s_joingamespyserver_server_title.generic.type = MTYPE_SEPARATOR;
	s_joingamespyserver_server_title.generic.name = "connect to...";
	s_joingamespyserver_server_title.generic.x    = 80;
	s_joingamespyserver_server_title.generic.y	   = 20;
#endif

	vidscale = Get_Vidscale();

	for (i = 0; i <= MAX_GAMESPY_MENU_SERVERS; i++)
	{
		strcpy (gamespy_server_names[i], NO_SERVER_STRING);
	}

	i = 0;

	for ( i = 0; i < vidscale; i++ )
	{
#if 0
		s_joingamespyserver_server_actions[i].generic.type	= MTYPE_ACTION;
		s_joingamespyserver_server_actions[i].generic.name	= gamespy_server_names[i+serverscale];
		s_joingamespyserver_server_actions[i].generic.flags	= QMF_LEFT_JUSTIFY;
		s_joingamespyserver_server_actions[i].generic.x		= 0;
		s_joingamespyserver_server_actions[i].generic.y		= 30 + i*10;
		s_joingamespyserver_server_actions[i].generic.callback = ConnectGamespyServerFunc;
		s_joingamespyserver_server_actions[i].generic.statusbar = "press ENTER to connect";
#endif
	}

//	Menu_AddItem( &s_joingamespyserver_menu, &s_joingamespyserver_search_action );
//	Menu_AddItem( &s_joingamespyserver_menu, &s_joingamespyserver_server_title );

	i = 0;

	for ( i = 0; i < vidscale; i++ )
	{
		if (i+serverscale > MAX_GAMESPY_MENU_SERVERS)
		{
			didBreak = true;
			break;
		}

//		Menu_AddItem( &s_joingamespyserver_menu, &s_joingamespyserver_server_actions[i] );
	}

#if 0
	s_joingamespyserver_prevpage_action.generic.type = MTYPE_ACTION;
	s_joingamespyserver_prevpage_action.generic.name	= "<Previous Page>";
	s_joingamespyserver_prevpage_action.generic.flags	= QMF_LEFT_JUSTIFY;
	s_joingamespyserver_prevpage_action.generic.x	= 0;
	s_joingamespyserver_prevpage_action.generic.y	= 30 + i*10;
	s_joingamespyserver_prevpage_action.generic.callback = JoinGamespyServer_PrevPageFunc;
	s_joingamespyserver_prevpage_action.generic.statusbar = "continue browsing list";
#endif

	i++;

#if 0
	s_joingamespyserver_nextpage_action.generic.type = MTYPE_ACTION;
	s_joingamespyserver_nextpage_action.generic.name	= "<Next Page>";
	s_joingamespyserver_nextpage_action.generic.flags	= QMF_LEFT_JUSTIFY;
	s_joingamespyserver_nextpage_action.generic.x	= 0;
	s_joingamespyserver_nextpage_action.generic.y	= 30 + i*10;
	s_joingamespyserver_nextpage_action.generic.callback = JoinGamespyServer_NextPageFunc;
	s_joingamespyserver_nextpage_action.generic.statusbar = "continue browsing list";
#endif

	if(serverscale)
	{
//		Menu_AddItem (&s_joingamespyserver_menu, &s_joingamespyserver_prevpage_action );
	}

	if(!didBreak)
	{
//		Menu_AddItem (&s_joingamespyserver_menu, &s_joingamespyserver_nextpage_action );
	}

	FormatGamespyList(); /* FS: Incase we changed resolution or ran slist2 in the console and went back to this menu... */
}
