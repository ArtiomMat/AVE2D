#ifndef G_STATE
#define G_STATE

// Game state stuff, externing some variables
// and defining some important structs

#include "d_draw.h"

#define CHARSIZE 24
#define MAPSIZE 20

typedef enum
{
	DIR_UP,
	DIR_LEFT,
	DIR_DOWN,
	DIR_RIGHT,
} mdir_t;

typedef struct mover_s
{
	// The ONLY thing the game should touch
	mdir_t dir;

	sprite_t *sp;
	float x, y;
	float speed;

	// 0 = not fucked
	// 1 = fucked
	// 2 = unfuckable
	char fucked;

	struct mover_s *prev;
	struct mover_s *next;
} mover_t;

extern mover_t *movers;

// Quick access, player is still in movers
// Actually should be the root mover.
extern mover_t *player;

// Obstacle map
// Describes
extern char g_omap[MAPSIZE][MAPSIZE];

// Uses g_omap
void G_InitLevel(void);

#endif /* G_STATE */

