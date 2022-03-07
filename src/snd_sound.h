#ifndef SND_SOUND
#define SND_SOUND

#include "i_defs.h"

// Sadly using OpenAL, will probably go for lower level shit later.

#define SND_LOOP		0B1
#define SND_REVERSE		0B10
#define SND_LOLFUNNY	0B100

typedef struct
{
	int vol;
	int flags;
} sndinst_t;

bool SND_Init(void);

void SND_GlobalVolume(int vol);

sndinst_t *SND_Open(const char *fp);
void SND_Play(sndinst_t *s);

#endif