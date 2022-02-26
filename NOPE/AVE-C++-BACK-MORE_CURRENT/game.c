#include <stdio.h>

#include "ave.h"

int main(int c, char **v)
{
	aveinit();
	puts("Initializing video module...");
	vidinit(500, 400);
	eventinfo ei;
	
	int alive = 1;
	while (alive)
	{
		while (popevent(&ei))
		{
			switch (ei.type)
			{
				case EVENT_KPRESS:
				printf("YO. %c WAS PRESSED.\n", ei.kbutton);

				default:
				defeventhandle(ei);
				break;
			}
		}
		endframe();
	}
	return 0;
}
