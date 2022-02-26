#include "g_state.h"
#include "d_draw.h"

char g_omap[MAPSIZE][MAPSIZE] =
{
	{1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,},
	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},
	{1,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,0,1,},
	{1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,},
	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},

	{1,0,0,0,0, 0,1,0,0,0, 0,0,0,1,0, 1,0,0,0,1,},
	{1,1,1,1,0, 1,1,1,1,0, 0,1,1,1,0, 1,1,1,1,1,},
	{1,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,1,},
	{1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,},
	{1,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,1,},

	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},
	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},
	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},
	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},
	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},

	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},
	{1,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,1,},
	{1,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,0,1,},
	{1,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,1,},
	{1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,},
};

// The sprite stuff for the map
imgdat_t mapid;
char mapid_data[CHARSIZE*CHARSIZE];
sprite_t *maps[MAPSIZE][MAPSIZE];

void G_InitLevel(void)
{
	mapid.width = CHARSIZE;
	mapid.height = CHARSIZE;
	mapid.frames = 1;
	// Setup mapid data
	for (int i = 0; i < CHARSIZE*CHARSIZE; i++)
	{
		int x = i % CHARSIZE, y = i / CHARSIZE;
		// Check if in corners
		if (y == 0 || x == 0 || x == CHARSIZE-1 || y == CHARSIZE-1)
			mapid_data[i] = 0;
		else
			mapid_data[i] = 3;
	}
	mapid.data = mapid_data;
	
	for (int y = 0; y < MAPSIZE; y++)
		for (int x = 0; x < MAPSIZE; x++)
		{
			if (g_omap[y][x])
				maps[x][y] = D_NewSprite (x*CHARSIZE, y*CHARSIZE, &mapid, 0);
		}
}
