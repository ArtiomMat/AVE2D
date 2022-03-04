#include "g_local.h"

#include "d_draw.h"
#include "a_ave.h"

aid_t *g_level;
sprite_t *s_trix, *s_lucy, *s_jessica, *s_ash, *s_popo, *s_fuckman;
// This one is special, using next and prev I'll cycle through them.
sprite_t *s_money;

// Current direction of fuckman
dir_t p_dir = NODIR;
// The direction to wait for when get the chance to switch direction
dir_t p_wdir = NODIR;
// Speed of Fuckman
int p_speed = 4; // CHARSIZE%p_speed must be 0!

// Mouth frame skip track
int mouthskipper = 0;
// Money left to collect, we increment it in G_InitLevel
int moneyleft = 0;

// The sprite stuff for the map
aid_t mapid;
char mapid_data[CHARSIZE*CHARSIZE];
sprite_t *maps[MAPSIZE][MAPSIZE];

#define Align(x, y)		(x/y*y)
#define Pressed(key)	i_keys[key] == 1 || i_keys[key] == 2

void Go(sprite_t *s, int speed, dir_t dir)
{
	switch (dir)
	{
		case NORTH:
		s->y-=speed;
		break;
		case EAST:
		s->x+=speed;
		break;
		case WEST:
		s->x-=speed;
		break;
		case SOUTH:
		s->y+=speed;
		break;
	}
}

dir_t InvertDir(dir_t dir)
{
	switch (dir)
	{
		case NORTH:
		return SOUTH;
		break;
		case EAST:
		return WEST;
		break;
		case WEST:
		return EAST;
		break;
		case SOUTH:
		return NORTH;
		break;
	}
	return NODIR;
}

bool AlignedToSize(sprite_t *s)
{
	int roundx = Align(s->x, s->aid->width);
	int roundy = Align(s->y, s->aid->height);

	if (roundx != s->x || roundy != s->y)
		return false;

	return true;
}

// Thing is, this really depends on alignment
bool CanGo(sprite_t *s, int speed, dir_t d)
{
	if (!d)
		return true;
	// Arbitrary game check
	int minpos = 0;
	int maxpos = MAPSIZE*CHARSIZE - CHARSIZE;
	if (
		(s->x <= minpos && d == WEST) || (s->x >= maxpos && d == EAST) ||
		(s->y <= minpos && d == NORTH) || (s->y >= maxpos && d == SOUTH)
	)
		return false;
	
	if (d)
	{

		int mapx = (s->x);
		int mapy = (s->y);
		// What I am doing is look forward to the player's direction.
		// This works fine, and never fails, but who knows.
		switch(d)
		{
			case NORTH:
			mapy-=1;
			break;
			case SOUTH:
			mapy+=s->aid->height+1;
			break;
			case EAST:
			mapx+=s->aid->width+1;
			break;
			case WEST:
			mapx-=1;
			break;
		}
		mapx /= s->aid->width;
		mapy /= s->aid->height;

		if (g_level->data[mapy*MAPSIZE + mapx] == WALLS)
			return false;
	}

	return true;
}

void G_Update(void)
{
	if (p_dir)
	{
		if (mouthskipper == 2)
		{
			mouthskipper = 0;
			s_fuckman->frame = !s_fuckman->frame;
		}
		else
			mouthskipper++;
	}
	else
	{
		mouthskipper = 0;
		s_fuckman->frame = 0;
	}

	dir_t lastdir = p_dir;

	// If pressed same direction, we will cancle the waiting direction
	// Since if you make a waiting direction, you can no longer cancle it
	// This variable solves the issue, and is used in a check down below.
	bool pressedsamedir = -1; // -1 = didn't press anything, 0 = didn't press same direction, 1 = did

	// Regular key stuff
	if (Pressed(KBUARROW))
	{
		pressedsamedir = p_dir == NORTH;
		p_dir = NORTH;
	}
	else if (Pressed(KBDARROW))
	{
		pressedsamedir = p_dir == SOUTH;
		p_dir = SOUTH;
	}
	else if (Pressed(KBLARROW))
	{
		pressedsamedir = p_dir == WEST;
		p_dir = WEST;
	}
	else if (Pressed(KBRARROW))
	{
		pressedsamedir = p_dir == EAST;
		p_dir = EAST;
	}
	// Money collection
	unsigned mapx = s_fuckman->x / s_fuckman->aid->width;
	unsigned mapy = s_fuckman->y / s_fuckman->aid->height;
	if (AlignedToSize(s_fuckman) && g_level->data[mapy*MAPSIZE + mapx] == WALKABLE && maps[mapx][mapy])
	{
		D_DeleteSprite(maps[mapx][mapy]);
		maps[mapx][mapy] = NULL; // To not fuck up
		moneyleft--;
	}
	
	if (CanGo(s_fuckman, p_speed, p_dir))
	{
		// Cancling the waiting direction
		if (pressedsamedir == 1)
			p_wdir = NODIR;

		if (!AlignedToSize(s_fuckman))
		{
			if (p_dir != lastdir)
			{
				p_wdir = p_dir;
				p_dir = lastdir;
			}
		}
		else if (p_wdir && CanGo(s_fuckman, p_speed, p_wdir))
		{
			p_dir = p_wdir;
			p_wdir = NODIR;
		}
	}
	else if (lastdir != p_dir && lastdir)
	{
		p_wdir = p_dir;
		p_dir = lastdir;
	}

	// If still can't go it means we are facing a wall!
	if (!CanGo(s_fuckman, p_speed, p_dir))
	{
		p_dir = NODIR;
		if (!CanGo(s_fuckman, p_speed, p_wdir))
			p_wdir = 0;
		else if (AlignedToSize(s_fuckman))
		{
			p_dir = p_wdir;
			p_wdir = NODIR;
		}
	}

	if (moneyleft == 0)
	{
		moneyleft--;
		puts("OH SHIT. TIME TO GET SOME BITCHES ON YOUR DICK!");
	}

	Go(s_fuckman, p_speed, p_dir);

	JessicaMove(s_jessica);
	AshleyMove(s_ash);
	LucyMove(s_lucy);
	TrixMove(s_trix);

	// TODO: collision check
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
				maps[x][y] = D_NewSprite(x*CHARSIZE, y*CHARSIZE, D_LoadAID(A_RelPath("DATA/MONEY.aid")), 0);
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

