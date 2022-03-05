#ifndef G_STATE
#define G_STATE

// Game state stuff, externing some variables
// and defining some important structs

#include "d_draw.h"

#define KIDSMODE

#define CHARSIZE 24
#define MAPSIZE 20

void AI_Init(void);

void G_InitLevel(void);
void G_Update(void);

#endif /* G_STATE */

