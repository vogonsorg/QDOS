CC   = gcc
STRIP= strip

# whether to use WatTCP for DOS UDP networking
USE_WATT32=yes
# whether to use Beame & Whiteside for DOS networking
USE_BWTCP=yes
# whether to use MPATH for DOS UDP networking under Win9x
USE_MPATH=no
# gamespy server browser
USE_GAMESPY=1
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

# common objects shared by Quake 1 and QuakeWorld
COMMON=../common

CFLAGS  = -g -Wall -Werror -O2 -DNDEBUG -DQUAKE1 -fno-strict-aliasing
CFLAGS += -I./ -I../common
CFLAGS += -fomit-frame-pointer
LIBS    =
EXE     = qdos.exe

ifeq ($(USE_SSE),1)
CFLAGS += -march=pentium3
else
CFLAGS += -mtune=pentium
endif

ifeq ($(GLQUAKE),1)
CFLAGS += -I../3rdparty/include -I../3rdparty/include/glide3 -DGLQUAKE
CFLAGS += -DUSE_3DFXGAMMA
CFLAGS += -DGL_DLSYM
#LIBS   += -L../3rdparty/lib -lgl
EXE     = qdosfx.exe
SYSOBJ_DXE=$(COMMON)/dxe.o
else
SYSOBJ_DXE=$(COMMON)/no_dxe.o
endif

ifeq ($(USE_SNDPCI),1)
CFLAGS += -DUSE_SNDPCI
CFLAGS += -I../3rdparty/include
LIBS   += -L../3rdparty/lib -lau
endif

ifeq ($(USE_BWTCP),yes)
CFLAGS += -DUSE_BWTCP
endif

ifeq ($(USE_MPATH),yes)
CFLAGS += -DUSE_MPATH
endif

ifeq ($(USE_WATT32),yes)
CFLAGS += -DUSE_WATT32 -DWATT32_NO_OLDIES
CFLAGS += -I../wat/inc
LIBS   += -L../wat/lib -lwatt
endif

ifeq ($(USE_OGG),yes)
CFLAGS += -DOGG_SUPPORT
CFLAGS += -I../3rdparty/include
ifneq ($(USE_TREMOR),1)
LIBS   += -L../3rdparty/lib -lvorbisfile -lvorbis -logg
else
CFLAGS += -DVORBIS_USE_TREMOR
LIBS   += -L../3rdparty/lib -lvorbisidec -logg
endif
endif

LIBS   += -lc -lgcc -lm

# objects:
DOSTCP :=
ifeq ($(USE_BWTCP),yes)
DOSTCP += net_bw.o
endif
ifeq ($(USE_MPATH),yes)
DOSTCP += net_mp.o mplpc.o
endif
ifeq ($(USE_WATT32),yes)
DOSTCP += net_udp.o
else
# get inet_addr() and inet_ntoa() either from Watt-32
# or from our local implementation 
DOSTCP += dos_inet.o inetaddr.o
endif
SYSOBJ_NET := $(DOSTCP)

ifeq ($(USE_GAMESPY),1)
CFLAGS += -DGAMESPY
GAMESPY = $(COMMON)/Goa/CEngine/darray.o \
	$(COMMON)/Goa/CEngine/gserver.o \
	$(COMMON)/Goa/CEngine/gserverlist.o \
	$(COMMON)/Goa/CEngine/hashtable.o \
	$(COMMON)/Goa/SDK/goautil.o \
	$(COMMON)/Goa/nonport.o
endif

# not too sophisticated dependency
OBJS=	$(SYSOBJ_DXE)   \
	$(SYSOBJ_NET)   \
	$(GAMESPY)      \
	$(COMMON)/cd_audio.o \
	chase.o    \
	cl_demo.o  \
	cl_input.o \
	cl_main.o  \
	cl_parse.o \
	cl_tent.o  \
	cmd.o      \
	common.o   \
	console.o  \
	$(COMMON)/crc.o      \
	cvar.o     \
	$(COMMON)/dos_v2.o   \
	$(COMMON)/dstring.o  \
	host.o     \
	host_cmd.o \
	$(COMMON)/in_dos.o   \
	keys.o     \
	$(COMMON)/math.o     \
	mathlib.o  \
	menu.o     \
	net_dgrm.o \
	net_dos.o  \
	net_ipx.o  \
	net_loop.o \
	net_main.o \
	net_ser.o  \
	pr_cmds.o  \
	pr_edict.o \
	pr_exec.o  \
	r_part.o   \
	sbar.o     \
	$(COMMON)/snd_dma.o  \
	$(COMMON)/snd_dos.o  \
	$(COMMON)/snd_gus.o  \
	$(COMMON)/snd_mem.o  \
	$(COMMON)/snd_mix.o  \
	$(COMMON)/snd_mixa.o \
	$(COMMON)/snd_pci.o  \
	$(COMMON)/snd_sb.o   \
	$(COMMON)/snd_strm.o \
	$(COMMON)/snd_wavstream.o  \
	sv_main.o  \
	sv_move.o  \
	sv_phys.o  \
	sv_user.o  \
	$(COMMON)/sys_dos.o  \
	$(COMMON)/sys_dosa.o \
	$(COMMON)/sys_find.o \
	view.o     \
	$(COMMON)/wad.o      \
	world.o    \
	$(COMMON)/zone.o

ifeq ($(GLQUAKE),0)
REFSOFT = $(COMMON)/d_copy.o \
	$(COMMON)/d_draw.o   \
	$(COMMON)/d_draw16.o \
	d_edge.o   \
	$(COMMON)/d_fill.o   \
	$(COMMON)/d_init.o   \
	$(COMMON)/d_modech.o \
	$(COMMON)/d_part.o   \
	$(COMMON)/d_parta.o  \
	$(COMMON)/d_polysa.o \
	$(COMMON)/d_polyse.o \
	$(COMMON)/d_scan.o   \
	$(COMMON)/d_scana.o  \
	$(COMMON)/d_sky.o    \
	$(COMMON)/d_spr8.o   \
	$(COMMON)/d_sprite.o \
	$(COMMON)/d_surf.o   \
	$(COMMON)/d_vars.o   \
	$(COMMON)/d_varsa.o  \
	$(COMMON)/d_zpoint.o \
	draw.o     \
	model.o    \
	$(COMMON)/r_aclip.o  \
	$(COMMON)/r_aclipa.o \
	$(COMMON)/r_alias.o  \
	$(COMMON)/r_aliasa.o \
	$(COMMON)/r_bsp.o    \
	$(COMMON)/r_draw.o   \
	$(COMMON)/r_drawa.o  \
	$(COMMON)/r_edge.o   \
	$(COMMON)/r_edgea.o  \
	r_efrag.o  \
	$(COMMON)/r_light.o  \
	r_main.o   \
	r_misc.o   \
	$(COMMON)/r_sky.o    \
	$(COMMON)/r_sprite.o \
	$(COMMON)/r_surf.o   \
	$(COMMON)/r_vars.o   \
	$(COMMON)/r_varsa.o  \
	screen.o   \
	$(COMMON)/surf16.o   \
	$(COMMON)/surf8.o    \
	vid_dos.o  \
	vid_ext.o  \
	vid_vga.o  \
	$(COMMON)/vregset.o  \
	$(COMMON)/worlda.o
endif

ifeq ($(GLQUAKE),1)
REFGL = $(COMMON)/dos_dmesa.o  \
	$(COMMON)/dos_fxmesa.o \
	$(COMMON)/dos_sage.o   \
	$(COMMON)/fx_gamma.o   \
	gl_draw.o    \
	$(COMMON)/gl_mesh.o    \
	gl_model.o   \
	$(COMMON)/gl_refrag.o  \
	$(COMMON)/gl_rlight.o  \
	gl_rmain.o   \
	gl_rmisc.o   \
	$(COMMON)/gl_rsurf.o   \
	gl_screen.o  \
	$(COMMON)/gl_test.o    \
	$(COMMON)/gl_warp.o    \
	$(COMMON)/gl_viddos.o
endif

OBJECTS = $(OBJS) $(REFSOFT) $(REFGL)

all: qdos

clean:
	rm -f *.o
	rm -f $(COMMON)/*.o
	rm -f $(COMMON)/Goa/*.o
	rm -f $(COMMON)/Goa/CEngine/*.o
	rm -f $(COMMON)/Goa/Sdk/*.o

qdos: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(EXE)

DO_AS=$(CC) -x assembler-with-cpp

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(DO_AS) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(DO_AS) $(CFLAGS) -c $< -o $@
