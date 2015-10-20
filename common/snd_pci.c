/* snd_pci.c -- PCI sound card support through libau */

#ifdef USE_SNDPCI

#include "quakedef.h"
#include "libau.h"

static au_context *ctx = NULL;

/*
==================
PCI_Init

Returns false if nothing is found.
==================
*/
qboolean PCI_Init(void)
{
	const struct auinfo_s *aui;
	unsigned int speed;
	unsigned int samplebits;
	unsigned int channels;

	if (!COM_CheckParm("-sndpci"))
		return false;

	ctx = AU_search(1);/* 1: stereo speaker output (meaningful only for Intel HDA chips) */
	if (!ctx)
	{
		Con_Printf("PCI Audio: Detection failed.\n");
		return false;
	}

	speed = 22050;
	samplebits = 16;
	channels = 2;
	if (s_khz->intValue >= 11025) /* FS */
		speed = s_khz->intValue;
	AU_setrate(ctx, &speed, &samplebits, &channels);

	aui = AU_getinfo(ctx);

	shm = &sn;
	shm->speed = aui->freq_card;
	shm->samplebits = aui->bits_set;
	shm->channels = aui->chan_set;
	if (shm->speed != s_khz->intValue) /* FS: our rate was not liked, so force the change. */
		Cvar_SetValue("s_khz", shm->speed);

	shm->soundalive = true;
	shm->splitbuffer = false;
	shm->samples = aui->card_dmasize / aui->bytespersample_card;
	shm->samplepos = 0;
	shm->submission_chunk = 1;
	shm->buffer = (unsigned char *) aui->card_DMABUFF;

	Con_Printf("%s\n", aui->infostr);

	AU_setmixer_all(ctx, 80); /* 80% volume */
	AU_start(ctx); /* also clears dma buffer */

	return true;
}

/*
==============
PCI_GetDMAPos

return the current sample position (in mono samples read)
inside the recirculating dma buffer, so the mixing code will know
how many sample are required to fill it up.
===============
*/
int PCI_GetDMAPos(void)
{
	shm->samplepos = AU_cardbuf_space(ctx);
	return shm->samplepos;
}

/*
==============
PCI_Shutdown

Stop and close the sound device for exiting
===============
*/
void PCI_Shutdown(void)
{
	AU_close(ctx);
	ctx = NULL;
}

#endif /* USE_SNDPCI */

