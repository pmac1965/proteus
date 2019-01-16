// File: prAchievement_linux.h
/**
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


#pragma once


#include "prAchievement_base.h"
#include "../core/prTypes.h"


// Class: prAchievement_Linux
//      Linux achievement provider
class prAchievement_Linux : public prAchievementBase
{
public:
    // Method: prAchievement_Linux
    //      Ctor
    //
    // Parameters:
    //      provider - The provider to use
    //
    // See Also:
    //      <prAchievementProvider>
    explicit prAchievement_Linux(prAchievementProvider provider) : prAchievementBase(provider)
    {
    }

    // Method: Initialise
    //      Initialises the achievement system for a specific platform
    void Initialise() override;

    // Method: Award
    //      Award an achievement.
    void Award(const char *name, s32 id);

    // Method: IsAwarded
    //      Checks if an achievement has been award.
    bool IsAwarded(const char *name, s32 id);
    
    // Method: IsReady
    //      Checks if the provider is ready to be used
    bool IsReady();
};
