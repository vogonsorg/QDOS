@echo off
del /s *.o
del /s gas2masm.exe
del /s *.ncb
del /s *.opt
del /s *.plg
del /s *.ilk
del /s *.ASM
del /s *.user
del /s *.aps
del /AH /s *.suo

@echo Trying Windows XP RD
rd Debug /S /Q
rd debug_gl /S /Q
rd Release /S /Q
rd release_gl /S /Q

@echo Trying BASH RM -RF
rm -rf Debug
rm -rf debug_gl
rm -rf Release
rm -rf release_gl

cd gas2masm
rd Debug /S /Q
rd Release /S /Q
rm -rf Debug
rm -rf Release

cd..

cd..
cd common
del /s *.o
cd..
cd qw
