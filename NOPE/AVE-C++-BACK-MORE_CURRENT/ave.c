#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#include <windows.h>

#define AVE
#include "ave.h"

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

// VIDEO AND WINDOWING
struct
{
	uint16 width, height;
	HWND hwnd;
	HDC hdc;

	HBITMAP dib;
	HDC dibhdc;
	uint8 *pixels;
	RGBQUAD *palette;

	MSG msg;
} wstate;

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

void vidinit(uint16 width, uint16 height)
{
	wstate.width = width;
	wstate.height = height;
	wstate.palette = malloc(sizeof(*wstate.palette) * 256);

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

	// TODO: Maybe make this more flexible?
	wstate.hwnd = CreateWindowEx(0, CLASSNAME, "AVE GAME",
		WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!wstate.hwnd)
		errorf(1, "Window creation failed.");
	
	wstate.hdc = GetDC(wstate.hwnd);

	bi.bmiHeader.biSize          = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth         = width;
	bi.bmiHeader.biHeight        = height;
	bi.bmiHeader.biPlanes        = 1;
	bi.bmiHeader.biBitCount      = 8;
	bi.bmiHeader.biCompression   = BI_RGB;
	bi.bmiHeader.biSizeImage     = 0;
	bi.bmiHeader.biXPelsPerMeter = 0;
	bi.bmiHeader.biYPelsPerMeter = 0;
	bi.bmiHeader.biClrUsed       = 256;
	bi.bmiHeader.biClrImportant  = 256;

	bi.colors[0].rgbBlue = 0xFF;
	bi.colors[1].rgbRed = 0xFF;
	bi.colors[2].rgbGreen = 0xFF;

	if (!( wstate.dib = CreateDIBSection(wstate.hdc, (BITMAPINFO*)&bi, DIB_RGB_COLORS, (void**)&wstate.pixels, NULL, 0) ))
		errorf(1, "Creation of DIB section failed.");
	
	if (!( wstate.dibhdc = CreateCompatibleDC( wstate.hdc ) ))
		errorf(1, "Creation of compatible DC failed failed.");
	
	SelectObject( wstate.dibhdc, wstate.dib );
	if (!( SetDIBColorTable(wstate.dibhdc, 0, 256, bi.colors) ))
		errorf(1, "palette was not created.");

	for (int i = 0; i < width * height; i++)
		wstate.pixels[i] = i%3;
	
}

void endframe()
{
	// Message pipe
	while(PeekMessage(&(wstate.msg), NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&(wstate.msg));
		DispatchMessage(&(wstate.msg));
	}

	BitBlt(wstate.hdc, 0, 0, wstate.width, wstate.height, wstate.dibhdc, 0, 0, SRCCOPY);
}

// EVENT STUFF
// TODO: Could use some dynamic resizing
#define NUMEVENTS	128
eventinfo einfos[NUMEVENTS];
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
	epos++;
	einfos[epos] = e;
}

void defeventhandle(eventinfo ei)
{
	switch (ei.type)
	{
		case EVENT_WINDOWXBUTTON:;
		// TODO
		int reply = MessageBox(NULL, "Do you truely want to quit the game? No, and I should emphesize this, NNNNOOOO progress will be saved.", "AVE Warning", MB_YESNO | MB_DEFBUTTON2 | MB_ICONWARNING);
		if (reply == IDYES)
			exit(1);
		break;
	}
}

// AVE2D AND SUCH
void aveinit(int argc, char **argv)
{
	
}

LRESULT CALLBACK 
wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	eventinfo ei;

	switch (msg)
	{
		// Big brain stuff.
		case WM_KEYUP:
		ei.type = EVENT_KRELEASE;
		case WM_KEYDOWN:
		ei.type = EVENT_KPRESS;

		ei.kbutton = wp;
		// TODO: Parse VK_ shit. I'm lazy AF right now.

		if (msg == WM_KEYDOWN)
			ei.type = EVENT_KPRESS;
		else
			ei.type = EVENT_KRELEASE;
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
