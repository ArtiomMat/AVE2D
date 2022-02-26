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

typedef struct
{
	int type;
	union
	{
		// Keyboard
		int kbutton;
		// Mouse
		enum	{ MBUTTON_L,MBUTTON_M,MBUTTON_R, } mbutton;
		struct	{ int x, y; } mpos;
		enum	{ MSCROLL_UP, MSCROLL_DOWN } mscroll;
	};
} eventinfo;

enum
{
	EVENT_NULL=0,
	
	EVENT_KPRESS,
	EVENT_KRELEASE,
	EVENT_MPRESS,
	EVENT_MRELEASE,
	
	EVENT_MPOS,
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

	KEY_F1,
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

	KEY_ESC,
	KEY_DEL,
	KEY_HOME,
	KEY_END,
	KEY_PGUP,
	KEY_PGDN,
	KEY_PRTSC,
	KEY_SCRLK,
	KEY_PAUSE,
	KEY_INSERT,

	KEY_DARROW,
	KEY_UARROW,
	KEY_RARROW,
	KEY_LARROW,

	KEY_LSHIFT,
	KEY_LCTRL,
	KEY_LALT,
	KEY_LWIN,

	KEY_RSHIFT,
	KEY_RCTRL,
	KEY_RALT,
	KEY_RWIN,

	KEY_TAB,
	KEY_CAPS,
};

// Return value is the type of the event
// ei is the information
int popevent(eventinfo *ei);
void defeventhandle(eventinfo ei);

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
		// Uses same system that keys use
		// to determine the state
		uint8 buttons[3];
	} mouse;

	// 0 = Nothing, key is inactive.
	// 1 = Pressed this frame.
	// 2 = Pressed and released this frame.
	// 3 = Pressed for more than a frame.
	// 4 = Released this frame.
	uint8 keys[256];

	// Uses key system
	uint8 tbar_close;
	uint8 tbar_ismax;
	uint8 tbar_hidden;
} inp;

// Video
typedef struct imgdat_s imgdat;

typedef struct
{
	uint16 x, y;
	imgdat *idp; // Image data pointer
} sprite;

void vidinit(uint16 width, uint16 height);
void endframe();

// AVE
void aveinit();

// Disk

// MOF(Mind fuck Of a Format) files:
// Slightly compressed files containing game data.
// 
void *mofimport(char *mfp, char *eid);
