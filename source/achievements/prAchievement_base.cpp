/**
 * prAchievement_base.cpp
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


#include "prAchievement_base.h"
#include "../debug/prTrace.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Updates the achievement system for a specific platform
/// ---------------------------------------------------------------------------
void prAchievementBase::Update(f32 dt)
{
    if (mPrevStatus != mCurrStatus)
    {
        mPrevStatus = mCurrStatus;

        switch(mCurrStatus)
        {
        case None:
            StatusUpdate(mCurrStatus);
            break;

        case InitialisingStart:
            StatusUpdate(mCurrStatus);
            break;

        case InitialisingSuccess:
            StatusUpdate(mCurrStatus);
            break;

        case InitialisingFailed:
            StatusUpdate(mCurrStatus);
            break;

        case AwardingStart:
            StatusUpdate(mCurrStatus);
            break;

        case AwardingPending:
            StatusUpdate(mCurrStatus);
            break;

        case AwardingSuccess:
            StatusUpdate(mCurrStatus);
            break;

        case AwardingFailure:
            StatusUpdate(mCurrStatus);
            break;

        default:
            prTrace(LogError, "Unknown achievement status message: %08x\n", mCurrStatus);
            break;
        }
    }
}    
