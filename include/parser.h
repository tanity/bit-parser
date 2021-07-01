#define _CRT_SECURE_NO_WARNINGS  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STOPWATCH
#define PS_NAME

#ifdef _WIN32
#ifdef STOPWATCH
#include <windows.h>
double tick()
{
	LARGE_INTEGER frequency;
	static LARGE_INTEGER time, otime;

	otime = time;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&time);
	return (double)(time.QuadPart - otime.QuadPart) / frequency.QuadPart * 1000;
}
#endif
#ifdef _WIN64
#define int __int64
#endif
#else
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>

#ifdef STOPWATCH
#include <time.h>
double tick()
{
	static clock_t time, otime;
	otime = time;
	time = clock();
	return (double)(time - otime) / CLOCKS_PER_SEC * 1000;
}
#endif

#endif

#include "lib.h"
#include "event.h"
#include "state.h"
#include "ps.h"
#include "tag.h"
#include "load.h"
