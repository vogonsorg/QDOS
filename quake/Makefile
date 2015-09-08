CC   = gcc
STRIP= strip

# whether to use WatTCP for DOS UDP networking
USE_WATT32=yes
# whether to use Beame & Whiteside for DOS networking
USE_BWTCP=yes
# whether to use MPATH for DOS UDP networking under Win9x
USE_MPATH=no
# whether to use OGG VORBIS
USE_OGG=yes
# use tremor library for ogg/vorbis:
USE_TREMOR=1
# pci sound card support:
USE_SNDPCI=1
# use SSE optimizations for P3 (could be dangerous)
USE_SSE=0
# compile Quake with fxMesa for 3DFX Voodoo Cards
GLQUAKE=0

CFLAGS = -g -Wall -Werror -O2 -DNDEBUG -fno-strict-aliasing -fomit-frame-pointer
LIBS   =
EXE = qdos.exe

ifeq ($(USE_SSE),1)
CFLAGS+= -mtune=pentium3 -march=pentium3 -mfpmath=sse
else
CFLAGS += -mtune=pentium
endif

ifeq ($(GLQUAKE),1)
CFLAGS+= -I../3rdparty/include -I../3rdparty/include/glide3 -DGLQUAKE -DUSE_3DFXGAMMA
LIBS+= -L../3rdparty/lib -lgl
EXE = qdosfx.exe
SYSOBJ_DXE=$(O)/dxe.o
else
SYSOBJ_DXE=$(O)/no_dxe.o
endif

ifeq ($(USE_SNDPCI),1)
CFLAGS+= -DUSE_SNDPCI
CFLAGS+= -I../3rdparty/include
LIBS += -L../3rdparty/lib -lau
endif

ifeq ($(USE_BWTCP),yes)
CFLAGS+= -DUSE_BWTCP
endif

ifeq ($(USE_MPATH),yes)
CFLAGS+= -DUSE_MPATH
endif

ifeq ($(USE_WATT32),yes)
CFLAGS+= -DUSE_WATT32 -DWATT32_NO_OLDIES
CFLAGS+= -I../wat/inc
LDFLAGS= -L../wat/lib
LIBS  += -lwatt
endif

ifeq ($(USE_OGG),yes)
CFLAGS+= -DOGG_SUPPORT
CFLAGS+= -I../3rdparty/include
ifneq ($(USE_TREMOR),1)
LIBS  += -L../3rdparty/lib -lvorbisfile -lvorbis -logg
else
CFLAGS+= -DVORBIS_USE_TREMOR
LIBS  += -L../3rdparty/lib -lvorbisidec -logg
endif
endif

LIBS  += -lm

# subdirectory for objects
O=dos

# objects:
DOSTCP :=
ifeq ($(USE_BWTCP),yes)
DOSTCP += $(O)/net_bw.o
endif
ifeq ($(USE_MPATH),yes)
DOSTCP += $(O)/net_mp.o $(O)/mplpc.o
endif
ifeq ($(USE_WATT32),yes)
DOSTCP += $(O)/net_udp.o
else
# get inet_addr() and inet_ntoa() either from Watt-32
# or from our local implementation 
DOSTCP += $(O)/dos_inet.o $(O)/inetaddr.o
endif
SYSOBJ_NET := $(DOSTCP)

# FIXME: make the gspy shit optional
GAMESPY = $(O)/Goa/CEngine/darray.o \
	$(O)/Goa/CEngine/gserver.o \
	$(O)/Goa/CEngine/gserverlist.o \
	$(O)/Goa/CEngine/hashtable.o \
	$(O)/Goa/SDK/goautil.o \
	$(O)/Goa/nonport.o

# not too sophisticated dependency
OBJS=	$(SYSOBJ_DXE)    \
	$(SYSOBJ_NET)    \
	$(GAMESPY)       \
	$(O)/cd_audio.o \
	$(O)/chase.o    \
	$(O)/cl_demo.o  \
	$(O)/cl_input.o \
	$(O)/cl_main.o  \
	$(O)/cl_parse.o \
	$(O)/cl_tent.o  \
	$(O)/cmd.o      \
	$(O)/common.o   \
	$(O)/console.o  \
	$(O)/crc.o      \
	$(O)/cvar.o     \
	$(O)/dos_v2.o   \
	$(O)/dstring.o   \
	$(O)/host.o     \
	$(O)/host_cmd.o \
	$(O)/in_dos.o   \
	$(O)/keys.o     \
	$(O)/math.o     \
	$(O)/mathlib.o  \
	$(O)/menu.o     \
	$(O)/net_dgrm.o \
	$(O)/net_dos.o  \
	$(O)/net_ipx.o  \
	$(O)/net_loop.o \
	$(O)/net_main.o \
	$(O)/net_ser.o  \
	$(O)/pr_cmds.o  \
	$(O)/pr_edict.o \
	$(O)/pr_exec.o  \
	$(O)/r_part.o \
	$(O)/sbar.o     \
	$(O)/snd_dma.o  \
	$(O)/snd_dos.o  \
	$(O)/snd_gus.o  \
	$(O)/snd_mem.o  \
	$(O)/snd_mix.o  \
	$(O)/snd_mixa.o \
	$(O)/snd_pci.o  \
	$(O)/snd_sb.o  \
	$(O)/snd_strm.o  \
	$(O)/snd_wavstream.o  \
	$(O)/sv_main.o  \
	$(O)/sv_move.o  \
	$(O)/sv_phys.o  \
	$(O)/sv_user.o  \
	$(O)/sys_dos.o \
	$(O)/sys_dosa.o \
	$(O)/sys_find.o  \
	$(O)/view.o     \
	$(O)/wad.o      \
	$(O)/world.o    \
	$(O)/zone.o

ifeq ($(GLQUAKE),0)
REFSOFT = $(O)/d_copy.o   \
	$(O)/d_draw.o   \
	$(O)/d_draw16.o \
	$(O)/d_edge.o   \
	$(O)/d_fill.o   \
	$(O)/d_init.o   \
	$(O)/d_modech.o \
	$(O)/d_part.o   \
	$(O)/d_parta.o  \
	$(O)/d_polysa.o \
	$(O)/d_polyse.o \
	$(O)/d_scan.o   \
	$(O)/d_scana.o  \
	$(O)/d_sky.o    \
	$(O)/d_spr8.o   \
	$(O)/d_sprite.o \
	$(O)/d_surf.o   \
	$(O)/d_vars.o   \
	$(O)/d_varsa.o  \
	$(O)/d_zpoint.o \
	$(O)/draw.o     \
	$(O)/model.o    \
	$(O)/r_aclip.o  \
	$(O)/r_aclipa.o \
	$(O)/r_alias.o  \
	$(O)/r_aliasa.o \
	$(O)/r_bsp.o    \
	$(O)/r_draw.o   \
	$(O)/r_drawa.o  \
	$(O)/r_edge.o   \
	$(O)/r_edgea.o  \
	$(O)/r_efrag.o  \
	$(O)/r_light.o  \
	$(O)/r_main.o   \
	$(O)/r_misc.o   \
	$(O)/r_sky.o    \
	$(O)/r_sprite.o \
	$(O)/r_surf.o   \
	$(O)/r_vars.o   \
	$(O)/r_varsa.o  \
	$(O)/screen.o   \
	$(O)/surf16.o   \
	$(O)/surf8.o    \
	$(O)/vid_dos.o  \
	$(O)/vid_ext.o  \
	$(O)/vid_vga.o  \
	$(O)/vregset.o  \
	$(O)/worlda.o
endif

ifeq ($(GLQUAKE),1)
REFGL = $(O)/fx_gamma.o \
	$(O)/gl_draw.o \
	$(O)/gl_mesh.o \
	$(O)/gl_model.o \
	$(O)/gl_refrag.o \
	$(O)/gl_rlight.o \
	$(O)/gl_rmain.o \
	$(O)/gl_rmisc.o \
	$(O)/gl_rsurf.o \
	$(O)/gl_screen.o \
	$(O)/gl_test.o \
	$(O)/gl_warp.o \
	$(O)/gl_viddos.o
endif

OBJECTS = $(OBJS) $(REFSOFT) $(REFGL)

all: qdos

clean:
	rm -f $(O)/*.o
	rm -f $(O)/Goa/*.o
	rm -f $(O)/Goa/CEngine/*.o
	rm -f $(O)/Goa/SDK/*.o

qdos: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(EXE)

DO_AS=$(CC) -x assembler-with-cpp

$(O)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
$(O)/%.o: %.S
	$(DO_AS) $(CFLAGS) -c $< -o $@
$(O)/%.o: %.s
	$(DO_AS) $(CFLAGS) -c $< -o $@
