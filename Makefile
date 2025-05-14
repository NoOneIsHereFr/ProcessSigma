all:
	echo off
	cls
	if exist .\trdasn.exe del trdasn.exe
	cl src\c\main.c src\c\autostart.c src\c\verbose.c src\c\tray.c src\c\prc.c src\c\update.c /link user32.lib shell32.lib runtimeobject.lib icon.res
	del *.obj
	ren main.exe trdasn.exe