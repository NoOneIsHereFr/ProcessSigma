all:
	@echo off
	cls
	rc /fo icon.res icon.rc
	cl src\c\main.c src\c\autostart.c src\c\verbose.c src\c\tray.c src\c\prc.c /link user32.lib shell32.lib runtimeobject.lib icon.res
	if not exist .\build mkdir .\build
	move *.obj .\build\