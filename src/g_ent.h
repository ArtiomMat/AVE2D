#ifndef G_ENT
#define G_ENT

#include "a_ave.h"

#define NTRAIT	0xFFFFFFFF

typedef struct
{
	sprite_t *s;
} t_drawable_t;

typedef struct
{
	
} t_ai_t;

typedef struct
{
	unsigned
	t_drawable,
	t_ai,
	t_movable,
	t_base;
} entinfo_t;

#endif
