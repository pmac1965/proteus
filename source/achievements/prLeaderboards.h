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


// Class: prLeaderboard
//      Simple cross platform interface for leaderboards
class prLeaderboard
{
public:
    // Method: Submit
    //      Submit a score to a leaderboard.
    void Submit(const char *name, u64 score);

    // Method: SubmitEnabled
    //      Allows submit to be disabled in case you need to remove it.
    void SubmitEnabled(PRBOOL state);
};


#endif//__PRLEADERBOARDS_H
