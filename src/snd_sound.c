#include "snd_sound.h"

#include "i_defs.h"

#include "al.h"
#include "alc.h"

ALCdevice *device;

bool SND_Init(void)
{
	if (!( alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT") ))
	{
		puts("SND_Init: Enumeration extension not present. Like I know what the fuck that means.");
		return false;
	}

	if (!( device = alcOpenDevice(NULL) ))
	{
		puts("SND_Init: Could not open device.");
		return false;
	}

	alGetError();

	return true;
}
