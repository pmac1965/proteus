// File: prAchievement_ios.h
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


// Class: prAchievement_Ios
//      IOS achievement provider
class prAchievement_Ios : public prAchievementBase
{
public:
    // Method: prAchievement_Ios
    //      Ctor
    //
    // Parameters:
    //      provider - The provider to use
    //
    // See Also:
    //      <prAchievementProvider>
    explicit prAchievement_Ios(prAchievementProvider provider) : prAchievementBase(provider)
    {
    }
    
    // Method: Initialise
    //      Initialises the achievement system for a specific platform
    void Initialise() override;
    
    // Method: StatusUpdate
    //      Called when the status has changed.
    void StatusUpdate(prAchievementProviderStatus status) override;
    
    // Method: Update
    //      Updates the achievement system for a specific platform
    virtual void Update(f32 dt) override;
    
    // Method: Award
    //      Award an achievement.
    void Award(const char *name, s32 id) override;
    
    // Method: IsAwarded
    //      Checks if an achievement has been awarded.
    bool IsAwarded(const char *name, s32 id)  override;
};
