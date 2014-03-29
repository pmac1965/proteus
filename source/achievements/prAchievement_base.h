// File: prAchievement_base.h
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


#ifndef __PRACHIEVEMENTBASE_H
#define __PRACHIEVEMENTBASE_H


#include "../core/prTypes.h"
#include "../core/prMacros.h"


// Class: prAchievementBase
//      Achievements provider base class.
class prAchievementBase
{
public:
    // Method: prAchievementBase
    //      Ctor
    prAchievementBase() {}

    // Method: ~prAchievementBase
    //      Dtor
    virtual ~prAchievementBase() {}

    // Method: Initialise
    //      Initialises the achievement system
    virtual void Initialise() {}    

    // Method: Award
    //      Award an achievement.
    virtual void Award(const char *name, s32 id) = 0;

    // Method: IsAwarded
    //      Checks if an achievement has been awarded.
    virtual bool IsAwarded(const char *name, s32 id)
    {
        PRUNUSED(name);
        PRUNUSED(id);
        return false;
    }

    // Method: IsReady
    //      Checks if the provider is ready to be used
    virtual bool IsReady() { return false; }
};


#endif//__PRACHIEVEMENTBASE_H
