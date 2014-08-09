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


// Locals
namespace
{
    PRBOOL submitAllowed = PRTRUE;
}


/// ---------------------------------------------------------------------------
/// Submit a score to a leaderboard.
/// ---------------------------------------------------------------------------
void prLeaderboard::Submit(const char *name, u64 score)
{
    TODO("Finish leaderboards")
    TODO("Remove scoreloop")

    if (name && *name && score > 0)
    {
        if (submitAllowed)
        {
        //#if defined(PLATFORM_IOS)    
        //    extern void Leaderboards_Submit(const char *name, u64 score);
        //    Leaderboards_Submit(name, score);
        //    Trace("Submit leaderboard %s, %llu\n", name, score);        

        //#elif defined(PLATFORM_ANDROID)
        //    extern void JNI_SubmitLeaderboardScore(const char *name, s32 score);
        //    JNI_SubmitLeaderboardScore(name, (int)score);
        //    Trace("Submit leaderboard %s, %llu\n", name, score);        

        //#endif
        }
    }
}


/// ---------------------------------------------------------------------------
/// Allows submit to be disable in case you need to remove it.
/// ---------------------------------------------------------------------------
void prLeaderboard::SubmitEnabled(PRBOOL state)
{
    submitAllowed = state;
}
