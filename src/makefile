cc = gcc
res = windres

lib = -lgdi32 -lwinmm
flags = -g -Wall -Wfatal-errors

ifeq ($(shell uname), Linux)	# Linux
	cc := i686-w64-mingw32-gcc
	res := i686-w64-mingw32-windres
endif

all: ../bin/GAME.exe

##################################################################

outs = a_dev.exe cl_main.exe a_splash.exe i_video.exe a_ave.exe g_game.exe i_misc.exe a_main.exe d_draw.exe g_ai.exe

a_dev.exe: a_dev.c
	$(cc) $(flags) -o $@ -c $^
a_splash.exe: a_splash.c
	$(cc) $(flags) -o $@ -c $^
cl_main.exe: cl_main.c
	$(cc) $(flags) -o $@ -c $^
i_video.exe: i_video.c
	$(cc) $(flags) -o $@ -c $^
a_ave.exe: a_ave.c
	$(cc) $(flags) -o $@ -c $^
g_game.exe: g_game.c
	$(cc) $(flags) -o $@ -c $^
i_misc.exe: i_misc.c
	$(cc) $(flags) -o $@ -c $^
a_main.exe: a_main.c
	$(cc) $(flags) -o $@ -c $^
d_draw.exe: d_draw.c
	$(cc) $(flags) -o $@ -c $^
g_ai.exe: g_ai.c
	$(cc) $(flags) -o $@ -c $^

##################################################################

../bin/GAME.exe: $(outs)
	$(cc) -o $@ $(flags) $(outs) $(lib)

clean:
	rm -rf *.exe

.PHONY: clean
#AVE.dll: ave.c ave.h
#	$(cc) -o $@ -shared $(flags) ave.c $(lib)
