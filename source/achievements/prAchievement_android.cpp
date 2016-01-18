/**
 * prAchievement_android.cpp
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


#include "prAchievement_android.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Initialises the achievement system for a specific platform
/// ---------------------------------------------------------------------------
void prAchievement_Android::Initialise()
{
    // Start engine initialisation process, which essentially mimics a more complex process with
    // network sign-in, to keep status messages similar.
    SetStatus(InitialisingStart);
}


/// ---------------------------------------------------------------------------
/// Called when the status has changed.
/// ---------------------------------------------------------------------------
void prAchievement_Android::StatusUpdate(prAchievementProviderStatus status)
{
    switch(status)
    {
            // We've started, so set success
        case InitialisingStart:
            SetStatus(InitialisingSuccess);
            break;
            
            // Okay, succeeded
        case InitialisingSuccess:
            SetStatus(None);
            SetReady(true);
            break;
    }
}


/// ---------------------------------------------------------------------------
/// Updates the achievement system for a specific platform
/// ---------------------------------------------------------------------------
void prAchievement_Android::Update(f32 dt)
{
    prAchievementBase::Update(dt);
}


/// ---------------------------------------------------------------------------
/// Award an achievement.
/// ---------------------------------------------------------------------------
void prAchievement_Android::Award(const char *name, s32 id)
{
    PRUNUSED(name);
    PRUNUSED(id);
}


/// ---------------------------------------------------------------------------
/// Checks if an achievement has been award.
/// ---------------------------------------------------------------------------
bool prAchievement_Android::IsAwarded(const char *name, s32 id)
{
    PRUNUSED(name);
    PRUNUSED(id);
    return false;
}



///// ---------------------------------------------------------------------------
///// Award an achievement.
///// ---------------------------------------------------------------------------
//void prAchievement_Android::Award(const char *name, s32 id)
//{
//    PRUNUSED(id);
//
//    if (name && *name)
//    {
//        //JNI_AwardAchievement(name);
//    }
//}
//
//
///// ---------------------------------------------------------------------------
///// Checks if an achievement has been awarded.
///// ---------------------------------------------------------------------------
//bool prAchievement_Android::IsAwarded(const char *name, s32 id)
//{
//    PRUNUSED(name);
//    PRUNUSED(id);
//    return false;
//}
//
//
///// ---------------------------------------------------------------------------
///// Checks if the provider is ready to be used
///// ---------------------------------------------------------------------------
//bool prAchievement_Android::IsReady()
//{ 
//    return true;
//}


#endif