cc = gcc
res = windres

lib = -lgdi32 -lwinmm
flags = -Wall

all = *.c *.h 
src = *.c

ifeq ($(shell uname), Linux)	# Linux
	cc := i686-w64-mingw32-gcc
	res := i686-w64-mingw32-windres
endif

all: AVE.dll GAME.exe

GAME.exe: game.c ave.h
	$(cc) -o $@ -L./ $(flags) game.c $(lib) -lAVE

AVE.dll: ave.c ave.h
	$(cc) -o $@ -shared $(flags) ave.c $(lib)

AID.exe: aid.c ave.h
	$(cc) -o $@ -L./ $(flags) aid.c $(lib) -lAVE
