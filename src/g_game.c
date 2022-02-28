#include "g_local.h"

#include "d_draw.h"
#include "a_ave.h"

aid_t *g_level;
sprite_t *s_trix, *s_lucy, *s_jessica, *s_ash, *s_popo, *s_fuckman;
// This one is special, using next and prev I'll cycle through them.
sprite_t *s_money;

enum
{
	   NORTH=1,
	WEST,  EAST,
	   SOUTH,
} p_dir = 0; // No direction at first

int mouthskipper = 0;
int speed = 4;
int moneyleft = 0;

// The sprite stuff for the map
aid_t mapid;
char mapid_data[CHARSIZE*CHARSIZE];
sprite_t *maps[MAPSIZE][MAPSIZE];

void G_Update(void)
{
	// Make fuckman open and close mouth
	if (mouthskipper == 2)
	{
		mouthskipper = 0;
		s_fuckman->frame = !s_fuckman->frame;
	}
	else
		mouthskipper++;

	// Key stuff
	if (i_keys[KBUARROW] == 1 || i_keys[KBUARROW] == 2)
		p_dir = NORTH;
	else if (i_keys[KBDARROW] == 1 || i_keys[KBDARROW] == 2)
		p_dir = SOUTH;
	else if (i_keys[KBLARROW] == 1 || i_keys[KBLARROW] == 2)
		p_dir = WEST;
	else if (i_keys[KBRARROW] == 1 || i_keys[KBRARROW] == 2)
		p_dir = EAST;

	switch (p_dir)
	{
		case NORTH:
		s_fuckman->y-=speed;
		break;
		case EAST:
		s_fuckman->x+=speed;
		break;
		case WEST:
		s_fuckman->x-=speed;
		break;
		case SOUTH:
		s_fuckman->y+=speed;
		break;
	}

	// TODO: collision check
	int maxpos = MAPSIZE*CHARSIZE - CHARSIZE;
	if ((s_fuckman->x <= 0 && p_dir == WEST) || (s_fuckman->x >= maxpos && p_dir == EAST) ||
		(s_fuckman->y <= 0 && p_dir == NORTH) || (s_fuckman->y >= maxpos && p_dir == SOUTH))
	{
		if (s_fuckman->x < 0)
			s_fuckman->x = 0;
		if (s_fuckman->y < 0)
			s_fuckman->y = 0;
		if (s_fuckman->x > maxpos)
			s_fuckman->x = maxpos;
		if (s_fuckman->y > maxpos)
			s_fuckman->y = maxpos;
		p_dir = 0;	
	}
}

void G_InitLevel(void)
{
	g_level = D_LoadAID(A_RelPath("DATA/LEVEL.aid"));

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
			mapid_data[i] = 0xFF;
	}
	mapid.data = mapid_data;
	
	for (int y = 0; y < MAPSIZE; y++)
		for (int x = 0; x < MAPSIZE; x++)
		{
			if (g_level->data[MAPSIZE*y+x] == WALKABLE)
			{
				moneyleft++;
				s_money = D_NewSprite(x*CHARSIZE, y*CHARSIZE, D_LoadAID(A_RelPath("DATA/MONEY.aid")), 0);
			}
			else if (g_level->data[MAPSIZE*y+x] == TRIX)
				s_trix = D_NewSprite(x*CHARSIZE, y*CHARSIZE, D_LoadAID(A_RelPath("DATA/TRIX.aid")), 1);
			else if (g_level->data[MAPSIZE*y+x] == LUCY)
				s_lucy = D_NewSprite(x*CHARSIZE, y*CHARSIZE, D_LoadAID(A_RelPath("DATA/LUCY.aid")), 1);
			else if (g_level->data[MAPSIZE*y+x] == JESSICA)
				s_jessica = D_NewSprite(x*CHARSIZE, y*CHARSIZE, D_LoadAID(A_RelPath("DATA/JESSICA.aid")), 1);
			else if (g_level->data[MAPSIZE*y+x] == ASH)
				s_ash = D_NewSprite(x*CHARSIZE, y*CHARSIZE, D_LoadAID(A_RelPath("DATA/ASH.aid")), 1);
			else if (g_level->data[MAPSIZE*y+x] == FUCKMAN)
				s_fuckman = D_NewSprite(x*CHARSIZE, y*CHARSIZE, D_LoadAID(A_RelPath("DATA/FUCK.aid")), 1);
			else if (g_level->data[MAPSIZE*y+x] == WALLS)
				maps[x][y] = D_NewSprite (x*CHARSIZE, y*CHARSIZE, &mapid, 0);
		}
}

