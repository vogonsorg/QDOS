# OpenWatcom tests makefile for Glide3
# This makefile MUST be processed by GNU make!!!
# Building under native DOS is not supported:
#		only tested under Win32 or Linux
#
#  Copyright (c) 2004 - Borca Daniel
#  Email : dborca@users.sourceforge.net
#  Web   : http://www.geocities.com/dborca
#

#
#  Available options:
#
#    Environment variables:
#	CPU		optimize for the given processor.
#			default = 5s (Pentium, stack)
#	DEBUG=1		disable optimizations and build for debug.
#			default = no
#
#    Targets:
#	<file.exe>	build a specific file
#

.PHONY: all
.SUFFIXES: .c .obj .exe
.SECONDARY: tlib.obj fxos.obj

FX_GLIDE_HW = h5
TOP = ../../..

# detect if running under unix by finding 'rm' in $PATH :
ifeq ($(wildcard $(addsuffix /rm,$(subst :, ,$(PATH)))),)
DOSMODE= 1
UNLINK = del $(subst /,\,$(1))
FIXPATH= $(subst /,\,$1)
else
DOSMODE= 0
UNLINK = $(RM) $(1)
FIXPATH= $1
endif

CC = wcl386
CFLAGS = -bt=dos -wx -zq
INCPATH = -I$(TOP)/$(FX_GLIDE_HW)/glide3/src -I$(TOP)/$(FX_GLIDE_HW)/incsrc
INCPATH += -I$(TOP)/swlibs/fxmisc
CFLAGS += -D__DOS__ -DH3
CFLAGS += -D__DOS32__
CFLAGS += $(call FIXPATH,$(INCPATH))

ifdef DEBUG
CFLAGS += -od -d2
else
CPU ?= 5s
CFLAGS += -ox -$(CPU)
endif

# pick either of causeway, dos4g, dos32a or stub32a as link target
LDFLAGS = -zq -k32768 -l=dos32a

LDLIBS = $(TOP)/$(FX_GLIDE_HW)/lib/glide3x.lib

.c.obj:
	$(CC) $(CFLAGS) -fo=$@ -c $<
%.exe: tlib.obj %.obj
	$(CC) -fe=$@ $(LDFLAGS) $^ $(call FIXPATH,$(LDLIBS))

all:
	$(error Must specify <filename.exe> to build)

sbench.exe: sbench.obj fxos.obj tlib.obj
	$(CC) -fe=$@ $(LDFLAGS) $^ $(call FIXPATH,$(LDLIBS))

fxos.obj: $(call FIXPATH,$(TOP)/swlibs/fxmisc/fxos.c)
	$(CC) $(CFLAGS) -fo=$@ -c $<
