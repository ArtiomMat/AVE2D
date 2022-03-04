#ifndef I_DEFS
#define I_DEFS

// Contains declerations for i_misc.c too

#ifdef _WIN32
	#include <windows.h>
	#include <windowsx.h>
#elif __linux__
	#include <X11/Xlib.h>
#endif

typedef unsigned long long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef long long int64_t;
typedef int int32_t;
typedef short int16_t;
typedef char int8_t;

typedef enum {false = 0, true = 1, both = 2} bool;

void I_Init(void);
void I_Kill(void);

void I_Errorf(char *str, ...);

// Normal-ass WINAPI Sleep function wrapper
void I_Sleep(unsigned time);
// Busy sleep
void I_BSleep(unsigned time);
// The most useful and accurate of them all
// Compensating sleep
void I_CSleep(unsigned time);

typedef enum
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
} key_t;
#endif
