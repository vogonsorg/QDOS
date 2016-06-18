@echo off

CALL E:\djgpp\setenv.bat

E:
cd\proj\qdos\quake
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile clean
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile USE_SSE=1
UPX -9 E:\PROJ\QDOS\QUAKE\QDOS.EXE
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile clean
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile USE_SSE=1 GLQUAKE=1
UPX -9 E:\PROJ\QDOS\QUAKE\QDOSFX.EXE

cd..

E:
cd\proj\qdos\qw
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile clean
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile USE_SSE=1
UPX -9 E:\PROJ\QDOS\QW\QW.EXE
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile clean
make %1 %2 %3 %3 %4 %5 %6 %7 %8 %9 -f makefile USE_SSE=1 GLQUAKE=1
UPX -9 E:\PROJ\QDOS\QW\QWDOSFX.EXE
