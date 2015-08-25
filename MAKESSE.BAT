@echo off

CALL C:\djgpp\setenv.bat

C:
cd\proj\qdos\quake
make -f makefile clean
make -f makefile GLQUAKE=1 USE_SSE=1
UPX -9 C:\PROJ\QDOS\QUAKE\QDOSFX.EXE

cd..

C:
cd\proj\qdos\qw
make -f makefile.gl clean
make -f makefile.gl USE_SSE=1
UPX -9 C:\PROJ\QDOS\QW\QWDOSFX.EXE
