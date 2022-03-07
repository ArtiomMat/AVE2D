#pragma once

#include <stdio.h>

#include "i_video.h"
#include "i_defs.h"

#include "a_dev.h"

#include "snd_sound.h"
#include "d_draw.h"

extern char **a_v;
extern int a_c;

void A_Init(int c, char **v);
// -1 if not found, index if found.
int A_FindArg(char *arg);

// Returns path where it starts from the EXE path. For example
// If exe path is 'bin/GAME.exe' then the return is 'bin/'+'fp'
// !! Not thread safe boiz !!
char *A_RelPath(const char *fp);

#ifndef NOSPASHSHIT
	// In seconds
	void A_Splash(char time);
#endif
