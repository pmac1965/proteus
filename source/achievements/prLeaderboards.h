// File: prleaderboards.h
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


#ifndef __PRLEADERBOARDS_H
#define __PRLEADERBOARDS_H


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace Achievements {


// Class: prLeaderboard
//      Simple cross platform interface for leaderboards
class prLeaderboard
{
public:
    // Method: Submit
    //      Submit a value to a leaderboard.
    //
    // Parameters:
    //      name  - The name of the leaderboard item
    //      value - The value to submit to the leaderboard
    void Submit(const char *name, Proteus::Core::u64 value);

    // Method: SubmitEnabled
    //      Allows submit to be disabled in case you need to remove it.
    //      For example in a free build where leaderboards may be disabled
    //      to encourage purchase of the full product
    void SubmitEnabled(Proteus::Core::PRBOOL state);
};


}}// Namespaces


#endif//__PRLEADERBOARDS_H
