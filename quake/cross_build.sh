#!/bin/sh

. ../cross_defs.dj

exec make CC=$TARGET-gcc STRIP=$TARGET-strip -f Makefile.o2 $*
