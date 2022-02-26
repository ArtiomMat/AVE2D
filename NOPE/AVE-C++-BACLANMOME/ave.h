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

typedef struct
{
	int type;
	union
	{
		// Keyboard
		int kbutton;
		// Mouse
		enum	{ MBUTTON_L,MBUTTON_M,MBUTTON_R, } mbutton;
		struct	{ int x, y; } position;
		enum	{ MSCROLL_UP, MSCROLL_DOWN } mscroll;
	};
} eventinfo;

enum
{
	EVENT_NULL=0,
	
	EVENT_KPRESS, // Keyboard
	EVENT_KRELEASE,
	EVENT_MPRESS, // Mouse
	EVENT_MRELEASE,
	
	EVENT_MPOS, // Mouse position updated
	EVENT_MSCROLL,

	EVENT_WINDOWXBUTTON,
};

enum
{
	KEY_0 = '0',
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,

	KEY_A = 'A',
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,

	KEY_SPACE = ' ',
	KEY_ENTER = '\n',
	KEY_BACKSPACE = '\b',

	// Comes from a US layout.
	KEY_MISC0 = '`',
	KEY_MISC1 = '-',
	KEY_MISC2 = '=',
	KEY_MISC3 = '[',
	KEY_MISC4 = ']',
	KEY_MISC5 = '\\',
	KEY_MISC6 = ';',
	KEY_MISC7 = '\'',
	KEY_MISC8 = ',',
	KEY_MISC9 = '.',
	KEY_MISC10 = '/',

	KEY_F1 = 0x70,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_F13,
	KEY_F14,
	KEY_F15,
	KEY_F16,
	KEY_F17,
	KEY_F18,
	KEY_F19,
	KEY_F20,
	KEY_F21,
	KEY_F22,
	KEY_F23,
	KEY_F24,

	KEY_ESC=0x1B,
	KEY_DEL=0x2E,
	KEY_HOME=0x24,
	KEY_END=0x23,
	KEY_PGUP=0x21,
	KEY_PGDN=0x22,
	KEY_PRTSC=0x2C,
	KEY_SCRLK=0x91,
	KEY_PAUSE=0x13,
	KEY_INSERT=0x2D,

	KEY_LARROW=0x25,
	KEY_UARROW,
	KEY_RARROW,
	KEY_DARROW,

	KEY_LSHIFT=0x10,
	KEY_LCTRL=0x11,
	KEY_LALT=0x12,
	KEY_LWIN=0x5B,

	KEY_RSHIFT=0xA1,
	KEY_RCTRL=0xA3,
	KEY_RALT=0x12, // I think??
	KEY_RWIN=0x5C,

	KEY_TAB=0x09,
	KEY_CAPS=0x14,
};

// Return value is the type of the event
// ei is the information
int popevent(eventinfo *ei);
void defeventhandle(eventinfo ei);

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


// Disk

// MOF(Mind fuck Of a Format) files:
// Slightly compressed files containing game data.
// 
aboolean mofexport(char *mfp, char *eid);
// Free this stuff using regular free() function
void *mofimport(char *mfp, char *eid);

