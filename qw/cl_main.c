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
// cl_main.c  -- client main loop

#include <ctype.h>

#include "quakedef.h"
#include "winquake.h"

#ifdef _WIN32
	#include "winsock.h"
#else
	#include <netinet/in.h>
#endif

#include "cfgfile.h" /* FS: Parse CFG early -- sezero */

#include "Goa/CEngine/goaceng.h" /* FS: For Gamespy */

// we need to declare some mouse variables here, because the menu system
// references them even when on a unix system.

qboolean noclip_anglehack;    // remnant from old quake

cvar_t	rcon_password = {"rcon_password", "", false};

cvar_t	rcon_address = {"rcon_address", ""};

cvar_t	cl_timeout = {"cl_timeout", "60"};

cvar_t	cl_shownet = {"cl_shownet","0"}; // can be 0, 1, or 2

cvar_t	cl_sbar     = {"cl_sbar", "0", true};
cvar_t	cl_hudswap  = {"cl_hudswap", "0", true};
cvar_t	cl_maxfps   = {"cl_maxfps", "0", true};

cvar_t	lookspring = {"lookspring","0", true};
cvar_t	lookstrafe = {"lookstrafe","0", true};
cvar_t	sensitivity = {"sensitivity","3", true};

cvar_t	m_pitch = {"m_pitch","0.022", true};
cvar_t	m_yaw = {"m_yaw","0.022"};
cvar_t	m_forward = {"m_forward","1"};
cvar_t	m_side = {"m_side","0.8"};

cvar_t	entlatency = {"entlatency", "20"};
cvar_t	cl_predict_players = {"cl_predict_players", "1"};
cvar_t	cl_predict_players2 = {"cl_predict_players2", "1"};
cvar_t	cl_solid_players = {"cl_solid_players", "1"};

cvar_t  localid = {"localid", ""};
cvar_t	net_broadcast_chat = {"net_broadcast_chat", "1", true, false, "Broadcast EZQ chats."}; /* FS: EZQ Chat */

static	qboolean allowremotecmd = true;

//
// info mirrors
//
cvar_t	password = {"password", "", false, true};
cvar_t	spectator = {"spectator", "0", false, true, "Enables connecting to supported servers as a spectator."};
cvar_t	name = {"name","unnamed", true, true, "Player name."};
cvar_t	team = {"team","", true, true};
cvar_t	skin = {"skin","", true, true};
cvar_t	topcolor = {"topcolor","0", true, true};
cvar_t	bottomcolor = {"bottomcolor","0", true, true};
cvar_t	rate = {"rate","2500", true, true, "Connection rate.  Values over 25000 are typically unnecessary."};
cvar_t	noaim = {"noaim","0", true, true};
cvar_t	msg = {"msg","1", true, true};
cvar_t	chat = {"chat", "", false, true, "Internal userinfo CVAR used for EZQ chat notifcations."}; /* FS: EZQ Chat */

extern	cvar_t cl_hightrack;

#ifdef PROTOCOL_VERSION_FTE
cvar_t  cl_pext_other = {"cl_pext_other", "1"};		// will break demos!
#endif
#ifdef FTE_PEXT_256PACKETENTITIES
cvar_t	cl_pext_256packetentities = {"cl_pext_256packetentities", "1"};
#endif
#ifdef FTE_PEXT_CHUNKEDDOWNLOADS
cvar_t  cl_pext_chunkeddownloads  = {"cl_pext_chunkeddownloads", "1"};
cvar_t  cl_chunksperframe  = {"cl_chunksperframe", "5"};
#endif
#ifdef FTE_PEXT_FLOATCOORDS
cvar_t  cl_pext_floatcoords  = {"cl_pext_floatcoords", "1"};
#endif
cvar_t	cl_downloadrate_hack = {"cl_downloadrate_hack", "1", true, false, "Skip rendering a few frames during downloads for faster downloading."}; /* FS: Gross download hack */

client_static_t   cls;
client_state_t cl;

entity_state_t cl_baselines[MAX_EDICTS];
efrag_t        cl_efrags[MAX_EFRAGS];
entity_t    cl_static_entities[MAX_STATIC_ENTITIES];
lightstyle_t   cl_lightstyle[MAX_LIGHTSTYLES];
dlight_t    cl_dlights[MAX_DLIGHTS];

// refresh list
// this is double buffered so the last frame
// can be scanned for oldorigins of trailing objects
int            cl_numvisedicts, cl_oldnumvisedicts;
entity_t    *cl_visedicts, *cl_oldvisedicts;
entity_t    cl_visedicts_list[2][MAX_VISEDICTS];

double         connect_time = -1;      // for connection retransmits

quakeparms_t host_parms;

qboolean host_initialized;    // true if into command execution
qboolean nomaster;

double      host_frametime;
double      realtime;            // without any filtering or bounding
double      oldrealtime;         // last frame run
int         host_framecount;

int         host_hunklevel;

byte     *host_basepal;
byte     *host_colormap;

netadr_t master_adr;          // address of the master server

cvar_t	host_speeds = {"host_speeds","0"};        // set for running times

cvar_t  show_fps = {"show_fps","0", true, false, "Show FPS counter on the screen."}; /* FS: Added */
cvar_t  show_time = {"show_time","0", true, false, "Show current time on the screen,  1 - Military.  2 - AM/PM."}; /* FS: Added */
cvar_t  show_uptime = {"show_uptime", "0", true, false, "Show current map uptime."}; /* FS: Added */
cvar_t	console_old_complete = {"console_old_complete", "0", true, false , "Use legacy style tab completion."}; /* FS: Added */
cvar_t	developer = {"developer","0", false, false, "Enable the use of developer messages. \nAvailable flags:\n  * All flags except verbose msgs - 1\n  * Standard msgs - 2\n  * Sound msgs - 4\n  * Network msgs - 8\n  * File IO msgs - 16\n  * Graphics renderer msgs - 32\n  * CD Player msgs - 64\n  * Memory management msgs - 128\n  * Physics msgs - 2048\n  * Entity msgs - 16384\n  * Extremely verbose msgs - 65536\n  * Extremely verbose gamespy msgs - 131072\n"};  /* FS: Added Description */
cvar_t	con_show_description = {"con_show_description", "1", true, false, "Show descriptions for CVARs."}; /* FS: Added */
cvar_t	con_show_dev_flags = {"con_show_dev_flags", "1", true, false, "Show developer flag options."}; /* FS: Added */

cvar_t	cl_ogg_music = {"cl_ogg_music", "1", true, false, "Play OGG tracks in the format of id1/music/trackXX.ogg if they exist."}; /* FS: Added */
cvar_t	cl_wav_music = {"cl_wav_music", "1", true, false, "Play WAV tracks in the format of id1/music/trackXX.wav if they exist."}; /* FS: Added */
cvar_t	cl_autorepeat_allkeys = {"cl_autorepeat_allkeys", "0", true, false, "Allow to autorepeat any key, not just Backspace, Pause, PgUp, and PgDn keys."}; /* FS: So I can autorepeat whatever I want, hoss. */

int         fps_count;

 /* FS: Flashlight */
qboolean bFlashlight;
void CL_Flashlight_f (void);

/* FS: Gamespy CVARs */
cvar_t	cl_master_server_ip = {"cl_master_server_ip", CL_MASTER_ADDR, true, false, "GameSpy Master Server IP."};
cvar_t	cl_master_server_port = {"cl_master_server_port", CL_MASTER_PORT, true, false, "GameSpy Master Server Port."};
cvar_t	cl_master_server_queries = {"cl_master_server_queries", "10", true, false, "Number of sockets to allocate for GameSpy."};
cvar_t	cl_master_server_timeout = {"cl_master_server_timeout", "3000", true, false, "Timeout (in milliseconds) to give up on pinging a server."};
cvar_t	cl_master_server_retries = {"cl_master_server_retries", "20", true, false, "Number of retries to attempt for receiving the server list.  Formula is 50ms + 10ms for each retry."};
cvar_t	snd_gamespy_sounds = {"snd_gamespy_sounds", "0", true, false, "Play the complete.wav and abort.wav from GameSpy3D if it exists in sounds/gamespy."};

/* FS: Gamespy prototypes */
static	GServerList	serverlist = NULL;
static	int		gspyCur;
gamespyBrowser_t browserList[MAX_SERVERS]; /* FS: Browser list for active servers */
gamespyBrowser_t browserListAll[MAX_SERVERS]; /* FS: Browser list for ALL servers */
static void GameSpy_Async_Think(void);
static void ListCallBack(GServerList serverlist, int msg, void *instance, void *param1, void *param2);
static void CL_Gspystop_f (void);
       void CL_PingNetServers_f (void);
static void CL_PrintBrowserList_f (void);

jmp_buf  host_abort;

void Master_Connect_f (void);
void CL_WriteConfig_f (void);

float server_version = 0;  // version of server we connected to

char emodel_name[] = 
   { 'e' ^ 0xff, 'm' ^ 0xff, 'o' ^ 0xff, 'd' ^ 0xff, 'e' ^ 0xff, 'l' ^ 0xff, 0 };
char pmodel_name[] = 
   { 'p' ^ 0xff, 'm' ^ 0xff, 'o' ^ 0xff, 'd' ^ 0xff, 'e' ^ 0xff, 'l' ^ 0xff, 0 };
char prespawn_name[] = 
   { 'p'^0xff, 'r'^0xff, 'e'^0xff, 's'^0xff, 'p'^0xff, 'a'^0xff, 'w'^0xff, 'n'^0xff,
      ' '^0xff, '%'^0xff, 'i'^0xff, ' '^0xff, '0'^0xff, ' '^0xff, '%'^0xff, 'i'^0xff, 0 };
char modellist_name[] = 
   { 'm'^0xff, 'o'^0xff, 'd'^0xff, 'e'^0xff, 'l'^0xff, 'l'^0xff, 'i'^0xff, 's'^0xff, 't'^0xff, 
      ' '^0xff, '%'^0xff, 'i'^0xff, ' '^0xff, '%'^0xff, 'i'^0xff, 0 };
char soundlist_name[] = 
   { 's'^0xff, 'o'^0xff, 'u'^0xff, 'n'^0xff, 'd'^0xff, 'l'^0xff, 'i'^0xff, 's'^0xff, 't'^0xff, 
      ' '^0xff, '%'^0xff, 'i'^0xff, ' '^0xff, '%'^0xff, 'i'^0xff, 0 };
 

/*
==================
CL_Quit_f
==================
*/
void CL_Quit_f (void)
{
	if (1 /* key_dest != key_console */ /* && cls.state != ca_dedicated */)
	{
		M_Menu_Quit_f ();
		return;
	}
	CL_Disconnect ();
	Sys_Quit ();
}

void CL_Fast_Quit_f (void)
{
	CL_Disconnect ();
	Host_Shutdown();
	exit(0);
}


/*
=======================
CL_Version_f
======================
*/
void CL_Version_f (void)
{
	Con_Printf ("Version %4.2f\n", VERSION);
	Con_Printf ("Exe: "__TIME__" "__DATE__"\n");
}

#ifdef PROTOCOL_VERSION_FTE
unsigned int CL_SupportedFTEExtensions (void)
{
	unsigned int fteprotextsupported = 0;

#ifdef FTE_PEXT_256PACKETENTITIES
	if (cl_pext_256packetentities.value)
	{
		fteprotextsupported |= FTE_PEXT_256PACKETENTITIES;
	}
#endif
#ifdef FTE_PEXT_CHUNKEDDOWNLOADS
	if (cl_pext_chunkeddownloads.value)
		fteprotextsupported |= FTE_PEXT_CHUNKEDDOWNLOADS;
#endif
#ifdef FTE_PEXT_FLOATCOORDS
	if (cl_pext_floatcoords.value)
		fteprotextsupported |= FTE_PEXT_FLOATCOORDS;
#endif
#ifdef FTE_PEXT_HLBSP
	fteprotextsupported |= FTE_PEXT_HLBSP;
#endif
#ifdef FTE_PEXT_MODELDBL
	fteprotextsupported |= FTE_PEXT_MODELDBL;
#endif
#ifdef FTE_PEXT_ENTITYDBL
	fteprotextsupported |= FTE_PEXT_ENTITYDBL;
#endif
#ifdef FTE_PEXT_ENTITYDBL2
	fteprotextsupported |= FTE_PEXT_ENTITYDBL2;
#endif
#ifdef FTE_PEXT_SPAWNSTATIC2
	fteprotextsupported |= FTE_PEXT_SPAWNSTATIC2;
#endif

	if (!cl_pext_other.value)
		fteprotextsupported &= (/*FTE_PEXT_CHUNKEDDOWNLOADS|*/FTE_PEXT_256PACKETENTITIES);

	return fteprotextsupported;
}
#endif


/*
=======================
CL_SendConnectPacket

called by CL_Connect_f and CL_CheckResend
======================
*/
void CL_SendConnectPacket (
#ifdef PROTOCOL_VERSION_FTE
							unsigned int ftepext
#endif
)
{
	netadr_t adr;
	dstring_t *data;
	double t1, t2;

// JACK: Fixed bug where DNS lookups would cause two connects real fast
//       Now, adds lookup time to the connect time.
//     Should I add it to realtime instead?!?!

	if (cls.state != ca_disconnected)
		return;

#ifdef PROTOCOL_VERSION_FTE
	cls.fteprotocolextensions  = (ftepext & CL_SupportedFTEExtensions());
#endif // PROTOCOL_VERSION_FTE

	t1 = Sys_DoubleTime ();

	if (!NET_StringToAdr (cls.servername->str, &adr))
	{
		Con_Printf ("Bad server address\n");
		connect_time = -1;
		return;
	}

	if (!NET_IsClientLegal(&adr))
	{
		Con_Printf ("Illegal server address\n");
		connect_time = -1;
		return;
	}

	if (adr.port == 0)
		adr.port = BigShort (27500);
	t2 = Sys_DoubleTime ();

	connect_time = realtime+t2-t1;   // for retransmit requests

	cls.qport = Cvar_VariableValue("qport");

	Info_SetValueForStarKey (cls.userinfo, "*ip", NET_AdrToString(adr), MAX_INFO_STRING);

	Con_Printf ("Connecting to %s...\n", cls.servername->str);

	data = dstring_new();
	dsprintf(data, "%c%c%c%cconnect %i %i %i \"%s\"\n",
                255, 255, 255, 255,     PROTOCOL_VERSION, cls.qport, cls.challenge, cls.userinfo);
#ifdef PROTOCOL_VERSION_FTE
	if (cls.fteprotocolextensions) 
	{
		char tmp[128];
		Com_sprintf(tmp, sizeof(tmp), "0x%x 0x%x\n", PROTOCOL_VERSION_FTE, cls.fteprotocolextensions);
		Con_DPrintf(DEVELOPER_MSG_NET, "0x%x is fte protocol ver and 0x%x is fteprotocolextensions\n", PROTOCOL_VERSION_FTE, cls.fteprotocolextensions);
		strcat(data->str, tmp);
	}
#endif // PROTOCOL_VERSION_FTE 
	NET_SendPacket (strlen(data->str), data->str, adr);
	dstring_delete(data);
}

/*
=================
CL_CheckForResend

Resend a connect message if the last one has timed out

=================
*/
void CL_CheckForResend (void)
{
	netadr_t adr;
	dstring_t *data;
	double t1, t2;

	if (connect_time == -1)
		return;
	if (cls.state != ca_disconnected)
		return;
	if (connect_time && realtime - connect_time < 5.0)
		return;

	t1 = Sys_DoubleTime ();

	if (!NET_StringToAdr (cls.servername->str, &adr))
	{
		Con_Printf ("Bad server address\n");
		connect_time = -1;
		return;
	}

	if (!NET_IsClientLegal(&adr))
	{
		Con_Printf ("Illegal server address\n");
		connect_time = -1;
		return;
	}

	if (adr.port == 0)
		adr.port = BigShort (27500);

	t2 = Sys_DoubleTime ();

	connect_time = realtime+t2-t1;   // for retransmit requests

	Con_Printf ("Connecting to %s...\n", cls.servername->str);

	data = dstring_new();

	dsprintf(data, "%c%c%c%cgetchallenge\n", 255, 255, 255, 255);
	NET_SendPacket (strlen(data->str), data->str, adr);

	dstring_delete(data);
}

void CL_BeginServerConnect(void)
{
   connect_time = 0;
   CL_CheckForResend();
}

/*
================
CL_Connect_f

================
*/
void CL_Connect_f (void)
{
	char  *server;

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("usage: connect <server>\n");
		return;  
	}
   
	server = Cmd_Argv (1);

	CL_Disconnect ();

	dstring_copystr (cls.servername, server); // taniwha
	CL_BeginServerConnect();
}


/*
=====================
CL_Rcon_f

  Send the rest of the command line over as
  an unconnected command.
=====================
*/
void CL_Rcon_f (void)
{
   char  message[1024];
   int      i;
   netadr_t to;

   if (!rcon_password.string)
   {
      Con_Printf ("You must set 'rcon_password' before\n"
               "issuing an rcon command.\n");
      return;
   }

   message[0] = 255;
   message[1] = 255;
   message[2] = 255;
   message[3] = 255;
   message[4] = 0;

   strcat (message, "rcon ");

   strcat (message, rcon_password.string);
   strcat (message, " ");

   for (i=1 ; i<Cmd_Argc() ; i++)
   {
      strcat (message, Cmd_Argv(i));
      strcat (message, " ");
   }

   if (cls.state >= ca_connected)
      to = cls.netchan.remote_address;
   else
   {
      if (!strlen(rcon_address.string))
      {
         Con_Printf ("You must either be connected,\n"
                  "or set the 'rcon_address' cvar\n"
                  "to issue rcon commands\n");

         return;
      }
      NET_StringToAdr (rcon_address.string, &to);
   }
   
   NET_SendPacket (strlen(message)+1, message
      , to);
}


/*
=====================
CL_ClearState

=====================
*/
void CL_ClearState (void)
{
   int         i;

   S_StopAllSounds (true);

   Con_DPrintf (DEVELOPER_MSG_MEM, "Clearing memory\n");
   D_FlushCaches ();
   Mod_ClearAll ();
   if (host_hunklevel)  // FIXME: check this...
      Hunk_FreeToLowMark (host_hunklevel);

   CL_ClearTEnts ();

// wipe the entire cl structure
   memset (&cl, 0, sizeof(cl));

   SZ_Clear (&cls.netchan.message);

// clear other arrays   
   memset (cl_efrags, 0, sizeof(cl_efrags));
   memset (cl_dlights, 0, sizeof(cl_dlights));
   memset (cl_lightstyle, 0, sizeof(cl_lightstyle));

//
// allocate the efrags and chain together into a free list
//
   cl.free_efrags = cl_efrags;
   for (i=0 ; i<MAX_EFRAGS-1 ; i++)
      cl.free_efrags[i].entnext = &cl.free_efrags[i+1];
   cl.free_efrags[i].entnext = NULL;
}

/*
=====================
CL_Disconnect

Sends a disconnect message to the server
This is also called on Host_Error, so it shouldn't cause any errors
=====================
*/
void CL_Disconnect (void)
{
	byte  final[10];

	connect_time = -1;

#ifdef _WIN32
	SetWindowText (mainwindow, "QWDOS: disconnected");
#endif

// stop sounds (especially looping!)
	S_StopAllSounds (true);
   
// if running a local server, shut it down
	if (cls.demoplayback)
		CL_StopPlayback ();
	else if (cls.state != ca_disconnected)
	{
		if (cls.demorecording)
		CL_Stop_f ();

		final[0] = clc_stringcmd;
		strcpy ((char *)final+1, "drop");
		Netchan_Transmit (&cls.netchan, 6, final);
		Netchan_Transmit (&cls.netchan, 6, final);
		Netchan_Transmit (&cls.netchan, 6, final);

		cls.state = ca_disconnected;

		cls.demoplayback = cls.demorecording = cls.timedemo = false;
	}
	Cam_Reset();

	CL_FinishDownload(false);

	CL_StopUpload();

	cls.qport++; /* FS: From EZQ -- A hack I picked up from qizmo. */

	SZ_Clear(&cls.cmdmsg); /* FS: From EZQ */

	key_dest = 0; /* FS: Fix so main menu still works after disconnect */
	cl.intermission = 0; /* FS: Baker fix */
}

void CL_Disconnect_f (void)
{
	CL_Disconnect ();
}

/*
====================
CL_User_f

user <name or userid>

Dump userdata / masterdata for a user
====================
*/
void CL_User_f (void)
{
   int      uid;
   int      i;

   if (Cmd_Argc() != 2)
   {
      Con_Printf ("Usage: user <username / userid>\n");
      return;
   }

   uid = atoi(Cmd_Argv(1));

   for (i=0 ; i<MAX_CLIENTS ; i++)
   {
      if (!cl.players[i].name[0])
         continue;
      if (cl.players[i].userid == uid
      || !strcmp(cl.players[i].name, Cmd_Argv(1)) )
      {
         Info_Print (cl.players[i].userinfo);
         return;
      }
   }
   Con_Printf ("User not in server.\n");
}

/*
====================
CL_Users_f

Dump userids for all current players
====================
*/
void CL_Users_f (void)
{
   int      i;
   int      c;

   c = 0;
   Con_Printf ("userid frags name\n");
   Con_Printf ("------ ----- ----\n");
   for (i=0 ; i<MAX_CLIENTS ; i++)
   {
      if (cl.players[i].name[0])
      {
         Con_Printf ("%6i %4i %s\n", cl.players[i].userid, cl.players[i].frags, cl.players[i].name);
         c++;
      }
   }

   Con_Printf ("%i total users\n", c);
}

void CL_Color_f (void)
{
	// just for quake compatability...
	int	top, bottom;
	char	num[16];

	if (Cmd_Argc() == 1)
	{
		Con_Printf ("\"color\" is \"%s %s\"\n",
			Info_ValueForKey (cls.userinfo, "topcolor"),
			Info_ValueForKey (cls.userinfo, "bottomcolor") );
		Con_Printf ("color <0-13> [0-13]\n");
		return;
	}

	if (Cmd_Argc() == 2)
		top = bottom = atoi(Cmd_Argv(1));
	else
	{
		top = atoi(Cmd_Argv(1));
		bottom = atoi(Cmd_Argv(2));
	}
   
	top &= 15;
	if (top > 13)
		top = 13;

	bottom &= 15;
	if (bottom > 13)
		bottom = 13;
   
	Com_sprintf (num, sizeof(num), "%i", top);
	Cvar_Set ("topcolor", num);
	Com_sprintf (num, sizeof(num), "%i", bottom);
	Cvar_Set ("bottomcolor", num);
}

/*
==================
CL_FullServerinfo_f

Sent by server when serverinfo changes
==================
*/
void CL_FullServerinfo_f (void)
{
   char *p;
   float v;

   if (Cmd_Argc() != 2)
   {
      Con_Printf ("usage: fullserverinfo <complete info string>\n");
      return;
   }

   strcpy (cl.serverinfo, Cmd_Argv(1));

   if ((p = Info_ValueForKey(cl.serverinfo, "*vesion")) && *p) {
      v = Q_atof(p);
      if (v) {
         if (!server_version)
            Con_Printf("Version %1.2f Server\n", v);
         server_version = v;
      }
   }
}

/*
==================
CL_FullInfo_f

Allow clients to change userinfo
==================
Casey was here :)
*/
void CL_FullInfo_f (void)
{
   char  key[512];
   char  value[512];
   char  *o;
   char  *s;

   if (Cmd_Argc() != 2)
   {
      Con_Printf ("fullinfo <complete info string>\n");
      return;
   }

   s = Cmd_Argv(1);
   if (*s == '\\')
      s++;
   while (*s)
   {
      o = key;
      while (*s && *s != '\\')
         *o++ = *s++;
      *o = 0;

      if (!*s)
      {
         Con_Printf ("MISSING VALUE\n");
         return;
      }

      o = value;
      s++;
      while (*s && *s != '\\')
         *o++ = *s++;
      *o = 0;

      if (*s)
         s++;

      if (!stricmp(key, pmodel_name) || !stricmp(key, emodel_name))
         continue;

      Info_SetValueForKey (cls.userinfo, key, value, MAX_INFO_STRING);
   }
}

/*
==================
CL_SetInfo_f

Allow clients to change userinfo
==================
*/
void CL_SetInfo_f (void)
{
   if (Cmd_Argc() == 1)
   {
      Info_Print (cls.userinfo);
      return;
   }
   if (Cmd_Argc() != 3)
   {
      Con_Printf ("usage: setinfo [ <key> <value> ]\n");
      return;
   }
   if (!stricmp(Cmd_Argv(1), pmodel_name) || !strcmp(Cmd_Argv(1), emodel_name))
      return;

   Info_SetValueForKey (cls.userinfo, Cmd_Argv(1), Cmd_Argv(2), MAX_INFO_STRING);
   if (cls.state >= ca_connected)
      Cmd_ForwardToServer ();
}

/*
====================
CL_Packet_f

packet <destination> <contents>

Contents allows \n escape character
====================
*/
void CL_Packet_f (void)
{
   char  send[2048];
   int      i, l;
   char  *in, *out;
   netadr_t adr;

   if (Cmd_Argc() != 3)
   {
      Con_Printf ("packet <destination> <contents>\n");
      return;
   }

   if (!NET_StringToAdr (Cmd_Argv(1), &adr))
   {
      Con_Printf ("Bad address\n");
      return;
   }

   in = Cmd_Argv(2);
   out = send+4;
   send[0] = send[1] = send[2] = send[3] = 0xff;

   l = strlen (in);
   for (i=0 ; i<l ; i++)
   {
      if (in[i] == '\\' && in[i+1] == 'n')
      {
         *out++ = '\n';
         i++;
      }
      else
         *out++ = in[i];
   }
   *out = 0;

   NET_SendPacket (out-send, send, adr);
}


/*
=====================
CL_NextDemo

Called to play the next demo in the demo loop
=====================
*/
void CL_NextDemo (void)
{
	char  str[1024];

	if (cls.demonum == -1)
		return;     // don't play demos

	if (!cls.demos[cls.demonum][0] || cls.demonum == MAX_DEMOS)
	{
		cls.demonum = 0;
		if (!cls.demos[cls.demonum][0])
		{
//			Con_Printf ("No demos listed with startdemos\n");
			cls.demonum = -1;
			return;
		}
	}

	Com_sprintf (str, sizeof(str), "playdemo %s\n", cls.demos[cls.demonum]);
	Cbuf_InsertText (str);
	cls.demonum++;
}


/*
=================
CL_Changing_f

Just sent as a hint to the client that they should
drop to full console
=================
*/
void CL_Changing_f (void)
{
   if (cls.download)  // don't change when downloading
      return;

   S_StopAllSounds (true);
   cl.intermission = 0;
   cls.state = ca_connected;  // not active anymore, but not disconnected
   Con_Printf ("\nChanging map...\n");
}


/*
=================
CL_Reconnect_f

The server is changing levels
=================
*/
void CL_Reconnect_f (void)
{
   if (cls.download)  // don't change when downloading
      return;

   S_StopAllSounds (true);

   if (cls.state == ca_connected) {
      Con_Printf ("reconnecting...\n");
      MSG_WriteChar (&cls.netchan.message, clc_stringcmd);
      MSG_WriteString (&cls.netchan.message, "new");
      return;
   }

        if (!*cls.servername->str) {
      Con_Printf("No server to reconnect to...\n");
      return;
   }

   CL_Disconnect();
   CL_BeginServerConnect();
}

/*
=================
CL_ConnectionlessPacket

Responses to broadcasts, etc
=================
*/
void CL_ConnectionlessPacket (void)
{
	char  *s;
	int      c;

#ifdef PROTOCOL_VERSION_FTE
	unsigned int pext = 0;
#endif

    MSG_BeginReading ();
    MSG_ReadLong ();        // skip the -1

	c = MSG_ReadByte ();
//	if (!cls.demoplayback)
//		Con_Printf ("%s: ", NET_AdrToString (net_from));
//	Con_DPrintf (DEVELOPER_MSG_NET, "%s", net_message.data + 5);
	if (c == S2C_CONNECTION)
	{
		Con_Printf ("%s: connection\n", NET_AdrToString (net_from));
		if (cls.state >= ca_connected)
		{
			if (!cls.demoplayback)
				Con_Printf ("Dup connect received.  Ignored.\n");
			return;
		}
		Netchan_Setup (&cls.netchan, net_from, cls.qport);
		MSG_WriteChar (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message, "new");  
		cls.state = ca_connected;
		Con_Printf ("Connected.\n");
		allowremotecmd = false; // localid required now for remote cmds
		return;
	}
	// remote command from gui front end
	if (c == A2C_CLIENT_COMMAND)
	{
		char  cmdtext[2048];

		Con_Printf ("%s: client command\n", NET_AdrToString (net_from));

		if ((*(unsigned *)net_from.ip != *(unsigned *)net_local_adr.ip
		&& *(unsigned *)net_from.ip != htonl(INADDR_LOOPBACK)) )
		{
			Con_Printf ("Command packet from remote host.  Ignored.\n");
			return;
		}
#ifdef _WIN32
		ShowWindow (mainwindow, SW_RESTORE);
		SetForegroundWindow (mainwindow);
#endif
		s = MSG_ReadString ();

		strncpy(cmdtext, s, sizeof(cmdtext) - 1);
		cmdtext[sizeof(cmdtext) - 1] = 0;

		s = MSG_ReadString ();

		while (*s && isspace(*s))
			s++;
		while (*s && isspace(s[strlen(s) - 1]))
			s[strlen(s) - 1] = 0;

		if (!allowremotecmd && (!*localid.string || strcmp(localid.string, s)))
		{
			if (!*localid.string)
			{
				Con_Printf("===========================\n");
			    Con_Printf("Command packet received from local host, but no "
							"localid has been set.  You may need to upgrade your server "
							"browser.\n");
				Con_Printf("===========================\n");
				return;
			}
			Con_Printf("===========================\n");
			Con_Printf("Invalid localid on command packet received from local host. "
						"\n|%s| != |%s|\n"
						"You may need to reload your server browser and QuakeWorld.\n", s, localid.string);
			Con_Printf("===========================\n");
			Cvar_Set("localid", "");
			return;
		}

		Cbuf_AddText (cmdtext);
		allowremotecmd = false;
		return;
	}

	// print command from somewhere
	if (c == A2C_PRINT)
	{
#ifdef FTE_PEXT_CHUNKEDDOWNLOADS
		if (net_message.cursize > 100 && !strncmp((char *)net_message.data + 5, "\\chunk", sizeof("\\chunk")-1)) 
		{
			CL_Parse_OOB_ChunkedDownload();
			return;
		}
#endif // FTE_PEXT_CHUNKEDDOWNLOADS
//		Con_Printf ("print\n");
		Con_Printf("%s: print\n", NET_AdrToString(net_from));

		s = MSG_ReadString ();
		Con_Print (s);
		return;
	}

	// ping from somewhere
	if (c == A2A_PING)
	{
		char  data[6];

		Con_Printf ("%s: ping\n", NET_AdrToString (net_from));

		data[0] = 0xff;
		data[1] = 0xff;
		data[2] = 0xff;
		data[3] = 0xff;
		data[4] = A2A_ACK;
		data[5] = 0;
      
		NET_SendPacket (6, &data, net_from);
		return;
	}

	if (c == S2C_CHALLENGE)
	{
		Con_Printf ("%s: challenge\n", NET_AdrToString (net_from));

		s = MSG_ReadString ();
		cls.challenge = atoi(s);
#ifdef PROTOCOL_VERSION_FTE
		for(;;)
		{
			c = MSG_ReadLong();
			if (msg_badread)
				break;
			if (c == PROTOCOL_VERSION_FTE)
				pext = MSG_ReadLong();
			else
				MSG_ReadLong();
		}

		CL_SendConnectPacket(pext);
#else
		CL_SendConnectPacket();
#endif // PROTOCOL_VERSION_FTE
		return;
	}

	if (c == svc_disconnect)
	{
		if(cls.demoplayback)
			Host_EndGame ("End of demo");
		else
			Con_Printf ("svc_disconnect\n");
		return;
	}
	Con_Printf ("%s: unknown: %c\n", NET_AdrToString (net_from), c);
}


/*
=================
CL_ReadPackets
=================
*/
void CL_ReadPackets (void)
{
// while (NET_GetPacket ())
	while (CL_GetMessage())
	{
		//
		// remote command packet
		//
		if (*(int *)net_message.data == -1)
		{
			CL_ConnectionlessPacket ();
			continue;
		}

		if (net_message.cursize < 8)
		{
			Con_Printf ("%s: Runt packet\n",NET_AdrToString(net_from));
			continue;
		}

		//
		// packet from server
		//
		if (!cls.demoplayback && !NET_CompareAdr (net_from, cls.netchan.remote_address))
		{
			Con_DPrintf (DEVELOPER_MSG_NET, "%s:sequenced packet without connection\n" ,NET_AdrToString(net_from));
			continue;
		}
		if (!Netchan_Process(&cls.netchan))
			continue;      // wasn't accepted for some reason
		CL_ParseServerMessage ();

//		if (cls.demoplayback && cls.state >= ca_active && !CL_DemoBehind())
//			return;
	}

	//
	// check timeout
	//
	if (cls.state >= ca_connected
	&& realtime - cls.netchan.last_received > cl_timeout.value)
	{
		Con_Printf ("\nServer connection timed out.\n");
		CL_Disconnect ();
		return;
	}
   
}

//=============================================================================

/*
=====================
CL_Download_f
=====================
*/
void CL_Download_f (void)
{
	char *p, *q;
	dstring_t *dlstr;

	if (cls.state == ca_disconnected)
	{
		Con_Printf ("Must be connected.\n");
		return;
	}

	if (Cmd_Argc() != 2)
	{
		Con_Printf ("Usage: download <datafile>\n");
		return;
	}

	dlstr = dstring_new();
	dsprintf (cls.downloadname, "%s/%s", com_gamedir, Cmd_Argv(1));
	p = cls.downloadname->str;
	for (;;)
	{
		if ((q = strchr(p, '/')) != NULL)
		{
			*q = 0;
			Sys_mkdir(cls.downloadname->str);
			*q = '/';
			p = q + 1;
		}
		else
			break;
	}

	dstring_copystr (cls.downloadtempname, cls.downloadname->str);
	cls.download = fopen (cls.downloadname->str, "wb");
	cls.downloadtype = dl_single;

	MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
	dsprintf(dlstr, "download %s\n", Cmd_Argv(1));
	SZ_Print (&cls.netchan.message, dlstr->str);
	dstring_delete(dlstr);
}

#ifdef _WINDOWS
#include <windows.h>
/*
=================
CL_Minimize_f
=================
*/
void CL_Windows_f (void)
{
//	if (modestate == MS_WINDOWED)
//		ShowWindow(mainwindow, SW_MINIMIZE);
//	else
		SendMessage(mainwindow, WM_SYSKEYUP, VK_TAB, 1 | (0x0F << 16) | (1<<29));
}
#endif

/*
=================
CL_Init
=================
*/
void CL_Init (void)
{
	extern   cvar_t	   baseskin;
	extern   cvar_t	   noskins;

	dstring_t *version = dstring_new();
	dsprintf(version, "QWDOS v%4.2f", VERSION);

	cls.state = ca_disconnected;

	SZ_Init(&cls.cmdmsg, cls.cmdmsg_data, sizeof(cls.cmdmsg_data));
	cls.cmdmsg.allowoverflow = true;

	Info_SetValueForKey (cls.userinfo, "name", "unnamed", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "topcolor", "0", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "bottomcolor", "0", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "rate", "2500", MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "msg", "1", MAX_INFO_STRING);
	Info_SetValueForStarKey (cls.userinfo, "*ver", version->str, MAX_INFO_STRING);
	Info_SetValueForKey (cls.userinfo, "chat", "", MAX_INFO_STRING); /* FS: EZQ Chat */

//	Info_SetValueForStarKey (cls.userinfo, "*cap", "h", MAX_INFO_STRING); /* FS: HTTP downloading from QuakeForge */
	CL_InitInput ();
	CL_InitTEnts ();
	CL_InitPrediction ();
	CL_InitCam ();
	Pmove_Init ();
   
//
// register our commands
//
	Cvar_RegisterVariable (&show_fps);
	Cvar_RegisterVariable (&show_time); /* FS: Added */
	Cvar_RegisterVariable (&show_uptime); /* FS: Added */
	Cvar_RegisterVariable (&console_old_complete); /* FS: Added */
	Cvar_RegisterVariable (&net_broadcast_chat); /* FS: EZQ Chat */
	Cvar_RegisterVariable (&host_speeds);
	Cvar_RegisterVariable (&developer);

	Cvar_RegisterVariable (&cl_warncmd);
	Cvar_RegisterVariable (&cl_upspeed);
	Cvar_RegisterVariable (&cl_forwardspeed);
	Cvar_RegisterVariable (&cl_backspeed);
	Cvar_RegisterVariable (&cl_sidespeed);
	Cvar_RegisterVariable (&cl_movespeedkey);
	Cvar_RegisterVariable (&cl_yawspeed);
	Cvar_RegisterVariable (&cl_pitchspeed);
	Cvar_RegisterVariable (&cl_anglespeedkey);
	Cvar_RegisterVariable (&cl_shownet);
	Cvar_RegisterVariable (&cl_sbar);
	Cvar_RegisterVariable (&cl_hudswap);
	Cvar_RegisterVariable (&cl_maxfps);
	Cvar_RegisterVariable (&cl_timeout);
	Cvar_RegisterVariable (&lookspring);
	Cvar_RegisterVariable (&lookstrafe);
	Cvar_RegisterVariable (&sensitivity);

	Cvar_RegisterVariable (&m_pitch);
	Cvar_RegisterVariable (&m_yaw);
	Cvar_RegisterVariable (&m_forward);
	Cvar_RegisterVariable (&m_side);

	Cvar_RegisterVariable (&rcon_password);
	Cvar_RegisterVariable (&rcon_address);

	Cvar_RegisterVariable (&entlatency);
	Cvar_RegisterVariable (&cl_predict_players2);
	Cvar_RegisterVariable (&cl_predict_players);
	Cvar_RegisterVariable (&cl_solid_players);

	Cvar_RegisterVariable (&localid);

	Cvar_RegisterVariable (&baseskin);
	Cvar_RegisterVariable (&noskins);

   //
   // info mirrors
   //
	Cvar_RegisterVariable (&name);
	Cvar_RegisterVariable (&password);
	Cvar_RegisterVariable (&spectator);
	Cvar_RegisterVariable (&skin);
	Cvar_RegisterVariable (&team);
	Cvar_RegisterVariable (&topcolor);
	Cvar_RegisterVariable (&bottomcolor);
	Cvar_RegisterVariable (&rate);
	Cvar_RegisterVariable (&msg);
	Cvar_RegisterVariable (&noaim);
	Cvar_RegisterVariable (&chat); /* FS: EZQ Chat */

#ifdef PROTOCOL_VERSION_FTE
	Cvar_RegisterVariable (&cl_pext_other);
#endif
#ifdef FTE_PEXT_256PACKETENTITIES
	Cvar_RegisterVariable (&cl_pext_256packetentities);
#endif
#ifdef FTE_PEXT_CHUNKEDDOWNLOADS
	Cvar_RegisterVariable (&cl_pext_chunkeddownloads);
	Cvar_RegisterVariable (&cl_chunksperframe);
#endif
#ifdef FTE_PEXT_FLOATCOORDS
	Cvar_RegisterVariable (&cl_pext_floatcoords);
#endif

	/* FS: New stuff */
	Cvar_RegisterVariable (&cl_downloadrate_hack);
	Cvar_RegisterVariable (&con_show_description);
	Cvar_RegisterVariable (&con_show_dev_flags);
	Cvar_RegisterVariable (&cl_ogg_music);
	Cvar_RegisterVariable (&cl_wav_music);
	Cvar_RegisterVariable (&cl_autorepeat_allkeys);

	Cmd_AddCommand ("writeconfig", CL_WriteConfig_f);

	/* FS: GameSpy CVARs */
	Cvar_RegisterVariable (&cl_master_server_ip);
	Cvar_RegisterVariable (&cl_master_server_port);
	Cvar_RegisterVariable (&cl_master_server_queries);
	Cvar_RegisterVariable (&cl_master_server_timeout);
	Cvar_RegisterVariable (&cl_master_server_retries);
	Cvar_RegisterVariable (&snd_gamespy_sounds);

	Cmd_AddCommand ("version", CL_Version_f);
	Cmd_AddCommand ("cl_flashlight", CL_Flashlight_f); /* FS: Flashlight */
	Cmd_AddCommand ("changing", CL_Changing_f);
	Cmd_AddCommand ("disconnect", CL_Disconnect_f);
	Cmd_AddCommand ("record", CL_Record_f);
	Cmd_AddCommand ("rerecord", CL_ReRecord_f);
	Cmd_AddCommand ("stop", CL_Stop_f);
	Cmd_AddCommand ("playdemo", CL_PlayDemo_f);
	Cmd_AddCommand ("timedemo", CL_TimeDemo_f);

	Cmd_AddCommand ("skins", Skin_Skins_f);
	Cmd_AddCommand ("allskins", Skin_AllSkins_f);

	Cmd_AddCommand ("quit", CL_Quit_f);
	Cmd_AddCommand ("quit!", CL_Fast_Quit_f); /* FS: Fast Quit */

	Cmd_AddCommand ("connect", CL_Connect_f);
	Cmd_AddCommand ("reconnect", CL_Reconnect_f);

	Cmd_AddCommand ("rcon", CL_Rcon_f);
	Cmd_AddCommand ("packet", CL_Packet_f);
	Cmd_AddCommand ("user", CL_User_f);
	Cmd_AddCommand ("users", CL_Users_f);

	Cmd_AddCommand ("setinfo", CL_SetInfo_f);
	Cmd_AddCommand ("fullinfo", CL_FullInfo_f);
	Cmd_AddCommand ("fullserverinfo", CL_FullServerinfo_f);

	Cmd_AddCommand ("color", CL_Color_f);
	Cmd_AddCommand ("download", CL_Download_f);

	Cmd_AddCommand ("nextul", CL_NextUpload);
	Cmd_AddCommand ("stopul", CL_StopUpload);

//
// forward to server commands
//
	Cmd_AddCommand ("kill", NULL);
	Cmd_AddCommand ("pause", NULL);
	Cmd_AddCommand ("say", NULL);
	Cmd_AddCommand ("say_team", NULL);
	Cmd_AddCommand ("serverinfo", NULL);

//
//  Windows commands
//
#ifdef _WINDOWS
	Cmd_AddCommand ("windows", CL_Windows_f);
#endif

	/* FS: Gamespy stuff */
	Cmd_AddCommand ("slist2", CL_PingNetServers_f);
	Cmd_AddCommand ("srelist", CL_PrintBrowserList_f);
	Cmd_AddCommand ("gspystop", CL_Gspystop_f);

	memset(&browserList, 0, sizeof(browserList));
	memset(&browserListAll, 0, sizeof(browserListAll));

	dstring_delete(version);
}


/*
================
Host_EndGame

Call this to drop to a console without exiting the qwcl
================
*/
void Host_EndGame (const char *message, ...)
{
	va_list      argptr;
	static dstring_t *string;

	if (!string)
		string = dstring_new ();

	va_start (argptr, message);
	dvsprintf (string, message, argptr);
	va_end (argptr);

	Con_Printf ("\n===========================\n");
	Con_Printf ("Host_EndGame: %s\n",string->str);
	Con_Printf ("===========================\n\n");
   
	CL_Disconnect ();

	longjmp (host_abort, 1);
}

/*
================
Host_Error

This shuts down the client and exits qwcl
================
*/
void Host_Error (const char *error, ...)
{
	va_list     argptr;
	static dstring_t       *string;
	static qboolean inerror = false;
   
	if(!string)
		string = dstring_new();

	if (inerror)
		Sys_Error ("Host_Error: recursively entered");

	inerror = true;
   
	va_start (argptr,error);
	dvsprintf (string,error,argptr);
	va_end (argptr);
	Con_Printf ("Host_Error: %s\n",string->str);
   
	CL_Disconnect ();
	cls.demonum = -1;

	inerror = false;

	Sys_Error ("Host_Error: %s\n",string->str);
}

/*
===============
Host_WriteConfiguration

Writes key bindings and archived cvars to config.cfg
===============
*/
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
	if (host_initialized)
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
// Knightmare added
/*
===============
CL_WriteConfig_f

===============
*/
void CL_WriteConfig_f (void)
{
	char cfgName[MAX_QPATH];

	if ((Cmd_Argc() == 1) || (Cmd_Argc() == 2))
	{
		if(Cmd_Argc() == 1)
			Com_sprintf(cfgName, sizeof(cfgName), "config");
		else
			strncpy (cfgName, Cmd_Argv(1), sizeof(cfgName));

		Host_WriteConfiguration (cfgName);
		Con_Printf ("Wrote config file %s/%s.cfg.\n", com_gamedir, cfgName);
	}
	else
		Con_Printf ("Usage: writeconfig <name>\n");
}

//============================================================================

/*
==================
Host_Frame

Runs all active servers
==================
*/
int      nopacketcount;
void Host_Frame (float time)
{
	static double	time1 = 0;
	static double	time2 = 0;
	static double	time3 = 0;
	int				pass1, pass2, pass3;
	float			fps;

	if (setjmp (host_abort) )
	return;        // something bad happened, or the server disconnected

	// decide the simulation time
	realtime += time;
	if (oldrealtime > realtime)
		oldrealtime = 0;

	if (cl_maxfps.intValue)
		fps = bound(30, cl_maxfps.intValue, 240);
	else
		fps = bound(30, cl_maxfps.intValue, 72);

	if (!cls.timedemo && realtime - oldrealtime < 1.0/fps)
	{
		return;        // framerate is too high
	}


	host_frametime = realtime - oldrealtime;
	oldrealtime = realtime;

	if (host_frametime > 0.2)
		host_frametime = 0.2;

	GameSpy_Async_Think();

	// get new key events
	Sys_SendKeyEvents ();

	// allow mice or other external controllers to add commands
	IN_Commands ();

	// process console commands
	Cbuf_Execute ();

	// fetch results from server
	CL_ReadPackets ();

	// send intentions now
	// resend a connection request if necessary
	if (cls.state == ca_disconnected)
		CL_CheckForResend ();
	else
		CL_SendCmd ();

	// Set up prediction for other players
	CL_SetUpPlayerPrediction(false);

	// do client side motion prediction
	CL_PredictMove ();

	// Set up prediction for other players
	CL_SetUpPlayerPrediction(true);

	// build a refresh entity list
	CL_EmitEntities ();

	// update video
	if (host_speeds.value)
		time1 = Sys_DoubleTime ();

	SCR_UpdateScreen ();

	if (host_speeds.value)
		time2 = Sys_DoubleTime ();
      
	// update audio
	if (cls.state == ca_active)
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
		time3 = Sys_DoubleTime ();
		pass2 = (time2 - time1)*1000;
		pass3 = (time3 - time2)*1000;

		Con_Printf ("%3i tot %3i server %3i gfx %3i snd\n",
					pass1+pass2+pass3, pass1, pass2, pass3);
	}

	if (spectator.modified) /* FS: Spectator reconnect hack */
	{
		spectator.modified = false;
		
		if (cls.state == ca_active)
		{
			Cbuf_AddText("reconnect\n");
		}
	}

	host_framecount++;
	fps_count++;
}

static void simple_crypt(char *buf, int len)
{
	while (len--)
		*buf++ ^= 0xff;
}

void Host_FixupModelNames(void)
{
	simple_crypt(emodel_name, sizeof(emodel_name) - 1);
	simple_crypt(pmodel_name, sizeof(pmodel_name) - 1);
	simple_crypt(prespawn_name,  sizeof(prespawn_name)  - 1);
	simple_crypt(modellist_name, sizeof(modellist_name) - 1);
	simple_crypt(soundlist_name, sizeof(soundlist_name) - 1);
}

//============================================================================

/*
====================
Host_Init
====================
*/
void Host_Init (quakeparms_t *parms)
{
	COM_InitArgv (parms->argc, parms->argv);
	COM_AddParm ("-game");
	COM_AddParm ("qw");

	Sys_mkdir("qw");

	if (COM_CheckParm ("-minmemory"))
		parms->memsize = MINIMUM_MEMORY;

	host_parms = *parms;

	if (parms->memsize < MINIMUM_MEMORY)
		Sys_Error ("Only %4.1f megs of memory reported, can't execute game", parms->memsize / (float)0x100000);

	Memory_Init (parms->membase, parms->memsize);
	Cbuf_Init ();
	Cmd_Init ();
	V_Init ();

	COM_Init ();
	Cvar_Init (); /* FS: FIXME Move this -- For cvarlist */

	CFG_OpenConfig("config.cfg"); /* FS: Parse CFG early -- sezero */

	cls.servername = dstring_newstr ();
	cls.downloadtempname = dstring_newstr ();
	cls.downloadname = dstring_newstr();
	cls.downloadurl = dstring_newstr();

	Host_FixupModelNames();
   
	NET_Init (PORT_CLIENT);
	Netchan_Init ();

	W_LoadWadFile ("gfx.wad");
	Key_Init ();
	Con_Init ();   
	M_Init ();  
	Mod_Init ();
   
	Con_Printf ("%4.1f megs RAM used.\n",parms->memsize/ (1024*1024.0));
   
	R_InitTextures ();
 
	host_basepal = (byte *)COM_LoadHunkFile ("gfx/palette.lmp");

	if (!host_basepal)
		Sys_Error ("Couldn't load gfx/palette.lmp");

	host_colormap = (byte *)COM_LoadHunkFile ("gfx/colormap.lmp");

	if (!host_colormap)
		Sys_Error ("Couldn't load gfx/colormap.lmp");

	VID_Init (host_basepal);
	Draw_Init ();
	SCR_Init ();
	R_Init ();
#ifndef _WIN32
	S_Init ();
#endif

	cls.state = ca_disconnected;
	CDAudio_Init ();
	Sbar_Init ();
	CL_Init ();
	IN_Init ();

	if(COM_CheckParm("-safevga")) /* FS: Safe VGA Mode */
	{
		Con_Printf("Safe VGA mode enabled\n");
		Cbuf_AddText("vid_mode 0");
	}

	Cbuf_InsertText ("exec quake.rc\n");
	Cbuf_AddText ("echo Type connect <internet address> or use GameSpy to connect to a game.\n");
	Cbuf_AddText ("cl_warncmd 1\n");

	Hunk_AllocName (0, "-HOST_HUNKLEVEL-");
	host_hunklevel = Hunk_LowMark ();

	host_initialized = true;

	Con_Printf ("\nClient Version %4.2f (Build %04d)\n\n", VERSION, build_number());

	Con_Printf ("€ QuakeWorld Initialized ‚\n"); 
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

	Host_WriteConfiguration ("config"); 
      
	CDAudio_Shutdown ();
	NET_Shutdown ();
	S_Shutdown();
	IN_Shutdown ();
	if (host_basepal)
		VID_Shutdown();
}

void CL_Flashlight_f (void) /* FS: Flashlight */
{
	if(bFlashlight)
		bFlashlight = false;
	else
		bFlashlight = true;
}

/* FS: Gamespy Stuff */
static void CL_Gamespy_Check_Error(GServerList lst, int error)
{
	if (error != GE_NOERROR) /* FS: Grab the error code */
	{
		Con_Printf("\x02GameSpy Error: ");
		Con_Printf("%s.\n", ServerListErrorDesc(lst, error));
	}
}

static void GameSpy_Async_Think(void)
{
	int error;

	if(!serverlist)
		return;

	if(ServerListState(serverlist) == sl_idle && cls.gamespyupdate)
	{
		if (key_dest != key_menu) /* FS: Only print this from an slist2 command, not the server browser. */
		{
			Con_Printf("Found %i active servers out of %i in %i seconds.\n", gspyCur, cls.gamespytotalservers, (((int)Sys_DoubleTime()-cls.gamespystarttime)) );
		}
		else
		{
			Update_Gamespy_Menu();
		}
		cls.gamespyupdate = 0;
		cls.gamespypercent = 0;
		ServerListClear(serverlist);
		ServerListFree(serverlist);
		serverlist = NULL; /* FS: This is on purpose so future ctrl+c's won't try to close empty serverlists */
	}
	else
	{
		error = ServerListThink(serverlist);
		CL_Gamespy_Check_Error(serverlist, error);
	}
}

static void CL_Gspystop_f (void)
{
	if(serverlist != NULL && cls.gamespyupdate) /* FS: Immediately abort gspy scans */
	{
		Con_Printf("\x02Server scan aborted!\n");
		S_GamespySound ("gamespy/abort.wav");
		ServerListHalt(serverlist);
	}
}

static void CL_PrintBrowserList_f (void)
{
	int i;
	int num_active_servers = 0;
	qboolean showAll = false;

	if(Cmd_Argc() > 1)
	{
		showAll = true;
	}

	for (i = 0; i < MAX_SERVERS; i++)
	{
		if(browserList[i].hostname[0] != 0)
		{
			if (browserList[i].curPlayers > 0)
			{
				Con_Printf("%02d:  %s:%d [%d] %s ", num_active_servers+1, browserList[i].ip, browserList[i].port, browserList[i].ping, browserList[i].hostname);
				Con_Printf("\x02%d", browserList[i].curPlayers); /* FS: Show the current players number in the green font */
				Con_Printf("/%d %s\n", browserList[i].maxPlayers, browserList[i].mapname);
				num_active_servers++;
			}
		}
		else /* FS: if theres nothing there the rest of the list is old garbage, bye. */
		{
			break;
		}
	}

	if (showAll)
	{
		int skip = 0;

		for (i = 0; i < MAX_SERVERS; i++)
		{
			if(browserListAll[i].hostname[0] != 0)
			{
				Con_Printf("%02d:  %s:%d [%d] %s %d/%d %s\n", (i+num_active_servers+1)-(skip), browserListAll[i].ip, browserListAll[i].port, browserListAll[i].ping, browserListAll[i].hostname, browserListAll[i].curPlayers, browserListAll[i].maxPlayers, browserListAll[i].mapname);
			}
			else /* FS: The next one could be 0 if we skipped over it previously in GameSpy_Sort_By_Ping.  So increment the number of skips counter so the server number shows sequentially */
			{
				skip++;
				continue;
			}
		}
	}
}

static void GameSpy_Sort_By_Ping(GServerList lst)
{
	int i;
	gspyCur = 0;

	for (i = 0; i < cls.gamespytotalservers; i++)
	{
		GServer server = ServerListGetServer(lst, i);
		if (server)
		{
			if(ServerGetIntValue(server, "numplayers", 0) <= 0)
				continue;

			if(i == MAX_SERVERS)
				break;

			Q_strlcpy(browserList[gspyCur].ip, ServerGetAddress(server), sizeof(browserList[gspyCur].ip));
			browserList[gspyCur].port = ServerGetQueryPort(server);
			browserList[gspyCur].ping = ServerGetPing(server);
			Q_strlcpy(browserList[gspyCur].hostname, ServerGetStringValue(server, "hostname","(NONE)"), sizeof(browserList[gspyCur].hostname));
			Q_strlcpy(browserList[gspyCur].mapname, ServerGetStringValue(server,"map","(NO MAP)"), sizeof(browserList[gspyCur].mapname));
			browserList[gspyCur].curPlayers = ServerGetIntValue(server,"numplayers",0);
			browserList[gspyCur].maxPlayers = ServerGetIntValue(server,"maxclients",0);

			gspyCur++;
		}
	}

	for (i = 0; i < cls.gamespytotalservers; i++)
	{
		GServer server = ServerListGetServer(lst, i);

		if (server)
		{
			if(ServerGetIntValue(server, "numplayers", 0) > 0) /* FS: We already added this so skip it */
			{
				continue;
			}

			if(Q_strncmp(ServerGetStringValue(server, "hostname","(NONE)"), "(NONE)", 6) == 0) /* FS: A server that timed-out or we aborted early */
			{
				continue;
			}

			if(i == MAX_SERVERS)
				break;

			Q_strlcpy(browserListAll[i].ip, ServerGetAddress(server), sizeof(browserListAll[i].ip));
			browserListAll[i].port = ServerGetQueryPort(server);
			browserListAll[i].ping = ServerGetPing(server);
			Q_strlcpy(browserListAll[i].hostname, ServerGetStringValue(server, "hostname","(NONE)"), sizeof(browserListAll[i].hostname));
			Q_strlcpy(browserListAll[i].mapname, ServerGetStringValue(server,"map","(NO MAP)"), sizeof(browserListAll[i].mapname));
			browserListAll[i].curPlayers = ServerGetIntValue(server,"numplayers",0);
			browserListAll[i].maxPlayers = ServerGetIntValue(server,"maxclients",0);
		}
	}
}

static void ListCallBack(GServerList lst, int msg, void *instance, void *param1, void *param2)
{
	GServer server;
	int percent;
	int numplayers;

	if (msg == LIST_PROGRESS)
	{
		server = (GServer)param1;
		numplayers = ServerGetIntValue(server,"numplayers",0);

		if(numplayers > 0) /* FS: Only show populated servers */
		{
			if (key_dest != key_menu) /* FS: Only print this from an slist2 command, not the server browser. */
			{
				Con_Printf("%s:%d [%d] %s ", ServerGetAddress(server), ServerGetQueryPort(server), ServerGetPing(server), ServerGetStringValue(server, "hostname","(NONE)"));
				Con_Printf("\x02%d", numplayers); /* FS: Show the current players number in the green font */
				Con_Printf("/%d %s\n", ServerGetIntValue(server,"maxclients",0), ServerGetStringValue(server,"map","(NO MAP)"));
			}
		}
		else if (cls.gamespyupdate == SHOW_ALL_SERVERS)
		{
			if (key_dest != key_menu) /* FS: Only print this from an slist2 command, not the server browser. */
			{
				Con_Printf("%s:%d [%d] %s %d/%d %s\n", ServerGetAddress(server), ServerGetQueryPort(server), ServerGetPing(server), ServerGetStringValue(server, "hostname","(NONE)"), ServerGetIntValue(server,"numplayers",0), ServerGetIntValue(server,"maxclients",0), ServerGetStringValue(server,"map","(NO MAP)"));
			}
		}

		if(param2)
		{
			percent = (int)(param2);
			cls.gamespypercent = percent;
		}
	}
	else if (msg == LIST_STATECHANGED)
	{
		switch(ServerListState(lst))
		{
			case sl_idle:
				ServerListSort(lst, true, "ping", cm_int);
				GameSpy_Sort_By_Ping(lst);
				break;
			default:
				break;
		}
	}
}

void CL_PingNetServers_f (void)
{
	char goa_secret_key[7];
	int error;
	int allocatedSockets;

	if(cls.gamespyupdate)
	{
		Con_Printf("Error: Already querying the GameSpy Master!\n");
		return;
	}

	gspyCur = 0;
	memset(&browserList, 0, sizeof(browserList));
	memset(&browserListAll, 0, sizeof(browserListAll));

	goa_secret_key[0] = 'F';
	goa_secret_key[1] = 'U';
	goa_secret_key[2] = '6';
	goa_secret_key[3] = 'V';
	goa_secret_key[4] = 'q';
	goa_secret_key[5] = 'n';
	goa_secret_key[6] = '\0'; /* FS: Gamespy requires a null terminator at the end of the secret key */

	if ((Cmd_Argc() == 1) || (key_dest == key_menu))
	{
		cls.gamespyupdate = SHOW_POPULATED_SERVERS;;
		Con_Printf("\x02Grabbing populated server list from GameSpy master. . .\n");
	}
	else
	{
		cls.gamespyupdate = SHOW_ALL_SERVERS;
		Con_Printf("\x02Grabbing all servers from GameSpy master. . .\n");
	}

	cls.gamespypercent = 0;
	cls.gamespystarttime = (int)Sys_DoubleTime();
	cls.gamespytotalservers = 0;

	allocatedSockets = bound(5, cl_master_server_queries.intValue, 100);

	SCR_UpdateScreen(); /* FS: Force an update so the percentage bar shows some progress */

	serverlist = ServerListNew("quakeworld","quakeworld",goa_secret_key,allocatedSockets,ListCallBack,GCALLBACK_FUNCTION,NULL);
	error = ServerListUpdate(serverlist,true); /* FS: Use Async now! */

	CL_Gamespy_Check_Error(serverlist, error);
}
