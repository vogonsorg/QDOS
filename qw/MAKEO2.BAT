@echo off
CALL setenv.bat
make -f makefile.o2 clean
make -f makefile.o2
