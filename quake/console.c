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
// console.c

#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#include "quakedef.h"

int 		con_linewidth;

float		con_cursorspeed = 4;

#define		CON_TEXTSIZE	65536 /* FS: Was 16384 */

qboolean 	con_forcedup;		// because no entities to refresh

int			con_totallines;		// total lines in console scrollback
int			con_backscroll;		// lines up from bottom to display
int			con_current;		// where next message will be printed
int			con_x;				// offset in current line for next print
char		*con_text=0;

cvar_t		con_notifytime = {"con_notifytime","3", false, false , "Time (in seconds) a console notification message is displayed."};		//seconds
cvar_t		con_logcenterprint = {"con_logcenterprint", "1", false, false, "Log centerprints to console."}; //johnfitz
cvar_t		timestamp = {"timestamp", "0", false, false, "Enables timestamps.  1 for military format.  2 for AM/PM format."}; /* FS: Timestamp */

char		con_lastcenterstring[1024]; //johnfitz

#define	NUM_CON_TIMES 4
float		con_times[NUM_CON_TIMES];	// realtime time the line was generated
								// for transparent notify lines

int			con_vislines;

qboolean	con_debuglog;

#define		MAXCMDLINE	256
extern	char	key_lines[32][MAXCMDLINE];
extern	int		edit_line;
extern	int		key_linepos;
		

qboolean	con_initialized;

int			con_notifylines;		// scan lines to clear for notify lines

extern void M_Menu_Main_f (void);

/*
================
Con_Quakebar -- johnfitz -- returns a bar of the desired length, but never wider than the console

includes a newline, unless len >= con_linewidth.
================
*/
char *Con_Quakebar (int len)
{
	static char bar[42];
	int i;

	len = MIN(len, sizeof(bar) - 2);
	len = MIN(len, con_linewidth);

	bar[0] = '\35';
	for (i = 1; i < len - 1; i++)
		bar[i] = '\36';
	bar[len-1] = '\37';

	if (len < con_linewidth)
	{
		bar[len] = '\n';
		bar[len+1] = 0;
	}
	else
		bar[len] = 0;

	return bar;
}

/*
================
Con_ToggleConsole_f
================
*/
void Con_ToggleConsole_f (void)
{
	if (key_dest == key_console)
	{
		if (cls.state == ca_connected)
		{
			key_dest = key_game;
			key_lines[edit_line][1] = 0;	// clear any typing
			key_linepos = 1;
		}
		else
		{
			M_Menu_Main_f ();
		}
	}
	else
		key_dest = key_console;
	
	SCR_EndLoadingPlaque ();
	memset (con_times, 0, sizeof(con_times));
}

/*
================
Con_Clear_f
================
*/
void Con_Clear_f (void)
{
	if (con_text)
		Q_memset (con_text, ' ', CON_TEXTSIZE);
}

						
/*
================
Con_ClearNotify
================
*/
void Con_ClearNotify (void)
{
	int		i;
	
	for (i=0 ; i<NUM_CON_TIMES ; i++)
		con_times[i] = 0;
}

						
/*
================
Con_MessageMode_f
================
*/
extern qboolean team_message;

void Con_MessageMode_f (void)
{
	key_dest = key_message;
	team_message = false;
}

						
/*
================
Con_MessageMode2_f
================
*/
void Con_MessageMode2_f (void)
{
	key_dest = key_message;
	team_message = true;
}

						
/*
================
Con_CheckResize

If the line width has changed, reformat the buffer.
================
*/
void Con_CheckResize (void)
{
	int		i, j, width, oldwidth, oldtotallines, numlines, numchars;
	char	tbuf[CON_TEXTSIZE];

	width = (vid.width >> 3) - 2;

	if (width == con_linewidth)
		return;

	if (width < 1)			// video hasn't been initialized yet
	{
		width = 38;
		con_linewidth = width;
		con_totallines = CON_TEXTSIZE / con_linewidth;
		Q_memset (con_text, ' ', CON_TEXTSIZE);
	}
	else
	{
		oldwidth = con_linewidth;
		con_linewidth = width;
		oldtotallines = con_totallines;
		con_totallines = CON_TEXTSIZE / con_linewidth;
		numlines = oldtotallines;

		if (con_totallines < numlines)
			numlines = con_totallines;

		numchars = oldwidth;
	
		if (con_linewidth < numchars)
			numchars = con_linewidth;

		Q_memcpy (tbuf, con_text, CON_TEXTSIZE);
		Q_memset (con_text, ' ', CON_TEXTSIZE);

		for (i=0 ; i<numlines ; i++)
		{
			for (j=0 ; j<numchars ; j++)
			{
				con_text[(con_totallines - 1 - i) * con_linewidth + j] =
						tbuf[((con_current - i + oldtotallines) %
							  oldtotallines) * oldwidth + j];
			}
		}

		Con_ClearNotify ();
	}

	con_backscroll = 0;
	con_current = con_totallines - 1;
}


/*
================
Con_Init
================
*/
void Con_Init (void)
{
#define MAXGAMEDIRLEN	1000
	char	temp[MAXGAMEDIRLEN+1];
	char	*t2 = "/qconsole.log";

	con_debuglog = COM_CheckParm("-condebug");

	if (con_debuglog)
	{
		if (strlen (com_gamedir) < (MAXGAMEDIRLEN - strlen (t2)))
		{
			Com_sprintf (temp, sizeof(temp), "%s%s", com_gamedir, t2);
			unlink (temp);
		}
	}

	con_text = Hunk_AllocName (CON_TEXTSIZE, "context");
	Q_memset (con_text, ' ', CON_TEXTSIZE);
	con_linewidth = -1;
	Con_CheckResize ();
	
	Con_Printf ("Console initialized.\n");

//
// register our commands
//
	Cvar_RegisterVariable (&con_notifytime);
	Cvar_RegisterVariable (&con_logcenterprint); //johnfitz
	Cvar_RegisterVariable (&timestamp);

	Cmd_AddCommand ("toggleconsole", Con_ToggleConsole_f);
	Cmd_AddCommand ("messagemode", Con_MessageMode_f);
	Cmd_AddCommand ("messagemode2", Con_MessageMode2_f);
	Cmd_AddCommand ("clear", Con_Clear_f);
	con_initialized = true;
}


/*
===============
Con_Linefeed
===============
*/
void Con_Linefeed (void)
{
	con_x = 0;
	con_current++;
	Q_memset (&con_text[(con_current%con_totallines)*con_linewidth]
	, ' ', con_linewidth);
}

/*
================
Con_Print

Handles cursor positioning, line wrapping, etc
All console printing must go through this in order to be logged to disk
If no console is visible, the notify window will pop up.
================
*/
void Con_Print (char *txt)
{
	int		y;
	int		c, l;
	static int	cr;
	int		mask;
	
	con_backscroll = 0;

	if (txt[0] == 1)
	{
		mask = 128;		// go to colored text
		S_LocalSound ("misc/talk.wav");
	// play talk wav
		txt++;
	}
	else if (txt[0] == 2)
	{
		mask = 128;		// go to colored text
		txt++;
	}
	else
		mask = 0;


	while ( (c = *txt) )
	{
	// count word length
		for (l=0 ; l< con_linewidth ; l++)
			if ( txt[l] <= ' ')
				break;

	// word wrap
		if (l != con_linewidth && (con_x + l > con_linewidth) )
			con_x = 0;

		txt++;

		if (cr)
		{
			con_current--;
			cr = false;
		}

		
		if (!con_x)
		{
			Con_Linefeed ();
		// mark time for transparent overlay
			if (con_current >= 0)
				con_times[con_current % NUM_CON_TIMES] = realtime;
		}

		switch (c)
		{
		case '\n':
			con_x = 0;
			break;

		case '\r':
			con_x = 0;
			cr = 1;
			break;

		default:	// display character and advance
			y = con_current % con_totallines;
			con_text[y*con_linewidth+con_x] = c | mask;
			con_x++;
			if (con_x >= con_linewidth)
				con_x = 0;
			break;
		}
		
	}
}


/*
================
Con_DebugLog
================
*/
void Con_DebugLog(const char *file, const char *fmt, ...)
{
    va_list argptr; 
	static dstring_t *data;
    int fd;

	if(!data)
		data = dstring_new();
    
    va_start(argptr, fmt);
    dvsprintf(data, fmt, argptr);
    va_end(argptr);
    fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0666);
    write(fd, data->str, strlen(data->str));
    close(fd);
}


/*
================
Con_Printf

Handles cursor positioning, line wrapping, etc
================
*/
void Con_Printf (const char *fmt, ...)
{
	va_list argptr;
	static dstring_t *msg;
#if 0
	static qboolean	inupdate;
#endif

	if (!msg)
		msg = dstring_new ();

	va_start (argptr,fmt);
	dvsprintf (msg,fmt,argptr);
	va_end (argptr);

	/* FS: Timestamp code */
	if(timestamp.intValue > 0)
	{
		struct tm *local;
		time_t utc;
		const char *timefmt;
		char st[80];

		utc = time (NULL);
		local = localtime (&utc);
#ifdef _MSC_VER
		if (timestamp.intValue == 1)
			timefmt = "[%m/%d/%y @ %H:%M:%S %p] ";
		else	timefmt = "[%m/%d/%y @ %I:%M:%S %p] ";
#else
		if (timestamp.intValue == 1)
			timefmt = "[%m/%d/%y @ %k:%M:%S %p] ";
		else	timefmt = "[%m/%d/%y @ %l:%M:%S %p] ";
#endif
		strftime (st, sizeof (st), timefmt, local);
		Sys_Printf("%s", st);

		if(con_initialized)
			Con_Print(st);
	}

// also echo to debugging console
	Sys_Printf("%s",msg->str); // also echo to debugging console

// log all messages to file
	if (con_debuglog)
		Con_DebugLog(va("%s/qconsole.log",com_gamedir), "%s", msg->str);

	if (!con_initialized)
		return;

	if (cls.state == ca_dedicated)
		return;		// no graphics mode

// write it to the scrollable buffer
	Con_Print (msg->str);

// update the screen immediately if the console is displayed
#if 0 /* FS: This makes scrolling painfully slow, and Quake 2 doesn't even use something like this */
	if (cls.state != ca_active)
	{
		// protect against infinite loop if something in SCR_UpdateScreen calls Con_Printf
		if (!inupdate)
		{
			inupdate = true;
			SCR_UpdateScreen ();
			inupdate = false;
		}
	}
#endif
}

/*
================
Con_Warning -- johnfitz -- prints a warning to the console
================
*/
void Con_Warning (const char *fmt, ...)
{
	va_list		argptr;
	static dstring_t *msg;

	if (!msg)
		msg = dstring_new();

	va_start (argptr,fmt);
	dvsprintf (msg,fmt,argptr);
	va_end (argptr);

	Con_SafePrintf ("\x02Warning: ");
	Con_Printf ("%s", msg->str);
}

/*
================
Con_DPrintf

A Con_Printf that only shows up if the "developer" cvar is set
================
*/
void Con_DPrintf (unsigned long developerFlags, const char *fmt, ...)
{
	va_list argptr;
	static dstring_t *msg;
	unsigned long devValue = 0; /* FS: Developer flags */

	if (!developer.value)
		return;			// don't confuse non-developers with techie stuff...

	if (!msg)
		msg = dstring_new();

	devValue = (unsigned long)developer.value;
	
	if (developer.value == 1)
		devValue = 65534;

	if (!(devValue & developerFlags))
		return;

	va_start (argptr,fmt);
	dvsprintf (msg,fmt,argptr);
	va_end (argptr);
	
	Con_Printf ("%s", msg->str);
}


/*
================
Con_CenterPrintf -- johnfitz -- pad each line with spaces to make it appear centered
================
*/
#define MAXPRINTMSG	8192
void Con_CenterPrintf (int linewidth, char *fmt, ...)
{
	va_list	argptr;
	static dstring_t *msg; //the original message
	char line[MAXPRINTMSG]; //one line from the message
	char spaces[21]; //buffer for spaces
	char *src, *dst;
	int	 len, s;

	if (!msg)
		msg = dstring_new();

	va_start (argptr,fmt);
	dvsprintf (msg,fmt,argptr);
	va_end (argptr);

	linewidth = MIN(linewidth, con_linewidth);
	for (src = msg->str; *src; )
	{
		dst = line;
		while (*src && *src != '\n')
			*dst++ = *src++;
		*dst = 0;
		if (*src == '\n')
			src++;

		len = strlen(line);
		if (len < linewidth)
		{
			s = (linewidth-len)/2;
			memset (spaces, ' ', s);
			spaces[s] = 0;
			Con_Printf ("%s%s\n", spaces, line);
		}
		else
			Con_Printf ("%s\n", line);
	}
}

/*
==================
Con_LogCenterPrint -- johnfitz -- echo centerprint message to the console
==================
*/
void Con_LogCenterPrint (char *str)
{
	if (!strcmp(str, con_lastcenterstring))
		return; //ignore duplicates

	if (cl.gametype == GAME_DEATHMATCH && con_logcenterprint.value != 2)
		return; //don't log in deathmatch

	strcpy(con_lastcenterstring, str);

	if (con_logcenterprint.value)
	{
		Con_Printf (Con_Quakebar(40));
		Con_CenterPrintf (40, "%s\n", str);
		Con_Printf (Con_Quakebar(40));
		Con_ClearNotify ();
	}
}
/*
==============================================================================

DRAWING

==============================================================================
*/


/*
================
Con_DrawInput

The input line scrolls horizontally if typing goes beyond the right edge
================
*/
void Con_DrawInput (void)
{
//	int		y;
	int		i;
	char	*text;

	if (key_dest != key_console && !con_forcedup)
		return;		// don't draw anything

	text = key_lines[edit_line];
	
// add the cursor frame
	text[key_linepos] = 10+((int)(realtime*con_cursorspeed)&1);
	
// fill out remainder with spaces
	for (i=key_linepos+1 ; i< con_linewidth ; i++)
		text[i] = ' ';
		
//	prestep if horizontally scrolling
	if (key_linepos >= con_linewidth)
		text += 1 + key_linepos - con_linewidth;
		
// draw it
//	y = con_vislines-16;

	for (i=0 ; i<con_linewidth ; i++)
		Draw_Character ( (i+1)<<3, con_vislines - 16, text[i]);

// remove cursor
	key_lines[edit_line][key_linepos] = 0;
}


/*
================
Con_DrawNotify

Draws the last few lines of output transparently over the game top
================
*/
void Con_DrawNotify (void)
{
	int		x, v;
	char	*text;
	int		i;
	float	time;
	extern char chat_buffer[];

	v = 0;
	for (i= con_current-NUM_CON_TIMES+1 ; i<=con_current ; i++)
	{
		if (i < 0)
			continue;
		time = con_times[i % NUM_CON_TIMES];
		if (time == 0)
			continue;
		time = realtime - time;
		if (time > con_notifytime.value)
			continue;
		text = con_text + (i % con_totallines)*con_linewidth;
		
		clearnotify = 0;
		scr_copytop = 1;

		for (x = 0 ; x < con_linewidth ; x++)
			Draw_Character ( (x+1)<<3, v, text[x]);

		v += 8;
	}


	if (key_dest == key_message)
	{
		clearnotify = 0;
		scr_copytop = 1;
	
		x = 0;
		
		Draw_String (8, v, "say:");
		while(chat_buffer[x])
		{
			Draw_Character ( (x+5)<<3, v, chat_buffer[x]);
			x++;
		}
		Draw_Character ( (x+5)<<3, v, 10+((int)(realtime*con_cursorspeed)&1));
		v += 8;
	}
	
	if (v > con_notifylines)
		con_notifylines = v;
}

/*
================
Con_DrawConsole

Draws the console with the solid background
The typing input line at the bottom should only be drawn if typing is allowed
================
*/
void Con_DrawConsole (int lines, qboolean drawinput)
{
	int				i, x, y;
	int				rows;
	char			*text;
	int				j, n;
	char			dlbar[1024];
	
	if (lines <= 0)
		return;

// draw the background
	Draw_ConsoleBackground (lines);

// draw the text
	con_vislines = lines;

	rows = (lines-16)>>3;		// rows of text to draw
	y = lines - 16 - (rows<<3);	// may start slightly negative
   
	if (con_current - con_backscroll != con_current) /* FS: Backscroll thingy from QW */
	{
		// draw arrows to show the buffer is backscrolled
		for (x=0; x<con_linewidth-1; x+=4)
			Draw_Character ( (x+1)<<3, lines-30, '^' );

		y -= 8;
		rows--;
	}
     
	for (i= con_current - rows + 1 ; i<=con_current ; i++, y+=8 )
	{
		j = i - con_backscroll;
		if (j<0)
			j = 0;
		text = con_text + (j % con_totallines)*con_linewidth;

		for (x=0 ; x<con_linewidth ; x++)
			Draw_Character ( (x+1)<<3, y, text[x]);
	}

	if (cls.gamespyupdate) /* FS: Gamespy stuff */
	{
		text = "Gamespy";

		x = con_linewidth - ((con_linewidth * 7) / 40);
		y = x - strlen(text) - 8;
		i = con_linewidth/3;
		if (strlen(text) > i)
		{
			y = x - i - 11;
			strncpy(dlbar, text, i);
			dlbar[i] = 0;
			strcat(dlbar, "...");
		}
		else
			strcpy(dlbar, text);
		strcat(dlbar, ": ");

		i = strlen(dlbar);
		dlbar[i++] = '\x80';
		// where's the dot go?
		if (cls.gamespypercent == 0)
			n = 0;
		else
			n = y * cls.gamespypercent / 100;

		for (j = 0; j < y; j++)
			if (j == n)
				dlbar[i++] = '\x83';
			else
				dlbar[i++] = '\x81';
		dlbar[i++] = '\x82';
		dlbar[i] = 0;

		sprintf(dlbar + strlen(dlbar), " %02d%%", cls.gamespypercent);

		// draw it
		y = con_vislines-22 + 8;
		for (i = 0; i < strlen(dlbar); i++)
			Draw_Character ( (i+1)<<3, y, dlbar[i]);
	}

// draw the input prompt, user text, and cursor if desired
	if (drawinput)
		Con_DrawInput ();
}

/*
==================
Con_NotifyBox
==================
*/
void Con_NotifyBox (char *text)
{
	double		t1, t2;

// during startup for sound / cd warnings
	Con_Printf("\n\n\35\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\37\n");

	Con_Printf (text);

	Con_Printf ("Press a key.\n");
	Con_Printf("\35\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\36\37\n");

	key_count = -2;		// wait for a key down and up
	key_dest = key_console;

	do
	{
		t1 = Sys_FloatTime ();
		SCR_UpdateScreen ();
		Sys_SendKeyEvents ();
		t2 = Sys_FloatTime ();
		realtime += t2-t1;		// make the cursor blink
	} while (key_count < 0);

	Con_Printf ("\n");
	key_dest = key_game;
	realtime = 0;				// put the cursor back to invisible
}


/*
==================
Con_SafePrintf

Okay to call even when the screen can't be updated
==================
*/
void Con_SafePrintf (const char *fmt, ...)
{
	va_list				argptr;
	int					temp;
	static dstring_t	*msg;
	
	if(!msg)
		msg = dstring_new();

	va_start (argptr,fmt);
	dvsprintf (msg,fmt,argptr);
	va_end (argptr);
	
	temp = scr_disabled_for_loading;
	scr_disabled_for_loading = true;

	Con_Printf ("%s", msg->str);

	scr_disabled_for_loading = temp;
}
