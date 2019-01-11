/**
 * prProfileEntry.cpp
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


#include "../prConfig.h"


#if defined(PLATFORM_PC)
  // Exclude MFC
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef WIN32_EXTRA_LEAN
  #define WIN32_EXTRA_LEAN
  #endif

  #include <windows.h>


#elif defined (PLATFORM_IOS)
  #include <mach/mach_time.h>

#elif defined(PLATFORM_ANDROID)
  #include <string.h>
  #include <time.h>

#endif


#include "prProfileEntry.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Defines
/// ---------------------------------------------------------------------------
#define INITIAL_TIME                999999.0f
#define BILLION                     1000000000LL


/// ---------------------------------------------------------------------------
/// Ctor.
/// ---------------------------------------------------------------------------
prProfileEntry::prProfileEntry(const char *name, float frequency) 
    : m_name        (name)
    , m_hash        (0)
    , m_hits        (0)
    , m_hitsPerFrame(0)
    , m_maxHits     (0)
    , m_fastest     ( INITIAL_TIME)
    , m_slowest     (-INITIAL_TIME)
    , m_average     (0.0f)
    , m_total       (0.0f)
    , m_frequency   (frequency)
    , m_timeStart   (0LL)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prProfileEntry::~prProfileEntry()
{
}


/// ---------------------------------------------------------------------------
/// Starts profiling.
/// ---------------------------------------------------------------------------
void prProfileEntry::Start()
{
#if defined(PLATFORM_PC)

    // Get start time
    LARGE_INTEGER timenow;
    if (QueryPerformanceCounter(&timenow))
    {
        m_timeStart = timenow.QuadPart;
    }
    
#elif defined (PLATFORM_IOS)
    
    // Get start time
    m_timeStart = mach_absolute_time();

#elif defined(PLATFORM_ANDROID)

    // Get start time
    m_startTime.tv_sec  = 0;
    m_startTime.tv_nsec = 0;
    m_endTime.tv_sec    = 0;
    m_endTime.tv_nsec   = 0;
//    clock_gettime(CLOCK_MONOTONIC_HR, &m_startTime);
//    clock_gettime(CLOCK_MONOTONIC_RAW, &m_startTime);
    clock_gettime(CLOCK_MONOTONIC, &m_startTime);

#endif
}


/// ---------------------------------------------------------------------------
/// Stops profiling.
/// ---------------------------------------------------------------------------
void prProfileEntry::Stop()
{
#if defined(PLATFORM_PC)

    // Get stop time
    LARGE_INTEGER timenow;
    if (QueryPerformanceCounter(&timenow))
    {
        u64 timeEnd = timenow.QuadPart;
        Update(timeEnd - m_timeStart);
    }
    
#elif defined (PLATFORM_IOS)
    
    // Get stop time
    u64 timeEnd = mach_absolute_time();
    u64 time    = timeEnd - m_timeStart;
    
    // Get the timebase info
    mach_timebase_info_data_t info;
    mach_timebase_info(&info);

    // Convert to nanoseconds
    time *= info.numer;
    time /= info.denom;
    
    Update(time);
    
#elif defined(PLATFORM_ANDROID)

    // Get stop time
    if (clock_gettime(CLOCK_MONOTONIC, &m_endTime) == 0)
    {
        // Calculate time it took
        u64 start  = (m_startTime.tv_sec * BILLION) + m_startTime.tv_nsec;
        u64 finish = (m_endTime.tv_sec   * BILLION) + m_endTime.tv_nsec;
        u64 period = finish - start;
        Update(period);
    }
#endif
}


/// ---------------------------------------------------------------------------
/// Updates a profiling entry.
/// ---------------------------------------------------------------------------
void prProfileEntry::Update(u64 period)
{
    float time = (float)period / m_frequency;

    // Set fastest time?
    if (time < m_fastest)
    {
        m_fastest = time;
    }

    // Set slowest time?
    if (time > m_slowest)
    {
        m_slowest = time;
    }

    // Update entry.
    m_hits++;
    m_total  += time;
    m_average = m_total / m_hits;

    // Max hits
    m_hitsPerFrame++;

    if (m_maxHits < m_hitsPerFrame)
    {
        m_maxHits = m_hitsPerFrame;
    }
}


/// ---------------------------------------------------------------------------
/// Resets the profiling stats data.
/// ---------------------------------------------------------------------------
void prProfileEntry::Reset()
{
    m_hits         =  0;
    m_hitsPerFrame =  0;
    m_maxHits      =  0;
    m_fastest      =  INITIAL_TIME;
    m_slowest      = -INITIAL_TIME;
    m_average      =  0.0f;
    m_total        =  0.0f;
}
