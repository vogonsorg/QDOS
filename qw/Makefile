CC   = gcc
STRIP= strip

# libcurl/http downloads:
USE_CURL=1
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

CFLAGS  = -g -Wall -Werror -O2 -DNDEBUG -DQUAKEWORLD -fno-strict-aliasing
CFLAGS += -I./ -I../common
CFLAGS += -fomit-frame-pointer -DWATT32_NO_OLDIES -I../wat/inc
LDFLAGS =
LIBS    =
EXE     = qw.exe

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
EXE     = qwdosfx.exe
SYSOBJ_DXE=$(COMMON)/dxe.o
else
SYSOBJ_DXE=$(COMMON)/no_dxe.o
endif

ifeq ($(USE_SNDPCI),1)
CFLAGS += -DUSE_SNDPCI
CFLAGS += -I../3rdparty/include
LIBS   += -L../3rdparty/lib -lau
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

ifeq ($(USE_GAMESPY),1)
CFLAGS += -DGAMESPY
GAMESPY = $(COMMON)/Goa/CEngine/darray.o \
	$(COMMON)/Goa/CEngine/gserver.o \
	$(COMMON)/Goa/CEngine/gserverlist.o \
	$(COMMON)/Goa/CEngine/gutil.o \
	$(COMMON)/Goa/CEngine/hashtable.o \
	$(COMMON)/Goa/nonport.o
endif

ifeq ($(USE_CURL),1)
CFLAGS += -DUSE_CURL -DCURL_NO_OLDIES
CFLAGS += -I../libcurl/include
LIBS   += -L../libcurl/lib -lcurl
endif
LIBS   += -L../wat/lib -lwatt

LIBS   += -lc -lgcc -lm

# not too sophisticated dependency
CLIENT = $(SYSOBJ_DXE)  \
	$(GAMESPY)      \
	$(COMMON)/cd_audio.o \
	cl_cam.o   \
	cl_demo.o  \
	cl_ents.o  \
	cl_http.o  \
	cl_input.o \
	cl_main.o  \
	cl_parse.o \
	cl_pred.o  \
	cl_tent.o  \
	cmd.o      \
	common.o   \
	console.o  \
	crc.o      \
	cvar.o     \
	$(COMMON)/dos_v2.o   \
	$(COMMON)/dstring.o  \
	$(COMMON)/in_dos.o   \
	keys.o     \
	math.o     \
	mathlib.o  \
	md4.o      \
	menu.o     \
	net_chan.o \
	net_udp.o  \
	pmove.o    \
	pmovetst.o \
	r_part.o   \
	sbar.o     \
	skin.o     \
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
	$(COMMON)/sys_dos.o  \
	$(COMMON)/sys_dosa.o \
	$(COMMON)/sys_find.o \
	view.o     \
	$(COMMON)/wad.o      \
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
	r_alias.o  \
	$(COMMON)/r_aliasa.o \
	r_bsp.o    \
	$(COMMON)/r_draw.o   \
	$(COMMON)/r_drawa.o  \
	r_edge.o   \
	$(COMMON)/r_edgea.o  \
	r_efrag.o  \
	$(COMMON)/r_light.o  \
	r_main.o   \
	r_misc.o   \
	$(COMMON)/r_sky.o    \
	$(COMMON)/r_sprite.o \
	r_surf.o   \
	$(COMMON)/r_vars.o   \
	$(COMMON)/r_varsa.o  \
	screen.o   \
	$(COMMON)/surf16.o   \
	$(COMMON)/surf8.o    \
	vid_dos.o  \
	vid_ext.o  \
	vid_vga.o  \
	$(COMMON)/vregset.o
endif

ifeq ($(GLQUAKE),1)
REFGL = $(COMMON)/dos_dmesa.o  \
	$(COMMON)/dos_fxmesa.o \
	$(COMMON)/dos_sage.o   \
	$(COMMON)/fx_gamma.o   \
	gl_draw.o    \
	$(COMMON)/gl_mesh.o    \
	gl_model.o   \
	gl_ngraph.o  \
	gl_refrag.o  \
	gl_rlight.o  \
	gl_rmain.o   \
	gl_rmisc.o   \
	gl_rsurf.o   \
	gl_screen.o  \
	$(COMMON)/gl_test.o    \
	$(COMMON)/gl_warp.o    \
	$(COMMON)/gl_viddos.o
endif

OBJECTS = $(CLIENT) $(REFSOFT) $(REFGL)

all:     qw

clean:
	rm -f *.o
	rm -f $(COMMON)/*.o
	rm -f $(COMMON)/Goa/*.o
	rm -f $(COMMON)/Goa/CEngine/*.o
	rm -f $(COMMON)/Goa/Sdk/*.o

qw: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $(EXE)

DO_AS=$(CC) -x assembler-with-cpp

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: %.S
	$(DO_AS) $(CFLAGS) -c $< -o $@
%.o: %.s
	$(DO_AS) $(CFLAGS) -c $< -o $@
