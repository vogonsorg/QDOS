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

char		allskins[128];
#define	MAX_CACHED_SKINS		128
skin_t		skins[MAX_CACHED_SKINS];
int			numskins;

/* FS: For Download skin queue checking */
typedef struct skinqueue_s
{
	char name[MAX_QPATH];
	qboolean queued;
} skinqueue_t;
skinqueue_t *queued_skins;

/*
================
Skin_Find

  Determines the best skin for the given scoreboard
  slot, and sets scoreboard->skin

================
*/
void Skin_Find (player_info_t *sc)
{
	skin_t *skin;
	int i;
	char name[128], *s;


	if (allskins[0])
		strcpy (name, allskins);
	else
	{
		s = Info_ValueForKey (sc->userinfo, "skin");
		if (s && s[0])
			strcpy (name, s);
		else
		strcpy (name, baseskin->string);
	}

	if (strstr (name, "..") || *name == '.')
		strcpy (name, "base");
	COM_StripExtension (name, name);

	for (i=0 ; i<numskins ; i++)
	{
		if (!strcmp (name, skins[i].name))
		{
			sc->skin = &skins[i];
			Skin_Cache (sc->skin);
			return;
		}
	}

	if (numskins == MAX_CACHED_SKINS)
	{	// ran out of spots, so flush everything
		Skin_Skins_f ();
		return;
	}

	skin = &skins[numskins];
	sc->skin = skin;
	numskins++;

	memset (skin, 0, sizeof(*skin));
	strncpy(skin->name, name, sizeof(skin->name) - 1);
}


/*
==========
Skin_Cache

Returns a pointer to the skin bitmap, or NULL to use the default
==========
*/
byte	*Skin_Cache (skin_t *skin)
{
	dstring_t *name;
	byte	*raw;
	byte	*out, *pix;
	pcx_t	*pcx;
	int		x = 0; /* FS: Compiler warning */
	int		y;
	int		dataByte;
	int		runLength;

	name = dstring_new();
	
	if (cls.downloadtype == dl_skin)
	{
		dstring_delete(name);
		return NULL;		// use base until downloaded
	}

	if (!allow_download_skins->intValue) /* FS: Was noskins */
	// JACK: So NOSKINS > 1 will show skins, but
	{
		dstring_delete(name);
		return NULL;	  // not download new ones.
	}

	if (skin->failedload)
	{
		dstring_delete(name);
		return NULL;
	}
	out = Cache_Check (&skin->cache);
	if (out)
	{
		dstring_delete(name);
		return out;
	}
//
// load the pic from disk
//
	dsprintf (name, "skins/%s.pcx", skin->name);

	Con_DPrintf (DEVELOPER_MSG_IO, "Loading skin: %s\n", name->str); /* FS */

	raw = COM_LoadTempFile (name->str);
	if (!raw)
	{
		Con_Printf ("Couldn't load skin %s\n", name->str);
		dsprintf (name, "skins/%s.pcx", baseskin->string);
		raw = COM_LoadTempFile (name->str);
		if (!raw)
		{
			skin->failedload = true;
			dstring_delete(name);
			return NULL;
		}
	}

//
// parse the PCX file
//
	pcx = (pcx_t *)raw;
	raw = &pcx->data;

	if (pcx->manufacturer != 0x0a
		|| pcx->version != 5
		|| pcx->encoding != 1
		|| pcx->bits_per_pixel != 8
		|| pcx->xmax >= 320
		|| pcx->ymax >= MAX_LBM_HEIGHT) /* FS: Was >= 200 */
	{
		skin->failedload = true;
		Con_Printf ("Bad skin %s\n", name->str);
		dstring_delete(name);
		return NULL;
	}
	
	out = Cache_Alloc (&skin->cache, 320*MAX_LBM_HEIGHT, skin->name); /* FS: Was 320*200 */
	if (!out)
		Sys_Error ("Skin_Cache: couldn't allocate");

	pix = out;
	memset (out, 0, 320*MAX_LBM_HEIGHT); /* FS: Was 320*200 */

	for (y=0 ; y<pcx->ymax ; y++, pix += 320)
	{
		for (x=0 ; x<=pcx->xmax ; )
		{
			if (raw - (byte*)pcx > com_filesize) 
			{
				Cache_Free (&skin->cache);
				skin->failedload = true;
				Con_Printf ("Skin %s was malformed.  You should delete it.\n", name->str);
				dstring_delete(name);
				return NULL;
			}
			dataByte = *raw++;

			if((dataByte & 0xC0) == 0xC0)
			{
				runLength = dataByte & 0x3F;
				if (raw - (byte*)pcx > com_filesize) 
				{
					Cache_Free (&skin->cache);
					skin->failedload = true;
					Con_Printf ("Skin %s was malformed.  You should delete it.\n", name->str);
					dstring_delete(name);
					return NULL;
				}
				dataByte = *raw++;
			}
			else
				runLength = 1;

			// skin sanity check
			if (runLength + x > pcx->xmax + 2) {
				Cache_Free (&skin->cache);
				skin->failedload = true;
				Con_Printf ("Skin %s was malformed.  You should delete it.\n", name->str);
				dstring_delete(name);
				return NULL;
			}
			while(runLength-- > 0)
				pix[x++] = dataByte;
		}

	}

	Con_DPrintf(DEVELOPER_MSG_IO, "Skin: %s, Size: %d, Width: %d\n", name->str, com_filesize, x); /* FS */
	
	if ( raw - (byte *)pcx > com_filesize)
	{
		Cache_Free (&skin->cache);
		skin->failedload = true;
		Con_Printf ("Skin %s was malformed.  You should delete it.\n", name->str);
		dstring_delete(name);
		return NULL;
	}

	skin->failedload = false;
	dstring_delete(name);
	return out;
}

void Skin_CheckQueue (char *name) /* FS: Check if we already queued this for download so huge custom TF servers don't show 20+ skins to grab though we actually need like 8 */
{
	int i;

	if(!name)
		return;

	strcpy(queued_skins[cls.downloadnumber].name, name);

	for(i = 0; i <= cls.downloadnumber; i++)
	{
		if(!queued_skins[i].name[0])
			continue;

		if(!strcmp(name, queued_skins[i].name))
		{
			if(!queued_skins[i].queued) /* FS: Okay, this bad boy isn't queued up yet, so let's increase the counter */
			{
				queued_skins[i].queued = true;
				cls.download_queue_total++;
			}
			else /* FS: We already got this queued for later, don't go any further */
				break;
		}
	}
}

/*
=================
Skin_NextDownload
=================
*/
void Skin_NextDownload (qboolean queue)
{
	player_info_t	*sc;

	if (queue && cls.downloadnumber == 0)
	{
		Con_Printf ("Checking skins...\n");
		cls.download_queue = cls.download_queue_total = 0;

		queued_skins = (skinqueue_t *)calloc(MAX_CACHED_SKINS, sizeof(skinqueue_t)); /* FS: FIXME: Should this be MAX_CLIENTS? */
		if(!queued_skins)
			Sys_Error("Failed to crated skin queue buffer!");
	}
	cls.downloadtype = dl_skin;

	for ( 
		; cls.downloadnumber != MAX_CLIENTS
		; cls.downloadnumber++)
	{
		sc = &cl.players[cls.downloadnumber];
		if (!sc->name[0])
			continue;
		if(!queue)
			Skin_Find (sc);
		if (!allow_download_skins->intValue)
			continue;
		if (queue)
		{
			if (!CL_CheckOrDownloadFile(va("skins/%s.pcx", sc->skin->name), true)) /* FS: Queue a download */
				Skin_CheckQueue(sc->skin->name);
		}
		else
		{
			if (!CL_CheckOrDownloadFile(va("skins/%s.pcx", sc->skin->name), false)) /* FS: Start a download */
				return;
		}
	}

	if (queue) /* FS: OK, we tallied how many assets we need... Start downloading */
	{
		cls.downloadnumber = 0;
		if(queued_skins)
			free(queued_skins);
		Skin_NextDownload(false);
	}
	else
		Skin_Precache();
}

void Skin_Precache (void)
{
	player_info_t	*sc;
	int i;

	cls.downloadtype = dl_none;

	// now load them in for real
	for (i=0 ; i<MAX_CLIENTS ; i++)
	{
		sc = &cl.players[i];
		if (!sc->name[0])
			continue;
		Skin_Cache (sc->skin);
#ifdef GLQUAKE
		sc->skin = NULL;
#endif
        }

	if (cls.state != ca_active)
	{	// get next signon phase
		MSG_WriteByte (&cls.netchan.message, clc_stringcmd);
		MSG_WriteString (&cls.netchan.message,
			va("begin %i", cl.servercount));
		Cache_Report ();		// print remaining memory
	}
}


/*
==========
Skin_Skins_f

Refind all skins, downloading if needed.
==========
*/
void	Skin_Skins_f (void)
{
	int		i;

	for (i=0 ; i<numskins ; i++)
	{
		if (skins[i].cache.data)
			Cache_Free (&skins[i].cache);
	}
	numskins = 0;

	if (cls.state == ca_disconnected) /* FS: QuakeForge fix */
		return;

	cls.downloadnumber = 0;
	cls.downloadtype = dl_skin;
	Skin_NextDownload (true);
}


/*
==========
Skin_AllSkins_f

Sets all skins to one specific one
==========
*/
void	Skin_AllSkins_f (void)
{
	strcpy (allskins, Cmd_Argv(1));
	Skin_Skins_f ();
}
