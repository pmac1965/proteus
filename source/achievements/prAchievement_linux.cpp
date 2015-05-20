/**
 * prAchievement_linux.cpp
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


#if defined(PLATFORM_LINUX)


#include "prAchievement_linux.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Award an achievement.
/// ---------------------------------------------------------------------------
void prAchievement_Linux::Award(const char *name, s32 id)
{
    PRUNUSED(id);

    if (name && *name)
    {
    }
}


/// ---------------------------------------------------------------------------
/// Checks if an achievement has been awarded.
/// ---------------------------------------------------------------------------
bool prAchievement_Linux::IsAwarded(const char *name, s32 id)
{
    PRUNUSED(name);
    PRUNUSED(id);
    return false;
}


/// ---------------------------------------------------------------------------
/// Checks if the provider is ready to be used
/// ---------------------------------------------------------------------------
bool prAchievement_Linux::IsReady()
{ 
    return true;
}


#endif
