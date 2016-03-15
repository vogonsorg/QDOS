#!/bin/sh

. ../cross_defs.dj

if test "$1" = "strip"; then
	echo $TARGET-strip qdos.exe
	$TARGET-strip qdos.exe qdosfx.exe
	exit 0
fi

exec make CC=$TARGET-gcc STRIP=$TARGET-strip $*
