/* FS: From Q2 */
#ifdef __DJGPP__
#include <dos.h>
#include <dir.h>
#include "quakedef.h"

static	struct ffblk	finddata;
static	int	findhandle = -1;
static	char	findbase[MAX_OSPATH];
static	char	findpath[MAX_OSPATH];

static qboolean CompareAttributes(const struct ffblk *ff,
				  unsigned musthave, unsigned canthave)
{
	/* . and .. never match */
	if (strcmp(ff->ff_name, ".") == 0 || strcmp(ff->ff_name, "..") == 0)
		return false;

	if (ff->ff_attrib & _A_VOLID) /* shouldn't happen */
		return false;

	if (ff->ff_attrib & _A_SUBDIR) {
		if (canthave & SFF_SUBDIR)
			return false;
	}
	else {
		if (musthave & SFF_SUBDIR)
			return false;
	}

	if (ff->ff_attrib & _A_RDONLY) {
		if (canthave & SFF_RDONLY)
			return false;
	}
	else {
		if (musthave & SFF_RDONLY)
			return false;
	}

	if (ff->ff_attrib & _A_HIDDEN) {
		if (canthave & SFF_HIDDEN)
			return false;
	}
	else {
		if (musthave & SFF_HIDDEN)
			return false;
	}

	if (ff->ff_attrib & _A_SYSTEM) {
		if (canthave & SFF_SYSTEM)
			return false;
	}
	else {
		if (musthave & SFF_SYSTEM)
			return false;
	}

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave)
{
	int attribs;

	if (findhandle == 0)
		Sys_Error ("Sys_BeginFind without close");

	COM_FilePath (path, findbase);
	memset (&finddata, 0, sizeof(finddata));

	attribs = FA_ARCH|FA_RDONLY;
	if (!(canthave & SFF_SUBDIR))
		attribs |= FA_DIREC;
	if (musthave & SFF_HIDDEN)
		attribs |= FA_HIDDEN;
	if (musthave & SFF_SYSTEM)
		attribs |= FA_SYSTEM;

	findhandle = findfirst(path, &finddata, attribs);
	if (findhandle != 0)
		return NULL;
	if (CompareAttributes(&finddata, musthave, canthave)) {
		sprintf (findpath, "%s/%s", findbase, finddata.ff_name);
		return findpath;
	}
	return Sys_FindNext(musthave, canthave);
}

char *Sys_FindNext (unsigned musthave, unsigned canthave)
{
	if (findhandle != 0)
		return NULL;

	while (findnext(&finddata) == 0) {
		if (CompareAttributes(&finddata, musthave, canthave)) {
			sprintf (findpath, "%s/%s", findbase, finddata.ff_name);
			return findpath;
		}
	}

	return NULL;
}

void Sys_FindClose (void)
{
	findhandle = -1;
}
#endif /* __DJGPP__ */

#ifdef WIN32
#include "quakedef.h"
#include "winquake.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>
#include <conio.h>

static char	findbase[MAX_OSPATH];
static char	findpath[MAX_OSPATH];
static int		findhandle;

static qboolean CompareAttributes( unsigned found, unsigned musthave, unsigned canthave )
{
	if ( ( found & _A_RDONLY ) && ( canthave & SFF_RDONLY ) )
		return false;
	if ( ( found & _A_HIDDEN ) && ( canthave & SFF_HIDDEN ) )
		return false;
	if ( ( found & _A_SYSTEM ) && ( canthave & SFF_SYSTEM ) )
		return false;
	if ( ( found & _A_SUBDIR ) && ( canthave & SFF_SUBDIR ) )
		return false;
	if ( ( found & _A_ARCH ) && ( canthave & SFF_ARCH ) )
		return false;

	if ( ( musthave & SFF_RDONLY ) && !( found & _A_RDONLY ) )
		return false;
	if ( ( musthave & SFF_HIDDEN ) && !( found & _A_HIDDEN ) )
		return false;
	if ( ( musthave & SFF_SYSTEM ) && !( found & _A_SYSTEM ) )
		return false;
	if ( ( musthave & SFF_SUBDIR ) && !( found & _A_SUBDIR ) )
		return false;
	if ( ( musthave & SFF_ARCH ) && !( found & _A_ARCH ) )
		return false;

	return true;
}

char *Sys_FindFirst (char *path, unsigned musthave, unsigned canthave )
{
	struct _finddata_t findinfo;

	if (findhandle)
		Sys_Error ("Sys_FindFirst without close");
	findhandle = 0;

	COM_FilePath (path, findbase);

	findhandle = _findfirst (path, &findinfo);
	if (findhandle == -1)
		return NULL;
	if ( !CompareAttributes( findinfo.attrib, musthave, canthave ) )
		return NULL;
	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, findinfo.name);
	return findpath;
}

char *Sys_FindNext ( unsigned musthave, unsigned canthave )
{
	struct _finddata_t findinfo;

	if (findhandle == -1)
		return NULL;
	if (_findnext (findhandle, &findinfo) == -1)
		return NULL;
	if ( !CompareAttributes( findinfo.attrib, musthave, canthave ) )
		return NULL;

	Com_sprintf (findpath, sizeof(findpath), "%s/%s", findbase, findinfo.name);
	return findpath;
}

void Sys_FindClose (void)
{
	if (findhandle != -1)
		_findclose (findhandle);
	findhandle = 0;
}
#endif /* WIN32 */

