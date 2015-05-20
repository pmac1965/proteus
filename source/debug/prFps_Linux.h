// File: prFps_Linux.h
/**
 * Copyright 2014 Paul Michael McNab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __PRFPS_LINUX_H
#define __PRFPS_LINUX_H


#include "prFps.h"
#include "../prConfig.h"
#include "../core/prTypes.h"


// Class: prFps_Linux
//      Class used to calculate the frame rate.
class prFps_Linux : public prFps
{
public:
    // Method: prFps_Linux
    //      Ctor
    prFps_Linux();

    // Method: ~prFps_Linux
    //      Dtor
    ~prFps_Linux();

    // Method: Reset
    //      Resets the timing data, should be called on game entry, etc.
    void Reset();

    // Method: Begin
    //      Begins the timing period.
    void Begin();

    // Method: End
    //      Ends the timing period.
    void End();


private:

    Proteus::Core::u64     timeStart;
    Proteus::Core::u64     timeEnd;
    Proteus::Core::u64     timeTotal;
    Proteus::Core::u64     ticksPerSecond;
};


#endif//__PRFPS_LINUX_H
