@echo off

CALL E:\djgpp\setenv.bat

E:
cd\proj\qdos\quake
make -f makefile clean
make -f makefile USE_SSE=1
UPX -9 E:\PROJ\QDOS\QUAKE\QDOS.EXE
make -f makefile clean
make -f makefile USE_SSE=1 GLQUAKE=1
UPX -9 E:\PROJ\QDOS\QUAKE\QDOSFX.EXE

cd..

E:
cd\proj\qdos\qw
make -f makefile clean
make -f makefile USE_SSE=1
UPX -9 E:\PROJ\QDOS\QW\QW.EXE
make -f makefile clean
make -f makefile USE_SSE=1 GLQUAKE=1
UPX -9 E:\PROJ\QDOS\QW\QWDOSFX.EXE
