@echo off
if not exist %1.asm goto end
@echo on
tasm %1.asm
pause
tlink %1.obj
@echo off
if exist %1.exe copy %1.exe D:\VirtualPC\DOS\driveE
if exist %1.exe @echo %1.exe copy ok! --luobing
pause
if exist *.obj del *.obj
if exist *.map del *.map
if exist *.exe del *.exe
:end