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

// Safe ave strcpy
// Returns 1 if copied all bytes to dest
char astrcpy(char *dest, const char *src, unsigned ds)
{
	unsigned i;
	unsigned ss = strlen(src);
	
	for (i = 0; i < ds-1 && i < ss; i++)
		dest[i] = src[i];
	dest[i] = 0; // Finish up with NULL character
	
	return ds == ss;
}

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

dvar *dvlast = NULL;

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

// EVENT STUFF
eventinfo *einfos;
int esize = 128;
int epos = -1;

int popevent(eventinfo *e)
{
	if (epos < 0)
	{
		epos = -1; // Just incase
		return 0;
	}
	
	*e = einfos[epos];
	epos--;
	return 1;
}

// Handles the addition of new events
void pushevent(eventinfo e)
{
	if (epos >= esize-1)
		einfos = realloc(einfos, esize*=2);
	epos++;
	einfos[epos] = e;
}

void defeventhandle(eventinfo ei)
{
	switch (ei.type)
	{
		case EVENT_WINDOWXBUTTON:;
		int reply = MessageBox(NULL, "Do you truely want to quit the game? No, and I should emphesize this, NNNNOOOO progress will be saved.", "AVE Warning", MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING);
		if (reply == IDYES)
			exit(1);
		break;
	}
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

// Holds the time where the new frame started
unsigned tstart = 0;
void endframe()
{
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
	
	// Initialize event system
	einfos = malloc(esize);

	// Initialize drawer
	for (int i = 0; i < MAXLAYERS; i++)
		drw.si[i] = 0;
	
	// Initialize sleep, for use with maxfps
	tstart = timeGetTime();
	initsleep();
}

// Keeps track of the state of the keys
// If they were pressed or not
uint8 keys[256] = {0};
uint8 buts[3] = {0};
// This variable saves the time of tstart
// It is updated every push of EVENT_K/M*
// There is a strange phenomenon where if you
// press and release a key in the same frame
// first the release is sent and then the press.
// This variable is used to compare with tstart
// and thus check if we are in the same frame, then
// block 
unsigned tsave = 0;

// FIXME: Sending release event right after press event
// ITS BECAUSE I AM FUCKING PUSHING AND POPPING
// I AM POPPING IN REVERSE YOU FUCKING ABSOLUTE DUMBFUCK.
LRESULT CALLBACK 
wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	eventinfo ei;

	switch (msg)
	{
		// Big brain stuff.
		case WM_KEYUP:
		case WM_KEYDOWN:
		// An extremely dirty way to check if we already
		// sent a message to the developer about the key
		// being pressed, since windows keeps sending the
		// WM_KEYDOWN message when the key is HELD DOWN.
		if (msg == WM_KEYDOWN)
		{
			puts("Press");
			
			if (keys[wp])
				return 0;
			else
				keys[wp] = 1;
			// Also, I'm setting the types here since
			// in the case statements they override each
			// other, and dont do what I want them to do
			ei.type = EVENT_KPRESS;
		}
		else
		{
			puts("Release");
			
			ei.type = EVENT_KRELEASE;
		}
		
		ei.kbutton = wp;
		
		// No parsing required, how great.

		pushevent(ei);
		break;
		
		/*case WM_RMOUSEDOWN:
		ei.type = EVENT_MPOS;
		goto _addmpos;
		// TODO: AAAAAAAAAAAAAAAAAAAAAAA LAZY
		*/
		case WM_MOUSEMOVE:
		ei.type = EVENT_MPOS;
		_addmpos:
		ei.position.x = GET_X_LPARAM(lp);
		ei.position.y = GET_Y_LPARAM(lp);
		pushevent(ei);
		break;
		
		case WM_CLOSE:
		pushevent((eventinfo){ EVENT_WINDOWXBUTTON });
		break;

		default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}
