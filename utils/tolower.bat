@echo off
REM Lowercase Rename for Win32
for /f "Tokens=*" %%f in ('dir /l/b/a-d') do (rename "%%f" "%%f")
REM Recursive version
REM for /f "Tokens=*" %%f in ('dir /l/b/a-d/s') do (rename "%%f" "%%f") 