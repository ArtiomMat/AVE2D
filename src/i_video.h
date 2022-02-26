#pragma once

#include "a_ave.h"

extern unsigned int i_delta;
// Raw pixel data pointer, enjoy :)
// This is aligned by 8 bytes, so that D_Draw
// can do some crazy shit when wiping the screen!
extern unsigned char *i_pixels;

// 0 = Nothing.
// 1 = Pressed this frame.
//		3 then next frame.
// 2 = Pressed & Released this frame.
//		0 the next frame.
// 3 = Pressed for more than 1 frames.
// 4 = Released this frame.
//		0 the next frame.
extern unsigned char i_keys[256];

extern unsigned char i_mouseb[4];
extern unsigned short i_mousex, i_mousey;

// pxsize: size of a FAKE video pixel in REAL pixels.
void I_InitVideo(int width, int height, char pxsize);
void I_KillVideo(void);
// Sequence of 256 RGB values.
// The first color in the palette is reserved for 100% transparency.
void I_SetPalette(char* palette);

void I_EndFrame(void);
