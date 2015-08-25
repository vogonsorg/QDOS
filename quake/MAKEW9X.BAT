@echo off
CALL C:\DJGPP\SETENV.BAT
make -f Makefile clean
make -f Makefile USE_MPATH=yes
