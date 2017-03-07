// File: prProfileEntry.h
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


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prString.h"


#if defined(PLATFORM_ANDROID)
#include <time.h>
#endif


// Class: prProfileEntry
//      Contains a class that is used to update a profiling entry in the profile manager.
//
// Notes:
//      You need to define 'PROFILE' before the includes in order for profile updating to occur.
//
// Notes:
//      This class is managed by the profile manager. You do not need to use it
class prProfileEntry
{
public:
    // Method: prProfileEntry
    //      Ctor
    prProfileEntry(const char *name, Proteus::Core::f32 frequency);

    // Method: ~prProfileEntry
    //      Dtor
    ~prProfileEntry();

    // Method: Start
    //      Starts profiling.
    void Start();

    // Method: Stop
    //      Stops profiling.
    void Stop();

    // Method: Update
    //      Updates a profiling entry.
    void Update(Proteus::Core::u64 period);

    // Method: Reset
    //      Resets the profiling stats data.
    void Reset();

    prString            m_name;
    Proteus::Core::u32  m_hash;
    Proteus::Core::u32  m_hits;
    Proteus::Core::u32  m_hitsPerFrame;
    Proteus::Core::u32  m_maxHits;
    Proteus::Core::f32  m_fastest;
    Proteus::Core::f32  m_slowest;
    Proteus::Core::f32  m_average;
    Proteus::Core::f32  m_total;
    Proteus::Core::f32  m_frequency;
    Proteus::Core::u64  m_timeStart;

#if defined(PLATFORM_ANDROID)
    
    timespec    m_startTime;
    timespec    m_endTime;

#endif
};
