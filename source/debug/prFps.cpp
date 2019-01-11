/**
 * prFps.cpp
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


#include "prFps.h"
#include "../core/prCore.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prFps::prFps() : prCoreSystem(PRSYSTEM_FPS, "prFps")
{
//    Reset();
    frames        = 0;
    frameRate     = 0;
    frameRateMin  = 1000000;
    frameRateMax  = -1000000;
}


/// ---------------------------------------------------------------------------
/// Resets the timing data, should be called on game entry, etc.
/// ---------------------------------------------------------------------------
void prFps::Reset()
{
    frames        = 0;
    frameRate     = 0;
    frameRateMin  = 1000000;
    frameRateMax  = -1000000;
}
