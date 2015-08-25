@echo off
CALL setenv.bat
make -f makefile clean
make -f makefile
