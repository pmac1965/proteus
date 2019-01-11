// File: prFps_ios.h
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


#pragma once


#include "prFps.h"
#include "../prConfig.h"
#include "../core/prTypes.h"


// Class: prFps_ios
//      Class used to calculate the frame rate.
class prFps_ios : public prFps
{
public:
    // Method: prFps_ios
    //      Ctor
    prFps_ios();

    // Method: ~prFps_ios
    //      Dtor
    ~prFps_ios();

    // Method: Reset
    //      Resets the timing data, should be called on game entry, etc.
    void Reset() override;

    // Method: Begin
    //      Begins the timing period.
    void Begin() override;

    // Method: End
    //      Ends the timing period.
    void End() override;

private:
    u64     timeStart;
    u64     timeEnd;
    u64     timeTotal;
    u64     ticksPerSecond;
};
