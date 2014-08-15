/**
 * prFps_PC.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
        //prTrace("beg\n");
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


        //prTrace("FPS: %llu, %llu, %llu\n", timeEnd, timeTotal, ticksPerSecond);


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

