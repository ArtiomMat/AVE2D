// This is the header that you are now worshipping.

#pragma once

#ifdef AVE
	#define EXTERN 
#else
	#define EXTERN extern
#endif

typedef long long int64;
typedef int int32;
typedef short int16;
typedef char int8;
typedef unsigned long long uint64;
typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;

typedef enum {false=0, true} aboolean;

enum ibutton
{
	KB0 = '0',
	KB1,
	KB2,
	KB3,
	KB4,
	KB5,
	KB6,
	KB7,
	KB8,
	KB9,

	KBA = 'A',
	KBB,
	KBC,
	KBD,
	KBE,
	KBF,
	KBG,
	KBH,
	KBI,
	KBJ,
	KBK,
	KBL,
	KBM,
	KBN,
	KBO,
	KBP,
	KBQ,
	KBR,
	KBS,
	KBT,
	KBU,
	KBV,
	KBW,
	KBX,
	KBY,
	KBZ,

	KBSPACE = ' ',
	KBENTER = '\n',
	KBBACKSPACE = '\b',

	// Comes from a US layout.
	KBMISC0 = '`',
	KBMISC1 = '-',
	KBMISC2 = '=',
	KBMISC3 = '[',
	KBMISC4 = ']',
	KBMISC5 = '\\',
	KBMISC6 = ';',
	KBMISC7 = '\'',
	KBMISC8 = ',',
	KBMISC9 = '.',
	KBMISC10 = '/',

	KBF1 = 0x70,
	KBF2,
	KBF3,
	KBF4,
	KBF5,
	KBF6,
	KBF7,
	KBF8,
	KBF9,
	KBF10,
	KBF11,
	KBF12,
	KBF13,
	KBF14,
	KBF15,
	KBF16,
	KBF17,
	KBF18,
	KBF19,
	KBF20,
	KBF21,
	KBF22,
	KBF23,
	KBF24,

	KBESC=0x1B,
	KBDEL=0x2E,
	KBHOME=0x24,
	KBEND=0x23,
	KBPGUP=0x21,
	KBPGDN=0x22,
	KBPRTSC=0x2C,
	KBSCRLK=0x91,
	KBPAUSE=0x13,
	KBINSERT=0x2D,

	KBLARROW=0x25,
	KBUARROW,
	KBRARROW,
	KBDARROW,

	KBLSHIFT=0x10,
	KBLCTRL=0x11,
	KBLALT=0x12,
	KBLWIN=0x5B,

	KBRSHIFT=0xA1,
	KBRCTRL=0xA3,
	KBRALT=0x12, // I think??
	KBRWIN=0x5C,

	KBTAB=0x09,
	KBCAPS=0x14,
};

// Updated every endframe()
EXTERN struct
{
	struct
	{
		// Relative to top left corner
		int x, y;
		
		// This is a difference value
		// Scrolling up increments it and vise versa
		// 0 obviously means no scrolling was done.
		char scroll;
		
		#define MBL 0
		#define MBM 1
		#define MBR 2
		// Uses same system as inp.keys
		uint8 buttons[3];
	} mouse;

	// 0 = Nothing, key is inactive.
	// 1 = Pressed this frame.
	// 2 = Pressed and released this frame.
	// 3 = Pressed for more than a frame.
	// 4 = Released this frame.
	uint8 keys[256];
} inp;

// Return value is the type of the event
// ei is the information

// Video

// Delta time in millis
EXTERN unsigned delt;

// Only one instance for every cunt
typedef struct
{
	uint16 w, h;
	// Frame count
	uint8 f;
	// The size of the buffer is w*h*f
	uint8 *data;
} imgdat;

// A sprite has to be aligned to pixels
// So it's coordinates are in ints, however
// Feel free to use floats for your characters
// And convert them to ints.
// 
// TODO: Make a linked list instead of array
// FUCK ARRAYS, ARRAYS ARE FOR PUSSIES.
typedef struct
{
	int16 x, y;

	imgdat *idp; // Image data pointer
	uint8 idf; // Current of idp frame to show
	
	aboolean show;
} sprite;
sprite *createsprite(int16 x, int16 y, imgdat *idp);

// Compensating sleep function, in milliseconds
// Thanks Cloaker :)
void csleep(unsigned time);
void vidinit(uint16 width, uint16 height, uint8 scale);
void endframe();

// DEV VARS
// - Debugging made easier
// Inspired by Quake CVARS
typedef struct dvar
{
	char name[24];
	char str[16];
	float val; // Cached float value
	void (*call)(char *);
	
	struct dvar *next;
} dvar;
// To make dev var not callabe simply set call to NULL
dvar *devnew(char *name, char *str, void (*call)(char *));
dvar *devdel(char *name);
float devgetval(char *name);
// If the dev var is callable it will set str to str
// And do call(str)
dvar *devset(char *name, char *str);
// Don't fuck around with the returned var if
// You don't know what you are doing, use functions.
dvar *devfind(char *name);
// Command syntax:
// NAME STR
// STR is optional, depending on your needs.
dvar *devcmd(char *cmd);

// Minimal collision functions
aboolean pointinrect(int x, int y,
						int rx, int ry, int rw, int rh);


// MOF(Mind fuck Of a Format) files:
// Slightly compressed files containing game data.
// 
aboolean mofexport(char *mfp, char *eid);
// Free this stuff using regular free() function
void *mofimport(char *mfp, char *eid);

