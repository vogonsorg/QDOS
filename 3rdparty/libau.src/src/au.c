#include "libaudef.h"

#define SOUNDCARD_BUFFER_PROTECTION 32 /* in bytes (required for PCI cards) */
#define AU_MIXCHANS_OUTS 4

static const unsigned int au_mixchan_outs[AU_MIXCHANS_OUTS]=
{
	AU_MIXCHAN_MASTER,
	AU_MIXCHAN_PCM,
	AU_MIXCHAN_HEADPHONE,
	AU_MIXCHAN_SPDIFOUT
};

extern one_sndcard_info SBLIVE_sndcard_info;
extern one_sndcard_info EMU20KX_sndcard_info;
extern one_sndcard_info CMI8X38_sndcard_info;
extern one_sndcard_info ES1371_sndcard_info;
extern one_sndcard_info ICH_sndcard_info;
extern one_sndcard_info IHD_sndcard_info;
extern one_sndcard_info VIA82XX_sndcard_info;

static one_sndcard_info *all_sndcard_info[]=
{
	&SBLIVE_sndcard_info,
	&EMU20KX_sndcard_info,
	&ES1371_sndcard_info,
	&CMI8X38_sndcard_info,
	&VIA82XX_sndcard_info,
	&ICH_sndcard_info,
	&IHD_sndcard_info,
	NULL
};

struct mpxplay_audioout_info_s au_infos;

char libau_istr[100];

const char* AU_search(unsigned int config)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;
	one_sndcard_info **asip;
	unsigned int i;

	aui->card_select_config=config;

	/* DEFAULT SETTING */

	for(i=0;i<AU_MIXCHANS_NUM;i++)
	{
		aui->card_mixer_values[i]=-1;
	}

	aui->card_wave_id=MPXPLAY_WAVEID_PCM_SLE;	// integer PCM
	aui->bits_set=16;

	asip=&all_sndcard_info[0];
	aui->card_handler=*asip;

	do
	{
		if(aui->card_handler->card_detect(aui))
		{
			aui->card_handler->card_info(aui);
			return libau_istr;
		}

		asip++;
		aui->card_handler=*asip;
	}
	while(aui->card_handler);

	return NULL;
}

void AU_start(void)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;

	if(!(aui->card_infobits&AUINFOS_CARDINFOBIT_PLAYING))
	{
		funcbit_smp_enable(aui->card_infobits,AUINFOS_CARDINFOBIT_PLAYING);
		aui->card_handler->cardbuf_clear();
		aui->card_handler->card_start(aui);
	}
}

void AU_stop(void)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;

	if(aui->card_infobits&AUINFOS_CARDINFOBIT_PLAYING)
	{
		funcbit_smp_disable(aui->card_infobits,AUINFOS_CARDINFOBIT_PLAYING);
		aui->card_handler->card_stop(aui);
#ifdef SDR
		MDma_bufpos();
#endif
		aui->card_dmaspace=aui->card_dmasize-aui->card_dmalastput;
	}
}

void AU_close(void)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;

	AU_stop();
	aui->card_handler->card_close(aui);
}

#ifndef SDR
unsigned int AU_cardbuf_space(void)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;

	if(aui->card_dmalastput>=aui->card_dmasize)	// checking
	{
		aui->card_dmalastput=0;
	}

	{
		unsigned long bufpos;

		if(aui->card_infobits&AUINFOS_CARDINFOBIT_PLAYING)
		{
			bufpos = MDma_bufpos();
		}
		else
		{
			bufpos=0;
		}

		if(bufpos>=aui->card_dmalastput)
		{
			aui->card_dmaspace=bufpos-aui->card_dmalastput;
		}
		else
		{
			aui->card_dmaspace=bufpos-aui->card_dmalastput+aui->card_dmasize;
		}
	}

	if(aui->card_dmaspace>aui->card_dmasize)	// checking
	{
		aui->card_dmaspace=aui->card_dmasize;
	}

	aui->card_dmafilled=aui->card_dmasize-aui->card_dmaspace;

	return (aui->card_dmaspace>aui->card_bufprotect) ? (aui->card_dmaspace-aui->card_bufprotect):0;
}

void AU_writedata(const char* pcm, long len)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;

	aui->card_outbytes =min(len,(aui->card_dmasize/4));
	aui->card_outbytes-=(aui->card_outbytes%aui->card_bytespersign);

	{
		unsigned long space;
		do
		{
			space=AU_cardbuf_space();

			if(space>=aui->card_bytespersign)
			{
				unsigned int outbytes_putblock=min(space,len);
				MDma_writedata(pcm,outbytes_putblock);
				pcm+=outbytes_putblock;
				len-=outbytes_putblock;
			}
		}
		while(len);
	}
}
#endif /* SDR */

void AU_setrate(unsigned int *fr, unsigned int *bt, unsigned int *ch)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;
	unsigned long buffer_protection;

	if(aui->card_infobits&AUINFOS_CARDINFOBIT_PLAYING)
	{
		AU_stop();
	}

	aui->freq_card=aui->freq_set=*fr;
	aui->bits_set=*bt;
	aui->chan_set=*ch;

	aui->card_handler->card_setrate(aui);
	aui->bytespersample_card=(aui->bits_card+7)/8;

	*fr=aui->freq_set=aui->freq_card;
	*bt=aui->bits_set=aui->bits_card;
	*ch=aui->chan_set=aui->chan_card;

	aui->card_bytespersign=aui->chan_card*aui->bytespersample_card;

	buffer_protection=SOUNDCARD_BUFFER_PROTECTION; // rounding to bytespersign
	buffer_protection+=aui->card_bytespersign-1;
	buffer_protection-=(buffer_protection%aui->card_bytespersign);
	aui->card_bufprotect=buffer_protection;

#ifndef SDR
	sprintf(libau_istr, "Ok! : set %iHz/%ibit/%ich -> DMA_size: %lu at address: %ph\n",
			*fr, *bt, *ch, aui->card_dmasize, aui->card_DMABUFF
	#ifdef __DJGPP__
							- __djgpp_conventional_base
	#endif
		);
#endif /* SDR */
}

aucards_onemixerchan_s *AU_search_mixerchan(aucards_allmixerchan_s *mixeri,unsigned int mixchannum)
{
	unsigned int i=0;

	while(*mixeri)
	{
		if((*mixeri)->mixchan==mixchannum)
			return (*mixeri);

		if(++i >= AU_MIXCHANS_NUM)
			break;

		mixeri++;
	}

	return NULL;
}

void AU_setmixer_one(unsigned int mixchannum,unsigned int setmode,int newvalue)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;
	one_sndcard_info *cardi=aui->card_handler;
	aucards_onemixerchan_s *onechi; // one mixer channel infos (master,pcm,etc.)
	unsigned int subchannelnum, sch, channel, function;
	int newpercentval;

	//mixer structure/values verifying
	function=AU_MIXCHANFUNCS_GETFUNC(mixchannum);

	if(function>=AU_MIXCHANFUNCS_NUM)
		return;

	channel=AU_MIXCHANFUNCS_GETCHAN(mixchannum);

	if(channel>AU_MIXCHANS_NUM)
		return;

	if(!cardi->card_writemixer || !cardi->card_readmixer || !cardi->card_mixerchans)
		return;

	onechi=AU_search_mixerchan(cardi->card_mixerchans,mixchannum);

	if(!onechi)
		return;

	subchannelnum=onechi->subchannelnum;

	if(!subchannelnum || (subchannelnum>AU_MIXERCHAN_MAX_SUBCHANNELS))
		return;

 //calculate new percent
	switch(setmode)
	{
	case MIXER_SETMODE_ABSOLUTE:
		newpercentval=newvalue;
		break;
	case MIXER_SETMODE_RELATIVE:
		if(function==AU_MIXCHANFUNC_VOLUME)
			newpercentval=aui->card_mixer_values[channel]+newvalue;
		else if(newvalue<0)
			newpercentval=0;
		else	newpercentval=100;
		break;
	default:
		return;
	}

	if(newpercentval<0)
		newpercentval=0;

	if(newpercentval>100)
		newpercentval=100;

	ENTER_CRITICAL;

	//read current register value, mix it with the new one, write it back
	for(sch=0;sch<subchannelnum;sch++)
	{
		aucards_submixerchan_s *subchi=&(onechi->submixerchans[sch]); // one subchannel infos (left,right,etc.)
		unsigned long currchval, newchval;

		if((subchi->submixch_register>AU_MIXERCHAN_MAX_REGISTER) || !subchi->submixch_max || (subchi->submixch_shift>AU_MIXERCHAN_MAX_BITS)) // invalid subchannel infos
			continue;

		newchval=(unsigned long)(((float)newpercentval*(float)subchi->submixch_max+49.0)/100.0); // percent to chval (rounding up)

		if(newchval>subchi->submixch_max)
			newchval=subchi->submixch_max;
	
		if(subchi->submixch_infobits&SUBMIXCH_INFOBIT_REVERSEDVALUE)   // reverse value if required
			newchval=subchi->submixch_max-newchval;

		newchval<<=subchi->submixch_shift;                             // shift to position

		currchval=cardi->card_readmixer(aui,subchi->submixch_register);// read current value
		currchval&=~(subchi->submixch_max<<subchi->submixch_shift);    // unmask
		newchval=(currchval|newchval);                                 // add new value

		cardi->card_writemixer(aui,subchi->submixch_register,newchval);// write it back
	}

	LEAVE_CRITICAL;

	if(function==AU_MIXCHANFUNC_VOLUME) {
		aui->card_mixer_values[channel]=newpercentval;
	}
}

/*
int AU_getmixer_one(unsigned int mixchannum)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;
	one_sndcard_info *cardi=aui->card_handler;
	aucards_onemixerchan_s *onechi; // one mixer channel infos (master,pcm,etc.)
	aucards_submixerchan_s *subchi; // one subchannel infos (left,right,etc.)
	unsigned long channel, function, subchannelnum, value;

	//mixer structure/values verifying
	function=AU_MIXCHANFUNCS_GETFUNC(mixchannum);

	if(function>=AU_MIXCHANFUNCS_NUM)
		return -1;

	channel=AU_MIXCHANFUNCS_GETCHAN(mixchannum);

	if(channel>AU_MIXCHANS_NUM)
		return -1;

	if(!cardi->card_readmixer || !cardi->card_mixerchans)
		return -1;

	onechi=AU_search_mixerchan(cardi->card_mixerchans,mixchannum);
	if(!onechi)
		return -1;

	subchannelnum=onechi->subchannelnum;
	if(!subchannelnum || (subchannelnum>AU_MIXERCHAN_MAX_SUBCHANNELS))
		return -1;

	// we read one (the left at stereo) sub-channel only
	subchi=&(onechi->submixerchans[0]);
	if((subchi->submixch_register>AU_MIXERCHAN_MAX_REGISTER) || (subchi->submixch_shift>AU_MIXERCHAN_MAX_BITS)) // invalid subchannel infos
		return -1;

	value=cardi->card_readmixer(aui,subchi->submixch_register);	// read
	value>>=subchi->submixch_shift;					// shift
	value&=subchi->submixch_max;					// mask

	if(subchi->submixch_infobits&SUBMIXCH_INFOBIT_REVERSEDVALUE)// reverse value if required
		value=subchi->submixch_max-value;

	value=(float)value*100.0/(float)subchi->submixch_max;		// chval to percent
	if(value>100)
		value=100;

	return value;
}
*/

void AU_setmixer_all(unsigned int vol)
{
	struct mpxplay_audioout_info_s *aui=&au_infos;
	unsigned int i;

	aui->card_master_volume=vol;

	for(i=0;i<AU_MIXCHANS_OUTS;i++)
	{
		AU_setmixer_one(AU_MIXCHANFUNCS_PACK(au_mixchan_outs[i],AU_MIXCHANFUNC_VOLUME),MIXER_SETMODE_ABSOLUTE,vol);
	}
}
