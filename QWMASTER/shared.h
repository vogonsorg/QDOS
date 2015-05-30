#ifndef SHARED_H
#define SHARED_H

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "DG_misc.h" // FS: Caedes special safe string stuff
#include "curl_dl.h" // FS: Libcurl

#define SRV_RUN		1
#define SRV_STOP	0
#define SRV_STOPPED	-1

#define VERSION "0.2f"

#ifndef NULL
#define NULL 0
#endif

#define KEY_LEN 32	// give us some space
#define MAXPENDING 16 // FS: Max pending TCP connections
#define MAX_INCOMING_LEN 1400 // FS: made this a #define, also gamespy doesnt send anything larger than 1024.  So make this close to the average maximum MTU size
#define MAX_GAMENAME_LEN 16 // FS: Max gamename length used for game table and server structs
#define DEFAULTHEARTBEAT 5*60 // FS: 5 minutes

#define MAXPRINTMSG 16384
#define MAX_INFO_STRING 64
#define MAX_DNS_NAMELEN 254
#define MAX_PORT_LEN 5
#define MAX_SERVERLIST_LINE MAX_DNS_NAMELEN+1+MAX_PORT_LEN+1+MAX_GAMENAME_LEN // FS: 1 == ',' separator
#define MAX_GSPY_VAL 89 // FS: See gsmalg.cpp

//
// These are Windows specific but need to be defined here so GCC won't barf
//
#define REGKEY_Q2MASTERSERVER "SOFTWARE\\DKMasterServer" // Our config data goes here
#define REGKEY_BIND_IP "Bind_IP"
#define REGKEY_BIND_PORT "Bind_Port"
#define REGKEY_BIND_PORT_TCP "Bind_Port_TCP" // FS: For gamespy TCP port

// Out of band data preamble
#define OOB_SEQ "\xff\xff\xff\xff" //32 bit integer (-1) as string sequence for out of band data
#define serverstring OOB_SEQ"servers " // 12 bytes for the serverstring header

extern int Debug;
extern int Timestamp;
extern int pinging;

// Knightmare 05/27/12- buffer-safe variant of vsprintf
// This may be different on different platforms, so it's abstracted
#ifdef _MSC_VER	// _WIN32
//#define DK_vsnprintf _vsnprintf	
__inline int DK_vsnprintf (char *Dest, size_t Count, const char *Format, va_list Args) {
	int ret = _vsnprintf(Dest, Count, Format, Args);
	Dest[Count-1] = 0;	// null terminate
	return ret;
}
#else
#define DK_vsnprintf vsnprintf	
#endif // _MSC_VER

#endif // SHARED_H
