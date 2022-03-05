#include <stdio.h>
#include <stdarg.h>

#include "a_ave.h"
#include "g_state.h"

void Errorf(char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	printf("CLIENT ERROR: %s\n", buf);
	exit(1);
}

void CL_Close(int code)
{
	A_WriteConfig();
	exit(code);
}

void CL_Init(void)
{
	// RemoveBu

	puts("=====================================\n\n");

	puts("Initializing system interfaces...");
	I_Init();

	int p;

	// Scaling
	int scale = 1;
	
	p = A_FindArg("-s");
	if (p != -1)
	{
		scale = atoi(a_v[p+1]);
		if (scale < 1 || scale > 2)
			Errorf("Scale value can only be between 1 and 2.");
	}
	
	I_InitVideo((MAPSIZE) * CHARSIZE, (MAPSIZE) * CHARSIZE, scale);
	// I_InitVideo(100, 100, 3);

	puts("Initializing drawing module...");
	D_Init(A_RelPath("DATA/PALETTE"));

	// Setting up some variables
	
	// puts("Reading RC file...");
	// A_ReadConfig();
	
	A_SetVar("maxfps", "24");
	A_Splash(7);

	puts("Generating the level...");
	G_InitLevel();
	
	puts("Initializing my very complex AI...");
	AI_Init();

	#ifndef KIDSMODE
	puts(
		"=======================\n"
		" WELCOME TO FUCKMAN tm \n"
		"=======================\n");
	#else
	puts(
		"=========================\n"
		" WELCOME TO YELLOWMAN tm \n"
		"=========================\n");
	#endif

	/*
	aid_t id;
	id.width = 10;
	id.height = 10;
	id.frames = 2;

	char data[200];
	for (int i = 0; i < 100; i ++)
		data[i] = 2;
	for (int i = 100; i < 100*2; i ++)
		data[i] = 1;
	
	id.data = data;

	sprite_t *s2 = D_NewSprite(25, 27, &id, 0);
	sprite_t *s3 = D_NewSprite(16, 19, &id, 2);
	s2->frame = 1;
	s3->frame = 1;
	*/
	bool g_pause = false;

	// Hard coded but it's alright
	aid_t *i_pause = D_LoadAID(A_RelPath("DATA/PAUSE.aid"));
	for (int i = 0; i < i_pause->width*i_pause->height; i++)
		if (i_pause->data[i] == 2)
			i_pause->data[i] = 0xFE;
		else if (i_pause->data[i] == 1)
			i_pause->data[i] = 0xFF;
	sprite_t *s_pause = D_NewSprite(MAPSIZE*CHARSIZE/2-120/2, MAPSIZE*CHARSIZE/2-24/2, i_pause, 2);
	s_pause->hide = 1; // Hide in beginning
	
	while (1)
	{
		if (i_keys[' '] == 1)
		{
			g_pause = !g_pause;
			if (g_pause)
				s_pause->hide = 0;
			else
				s_pause->hide = 1;
		}
		if (!g_pause)
			G_Update();
		D_Draw();
	}
}
