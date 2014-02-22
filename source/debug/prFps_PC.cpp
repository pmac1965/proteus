/**
 * prFps_PC.cpp
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)


// Exclude MFC
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WIN32_EXTRA_LEAN
#define WIN32_EXTRA_LEAN
#endif


#include <windows.h>
#include "prFps_PC.h"
#include "prTrace.h"
#include "prDebug.h"


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prFps_PC::prFps_PC()
{
    LARGE_INTEGER countsPerSecond;

    if (QueryPerformanceFrequency(&countsPerSecond))
    {
        ticksPerSecond = countsPerSecond.QuadPart;
    }
    else
    {
        ticksPerSecond = 0LL;
    }

    Reset();
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prFps_PC::~prFps_PC()
{
}


/// ---------------------------------------------------------------------------
/// Resets the timing data, should be called on game entry, etc.
/// ---------------------------------------------------------------------------
void prFps_PC::Reset()
{
    prFps::Reset();

    timeTotal     = 0LL;
    timeStart     = 0LL;
    timeEnd       = 0LL;
}


/// ---------------------------------------------------------------------------
/// Begins the timing period.
/// ---------------------------------------------------------------------------
void prFps_PC::Begin()
{
    LARGE_INTEGER timenow;

    if (QueryPerformanceCounter(&timenow))
    {
        timeStart = timenow.QuadPart;
    }
}


/// ---------------------------------------------------------------------------
/// Ends the timing period.
/// ---------------------------------------------------------------------------
void prFps_PC::End()
{
    LARGE_INTEGER timenow;

    if (QueryPerformanceCounter(&timenow))
    {
        // Find the time taken.
        timeEnd    = timenow.QuadPart;
        timeTotal += (timeEnd - timeStart);


        // Exceeded one second?
        if (timeTotal >= ticksPerSecond)
        {
            frameRate  = frames;            // Sets the number of frames completed.
            frames     = 0;                 // Resets the frame count.
            timeTotal -= ticksPerSecond;    // Removes one second from the total time taken.

            // Set min/max
            if (frameRate < frameRateMin)
                frameRateMin = frameRate;

            if (frameRate > frameRateMax)
                frameRateMax = frameRate;
        }
        else
        {
            // Okay, completed another frame.
            frames++;
        }
    }
}


#endif//PLATFORM_PC

