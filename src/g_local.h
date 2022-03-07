#ifndef G_LOCAL
#define G_LOCAL

#include "g_state.h"
#define WALKABLE	0
#define HEARTS		1
#define ASH			2
#define JESSICA		3
#define LUCY		4
#define TRIX		5
#define FUCKMAN		6
#define WALLS		7

#define UNCENSORED	1
#define CENSORED	2

typedef enum
{
	NODIR=0,
	   NORTH=1,
	WEST,   EAST,
	   SOUTH,
} dir_t;

extern sprite_t *s_fuckman;
// extern char mapid_data[CHARSIZE*CHARSIZE];
extern aid_t *g_level;

extern dir_t p_dir;

extern bool jalive, aalive, lalive, talive;

extern int moneyleft;

void JessicaMove(sprite_t *jessica);
void AshleyMove(sprite_t *jessica);
void TrixMove(sprite_t *jessica);
void LucyMove(sprite_t *jessica);

bool CanGo(sprite_t *s, int speed, dir_t d);

#endif
