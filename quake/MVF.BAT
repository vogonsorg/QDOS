@echo off
move /Y dquake.exe D:\quake
move /y qdos.exe D:\quake
cd D:\quake
del quake.exe
copy qdos.exe quake.exe
