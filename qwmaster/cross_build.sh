#!/bin/sh

. ../cross_defs.dj

if test "$1" = "strip"; then
	echo $TARGET-strip gspydos.exe
	$TARGET-strip gspydos.exe
	exit 0
fi

exec make CC=$TARGET-g++ STRIP=$TARGET-strip -f makefile.dj $*
