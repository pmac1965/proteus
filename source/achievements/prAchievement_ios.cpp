/**
 * prAchievement_ios.cpp
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


#if defined(PLATFORM_IOS)


#include <stdio.h>
#include "prAchievement_ios.h"


// Externs
//extern void Achievements_Award(const char *name);
//extern bool Achievements_IsAwarded(const char *name);
//extern bool Achievements_GotGameCenter();
//extern bool Achievements_AreLoaded();
//extern bool GameCenter_IsInitialised();
//extern bool DoWeHaveAConnection();


// ----------------------------------------------------------------------------
// Award an achievement.
// ----------------------------------------------------------------------------
void prAchievement_Ios::Award(const char *name, s32 id)
{
    PRUNUSED(id);   
    
    if (name && *name)
    {
//        Achievements_Award(name);
    }
}


// ----------------------------------------------------------------------------
// Checks if an achievement has been awarded.
// ----------------------------------------------------------------------------
bool prAchievement_Ios::IsAwarded(const char *name, s32 id)
{
    PRUNUSED(id);   

    bool result = false;

    if (name && *name)
    {
//        result = Achievements_IsAwarded(name);
    }

    return result;
}


// ----------------------------------------------------------------------------
// Checks if the provider is ready to be used
// ----------------------------------------------------------------------------
bool prAchievement_Ios::IsReady()
{
    bool result = false;
    
    /*if (Achievements_GotGameCenter())
    {
        if (GameCenter_IsInitialised())
        {
            if (Achievements_AreLoaded())
            {
                if (DoWeHaveAConnection())
                {
                    result = true;
                }
            }
        }
    }*/
    
    return result;
}


#endif
