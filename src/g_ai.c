// Full AI logic here.

#include "g_local.h"

#include <math.h>

UCHAR trixp[2] = {0, 0}; // Palette indices
extern sprite_t *s_trix;
void AI_Init(void)
{
	// Find c0 and c1 for trix
	// Like a loser. I am just so lazy.
	for (int i = 0; i < 500; i++)
	{
		if (s_trix->aid->data[i] != 0)
		{
			if (trixp[0])
			{
				if (trixp[0] != s_trix->aid->data[i])
				{
					trixp[1] = s_trix->aid->data[i];
					break;
				}
			}
			else
				trixp[0] = s_trix->aid->data[i];
		}
	}
}

dir_t DirTo(sprite_t *a, sprite_t *b)
{
	int aa[2] = {a->x/CHARSIZE, a->y/CHARSIZE};
	int ab[2] = {b->x/CHARSIZE, b->y/CHARSIZE};
	
	if (aa[0] > ab[0])
		return WEST;
	else if (aa[0] < ab[0])
		return EAST;
	else if (aa[1] > ab[1])
		return NORTH;
	else if (aa[1] < ab[1])
		return SOUTH;
	
	return NODIR;
}

// FIXME: Still sees through walls!!! Wallhack!!! BAN!!!!
char InSight(sprite_t *a, sprite_t *b)
{
	// Life hax with Artiom:
	// You can round integers to other integers by dividing and multiplying them!
	int aa[2] = {a->x/CHARSIZE, a->y/CHARSIZE};
	int ab[2] = {b->x/CHARSIZE, b->y/CHARSIZE};
	int i = 0;
	
	if (aa[0] == ab[0])
		i=1;
	else if (aa[1] != ab[1])
		return 0;
	
	while (1)
	{
		if (aa[i] > ab[i])
			aa[i]--;
		else
			aa[i]++;
		if (g_level->data[aa[0] + aa[1]*MAPSIZE] == WALLS || aa[i] > MAPSIZE)
			return 0;
		else if (aa[i] == ab[i])
			break;
	}
	
	return 1;
}

// Best dir for a to run away from b
// TODO
dir_t BestDirOut(sprite_t *ai)
{
	// Don't go directly away chance 2/3
	dir_t dont = rand() % 3;
	dir_t away = InvertDir(DirTo(ai, s_fuckman));
	
	for (int i = 1; i <= 4; i++)
	{
		if (!dont && i == away)
			continue;
		if (i != DirTo(ai, s_fuckman) && CanGo(ai, 4, i))
			return i;
	}
	// Maybe the only option is away
	if (CanGo(ai, 4, away))
		return away;
	return NODIR;
}

// From Github Copilot, nice job Microsoft.
bool IsInside(sprite_t *s1, sprite_t *s2)
{
	const int limit = 6;
	if (s1->x+s1->aid->width-limit < s2->x || s1->x > s2->x+s2->aid->width-limit)
		return false;
	if (s1->y+s1->aid->height-limit < s2->y || s1->y > s2->y+s2->aid->height-limit)
		return false;
	return true;
}

dir_t Wander(sprite_t *ai, dir_t dir)
{
	dir_t nogo = InvertDir(dir); // Can't go back!
	dir_t random = rand() % 4 + 1;
	if (random != dir)
	{
		if (CanGo(ai, 4, random) && random != nogo)
			return random;
		else
		{
			// Random didn't work so look for something else
			for (dir_t i = 1; i <= 4; i++)
				if (i != random && i!=nogo && CanGo(ai, 4, i))
				{
					return i;
					break;
				}
			// If we found nothing dir stays NODIR
		}
	}
}

void JessicaMove(sprite_t *jessica)
{
	static const unsigned treact = 1; // Reaction time in millis
	static const unsigned twander = 1;
	static unsigned tseenoevil = 0; // How long did we stop seeing
	static unsigned tsaw = 0; // How long did we see fuckman?
	
	static dir_t dir = NODIR; // Current direction

	if (IsInside(s_fuckman, jessica))
		jalive = 0;

	if ( InSight(jessica, s_fuckman))
		tsaw += d_delt;
	else if (AlignedToSize(jessica) && !InSight(jessica, s_fuckman) && tsaw)
		tsaw = 0;
	if (AlignedToSize(jessica))
	{
		if (tsaw > treact)
		{
			//int fx = s_fuckman->x/CHARSIZE, fy = s_fuckman->y/CHARSIZE;
			dir = BestDirOut(jessica);
		}
		else
		{
			tseenoevil += d_delt;
			// Make her go all the way
			if (tseenoevil > twander)
			{
				dir = Wander(jessica, dir);
				tseenoevil = 0;
			}
		}
	}
	if (CanGo(jessica, 4, dir))
		Go(jessica, 4, dir);
}

void AshleyMove(sprite_t *ash)
{
	// CONSTANTS
	static const unsigned treact = 1; // Reaction time in millis
	static const unsigned twander = 1; // How long before trying to change direction
	static const unsigned tragefor = 6000; // The time to rage
	static const unsigned tminnextrage = 5000; // Minimum time till next rage
	static const unsigned tmaxnextrage = 15000; // Minimum time till next rage

	// REALTIME
	static unsigned tseenoevil = 0; // How long did we stop seeing
	static unsigned tsaw = 0; // How long did we see fuckman?
	static unsigned trage = tragefor; // How long have we been raging?
	static int tnextrage = 0; // Timer for next rage attack

	static unsigned lastx, lasty;
	static dir_t lastdir;

	static bool rage = 1;

	static dir_t dir = NODIR; // Current direction

	if (IsInside(s_fuckman, ash))
	{
		if (rage)
			CL_Close(0);
		else
			aalive = 0;
	}
	if (rage)
	{
		trage+=d_delt;
		if (trage > tragefor)
		{
			trage = 0;
			rage = false;
			tnextrage = rand() % (tmaxnextrage - tminnextrage) + tminnextrage;
			ash->frame = 0;
		}
	}
	else
	{
		tnextrage-=d_delt;
		if (tnextrage <= 0)
		{
			lastx = 0;
			lasty = 0;
			lastdir = NODIR;
			
			trage = 0;
			rage = true;
			ash->frame = 1;
		}
	}

	// Well I can't put it in the other if since they both gotta be aligned
	if (AlignedToSize(s_fuckman) && InSight(ash, s_fuckman))
	{
		lastx = s_fuckman->x;///s_fuckman->aid->width;
		lasty = s_fuckman->y;///s_fuckman->aid->height;
	}
	if ( InSight(ash, s_fuckman))
		{
			tsaw += d_delt;
		}
		else if (AlignedToSize(ash) && !InSight(ash, s_fuckman) && tsaw)
		{
			lastdir = p_dir;
			tsaw = 0;
		}
	if (AlignedToSize(ash))
	{
		
		
		if (tsaw > treact)
		{
			//int fx = s_fuckman->x/CHARSIZE, fy = s_fuckman->y/CHARSIZE;
			if (rage)
				dir = DirTo(ash, s_fuckman);
			else
				dir = BestDirOut(ash);
		}
		else
		{
			if (rage && lastdir)
			{
				sprite_t tmp;
				tmp.x = lastx;
				tmp.y = lasty;
				if (ash->x == tmp.x && ash->y == tmp.y)
				{
					dir = lastdir; // Go to his last direction
					lastdir = NODIR; // So that we don't go here when we don't see him
				}
				else
				{
					dir = DirTo(ash, &tmp);
				}
			}
			else
			{
				lastdir = NODIR; // So that we don't go here when we don't see him
				tseenoevil += d_delt;
				// Make her go all the way
				if (tseenoevil > twander)
				{
					dir = Wander(ash, dir);
					tseenoevil = 0;
				}
			}
		}
	}
	if (CanGo(ash, 4, dir))
	{
		if (rage)
			Go(ash, 4, dir);
		else
			Go(ash, 4, dir);
	}
}

void LucyMove(sprite_t *lucy)
{
	static const unsigned treact = 300; // Reaction time in millis

	static unsigned tsaw = 0;

	static dir_t dir = NODIR; // Current direction

	if (IsInside(s_fuckman, lucy))
	{
		lalive = 0;
	}

	if ( InSight(lucy, s_fuckman))
		tsaw += d_delt;
	else if (AlignedToSize(lucy) && !InSight(lucy, s_fuckman) && tsaw)
		tsaw = 0;
	if (AlignedToSize(lucy))
	{
		
		if (tsaw > treact)
			dir = DirTo(lucy, s_fuckman);
		else
			dir = Wander(lucy, dir);
	}

	if (CanGo(lucy, 3, dir))
		Go(lucy, 3, dir);
}

// When she is about to stop she blinks
// After blinking she becomes white
// Then she is a wall for a while

void BlinkTrix(bool whiten, bool useskipper)
{
	static unsigned skipper = 0;
	if (useskipper)
	{
		skipper++;
		// NOTE: it has to be even(0 is 1, 1 is 2...), since whiten keeps changing
		// And it hits every time it's the same
		if (skipper==3)
		{
			skipper = 0;
			return;
		}
	}
	int c[2] = {whiten?trixp[1]:0xFE, whiten?trixp[0]:0xFF};
	int o[2] = {whiten?0xFE:trixp[1], whiten?0xFF:trixp[0]};
	for (int i = 0; i < s_trix->aid->height*s_trix->aid->width; i++)
	{
		if (c[0] == s_trix->aid->data[i])
			s_trix->aid->data[i] = o[1];
		else if(c[1] == s_trix->aid->data[i])
			s_trix->aid->data[i] = o[0];
	}
}

void TrixMove(sprite_t *trix)
{
	static const unsigned tstopmin = 3000;
	static const unsigned tstopmax = 10000;
	static const unsigned tgomin = 4000;
	static const unsigned tgomax = 15000;

	static int tgo = 0;
	static int tstop = 0;

	static bool go = 0;
	static bool whiten = 0;

	static dir_t dir = NODIR; // Current direction

	if (IsInside(s_fuckman, trix) && moneyleft <= 0)
	{
		talive = 0;
	}

	static int lastmapx = -1, lastmapy = -1;
	if (go)
	{
		if (tgo <= 0 && AlignedToSize(trix))
		{
			// Turn her into a fucking wall
			int mapx = trix->x/trix->aid->width, mapy = trix->y/trix->aid->height;

			g_level->data[mapy * MAPSIZE + mapx] = WALLS;

			lastmapx = mapx;
			lastmapy = mapy;
			tstop = rand() % (tstopmax - tstopmin) + tstopmin;
			dir = NODIR;
			go = 0;

			BlinkTrix(1, 0);
		}
		else
		{
			if (tgo < 1000)
				BlinkTrix(whiten=!whiten, 1);
			tgo -= d_delt;
		}
	}
	else
	{
		if (tstop <= 0)
		{
			// Remove the wall previously
			if (lastmapx != -1)
				g_level->data[lastmapy * MAPSIZE + lastmapx] = WALKABLE;
			tgo = rand() % (tgomax - tgomin) + tgomin;
			go = 1;

			BlinkTrix(0, 0);
		}
		else
		{
			if (tstop < 1000)
				BlinkTrix(whiten=!whiten, 1);
			tstop -= d_delt;
		}
	}

	if (AlignedToSize(trix))
	{
		if (go)
			dir = Wander(trix, dir);
	}

	if (CanGo(trix, 2, dir))
		Go(trix, 2, dir);
}









