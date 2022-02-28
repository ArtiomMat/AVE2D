// Full AI logic here.

#include "g_local.h"
#include <math.h>

char InSight(sprite_t a, sprite_t b)
{
	// Life hax with Artiom:
	// You can round integers to other integers by dividing and multiplying them!
	int ax = a.x/MAPSIZE*MAPSIZE,  ay = a.y/MAPSIZE*MAPSIZE;
	int bx = b.x/MAPSIZE*MAPSIZE,  by = b.y/MAPSIZE*MAPSIZE;

	if (ax == bx)
	{
		while (1)
		{
			if (ay > by)
				ay++;
			else
				ay--;
			if (mapid_data[ax + ay*MAPSIZE] == WALLS)
				return 0;
			if (ay == by)
				return 1;

		}
	}
	else if (ay == by)
	{

	}
	// Not in front, so no point checking
	else
		return 0;

	return 1;
}

void JessicaMove(sprite_t *m)
{

}

void AshlyMove(sprite_t *m)
{

}

void LucyMove(sprite_t *m)
{

}

void TrixMove(sprite_t *m)
{

}

void PopoMove(sprite_t *m0, sprite_t *m1)
{

}
