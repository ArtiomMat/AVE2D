// Full AI logic here.

#include "g_state.h"
#include <math.h>

char InSight(mover_t a, mover_t b)
{
	int ax = (int)round(a.x),  ay = (int)round(a.y);
	int bx = (int)round(b.x),  by = (int)round(b.y);

	if (ax == bx)
	{
		while (1)
		{
			if (ay > by)
				ay++;
			else
				ay--;
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

void JessicaMove(mover_t *m)
{

}

void AshlyMove(mover_t *m)
{

}

void LucyMove(mover_t *m)
{

}

void TrixMove(mover_t *m)
{

}

void PopoMove(mover_t *m0, mover_t *m1)
{

}
