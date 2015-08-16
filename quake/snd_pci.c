/* snd_pci.c
 * PCI sound card support through libau
 */

#ifdef USE_SNDPCI

#include <dos.h>
#include "quakedef.h"
#include "dosisms.h"
#include "libaudef.h"

/*
==================
PCI_Init

Returns false if nothing is found.
==================
*/
qboolean PCI_Init(void)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;
	const char *c;

	c = AU_search(1);/* 1: stereo speaker output (meaningful only for Intel HDA chips) */

	if(c)
	{
		unsigned int speed=22050;
		unsigned int samplebits=16;
		unsigned int channels=2;

		Con_Printf("PCI Audio: %s\n",c);
		if(s_khz.intValue >= 11025) /* FS */
			speed = s_khz.intValue;

		shm = &sn;

		shm->speed=speed;
		shm->samplebits=samplebits;
		shm->channels=channels;
		AU_setrate(&speed,&samplebits,&channels);

		/* not sure but I think these can change in the setrate */
		shm->speed=aui->freq_card;
		shm->samplebits=aui->bits_set;
		shm->channels=aui->chan_set;
		if(shm->speed != s_khz.intValue) /* FS: In theory, our rate was not liked, so force the change. */
			Cvar_SetValue("s_khz", shm->speed);
		Con_DPrintf(DEVELOPER_MSG_SOUND, "Post AU_setrate %d/%d/%d\n",shm->speed,shm->samplebits,shm->channels);

		shm->soundalive = true;
		shm->splitbuffer = false;
		shm->samples = aui->card_dmasize/aui->bytespersample_card;
		shm->samplepos = 0;
		shm->submission_chunk = 1;

		memset(aui->card_DMABUFF, 0, aui->card_dmasize); /* FS: Clear the dma buffer on Init */
		shm->buffer = (unsigned char *) aui->card_DMABUFF;

		AU_setmixer_all(80); /* 80% volume */
		AU_start();

		return true;
	}

	Con_Printf("PCI Audio: Detection failed.\n");

	return false;
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
	shm->samplepos = AU_cardbuf_space();
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
	AU_close();
}

#endif /* USE_SNDPCI */

