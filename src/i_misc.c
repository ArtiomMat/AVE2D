// Stuff I had no idea where to put is here

#include <stdarg.h>
#include <stdio.h>

#include "i_defs.h"

// For sleep functions. Setting the highest resolution of the cloak
static TIMECAPS tc;

void I_Init(void)
{
	// Set sleep to highest resolution
	timeGetDevCaps(&tc, sizeof (TIMECAPS));
	timeBeginPeriod(tc.wPeriodMin);
}

void I_Kill(void)
{
	// Set the resolution back
	timeEndPeriod(tc.wPeriodMin);
}

void I_Errorf(char *fmt, ...)
{
	va_list args;
	char buf[1024];

	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);

	MessageBox(NULL, buf, "AVE Error", MB_OK | MB_ICONERROR);
	exit(1);
}

// The function from WinAPI but with cast
#define QueryPerformanceCounter(X)\
	QueryPerformanceCounter((LARGE_INTEGER*)X);

void I_Sleep(unsigned time)
{
	Sleep(time);
}

void I_BSleep(unsigned time)
{
	long qpc;
	QueryPerformanceCounter(&qpc);
	long end = qpc + time;
	while (qpc < end)
		QueryPerformanceCounter(&qpc);
}

void I_CSleep(unsigned time)
{
	I_Sleep((time / tc.wPeriodMin) * tc.wPeriodMin);
	I_BSleep(time % tc.wPeriodMin);
}
