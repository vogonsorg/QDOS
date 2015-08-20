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
// host.c -- coordinates spawning and killing of local servers

#include <ctype.h>
#include "quakedef.h"
#include "r_local.h"
#include "cfgfile.h" /* FS: Parse CFG early -- sezero */

/*

A server can allways be started, even if the system started out as a client
to a remote system.

A client can NOT be started if the system started as a dedicated server.

Memory is cleared / released when a server or client begins, not when they end.

*/

quakeparms_t host_parms;

qboolean host_initialized;    // true if into command execution

double      host_frametime;
double      host_time;
double      realtime;            // without any filtering or bounding
double      oldrealtime;         // last frame run
int         host_framecount;
int			fps_count; /* FS: for show_fps */

int         host_hunklevel;

int         minimum_memory;

client_t *host_client;        // current client

jmp_buf  host_abortserver;

byte     *host_basepal;
byte     *host_colormap;

cvar_t	host_framerate = {"host_framerate","0"};  // set for slow motion
cvar_t	host_speeds = {"host_speeds","0"};        // set for running times
cvar_t	cl_maxfps = {"cl_maxfps", "72.0", true, false, "Maximum frames pers second to render."}; /* FS: Technically it was host_maxfps, but cl_maxfps is standard in other Quake games */ //johnfitz
cvar_t	host_timescale = {"host_timescale", "0"}; //johnfitz
cvar_t	max_edicts = {"max_edicts", "2048", false, false, "Maximum number of edicts allowed."}; //johnfitz

cvar_t	sys_ticrate = {"sys_ticrate","0.05"};
cvar_t	serverprofile = {"serverprofile","0"};

cvar_t	fraglimit = {"fraglimit","0",false,true, "Fraglimit in a deathmatch game."};
cvar_t	timelimit = {"timelimit","0",false,true, "Timelimit in a deathmatch game."};
cvar_t	teamplay = {"teamplay","0",false,true, "Enable team deathmatch."};

cvar_t	samelevel = {"samelevel","0", false, false, "Repeats the same level if an endlevel is triggered."};
cvar_t	noexit = {"noexit","0", false, true, "Do not allow exiting in a game."};

cvar_t	developer = {"developer","0", false, false, "Enable the use of developer messages. \nAvailable flags:\n  * All flags except verbose msgs - 1\n  * Standard msgs - 2\n  * Sound msgs - 4\n  * Network msgs - 8\n  * File IO msgs - 16\n  * Graphics renderer msgs - 32\n  * CD Player msgs - 64\n  * Memory management msgs - 128\n  * Server msgs - 256\n  * Progs msgs - 512\n  * Physics msgs - 2048\n  * Entity msgs - 16384\n  * Save/Restore msgs - 32768\n  * Extremely verbose msgs - 65536\n"};

cvar_t	skill = {"skill","1", false, false, "Sets the skill.  Valid values are 0 through 3."};                 // 0 - 3
cvar_t	deathmatch = {"deathmatch","0", false, false, "Enable a deathmatch game.  Coop must be set to 0."};       // 0, 1, or 2
cvar_t	coop = {"coop","0", false, false, "Enable a coop game.  Deathmatch must be set to 0."};       // 0 or 1

cvar_t	pausable = {"pausable","1"};

cvar_t	temp1 = {"temp1","0"};

/* FS: New stuff */
cvar_t	con_show_description = {"con_show_description", "1", true, false, "Show descriptions for CVARs."};
cvar_t	con_show_dev_flags = {"con_show_dev_flags", "1", true, false, "Show developer flag options."};

/*
================
Max_Edicts_f -- johnfitz
================
*/
void Max_Edicts_f (void)
{
	static float oldval = 1024; //must match the default value for max_edicts

	//TODO: clamp it here?

	if (max_edicts.value == oldval)
		return;

	if (cls.state == ca_connected || sv.active)
		Con_Printf ("changes will not take effect until the next level load.\n");

	oldval = max_edicts.value;
}

/*
================
Host_EndGame
================
*/
void Host_EndGame (const char *message, ...)
{
	va_list     argptr;
	static dstring_t *string;

	if (!string)
		string = dstring_new();

	va_start (argptr,message);
	dvsprintf (string,message,argptr);
	va_end (argptr);
	Con_DPrintf (DEVELOPER_MSG_NET, "Host_EndGame: %s\n",string->str);

	if (sv.active)
		Host_ShutdownServer (false);

	if (cls.state == ca_dedicated)
		Sys_Error ("Host_EndGame: %s\n",string->str);  // dedicated servers exit

	if (cls.demonum != -1)
		CL_NextDemo ();
	else
		CL_Disconnect ();

	longjmp (host_abortserver, 1);
}

/*
================
Host_Error

This shuts down both the client and server
================
*/
void Host_Error (const char *error, ...)
{
	va_list     argptr;
	static dstring_t *string;
	static   qboolean inerror = false;

	if (!string)
		string = dstring_new();
   
	if (inerror)
		Sys_Error ("Host_Error: recursively entered");
	inerror = true;

	SCR_EndLoadingPlaque ();      // reenable screen updates

	va_start (argptr,error);
	dvsprintf (string,error,argptr);
	va_end (argptr);
	Con_Printf ("Host_Error: %s\n",string->str);

	if (sv.active)
		Host_ShutdownServer (false);

	if (cls.state == ca_dedicated)
		Sys_Error ("Host_Error: %s\n",string->str); // dedicated servers exit

	CL_Disconnect ();
	cls.demonum = -1;
	cl.intermission = 0; //johnfitz -- for errors during intermissions (changelevel with no map found, etc.)

	inerror = false;

	longjmp (host_abortserver, 1);
}

/*
================
Host_FindMaxClients
================
*/
void  Host_FindMaxClients (void)
{
   int      i;

   svs.maxclients = 1;
      
   i = COM_CheckParm ("-dedicated");
   if (i)
   {
      cls.state = ca_dedicated;
      if (i != (com_argc - 1))
      {
         svs.maxclients = Q_atoi (com_argv[i+1]);
      }
      else
         svs.maxclients = 8;
   }
   else
      cls.state = ca_disconnected;

   i = COM_CheckParm ("-listen");
   if (i)
   {
      if (cls.state == ca_dedicated)
         Sys_Error ("Only one of -dedicated or -listen can be specified");
      if (i != (com_argc - 1))
         svs.maxclients = Q_atoi (com_argv[i+1]);
      else
         svs.maxclients = 8;
   }
   if (svs.maxclients < 1)
      svs.maxclients = 8;
   else if (svs.maxclients > MAX_SCOREBOARD)
      svs.maxclients = MAX_SCOREBOARD;

   svs.maxclientslimit = svs.maxclients;
   if (svs.maxclientslimit < 4)
      svs.maxclientslimit = 4;
   svs.clients = Hunk_AllocName (svs.maxclientslimit*sizeof(client_t), "clients");

   if (svs.maxclients > 1)
      Cvar_SetValue ("deathmatch", 1.0);
   else
      Cvar_SetValue ("deathmatch", 0.0);
}


/*
=======================
Host_InitLocal
======================
*/
void Host_InitLocal (void)
{
	Host_InitCommands ();
   
	Cvar_RegisterVariable (&host_framerate);
	Cvar_RegisterVariable (&host_speeds);
	Cvar_RegisterVariable (&cl_maxfps); /* FS: Technically it was host_maxfps, but cl_maxfps is standard in other Quake games */ //johnfitz
	Cvar_RegisterVariable (&host_timescale); //johnfitz
	Cvar_RegisterVariable (&sys_ticrate);
	Cvar_RegisterVariable (&serverprofile);

	Cvar_RegisterVariable (&max_edicts); //johnfitz

	Cvar_RegisterVariable (&fraglimit);
	Cvar_RegisterVariable (&timelimit);
	Cvar_RegisterVariable (&teamplay);
	Cvar_RegisterVariable (&samelevel);
	Cvar_RegisterVariable (&noexit);
	Cvar_RegisterVariable (&skill);
	Cvar_RegisterVariable (&developer);
	Cvar_RegisterVariable (&deathmatch);
	Cvar_RegisterVariable (&coop);

	Cvar_RegisterVariable (&pausable);

	Cvar_RegisterVariable (&temp1);

	/* FS: New stuff */
	Cvar_RegisterVariable(&con_show_description);
	Cvar_RegisterVariable(&con_show_dev_flags);

	Host_FindMaxClients ();
   
	host_time = 1.0;     // so a think at time 0 won't get called
}

/*
===============
Host_WriteConfiguration

Writes key bindings and archived cvars to config.cfg
===============
*/
extern qboolean   isDedicated;

/* FS: There is some slight stupidity in quake.rc executing config.cfg so a hack would
       be needed for allowing alternate non-default configs.
	   I would prefer such things so we don't stomp over other ports that rely on
	   config.cfg and I'm not a fan of bundling new files or pak files if we don't
	   have to.
*/
// Knightmare- this now takes cfgname as a parameter
void Host_WriteConfiguration (char *cfgName)
{
	FILE	*f;
	char	path[MAX_QPATH];

// dedicated servers initialize the host but don't parse and set the
// config.cfg cvars
	if (host_initialized & !isDedicated)
	{
		if(!(cfgName) || (cfgName[0] == 0)) /* FS: Sanity check */
			Com_sprintf (path, sizeof(path),"%s/config.cfg", com_gamedir);
		else
			Com_sprintf (path, sizeof(path),"%s/%s.cfg", com_gamedir, cfgName);
		f = fopen (path, "w");
		if (!f)
		{
			Con_Printf ("Couldn't write %s.cfg.\n", cfgName);
			return;
		}

		fprintf (f, "// This file is generated by QDOS, do not modify.\n");
		fprintf (f, "// Use autoexec.cfg for adding custom settings.\n");

		Key_WriteBindings (f);
		Cvar_WriteVariables (f);

		fclose (f);
	}
}

/*
=================
SV_ClientPrintf

Sends text across to be displayed 
FIXME: make this just a stuffed echo?
=================
*/
void SV_ClientPrintf (const char *fmt, ...)
{
	va_list	argptr;
	static	dstring_t	*string;

	if (!string)
		string = dstring_new();

	va_start (argptr,fmt);
	dvsprintf (string, fmt,argptr);
	va_end (argptr);
   
	MSG_WriteByte (&host_client->message, svc_print);
	MSG_WriteString (&host_client->message, string->str);
}

/*
=================
SV_BroadcastPrintf

Sends text to all active clients
=================
*/
void SV_BroadcastPrintf (const char *fmt, ...)
{
	va_list	argptr;
	static	dstring_t *string;
	int		i;
   
	if(!string)
		string = dstring_new();

	va_start (argptr,fmt);
	dvsprintf (string, fmt,argptr);
	va_end (argptr);
   
	for (i=0 ; i<svs.maxclients ; i++)
	{
		if (svs.clients[i].active && svs.clients[i].spawned)
		{
			MSG_WriteByte (&svs.clients[i].message, svc_print);
			MSG_WriteString (&svs.clients[i].message, string->str);
		}
	}
}

/*
=================
Host_ClientCommands

Send text over to the client to be executed
=================
*/
void Host_ClientCommands (const char *fmt, ...)
{
	va_list	argptr;
	static	dstring_t *string;

	if(!string)
		string = dstring_new();
   
	va_start (argptr,fmt);
	dvsprintf (string, fmt,argptr);
	va_end (argptr);
   
	MSG_WriteByte (&host_client->message, svc_stufftext);
	MSG_WriteString (&host_client->message, string->str);
}

/*
=====================
SV_DropClient

Called when the player is getting totally kicked off the host
if (crash = true), don't bother sending signofs
=====================
*/
void SV_DropClient (qboolean crash)
{
   int      saveSelf;
   int      i;
   client_t *client;

   if (!crash)
   {
      // send any final messages (don't check for errors)
      if (NET_CanSendMessage (host_client->netconnection))
      {
         MSG_WriteByte (&host_client->message, svc_disconnect);
         NET_SendMessage (host_client->netconnection, &host_client->message);
      }
   
      if (host_client->edict && host_client->spawned)
      {
      // call the prog function for removing a client
      // this will set the body to a dead frame, among other things
         saveSelf = pr_global_struct->self;
         pr_global_struct->self = EDICT_TO_PROG(host_client->edict);
         PR_ExecuteProgram (pr_global_struct->ClientDisconnect);
         pr_global_struct->self = saveSelf;
      }

      Sys_Printf ("Client %s removed\n",host_client->name);
   }

// break the net connection
   NET_Close (host_client->netconnection);
   host_client->netconnection = NULL;

// free the client (the body stays around)
   host_client->active = false;
   host_client->name[0] = 0;
   host_client->old_frags = -999999;
   net_activeconnections--;

// send notification to all clients
   for (i=0, client = svs.clients ; i<svs.maxclients ; i++, client++)
   {
      if (!client->active)
         continue;
      MSG_WriteByte (&client->message, svc_updatename);
      MSG_WriteByte (&client->message, host_client - svs.clients);
      MSG_WriteString (&client->message, "");
      MSG_WriteByte (&client->message, svc_updatefrags);
      MSG_WriteByte (&client->message, host_client - svs.clients);
      MSG_WriteShort (&client->message, 0);
      MSG_WriteByte (&client->message, svc_updatecolors);
      MSG_WriteByte (&client->message, host_client - svs.clients);
      MSG_WriteByte (&client->message, 0);
   }
}

/*
==================
Host_ShutdownServer

This only happens at the end of a game, not between levels
==================
*/
void Host_ShutdownServer(qboolean crash)
{
   int      i;
   int      count;
   sizebuf_t   buf;
   char     message[4];
   double   start;

   if (!sv.active)
      return;

   sv.active = false;

// stop all client sounds immediately
   if (cls.state == ca_connected)
      CL_Disconnect ();

// flush any pending messages - like the score!!!
   start = Sys_FloatTime();
   do
   {
      count = 0;
      for (i=0, host_client = svs.clients ; i<svs.maxclients ; i++, host_client++)
      {
         if (host_client->active && host_client->message.cursize)
         {
            if (NET_CanSendMessage (host_client->netconnection))
            {
               NET_SendMessage(host_client->netconnection, &host_client->message);
               SZ_Clear (&host_client->message);
            }
            else
            {
               NET_GetMessage(host_client->netconnection);
               count++;
            }
         }
      }
      if ((Sys_FloatTime() - start) > 3.0)
         break;
   }
   while (count);

// make sure all the clients know we're disconnecting
   buf.data = (byte *)message;
   buf.maxsize = 4;
   buf.cursize = 0;
   MSG_WriteByte(&buf, svc_disconnect);
   count = NET_SendToAll(&buf, 5.0);
   if (count)
      Con_Printf("Host_ShutdownServer: NET_SendToAll failed for %u clients\n", count);

   for (i=0, host_client = svs.clients ; i<svs.maxclients ; i++, host_client++)
      if (host_client->active)
         SV_DropClient(crash);

//
// clear structures
//
   memset (&sv, 0, sizeof(sv));
   memset (svs.clients, 0, svs.maxclientslimit*sizeof(client_t));
}


/*
================
Host_ClearMemory

This clears all the memory used by both the client and server, but does
not reinitialize anything.
================
*/
void Host_ClearMemory (void)
{
	Con_DPrintf (DEVELOPER_MSG_MEM, "Clearing memory\n");
	D_FlushCaches ();
	Mod_ClearAll ();
#ifndef GLQUAKE
	R_ClearDynamic(); /* FS */
#endif

	if (host_hunklevel)
		Hunk_FreeToLowMark (host_hunklevel);

	cls.signon = 0;
	memset (&sv, 0, sizeof(sv));
	memset (&cl, 0, sizeof(cl));
}


//============================================================================


/*
===================
Host_FilterTime

Returns false if the time is too short to run a frame
===================
*/
qboolean Host_FilterTime (float time)
{
	float maxfps; //johnfitz

	realtime += time;

	//johnfitz -- max fps cvar
	maxfps = CLAMP (10.0, cl_maxfps.value, 1000.0); /* FS: Technically it was host_maxfps, but cl_maxfps is standard in other Quake games */
	if (!cls.timedemo && realtime - oldrealtime < 1.0/maxfps)
		return false; // framerate is too high
	//johnfitz

	host_frametime = realtime - oldrealtime;
	oldrealtime = realtime;

	//johnfitz -- host_timescale is more intuitive than host_framerate
	if (host_timescale.value > 0)
		host_frametime *= host_timescale.value;
	//johnfitz
	else if (host_framerate.value > 0)
		host_frametime = host_framerate.value;
	else // don't allow really long or short frames
		host_frametime = CLAMP (0.001, host_frametime, 0.1); //johnfitz -- use CLAMP

	return true;
}

/*
===================
Host_GetConsoleCommands

Add them exactly as if they had been typed at the console
===================
*/
void Host_GetConsoleCommands (void)
{
   char  *cmd;

   while (1)
   {
      cmd = Sys_ConsoleInput ();
      if (!cmd)
         break;
      Cbuf_AddText (cmd);
   }
}


/*
==================
Host_ServerFrame

==================
*/
#ifdef FPS_20

void _Host_ServerFrame (void)
{
// run the world state  
   pr_global_struct->frametime = host_frametime;

// read client messages
   SV_RunClients ();
   
// move things around and think
// always pause in single player if in console or menus
   if (!sv.paused && (svs.maxclients > 1 || key_dest == key_game) )
      SV_Physics ();
}

void Host_ServerFrame (void)
{
   float save_host_frametime;
   float temp_host_frametime;

// run the world state  
   pr_global_struct->frametime = host_frametime;

// set the time and clear the general datagram
   SV_ClearDatagram ();
   
// check for new clients
   SV_CheckForNewClients ();

   temp_host_frametime = save_host_frametime = host_frametime;
   while(temp_host_frametime > (1.0/72.0))
   {
      if (temp_host_frametime > 0.05)
         host_frametime = 0.05;
      else
         host_frametime = temp_host_frametime;
      temp_host_frametime -= host_frametime;
      _Host_ServerFrame ();
   }
   host_frametime = save_host_frametime;

// send all messages to the clients
   SV_SendClientMessages ();
}

#else

void Host_ServerFrame (void)
{
// run the world state  
   pr_global_struct->frametime = host_frametime;

// set the time and clear the general datagram
   SV_ClearDatagram ();
   
// check for new clients
   SV_CheckForNewClients ();

// read client messages
   SV_RunClients ();
   
// move things around and think
// always pause in single player if in console or menus
   if (!sv.paused && (svs.maxclients > 1 || key_dest == key_game) )
      SV_Physics ();

// send all messages to the clients
   SV_SendClientMessages ();
}

#endif


/*
==================
Host_Frame

Runs all active servers
==================
*/
void _Host_Frame (float time)
{
	static double	time1 = 0;
	static double	time2 = 0;
	static double	time3 = 0;
	int				pass1, pass2, pass3;

	if (setjmp (host_abortserver) )
		return;        // something bad happened, or the server disconnected

// keep the random time dependent
	rand ();

// decide the simulation time
	if (!Host_FilterTime (time))
		return;        // don't run too fast, or packets will flood out

	/*FS: Gamespy Stuff*/
	GameSpy_Async_Think();

// get new key events
	Sys_SendKeyEvents ();

// allow mice or other external controllers to add commands
	IN_Commands ();

// process console commands
	Cbuf_Execute ();

	NET_Poll();

// if running the server locally, make intentions now
	if (sv.active)
		CL_SendCmd ();
   
//-------------------
//
// server operations
//
//-------------------

// check for commands typed to the host
	Host_GetConsoleCommands ();
   
	if (sv.active)
		Host_ServerFrame ();

//-------------------
//
// client operations
//
//-------------------

// if running the server remotely, send intentions now after
// the incoming messages have been read
	if (!sv.active)
		CL_SendCmd ();

	host_time += host_frametime;

// fetch results from server
	if (cls.state == ca_connected)
	{
		CL_ReadFromServer ();
	}

// update video
	if (host_speeds.value)
		time1 = Sys_FloatTime ();
      
	SCR_UpdateScreen ();

	if (host_speeds.value)
		time2 = Sys_FloatTime ();
      
// update audio
	if (cls.signon == SIGNONS)
	{
		S_Update (r_origin, vpn, vright, vup);
		CL_DecayLights ();
	}
	else
		S_Update (vec3_origin, vec3_origin, vec3_origin, vec3_origin);
   
	CDAudio_Update();

	if (host_speeds.value)
	{
		pass1 = (time1 - time3)*1000;
		time3 = Sys_FloatTime ();
		pass2 = (time2 - time1)*1000;
		pass3 = (time3 - time2)*1000;
		Con_Printf ("%3i tot %3i server %3i gfx %3i snd\n",
					pass1+pass2+pass3, pass1, pass2, pass3);
	}
   
	host_framecount++;
	fps_count++; /* FS: for show_fps */
}

void Host_Frame (float time)
{
   double   time1, time2;
   static double  timetotal;
   static int     timecount;
   int      i, c, m;

   if (!serverprofile.value)
   {
      _Host_Frame (time);
      return;
   }
   
   time1 = Sys_FloatTime ();
   _Host_Frame (time);
   time2 = Sys_FloatTime ();  
   
   timetotal += time2 - time1;
   timecount++;
   
   if (timecount < 1000)
      return;

   m = timetotal*1000/timecount;
   timecount = 0;
   timetotal = 0;
   c = 0;
   for (i=0 ; i<svs.maxclients ; i++)
   {
      if (svs.clients[i].active)
         c++;
   }

   Con_Printf ("serverprofile: %2i clients %2i msec\n",  c,  m);
}

//============================================================================

/*
====================
Host_Init
====================
*/
void Host_Init (quakeparms_t *parms)
{
	if (standard_quake)
		minimum_memory = MINIMUM_MEMORY;
	else
		minimum_memory = MINIMUM_MEMORY_LEVELPAK;

	if (COM_CheckParm ("-minmemory"))
		parms->memsize = minimum_memory;

	host_parms = *parms;

	if (parms->memsize < minimum_memory)
		Sys_Error ("Only %4.1f megs of memory available, can't execute game", parms->memsize / (float)0x100000);

	com_argc = parms->argc;
	com_argv = parms->argv;

	Memory_Init (parms->membase, parms->memsize);
	Cbuf_Init ();
	Cmd_Init ();   
	V_Init ();
	Chase_Init ();
	COM_Init (parms->basedir);
	Cvar_Init();
	CFG_OpenConfig("config.cfg"); /* FS: Parse CFG early -- sezero */
	Host_InitLocal ();
	W_LoadWadFile ("gfx.wad");
	Key_Init ();
	Con_Init ();   
	M_Init ();  
	PR_Init ();
	Mod_Init ();
	NET_Init ();
	SV_Init ();

	Con_Printf ("Exe: "__TIME__" "__DATE__"\n");
	Con_Printf ("%4.1f megabyte heap\n",parms->memsize/ (1024*1024.0));
   
	R_InitTextures ();      // needed even for dedicated servers
 
	if (cls.state != ca_dedicated)
	{
		host_basepal = (byte *)COM_LoadHunkFile ("gfx/palette.lmp");
		if (!host_basepal)
			Sys_Error ("Couldn't load gfx/palette.lmp");

		host_colormap = (byte *)COM_LoadHunkFile ("gfx/colormap.lmp");
		if (!host_colormap)
			Sys_Error ("Couldn't load gfx/colormap.lmp");

#ifndef _WIN32 // on non win32, mouse comes before video for security reasons
		IN_Init ();
#endif
		VID_Init (host_basepal);
		Draw_Init ();
		SCR_Init ();
		R_Init ();
#ifndef _WIN32 /* FS: Tired of warnings about things already registered.  See vid_win.c */
		S_Init ();
#else
#ifdef	GLQUAKE
		S_Init ();
#endif
#endif	// _WIN32
		CDAudio_Init ();
		Sbar_Init ();
		CL_Init ();
#ifdef _WIN32 // on non win32, mouse comes before video for security reasons
		IN_Init ();
#endif

		if(COM_CheckParm("-safevga")) /* FS: Safe VGA mode */
		{
			Con_Printf("Safe VGA mode enabled\n");
			Cbuf_AddText("vid_mode 0");
		}
	}

	Cbuf_InsertText ("exec quake.rc\n");
	Cbuf_AddText ("cl_warncmd 1\n"); /* FS: From QW */

	Hunk_AllocName (0, "-HOST_HUNKLEVEL-");
	host_hunklevel = Hunk_LowMark ();

	host_initialized = true;
   
	Sys_Printf ("========Quake Initialized=========\n");  
}


/*
===============
Host_Shutdown

FIXME: this is a callback from Sys_Quit and Sys_Error.  It would be better
to run quit through here before the final handoff to the sys code.
===============
*/
void Host_Shutdown(void)
{
   static qboolean isdown = false;
   
   if (isdown)
   {
      printf ("recursive shutdown\n");
      return;
   }
   isdown = true;

// keep Con_Printf from trying to update the screen
   scr_disabled_for_loading = true;

   Host_WriteConfiguration ("config"); 

   CDAudio_Shutdown ();
   NET_Shutdown ();
   S_Shutdown();
   IN_Shutdown ();

   if (cls.state != ca_dedicated)
   {
      VID_Shutdown();
   }
}

