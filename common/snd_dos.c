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

/* snddma_dos.c -- interface to dos audio drivers. */

#include <dos.h>
#include "quakedef.h"
#include "dosisms.h"

typedef enum
{
	dma_none,
	dma_blaster,
	dma_gus,
#ifdef USE_SNDPCI
	dma_pci /* FS: From Ruslan's patch */
#endif
} dmacard_t;

static dmacard_t	dmacard;
/* GUS SUPPORT */
qboolean GUS_Init (void);
int GUS_GetDMAPos (void);
void GUS_Shutdown (void);

/* SOUND BLASTER SUPPORT */
qboolean BLASTER_Init (void);
int BLASTER_GetDMAPos (void);
void BLASTER_Shutdown (void);

#ifdef USE_SNDPCI
/* PCI SOUND CARD SUPPORT */
qboolean PCI_Init(void);
int PCI_GetDMAPos(void);
void PCI_Shutdown(void);
#endif

/*
===============================================================================

INTERFACE

===============================================================================
*/

static void snd_shutdown_f (void) /* FS: SND_SHUTDOWN */
{
	SNDDMA_Shutdown();
	Con_Printf("\nSound Shutdown.\n");
	Cache_Flush();
}

static void snd_restart_f (void) /* FS: Added */
{
	SNDDMA_Shutdown();
	Con_Printf("\nSound Restarting\n");
	Cache_Flush();
	SNDDMA_Init();
	S_StopAllSoundsC(); /* FS: For GUS Clear buffer fix */
	Con_Printf ("Sound sampling rate: %i\n", shm->speed);
}

qboolean SNDDMA_Init(void)
{
	if (COM_CheckParm("-nosound"))
		goto nocard;
	if (!host_initialized)
	{
		Cmd_AddCommand ("snd_restart", snd_restart_f); /* FS: Added */
		Cmd_AddCommand ("snd_shutdown", snd_shutdown_f); /* FS: Added */
	}
#ifdef USE_SNDPCI
	if (PCI_Init ()) /* FS: Ruslans patch */
	{
		Con_DPrintf(DEVELOPER_MSG_SOUND, "PCI_Init\n");
		dmacard = dma_pci;
		return true;
	}
#endif
	if (GUS_Init ())
	{
		Con_DPrintf(DEVELOPER_MSG_SOUND, "GUS_Init\n");
		dmacard = dma_gus;
		S_StopAllSoundsC(); /* FS: For GUS Buffer Clear Fix */
		return true;
	}
	if (BLASTER_Init ())
	{
		Con_DPrintf(DEVELOPER_MSG_SOUND, "BLASTER_Init\n");
		dmacard = dma_blaster;
		return true;
	}
nocard:
	dmacard = dma_none;
	return false;
}

int	SNDDMA_GetDMAPos(void)
{
	switch (dmacard)
	{
	case dma_blaster:
		return BLASTER_GetDMAPos ();
	case dma_gus:
		return GUS_GetDMAPos ();
#ifdef USE_SNDPCI
	case dma_pci: /* FS: Ruslans patch */
		return PCI_GetDMAPos ();
#endif
	case dma_none:
		break;
	}
	return 0;
}

void SNDDMA_Shutdown(void)
{
	switch (dmacard)
	{
	case dma_blaster:
		BLASTER_Shutdown ();
		break;
	case dma_gus:
		GUS_Shutdown ();
		break;
#ifdef USE_SNDPCI
	case dma_pci: /* FS: Ruslans patch */
		PCI_Shutdown ();
		break;
#endif
	case dma_none:
		break;
	}

	shm->buffer = NULL;
	dmacard = dma_none;
}

void SNDDMA_Submit(void)
{
	/* Nothing in DOS */
}

