#!/bin/sh

. ../cross_defs.dj

if test "$1" = "strip"; then
	echo strip qw.exe
	$TARGET-strip qw.exe
	exit 0
fi

exec make CC=$TARGET-gcc STRIP=$TARGET-strip $*
