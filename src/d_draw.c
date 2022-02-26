#include "d_draw.h"
#include "i_video.h"

sprite_t *lasts[SPRITELAYERS] = {NULL, NULL, NULL,};

unsigned d_delt = 0;

// Externs from i_video.c
extern int wwidth, wheight;
extern char wpxsize;

void D_Init(const char *palfp)
{
	A_NewVar("maxfps", "30", NULL);

	FILE *f = fopen(palfp, "rb");
	if (f)
	{
		char *pal = malloc(3 * 256);
		fread(pal, 3, 256, f);

		I_SetPalette(pal);
		
		free(pal);
		fclose(f);
	}
}

sprite_t *D_NewSprite(int x, int y, imgdat_t *idp, int layer)
{
	sprite_t *s = malloc(sizeof (*s));
	s->x = x;
	s->y = y;
	s->idp = idp;
	s->prev = lasts[layer];
	s->layer = layer;
	
	// Defaults
	s->next = NULL;
	s->frame = 0;

	// Set prev for last sprite
	if (lasts[layer])
		lasts[layer]->next = s;

	lasts[layer] = s;

	return s;
}

// NULL > level > ave > NULL

// TODO: My brain is broken
// Check if it's fine.
void D_DeleteSprite(sprite_t *s)
{
	// Join the prev and next of s
	if (s->prev)
		s->prev->next = s->next;
	if (s->next)
		s->next->prev = s->prev;
	// If it doesn't have a next, it's last!
	// If it's last, we will still be pointing to it in lasts[]
	else
		lasts[s->layer] = s->prev;

	free(s);
}

unsigned tstart = 0;
void D_Draw(void)
{
	for (int i = 0; i < wwidth*wheight; i++)
		i_pixels[i] = 0;

	for (int l = 0; l < SPRITELAYERS; l++)
	{
		for (sprite_t *s = &lasts[l][0]; s; s = s->prev)
		{
			// Cap frame
			if (s->frame > s->idp->frames-1)
				s->frame = s->idp->frames-1;
			
			// Get pixel data from current frame
			char *fdat = s->idp->data + 
						s->frame * s->idp->width * s->idp->height;
			
			// FIXME: Segfault when the X and Y are below 0.
			// No wonder.
			for (int y = 0; y < s->idp->height && (s->y+y) < wheight; y++)
			{
				for (int x = 0; x < s->idp->width && (s->x+x) < wwidth; x++)
				{
					char p = fdat[y*s->idp->width + x]; // Pixel to put
					// NULL color?
					if (p == 0)
						continue;
					i_pixels[(s->y+y)*wwidth + (s->x+x)] = p;
				}
			}
		}
	}

	I_EndFrame();
	
	// FPSMAX

	// Delta between start and end of the frame.
	// WITHOUT THE SLEEPING!
	d_delt = timeGetTime() - tstart;
	
	unsigned maxfps = (unsigned)(A_VarValue("maxfps"));
	if (maxfps)
	{
		int tosleep = 1000/maxfps - d_delt;
		if (tosleep > 0)
			I_CSleep(tosleep);
	}
	
	// This is the actual delta we want to send the developer
	d_delt = timeGetTime() - tstart;
	
	tstart = timeGetTime();
}

imgdat_t *D_LoadAID(const char *fp)
{
	FILE *f = fopen(fp, "rb");
	if (!f)
		return NULL;
	
	char magic[4];
	fread(magic, 4, 1, f);
	
	imgdat_t *ret = malloc(sizeof (imgdat_t));
	fread(&ret->width, 4, 1, f);
	fread(&ret->height, 4, 1, f);
	fread(&ret->frames, 4, 1, f);

	unsigned datalen = ret->width * ret->height * ret->frames;
	ret->data = malloc (datalen);
	fread(ret->data, datalen, 1, f);

	fclose(f);
	return ret;
}

void D_UnloadAID(imgdat_t *id)
{
	free(id->data);
	free(id);
}
