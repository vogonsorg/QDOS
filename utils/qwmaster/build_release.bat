@ECHO OFF
TASKKILL /F /T /IM MSDEV.EXE 
TASKKILL /F /T /IM MSDEV.COM
msdev master.dsw /MAKE "master - Win32 Release" /CLEAN
msdev master.dsw /MAKE "master - Win32 Release" /BUILD
