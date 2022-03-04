// Full AI logic here.

#include "g_local.h"

#include <math.h>

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

	if (AlignedToSize(jessica))
	{
		if ( InSight(jessica, s_fuckman))
			tsaw += d_delt;
		else if (AlignedToSize(jessica) && !InSight(jessica, s_fuckman) && tsaw)
			tsaw = 0;
		
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

	static bool rage = 1;

	static dir_t dir = NODIR; // Current direction

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
			trage = 0;
			rage = true;
			ash->frame = 1;
		}
	}

	if (AlignedToSize(ash))
	{
		if ( InSight(ash, s_fuckman))
			tsaw += d_delt;
		else if (AlignedToSize(ash) && !InSight(ash, s_fuckman) && tsaw)
			tsaw = 0;
		
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
			tseenoevil += d_delt;
			// Make her go all the way
			if (tseenoevil > twander)
			{
				dir = Wander(ash, dir);
				tseenoevil = 0;
			}
		}
	}
	if (CanGo(ash, 4, dir))
		Go(ash, 4, dir);
}

void LucyMove(sprite_t *jessica)
{
	static dir_t dir = NODIR; // Current direction

	if (AlignedToSize(jessica))
	{
		dir = Wander(jessica, dir);
	}
	if (CanGo(jessica, 4, dir))
		Go(jessica, 4, dir);
}

/*
	Whenever you touch her:
	her body guard comes out to fuck you up
*/

void TrixMove(sprite_t *jessica)
{

}

