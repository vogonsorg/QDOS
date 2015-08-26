# Makefile for Mesa for VMS
# contributed by Jouk Jansen  joukj@hrem.stm.tudelft.nl

all :
	set default [.main]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.glapi]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.drivers.x11]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.osmesa]
	$(MMS)$(MMSQUALIFIERS)
	set default [--.math]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.tnl]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.swrast]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.swrast_setup]
	$(MMS)$(MMSQUALIFIERS)
	set default [-.array_cache]
	$(MMS)$(MMSQUALIFIERS)
