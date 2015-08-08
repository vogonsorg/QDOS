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
// comndef.h  -- general definitions

#include "DG_misc.h" /* FS: Caedes special safe string stuff */

#ifdef _MSC_VER /* FS: VS6 */
	#define __attribute__(x)
#endif

typedef unsigned char           byte;
#define _DEF_BYTE_

// KJB Undefined true and false defined in SciTech's DEBUG.H header
#undef true
#undef false

typedef enum {false, true}	qboolean;

#define MAX_INFO_STRING 1024 /* FS: Was 196 */
#define MAX_SERVERINFO_STRING 2048  /* FS: Was 1024 */
#define MAX_LOCALINFO_STRING    32768 /* JASON upped from 512 -- Spoike */

#define qbool qboolean /* FS: Tired of importing code from EZQ and getting errors related to this */

//============================================================================

typedef struct sizebuf_s
{
	qboolean	allowoverflow;	// if false, do a Sys_Error
	qboolean	overflowed;		// set to true if the buffer size failed
	byte		*data;
	int			maxsize;
	int			cursize;
} sizebuf_t;

void SZ_InitEx (sizebuf_t *buf, byte *data, int length, qbool allowoverflow); /* FS: From EZQ */
void SZ_Init (sizebuf_t *buf, byte *data, int length); /* FS: From EZQ */
void SZ_Clear (sizebuf_t *buf);
void *SZ_GetSpace (sizebuf_t *buf, int length);
void SZ_Write (sizebuf_t *buf, void *data, int length);
void SZ_Print (sizebuf_t *buf, char *data);	// strcats onto the sizebuf

//============================================================================

typedef struct link_s
{
	struct link_s	*prev, *next;
} link_t;


void ClearLink (link_t *l);
void RemoveLink (link_t *l);
void InsertLinkBefore (link_t *l, link_t *before);
void InsertLinkAfter (link_t *l, link_t *after);

// (type *)STRUCT_FROM_LINK(link_t *link, type, member)
// ent = STRUCT_FROM_LINK(link,entity_t,order)
// FIXME: remove this mess!
#define	STRUCT_FROM_LINK(l,t,m) ((t *)((byte *)l - (int)&(((t *)0)->m)))

//============================================================================

#ifndef NULL
#define NULL ((void *)0)
#endif

#define Q_MAXCHAR	((char)0x7f)
#define Q_MAXSHORT	((short)0x7fff)
#define Q_MAXINT	((int)0x7fffffff)
#define Q_MAXLONG	((int)0x7fffffff)
#define Q_MAXFLOAT	((int)0x7fffffff)

#define Q_MINCHAR	((char)0x80)
#define Q_MINSHORT	((short)0x8000)
#define Q_MININT	((int)0x80000000)
#define Q_MINLONG	((int)0x80000000)
#define Q_MINFLOAT	((int)0x7fffffff)

//============================================================================

extern	qboolean	bigendien;

extern	short	(*BigShort) (short l);
extern	short	(*LittleShort) (short l);
extern	int		(*BigLong) (int l);
extern	int		(*LittleLong) (int l);
extern	float	(*BigFloat) (float l);
extern	float	(*LittleFloat) (float l);

//============================================================================

/* FS: Gamespy stuff */
#define SHOW_POPULATED_SERVERS 1
#define SHOW_ALL_SERVERS 2

/* FS: FIXME TODO HACK.  The MSG_Write/Read for PEXT_FLOATCOORDS is read before protocol.h, so i'm putting it here. */
// fte protocol extensions.
#define PROTOCOL_VERSION_FTE	(('F'<<0) + ('T'<<8) + ('E'<<16) + ('X' << 24)) //fte extensions.

#ifdef PROTOCOL_VERSION_FTE 

//#define	FTE_PEXT_TRANS				0x00000008	// .alpha support
//#define FTE_PEXT_ACCURATETIMINGS	0x00000040
#define FTE_PEXT_HLBSP				0x00000200	//stops fte servers from complaining
#define FTE_PEXT_MODELDBL			0x00001000
#define FTE_PEXT_ENTITYDBL			0x00002000	//max of 1024 ents instead of 512
#define FTE_PEXT_ENTITYDBL2			0x00004000	//max of 1024 ents instead of 512
#define FTE_PEXT_FLOATCOORDS		0x00008000	//supports floating point origins.
#define FTE_PEXT_SPAWNSTATIC2		0x00400000	//Sends an entity delta instead of a baseline.
#define FTE_PEXT_256PACKETENTITIES	0x01000000	//Client can recieve 256 packet entities.
#define FTE_PEXT_CHUNKEDDOWNLOADS	0x20000000	//alternate file download method. Hopefully it'll give quadroupled download speed, especially on higher pings.

#endif

#ifdef FTE_PEXT_FLOATCOORDS

typedef union {	//note: reading from packets can be misaligned
	int b4;
	float f;
	short b2;
	char b[4];
} coorddata;

extern int msg_coordsize; // 2 or 4.
extern int msg_anglesize; // 1 or 2.

float MSG_FromCoord(coorddata c, int bytes);
coorddata MSG_ToCoord(float f, int bytes);	//return value should be treated as (char*)&ret;
coorddata MSG_ToAngle(float f, int bytes);	//return value is NOT byteswapped.

#endif

struct usercmd_s;

extern struct usercmd_s nullcmd;

void MSG_WriteChar (sizebuf_t *sb, int c);
void MSG_WriteByte (sizebuf_t *sb, int c);
void MSG_WriteShort (sizebuf_t *sb, int c);
void MSG_WriteLong (sizebuf_t *sb, int c);
void MSG_WriteFloat (sizebuf_t *sb, float f);
void MSG_WriteString (sizebuf_t *sb, char *s);
void MSG_WriteCoord (sizebuf_t *sb, float f);
void MSG_WriteAngle (sizebuf_t *sb, float f);
void MSG_WriteAngle16 (sizebuf_t *sb, float f);
void MSG_WriteDeltaUsercmd (sizebuf_t *sb, struct usercmd_s *from, struct usercmd_s *cmd);
void MSG_ReadData (void *data, int len); /* FS: For chunked downloads */

extern	int			msg_readcount;
extern	qboolean	msg_badread;		// set if a read goes beyond end of message

void MSG_BeginReading (void);
int MSG_GetReadCount(void);
int MSG_ReadChar (void);
int MSG_ReadByte (void);
int MSG_ReadShort (void);
int MSG_ReadLong (void);
float MSG_ReadFloat (void);
char *MSG_ReadString (void);
char *MSG_ReadStringLine (void);

float MSG_ReadCoord (void);
float MSG_ReadAngle (void);
float MSG_ReadAngle16 (void);
void MSG_ReadDeltaUsercmd (struct usercmd_s *from, struct usercmd_s *cmd);

//============================================================================

#define Q_memset(d, f, c) memset((d), (f), (c))
#define Q_memcpy(d, s, c) memcpy((d), (s), (c))
#define Q_memcmp(m1, m2, c) memcmp((m1), (m2), (c))
#define Q_strcpy(d, s) strcpy((d), (s))
#define Q_strncpy(d, s, n) strncpy((d), (s), (n))
#define Q_strlen(s) ((int)strlen(s))
#define Q_strrchr(s, c) strrchr((s), (c))
#define Q_strcat(d, s) strcat((d), (s))
#define Q_strcmp(s1, s2) strcmp((s1), (s2))
#define Q_strncmp(s1, s2, n) strncmp((s1), (s2), (n))

#ifdef _WIN32

#define Q_strcasecmp(s1, s2) _stricmp((s1), (s2))
#define Q_strncasecmp(s1, s2, n) _strnicmp((s1), (s2), (n))

#else

#define Q_strcasecmp(s1, s2) strcasecmp((s1), (s2))
#define Q_strncasecmp(s1, s2, n) strncasecmp((s1), (s2), (n))

#endif

int	Q_atoi (char *str);
float Q_atof (char *str);
size_t Q_strlcpy (char *dst, const char *src, size_t siz); /* FS: From OpenBSD */

//============================================================================

extern	char		com_token[1024];
extern	qboolean	com_eof;

char *COM_Parse (char *data);


extern	int		com_argc;
extern	char	**com_argv;

int COM_CheckParm (char *parm);
void COM_AddParm (char *parm);

void COM_Init (void);
void COM_InitArgv (int argc, char **argv);

char *COM_SkipPath (char *pathname);
void COM_StripExtension (char *in, char *out);
void COM_FileBase (char *in, char *out);
void COM_DefaultExtension (char *path, char *extension);

// does a varargs printf into a temp buffer
char	*va(const char *format, ...) __attribute__((format(printf,1,2)));
// does a varargs printf into a malloced buffer
char	*nva(const char *format, ...) __attribute__((format(printf,1,2)));

//============================================================================

extern int com_filesize;
struct cache_user_s;

extern	char	com_gamedir[MAX_OSPATH];

void COM_WriteFile (char *filename, void *data, int len);
int COM_OpenFile (char *filename, int *hndl); /* FS: From Q1 */
int COM_FOpenFile (char *filename, FILE **file);

byte *COM_LoadStackFile (char *path, void *buffer, int bufsize);
byte *COM_LoadTempFile (char *path);
byte *COM_LoadHunkFile (char *path);
void COM_LoadCacheFile (char *path, struct cache_user_s *cu);
void COM_CreatePath (char *path);
void COM_Gamedir (char *dir);

/* FS: From Q2 */
char *COM_NextPath (char *prevpath);
void COM_FreeFileList (char **list, int n);
qboolean COM_ItemInList (char *check, int num, char **list);
char **COM_ListFiles( char *findname, int *numfiles, unsigned musthave, unsigned canthave );

extern	struct cvar_s	registered;
extern	qboolean		standard_quake, rogue, hipnotic;

char *Info_ValueForKey (char *s, char *key);
void Info_RemoveKey (char *s, char *key);
void Info_RemovePrefixedKeys (char *start, char prefix);
void Info_SetValueForKey (char *s, char *key, char *value, int maxsize);
void Info_SetValueForStarKey (char *s, char *key, const char *value, int maxsize);
void Info_Print (char *s);

unsigned Com_BlockChecksum (void *buffer, int length);
void Com_BlockFullChecksum (void *buffer, int len, unsigned char *outbuf);
byte    COM_BlockSequenceCheckByte (byte *base, int length, int sequence, unsigned mapchecksum);
byte    COM_BlockSequenceCRCByte (byte *base, int length, int sequence);

int build_number( void );
void CompleteCommand (void); /* FS: Autocomplete commands */
void Com_sprintf(dstring_t *dst, const char *fmt, ...); /* FS: Added */
void Com_strcpy (char *dest, int destSize, const char *src); /* FS: Added */