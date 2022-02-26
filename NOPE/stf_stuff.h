#ifndef FILES
#define FILES

#include "i_defs.h"

// STUFF files + Verbis interface

// Loop
#define OGG_LOOP		0b1
// Play in reverse
#define OGG_REVERSE		0b10
// Override all others
#define OGG_OVERR		0b100

typedef struct
{
	char *data;
	unsigned loc;
} ogg_t;

ogg_t *OGG_Open(const char *fp);
void OGG_Play(ogg_t *o, int flags);
void OGG_Stop(ogg_t *o);
void OGG_Close(ogg_t *o);

// One stuff file can be open at once
// Returns 0 if failed
bool STF_Open(const char *fp);
// Returns -1 if id was not found
// Returns actual size if success
int STF_SizeOf(const char *id);
// Remove stuff from the file
bool STF_Erase(const char *id);
// NOTE: If id is longer than 8, it will only compare the first 8 bytes.
// If id null terminated before the 8 mark, it will only be compared up to that
bool STF_Read(const char *id, char *where, unsigned maxsize);
// NOTE: If id is null terminated before the 8 mark, the empty space left 
// is filled with 0 in the STUFF file. The stuff is immidetly fwritten into
// the file.
bool STF_Write(const char *id, const char *stuff, unsigned size);
// Just fcloses and does some frees
void STF_Close(void);

#endif /* FILES */
