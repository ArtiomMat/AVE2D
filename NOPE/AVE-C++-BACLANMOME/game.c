#include <stdio.h>
#include <stdlib.h>

#include "ave.h"

int main(int c, char **v)
{
	puts("Initializing video module...");
	vidinit(320, 200, 2);
	devset("maxfps", "12");
	
	imgdat *i = malloc(sizeof (*i));
	i->w = 20;
	i->h = 20;
	i->f = 2;
	i->data = malloc(20*20*2);
	for (int j = 0; j < 20*20; j++)
		i->data[j] = 1;
	i->data[110] = 0;
	i->data[120] = 0;
	i->data[130] = 0;
	i->data[140] = 0;
	i->data[150] = 0;
	for (int j = 0; j < 20*20; j++)
		i->data[j+20*20] = 2;
	
	sprite *s = malloc(sizeof (*s));
	s->x = 1;
	s->y = 2;
	s->idp = i;
	s->idf = 0;
	addsprite(s, 1);
	sprite *s2 = malloc(sizeof (*s));
	s2->x = 30;
	s2->y = 54;
	s2->idp = i;
	s2->idf = 1;
	addsprite(s2, 0);

	eventinfo ei;
	int alive = 1, move = 0, speed = 5;
	while (alive)
	{
		while (popevent(&ei)) // ei is of type eventinfo
		{
			switch (ei.type) // as mentioned, there are event types
			{
				case EVENT_KPRESS: // a keyboard key was pressed
				if (ei.kbutton == KEY_UARROW)
					move = 1;
				if (ei.kbutton == KEY_LARROW)
					move = 2;
				if (ei.kbutton == KEY_DARROW)
					move = 3;
				if (ei.kbutton == KEY_RARROW)
					move = 4;
				break;

				case EVENT_KRELEASE:
				move = 0;
				break;
				
				default:
				defeventhandle(ei); // Default function that handles events the user doesn't care about
				break;
			}
		}
		
		if (move == 1 && s->y > 2)
			s->y-=speed;
		else if (move == 2 && s->x > 2)
			s->x-=speed;
		else if (move == 3 && s->y+40 < 200)
			s->y+=speed;
		else if (move == 4 && s->x+20 < 320)
			s->x+=speed;
		
		// printf("%d\n", delt);
		endframe();
	}

	killsleep();
	return 0;
}
