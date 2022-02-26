#ifndef F_FILES
#define F_FILES

#include "d_draw.h"

/* Ave Image Data format:
	HEADER:			0x69 'A' 'I' 'D'
	IMAGE INFO:		width(uint32_t), height(same), frames(same)
	DATA:			Sequence of palette indices, size must be width*height*frames.

	This is used for levels too in FUCKMAN. Why waste time bullshitting out identical formats?
*/

imgdat_t *AID_Load(const char *fp);
// Just throws a free at the image, you can do this yourself with half the effort.
void AID_Free(imgdat_t *id);

/* PALette format:
	ENTRIES:		256 RGB entries, each channel represented by 1 byte.
*/

char *PAL_Load(const char *fp);
char *PAL_Free(char *pal);

#endif F_FILES
