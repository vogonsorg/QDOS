@echo off
del /s gas2masm.exe
del /s *.ncb
del /s *.opt
del /s *.plg
del /s *.ilk
del /s *.ASM

@echo Trying Windows XP RD
rd Debug /S /Q
rd Release /S /Q

@echo Trying BASH RM -RF
rm -rf Debug
rm -rf Release

cd gas2masm
rd Debug /S /Q
rd Release /S /Q
rm -rf Debug
rm -rf Release

cd ..
cd DOS
del *.o
