@echo off
del /s gas2masm.exe
del /s *.ncb
del /s *.opt
del /s *.plg
del /s *.ilk
del /s *.ASM

rd Debug /S /Q
rd Release /S /Q

cd gas2masm
rd Debug /S /Q
rd Release /S /Q
