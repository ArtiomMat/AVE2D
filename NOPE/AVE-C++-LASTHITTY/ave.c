#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <windows.h>
#include <windowsx.h>

#define AVE
#include "ave.h"

// COMMON FUNCTIONS



static void errorf(boolean fatal, char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	if (fatal)
	{
		MessageBox(NULL, buf, "AVE Fatal", MB_OK | MB_ICONERROR);
		exit(1);
	}
	else
		MessageBox(NULL, buf, "AVE Warning", MB_OK | MB_ICONWARNING);
}

static dvar *dvlast = NULL;

// DEVELOPER VARIABLES
dvar *devnew(char *name, char *str, void (*call)(char *))
{
	dvar *dv = malloc(sizeof (*dv));
	// Set up the variable
	astrcpy(dv->name, name, 24);// name
	astrcpy(dv->str, str, 16);	// str
	dv->val = atof(dv->str);	// val
	dv->call = call;			// call
	dv->next = dvlast;			// next
	
	dvlast = dv;
	return dv;
}

dvar *devdel(char *name)
{
	// We need: PREV -> SELF -> NEXT
	// devfind wont help us much here, so...
	dvar *prev, *self, *next;
	for (prev = dvlast; prev; prev = prev->next)
		if (!strcmp(prev->next->name, name))
		{
			self = prev->next;
			next = self->next;
			break;
		}
	// If there is no self, nothing was found.
	if (!self)
		return NULL;
	
	free(self);
	prev->next = next;
	return prev;
}

float devgetval(char *name)
{
	dvar *dv = devfind(name);
	if (dv)
		return dv->val;
	return 0;
}

dvar *devset(char *name, char *str)
{
	dvar *dv = devfind(name);
	if (dv)
	{
		astrcpy(dv->str, str, 16);
		dv->val = atof(dv->str);
		// Call if callable
		if (dv->call)
			dv->call(dv->str);
	}
	
	return dv;
}

dvar *devfind(char *name)
{
	dvar *dv;
	for (dv = dvlast; dv; dv = dv->next)
		if (!strcmp(name, dv->name))
			return dv;
	
	return NULL;
}

// TODO
dvar *devcmd(char *cmd)
{
	return NULL;
}

// TIME FUNCTIONS
static TIMECAPS tc;

void initsleep()
{
	// Set sleep to highest resolution
	timeGetDevCaps(&tc, sizeof (TIMECAPS));
	timeBeginPeriod(tc.wPeriodMin);
}

void killsleep()
{
	timeEndPeriod(tc.wPeriodMin);
}

// The function from WinAPI but with cast
#define QueryPerformanceCounter(X)\
	QueryPerformanceCounter((LARGE_INTEGER*)X);

void sleep(unsigned time)
{
	Sleep(time);
}

void busysleep(unsigned time)
{
	long qpc;
	QueryPerformanceCounter(&qpc);
	long end = qpc + time;
	while (qpc < end)
		QueryPerformanceCounter(&qpc);
}

void csleep(unsigned time)
{
	sleep((time / tc.wPeriodMin) * tc.wPeriodMin);
	busysleep(time % tc.wPeriodMin);
}

// DRAWING
// TODO: Make a linked list instead of array
// FUCK ARRAYS, ARRAYS ARE FOR PUSSIES.

#define MAXSPRITES	256
#define MAXLAYERS	4
struct
{
	sprite *ss[MAXSPRITES][MAXLAYERS];
	uint8 si[MAXLAYERS];
} drw;

int addsprite(sprite *s, int layer)
{
	// Not overflowing current layer? + Layer within range?
	if (drw.si[layer] >= MAXSPRITES-1 || layer >= MAXLAYERS)
		return -1;
	drw.ss[drw.si[layer]][layer] = s;
	return drw.si[layer]++;
}

// video and windowing struct
struct
{
	uint16 width, height;
	HWND hwnd;
	HDC hdc;

	HBITMAP dib;
	HDC dibhdc;
	uint8 *pixels;
	RGBQUAD *palette;

	uint8 pxsize;

	MSG msg;
} wstate;

#define KEYQSIZE 128

// Pressed and released key index queue
// Every time a key is pressed for the first time (1)
// It goes here, then the next endframe call it becomes
// held down (3)
// Same goes for rkeys but when it's 2 or 4 and it is set
// back to 0
//
// Why is it 128 keys long? Even if someone throws
// their keyboard at the wall, good luck pressing/releasing
// 128 keys at once.
uint8	pkeys[KEYQSIZE] = {0},
		rkeys[KEYQSIZE] = {0},
		pki = 0, rki = 0;

// Holds the time where the new frame started
unsigned tstart = 0;
void endframe()
{
	int i;

	// Setting all inp.keys that are 1 to 3
	for (i = 0; i < pki; i++)
		inp.keys[pkeys[i]] = 3;

	// Setting all inp.keys that are 2/4 to 0
	for (i = 0; i < rki; i++)
		inp.keys[rkeys[i]] = 0;

	// Message pipe
	while(PeekMessage(&(wstate.msg), NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&(wstate.msg));
		DispatchMessage(&(wstate.msg));
	}
	
	for (int i = 0; i < wstate.width*wstate.height; i++)
		wstate.pixels[i] = 0;

	// Real window dimentions
	uint16 rw = wstate.width*wstate.pxsize, rh = wstate.height*wstate.pxsize;
	
	for (int l = 0; l < MAXLAYERS; l++)
	{
	for (int i = 0; i < drw.si[l]; i++)
	{
		sprite *s = drw.ss[i][l];
		// Cap frame
		if (s->idf > s->idp->f-1)
			s->idf = s->idp->f-1;
		
		// Get pixel data from current frame
		uint8 *fdat = s->idp->data + 
					 s->idf * s->idp->w * s->idp->h;
		
		// FIXME: Segfault when the X and Y are below 0.
		// No wonder.
		for (int y = 0; y < s->idp->h && (s->y+y) < wstate.height; y++)
		for (int x = 0; x < s->idp->w && (s->x+x) < wstate.width; x++)
		{
			uint8 p = fdat[y*s->idp->w + x]; // Pixel to put
			// NULL color?
			if (p == 0)
				continue;
			wstate.pixels[(s->y+y)*wstate.width + (s->x+x)] = p;
		}
	}
	}
	
	/*for (int i = 0; i < 10; i++)
	for (int j = 0; j < 10; j++)
		wstate.pixels[i*fw + j] = i%3;*/

	if (wstate.pxsize > 1)
		StretchBlt(wstate.hdc, 0, 0, rw, rh, wstate.dibhdc, 0,0, wstate.width, wstate.height, SRCCOPY);
	else
		BitBlt(wstate.hdc, 0, 0, wstate.width, wstate.height, wstate.dibhdc, 0, 0, SRCCOPY);
	
	// Delta between start and end of the frame.
	// WITHOUT THE SLEEPING!
	delt = timeGetTime() - tstart;
	
	unsigned maxfps = (unsigned)(devgetval("maxfps"));
	if (maxfps)
	{
		int tosleep = 1000/maxfps - delt;
		if (tosleep > 0)
			csleep(tosleep);
	}
	
	// This is the actual delta we want to send
	delt = timeGetTime() - tstart;
	
	tstart = timeGetTime();
}

// AVE2D AND SUCH
void aveinit(int argc, char **argv)
{
}

struct
{
	BITMAPINFOHEADER bmiHeader;
	RGBQUAD colors[256];
} bi;

void setpalettecolor(uint8 index, uint8 r, uint8 g, uint8 b)
{
	wstate.palette[index].rgbRed = r;
	wstate.palette[index].rgbGreen = g;
	wstate.palette[index].rgbBlue = b;
}

LRESULT CALLBACK 
wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

void vidinit(uint16 width, uint16 height, uint8 pxsize)
{
	wstate.width = width;
	wstate.height = height;
	wstate.palette = malloc(sizeof(*wstate.palette) * 256);
	wstate.pxsize = pxsize;

	// Create the class
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = wndproc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	#define CLASSNAME "AVE2D"
	wc.lpszClassName = CLASSNAME;
	if (!RegisterClassEx(&wc))
		errorf(1, "Class failed to register.");

	wstate.hwnd = CreateWindowEx(0, CLASSNAME, "AVE GAME",
		WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wstate.width*pxsize, wstate.height*pxsize,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!wstate.hwnd)
		errorf(1, "Window creation failed.");
	
	wstate.hdc = GetDC(wstate.hwnd);

	bi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth         = width;
	bi.bmiHeader.biHeight        = -height;
	bi.bmiHeader.biPlanes        = 1;
	bi.bmiHeader.biBitCount      = 8;
	bi.bmiHeader.biCompression   = BI_RGB;
	bi.bmiHeader.biSizeImage     = 0;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed       = 256;
	bi.bmiHeader.biClrImportant  = 256;

	bi.colors[1].rgbRed = 0xFF;
	bi.colors[2].rgbGreen = 0xFF;

	if (!( wstate.dib = CreateDIBSection(wstate.hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&wstate.pixels, NULL, 0) ))
		errorf(1, "Creation of DIB section failed.");
	
	if (!( wstate.dibhdc = CreateCompatibleDC( wstate.hdc ) ))
		errorf(1, "Creation of compatible DC failed failed.");
	
	SelectObject( wstate.dibhdc, wstate.dib );
	if (!( SetDIBColorTable(wstate.dibhdc, 0, 256, bi.colors) ))
		errorf(1, "Palette was not created.");
	
	// Make some dev vars
	// TODO: Figure out how to make endframe()
	// know about maxfps's value without calling
	// devgetval() every time
	devnew("maxfps", "30", NULL);

	// Initialize drawer
	for (int i = 0; i < MAXLAYERS; i++)
		drw.si[i] = 0;
	
	// Initialize sleep, for use with maxfps
	tstart = timeGetTime();
	initsleep();
}

// FIXME: Sending release event right after press event
// ITS BECAUSE I AM FUCKING PUSHING AND POPPING
// I AM POPPING IN REVERSE YOU FUCKING ABSOLUTE DUMBFUCK.
LRESULT CALLBACK 
wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		// Big brain stuff.
		case WM_KEYUP:
		case WM_KEYDOWN:;
		

		// Translation TODO
		uint8 i = wp;
		switch (i)
		{
			case VK_OEM_2:
			i = '/';
			case VK_OEM_3:
			i = '`';
		}

		if (msg == WM_KEYDOWN && !inp.keys[i])
		{
			inp.keys[i] = 1;
			pkeys[pki++] = i;
		}
		// WM_KEYUP
		else
		{
			if (inp.keys[i] == 1)
				inp.keys[i] = 2;
			else
				inp.keys[i] = 4;
			rkeys[rki++] = i;
		}

		break;
		
		
		case WM_MOUSEMOVE:
		
		_updatempos:
		
		inp.mouse.x = GET_X_LPARAM(lp);
		inp.mouse.y = GET_Y_LPARAM(lp);
		break;
		
		case WM_CLOSE:;
		int reply = MessageBox(NULL, "Are you sure about this?", "Going so soon?", MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING);
		if (reply == IDYES)
			exit(0);
		break;

		default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}
