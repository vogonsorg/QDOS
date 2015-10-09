@echo off

CALL C:\djgpp\setenv.bat

C:
cd\proj\qdos\quake
make -f makefile clean
make -f makefile USE_SSE=1
UPX -9 C:\PROJ\QDOS\QUAKE\QDOS.EXE
make -f makefile clean
make -f makefile USE_SSE=1 GLQUAKE=1
UPX -9 C:\PROJ\QDOS\QUAKE\QDOSFX.EXE

cd..

C:
cd\proj\qdos\qw
make -f makefile clean
make -f makefile USE_SSE=1
UPX -9 C:\PROJ\QDOS\QW\QW.EXE
make -f makefile clean
make -f makefile USE_SSE=1 GLQUAKE=1
UPX -9 C:\PROJ\QDOS\QW\QWDOSFX.EXE
