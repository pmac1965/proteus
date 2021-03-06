// File: prLeaderboardManager.h
// About:
//      This class is a cross platform interface to leaderboards.
//      If they are available on your chosen platform
//
//      Leaderboards are quite simple and don't require a large interface
//
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


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Namespaces
namespace Proteus {
namespace Achievements {


// Class: prLeaderboard
//      Simple cross platform interface for a leaderboard
class prLeaderboardManager //: public prCoreSystem
{
public:
    // Method: prLeaderboardManager
    //      Ctor
    prLeaderboardManager();

    // Method: ~prLeaderboardManager
    //      Dtor
    ~prLeaderboardManager();

    //// Method: Submit
    ////      Submit a value to a leaderboard.
    ////
    //// Parameters:
    ////      name  - The name of the leaderboard
    ////      value - The value to submit to the leaderboard
    //void Submit(const char *name, u64 value);

    //// Method: Enable
    ////      Allows submit to be disabled in case you need to remove it.
    ////      For example in a free build where leaderboards may be disabled
    ////      to encourage purchase of the full product
    //void Enable(bool state);

    //// Method: Available
    ////      Determines if the leaderboard interface is available for the current platform
    //bool Available() const;
};


}}// Namespaces
