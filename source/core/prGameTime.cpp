/**
 * prGameTime.cpp
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


#include <windows.h>
#include "prGameTime.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../debug/prAssert.h"


//using namespace Proteus::Core;


//#define MILLISECONDS    1000.0f
const float MILLISECONDS    = 1000.0f;


// Implementation data.
typedef struct GameTimeImplementation
{
    // Ctor
    GameTimeImplementation() : timePrev    (0LL)
                             , frequency   (0.0f)
                             , runTime     (0.0f)
                             , elapsedTime (0.0f)
    {
        LARGE_INTEGER countsPerSecond;

        if (QueryPerformanceFrequency(&countsPerSecond))
        {
            u64 ticksPerSecond = countsPerSecond.QuadPart;

            frequency = (float)(ticksPerSecond / MILLISECONDS);

            // Set base time.
            LARGE_INTEGER timenow;
            if (QueryPerformanceCounter(&timenow))
            {
                timePrev = timenow.QuadPart;
            }
        }
    }

    // Data
    u64     timePrev;
    float   frequency;
    float   runTime;
    float   elapsedTime;

} GameTimeImplementation;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prGameTime::prGameTime() : pImpl (new GameTimeImplementation())
                         , imp   (*pImpl)
{
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prGameTime::~prGameTime()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Update the game time
/// ---------------------------------------------------------------------------
void prGameTime::Update()
{
    PRASSERT(pImpl);

    LARGE_INTEGER timenow;
    if (QueryPerformanceCounter(&timenow))
    {
        u64 currTime = timenow.QuadPart;

        float diff = (float)((currTime - imp.timePrev) / imp.frequency);

        imp.elapsedTime = diff;
        imp.runTime    += diff;

        imp.timePrev = currTime;
        //prTrace(LogError, "%0.2f, %0.2f\n", imp.runTime, imp.elapsedTime);
    }
}


/// ---------------------------------------------------------------------------
/// Gets the total runtime in milliseconds
/// ---------------------------------------------------------------------------
f32 prGameTime::RunTime() const
{
    PRASSERT(pImpl);
    return imp.runTime;
}


/// ---------------------------------------------------------------------------
/// Gets the elapsed time bewteen 1 frame and the next.
/// ---------------------------------------------------------------------------
f32 prGameTime::ElapsedTime() const
{
    PRASSERT(pImpl);
    //prTrace(LogError, "%0.2f\n", imp.elapsedTime);
    return imp.elapsedTime;
}


#endif
