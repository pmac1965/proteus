/**
 * prFps_Android.cpp
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


#if defined(PLATFORM_ANDROID)


#include <time.h>
#include "prFps_Android.h"
#include "prTrace.h"


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prFps_Android::prFps_Android()
{
    // Repeated these four to keep cppcheck happy
    ticksPerSecond = 0LL;
    timeTotal      = 0LL;
    timeEnd        = 0LL;
    timeStart      = 0LL;
    Reset();
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prFps_Android::~prFps_Android()
{
}


/// ---------------------------------------------------------------------------
/// Resets the timing data, should be called on game entry, etc.
/// ---------------------------------------------------------------------------
void prFps_Android::Reset()
{
    prFps::Reset();
    ticksPerSecond = 0LL;
    timeTotal      = 0LL;
    timeEnd        = 0LL;
    timeStart      = clock();
}


/// ---------------------------------------------------------------------------
/// Begins the timing period.
/// ---------------------------------------------------------------------------
void prFps_Android::Begin()
{
    // Find the time taken.
    timeTotal = (clock() - timeStart);

    // Exceeded one second?
    if (timeTotal >= CLOCKS_PER_SEC)
    {
        frameRate  = frames;                // Sets the number of frames completed.
        frames     = 0;                     // Resets the frame count.
        timeStart  = clock();               // Reset the start time.

        // Remove seconds from the total time taken.
        while (timeTotal >= CLOCKS_PER_SEC)
        {
            timeTotal -= CLOCKS_PER_SEC;
        }

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


#endif
