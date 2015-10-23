/**
 * prLeaderboards.cpp
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
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "prLeaderboards.h"
#include "../core/prCore.h"


// Platform specific includes
#if defined(PLATFORM_ANDROID)
#include "../android/prJNIInterface.h"
#endif


using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Achievements {


// Locals
namespace
{
    #if defined(PLATFORM_IOS)    
        bool submitAllowed = true;

    #elif defined(PLATFORM_ANDROID)
        bool submitAllowed = true;

    #elif defined(PLATFORM_PC)
        bool submitAllowed = false;

    #elif defined(PLATFORM_LINUX)
        bool submitAllowed = false;

    #elif defined(PLATFORM_MAC)
        bool submitAllowed = false;

    #else
        #error Unsupported platfrom

    #endif
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prLeaderboard::prLeaderboard() : prCoreSystem(PRSYSTEM_LEADERBOARDS, "prLeaderboard")
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prLeaderboard::~prLeaderboard()
{
}


/// ---------------------------------------------------------------------------
/// Submit a score to a leaderboard.
/// ---------------------------------------------------------------------------
void prLeaderboard::Submit(const char *name, u64 value)
{
    TODO("Finish leaderboards - original code removed so we can improve")

    if (name && *name && value > 0)
    {
        if (submitAllowed)
        {
        #if defined(PLATFORM_IOS)    
        //    extern void Leaderboards_Submit(const char *name, u64 score);
        //    Leaderboards_Submit(name, score);

        #elif defined(PLATFORM_ANDROID)
        //    extern void JNI_SubmitLeaderboardScore(const char *name, s32 score);

        #elif defined(PLATFORM_PC)
            //

        #elif defined(PLATFORM_LINUX)
            //

        #elif defined(PLATFORM_MAC)
            //

        #else
            #error Unsupported platfrom

        #endif

            prTrace(LogError, "Submit to leaderboard '%s' - %llu\n", name, value);        
        }
    }
}


/// ---------------------------------------------------------------------------
/// Allows submit to be disable in case you need to remove it.
/// ---------------------------------------------------------------------------
void prLeaderboard::Enable(bool state)
{
    submitAllowed = state;
}


/// ---------------------------------------------------------------------------
/// Determines if leaderboards are available for the current platform
/// ---------------------------------------------------------------------------
bool prLeaderboard::Available() const
{
    #if defined(PLATFORM_IOS)    
        return false;

    #elif defined(PLATFORM_ANDROID)
        return false;

    #elif defined(PLATFORM_PC)
        return false;

    #elif defined(PLATFORM_LINUX)
        return false;

    #elif defined(PLATFORM_MAC)
        return false;

    #else
        #error Unsupported platfrom

    #endif
}


}}// Namespaces
