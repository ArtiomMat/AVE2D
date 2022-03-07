// Windows

#include "cl_main.h"
#include "a_dev.h"
#include "i_video.h"

#define WSTYLE	WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE
#define CLASSNAME "AVE2D"

typedef struct
{
	BITMAPINFOHEADER header;
	RGBQUAD colors[256];
} dibinfo_t;


// I shamelesly copied from Quake 2
typedef struct
{
	WORD palVersion;
	WORD palNumEntries;
	PALETTEENTRY palEntries[256];
} identitypalette_t;
static identitypalette_t s_ipal;

HWND hwnd;
HDC hdc;

HBITMAP hdib;
HDC hdibdc;

HPALETTE hpal;
HPALETTE hpalold;

MSG msg;

int wwidth = 0, wheight = 0;
char wpxsize = 1;

// Globals
unsigned int i_delta = 0;

unsigned char *i_pixels __attribute__ ((aligned (8)));

unsigned char i_keys[256];

unsigned char i_mouseb[4];
unsigned short i_mousex, i_mousey;

LRESULT CALLBACK 
wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

void I_InitVideo(int width, int height, char pxsize)
{
	wwidth = width;
	wheight = height;

	wpxsize = pxsize;

	// Create the class
	WNDCLASSEX wc = {0};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = wndproc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = CLASSNAME;
	if (!RegisterClassEx(&wc))
		I_Errorf("Class failed to register.");

	// Adjusting the window size
	// Thank you so much Id for making Quake open source.
	RECT r;
	int w, h;
	
	r.left = 0;
	r.top = 0;
	r.right = width*pxsize;
	r.bottom = height*pxsize;
	
	AdjustWindowRect (&r, WSTYLE, 0);
	w = r.right - r.left;
	h = r.bottom - r.top;

	hwnd = CreateWindowEx(0, CLASSNAME, "AVE GAME",
		WSTYLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		w, h,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	if (!hwnd)
		I_Errorf("Window creation failed.");
	
	hdc = GetDC(hwnd);
	
	dibinfo_t di;
	di.header.biSize          = sizeof(BITMAPINFOHEADER);
	di.header.biWidth         = width;
	di.header.biHeight        = -height;
	di.header.biPlanes        = 1;
	di.header.biBitCount      = 8;
	di.header.biCompression   = BI_RGB;
	di.header.biSizeImage     = 0;
	di.header.biXPelsPerMeter = 0;
	di.header.biYPelsPerMeter = 0;
	di.header.biClrUsed       = 256;
	di.header.biClrImportant  = 256;

	di.colors[0].rgbBlue = 0x00;
	di.colors[0].rgbRed = 0x00;
	di.colors[0].rgbGreen = 0x00;

	di.colors[1].rgbRed = 0xFF;
	di.colors[1].rgbBlue = 0xFF;
	di.colors[1].rgbGreen = 0xFF;

	di.colors[2].rgbGreen = 0xFF;
	di.colors[2].rgbRed = 0x00;
	di.colors[2].rgbBlue = 0x00;
	
	di.colors[3].rgbGreen = 0x99;
	di.colors[3].rgbRed = 0x99;
	di.colors[3].rgbBlue = 0x99;

	if (!( hdib = CreateDIBSection( hdc, (BITMAPINFO*)&di, DIB_RGB_COLORS, (void**)&i_pixels, NULL, 0) ))
		I_Errorf("Creation of DIB section failed.");
	
	if (!( hdibdc = CreateCompatibleDC( hdc ) ))
		I_Errorf("Creation of compatible DC failed failed.");
	
	SelectObject( hdibdc, hdib );
	if (!( SetDIBColorTable(hdibdc, 0, 256, di.colors) ))
		I_Errorf("Palette was not created.");
	
}

void UpdateKeyStates()
{
	for (int i = 0; i < 256; i++)
	{
		// Held down for more than 1 frame...
		if (i_keys[i] == 1)
			i_keys[i] = 3;
		// Released in any way...
		else if (i_keys[i] == 2 || i_keys[i] == 4)
			i_keys[i] = 0;
	}
}

// This is about 90% stolen from Quake 2 source code, thanks :)
void I_SetPalette(char* pal)
{
	char *_pal = pal; // Save of pal
	LOGPALETTE *logpal = ( LOGPALETTE * ) &s_ipal;
	RGBQUAD			colors[256];

	for (int i = 0; i < 254; i++, pal += 3 )
	{
		colors[i].rgbRed   = pal[0];
		colors[i].rgbGreen = pal[1];
		colors[i].rgbBlue  = pal[2];
		colors[i].rgbReserved = 0;
	}
	colors[0].rgbRed   = 0;
	colors[0].rgbGreen = 0;
	colors[0].rgbBlue  = 0;
	colors[0].rgbReserved = 0;
	
	colors[255].rgbRed   = 0xFF;
	colors[255].rgbGreen = 0xFF;
	colors[255].rgbBlue  = 0xFF;
	colors[255].rgbReserved = 0;

	colors[254].rgbRed   = 0;
	colors[254].rgbGreen = 0;
	colors[254].rgbBlue  = 0;
	colors[254].rgbReserved = 0;

	SetDIBColorTable(hdibdc, 0, 256, colors);
/*
		int i;
		HPALETTE hpalOld;

		if ( SetSystemPaletteUse( hdc, SYSPAL_NOSTATIC ) == SYSPAL_ERROR )
		{
			I_Errorf("Failed to set system palette.");
		}

		if ( hpal )
		{
			DeleteObject( hpal );
			hpal = 0;
		}

		logpal->palVersion		= 0x300;
		logpal->palNumEntries	= 256;

		for ( i = 0, pal = _pal; i < 256; i++, pal += 3 )
		{
			logpal->palPalEntry[i].peRed	= pal[0];
			logpal->palPalEntry[i].peGreen	= pal[1];
			logpal->palPalEntry[i].peBlue	= pal[2];
			logpal->palPalEntry[i].peFlags	= PC_RESERVED | PC_NOCOLLAPSE;
		}

		if ( ( hpal = CreatePalette( logpal ) ) == NULL )
		{
			I_Errorf(" Creating a palette failed(%x)\n", GetLastError() );
		}

		if ( ( hpalOld = SelectPalette( hdc, hpal, FALSE ) ) == NULL )
		{
			I_Errorf(" SelectPalette failed(%x)\n",GetLastError() );
		}

		if ( hpalold == NULL )
			hpalold = hpalold;

		unsigned ret;
		if ( ( ret = RealizePalette( hdc ) ) != logpal->palNumEntries ) 
		{
			I_Errorf(" RealizePalette set %d only entries.\n", ret );
		}*/
}

// Holds the time where the new frame started
void I_EndFrame(void)
{
	UpdateKeyStates();

	// Message pipe
	while(PeekMessage(&(msg), NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&(msg));
		DispatchMessage(&(msg));
	}

	int rw = wwidth*wpxsize,
		rh = wheight*wpxsize;

	if (wpxsize > 1)
		StretchBlt(hdc, 0, 0, rw, rh, hdibdc, 0,0, wwidth, wheight, SRCCOPY);
	else
		BitBlt(hdc, 0, 0, wwidth, wheight, hdibdc, 0, 0, SRCCOPY);
}

// Just a note: the wndproc pretty much redirects shit to their
// modules, parsing of messages actually happens in different parts
// of the source code! Thanks!
LRESULT CALLBACK 
wndproc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
		case WM_KEYDOWN:
		case WM_KEYUP:;
		
		int k = wp;
		switch (k)
		{
			case VK_OEM_2:
			k = '/';
			case VK_OEM_3:
			k = '`';
		}


		if (msg == WM_KEYDOWN && !i_keys[k])
		{
			i_keys[k] = 1;
		}
		
		else if (msg == WM_KEYUP)
		{
			if (i_keys[k] == 1)
				i_keys[k] = 2;
			else
				i_keys[k] = 4;
		}
		break;

		case WM_CLOSE:
		CL_Close(0);

		default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
	return 0;
}
