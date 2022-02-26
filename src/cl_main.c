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

	printf("CLIENT ERROR: %s", buf);
	exit(1);
}

void CL_Close(int code)
{
	exit(0);
}

void CL_Init(void)
{
	// RemoveBu

	puts(
		"=====================================\n\n"
		"Initializing system interfaces...");
	I_Init();

	int p;

	// Scaling
	int scale = 1;
	
	p = A_FindArg("-s");
	if (p != -1)
	{
		scale = atoi(ave_v[p+1]);
		if (scale < 1 || scale > 2)
			Errorf("Scale value can only be between 1 and 2");
	}
	
	I_InitVideo((MAPSIZE) * CHARSIZE, (MAPSIZE) * CHARSIZE, scale);
	// I_InitVideo(100, 100, 3);

	puts("Initializing drawing module...");
	D_Init("PAL");

	// Setting up some variables
	
	A_SetVar("maxfps", "12");
	A_Splash(2);

	puts("Generating the level...");
	G_InitLevel();

	puts(
		"=======================\n"
		" WELCOME TO FUCKMAN tm \n"
		"=======================\n");

	/*
	imgdat_t id;
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
	imgdat_t *id = D_LoadAID("FUCK.aid");
		// printf("FRAMES %d\nWIDHT %d\nHEIGHT %d\nDATA %d\n", id->frames, id->width, id->height, id->data[0]);
	sprite_t *s = D_NewSprite(20, 24, id, 1);
	while (1)
	{
		D_Draw();
	}
}
