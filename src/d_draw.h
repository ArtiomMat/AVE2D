#ifndef D_DRAW
#define D_DRAW

#include "i_defs.h"

#define SPRITELAYERS	3

extern unsigned d_delt;

typedef struct imgdat_s
{
	unsigned width, height, frames;
	char *data;
} aid_t;

typedef struct sprite_s
{
	int x, y, frame;
	aid_t *aid;
	
	unsigned char layer;
	struct sprite_s *next;
	struct sprite_s *prev; 
} sprite_t;

void D_Init(const char *palfp);

void D_Draw(void);

aid_t *D_LoadAID(const char *fp);
void D_UnloadAID(aid_t *id);

// You better not leak out this pointer.
sprite_t *D_NewSprite(int x, int y, aid_t *aid, int layer);
void D_DeleteSprite(sprite_t *s);
void D_DeleteAllSprites(void);

#endif /* D_DRAW */
