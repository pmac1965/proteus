/**
 * prProfileManager.cpp
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
#include "../debug/prAssert.h"


/// Defines
#define PROFILE                     // Do not remove or we won't get the class definition.


#if defined(PLATFORM_PC)
  // Exclude MFC
  #ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
  #endif
  #ifndef WIN32_EXTRA_LEAN
  #define WIN32_EXTRA_LEAN
  #endif

  #include <windows.h>

#endif


#include <stdio.h>
#include "prProfileManager.h"
#include "prProfileEntry.h"
#include "prAssert.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../display/prFixedWidthFont.h"


prProfileManager prProfileManager::m_instance;


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prProfileManager::prProfileManager()
{
    // Just in case its not set below 
    m_frequency = 0.0f;

    // Sets all entries to null
    for (u32 i=0; i<PROFILE_MAX_ENTRIES; i++)
    {
        m_entries[i] = nullptr;
    }

#if defined(PLATFORM_PC)

    // Calculate frequency here, so we only do it once
    LARGE_INTEGER countsPerSecond;
    if (QueryPerformanceFrequency(&countsPerSecond))
    {
        // Get the timers counts per millisecond.
        m_frequency = ((float)countsPerSecond.QuadPart) / 1000.0f;
    }
    else
    {
        PRWARN("High performance timer not supported. Profiling will not work.");    
        m_frequency = 0.0f;
    }

#elif defined(PLATFORM_IOS)

    // Its fixed for IOS.
    m_frequency = 1000000.0f;
    
#elif defined(PLATFORM_ANDROID)

    // Its fixed for android.
    m_frequency = 1000000.0f;
    
#endif

    m_update    = false;
    m_enabled   = (m_frequency > 0.0f);
    m_exp0      = false;
    m_exp1      = false;

    TODO("With create - add reference to the ID var, so it can be cleared on destroy")
    TODO("Add cursor class to mouse")
}


// ------------------------------------------------------------------------------------------------
// Destructor.
// ------------------------------------------------------------------------------------------------
prProfileManager::~prProfileManager()
{
    Destroy();
}


// ------------------------------------------------------------------------------------------------
// Resets all the profile entries to their initial state.
// ------------------------------------------------------------------------------------------------
void prProfileManager::Reset()
{
    for (u32 i=0; i<PROFILE_MAX_ENTRIES; i++)
    {
        if (m_entries[i])
        {
            m_entries[i]->Reset();
        }
    }
}


// ------------------------------------------------------------------------------------------------
// Begins a profiling cycle.
// ------------------------------------------------------------------------------------------------
void prProfileManager::Begin()
{
    for (u32 i=0; i<PROFILE_MAX_ENTRIES; i++)
    {
        if (m_entries[i])
        {
            m_entries[i]->m_hitsPerFrame =  0;
        }
    }
}


// ------------------------------------------------------------------------------------------------
// Creates a new profiling entry.
// ------------------------------------------------------------------------------------------------
s32 prProfileManager::Create(const char *name)
{
    if (name && *name)
    {
        prProfileEntry* entry = new prProfileEntry(name, m_frequency);
        PRASSERT(entry);
        if (entry)
        {
            for (u32 i=0; i<PROFILE_MAX_ENTRIES; i++)
            {
                if (m_entries[i] == nullptr)
                {
                    m_entries[i] = entry;
                    return (s32)i;
                }
            }

            // All full up.
            PRWARN("Failed to create profiling entry");
            PRSAFE_DELETE(entry);
        }
    }
    else
    {
        PRWARN("Failed to create profiling entry.");
    }

    return -1;
}


// ------------------------------------------------------------------------------------------------
// Destroys all profiling entries.
// ------------------------------------------------------------------------------------------------
void prProfileManager::Destroy()
{
    for (u32 i=0; i<PROFILE_MAX_ENTRIES; i++)
    {
        PRSAFE_DELETE(m_entries[i]);
    }
    
    m_update = false;
}


// ------------------------------------------------------------------------------------------------
// Displays the profile timings.
// ------------------------------------------------------------------------------------------------
void prProfileManager::Display(s32 x, s32 y, prFixedWidthFont* font, s32 spacing, u32 first)
{
    PRASSERT(font);
    PRASSERT(spacing > 0);

    if (font && m_update && m_enabled)
    {
        char buffer[1024];

        // Count the entries.
        int  count = Count();

        // Got any
        if (count == 0)
            return;

        u32 index = 0;
        for (u32 i=0; i<PROFILE_MAX_ENTRIES; i++)
        {
            prProfileEntry* entry = m_entries[i];

            if (entry && entry->m_hits > 0)
            {
                TODO("Add configurable output")

                if (index >= first)
                {
                    sprintf
                    (
                        buffer,
                        "[%*s] Slowest:%03.04f, Fastest:%03.04f, Avg:%03.04f, H:%u",
                        10,
                        entry->m_name.Text(),
                        entry->m_slowest,
                        entry->m_fastest,
                        entry->m_average,
                        entry->m_maxHits
                        //entry->m_hits
                    );

                    font->Draw((f32)x, (f32)y, buffer);
                    y += spacing;
                }

                index++;
            }
        }
    }
}


// ------------------------------------------------------------------------------------------------
// Start profiling
// ------------------------------------------------------------------------------------------------
void prProfileManager::Start(u32 index)
{
    PRASSERT(index < PROFILE_MAX_ENTRIES);

    // Update allowed?
    if (m_update && m_enabled)
    {
        prProfileEntry* entry = m_entries[index];
        PRASSERT(entry);
        if (entry)
        {
            entry->Start();
        }
    }
}


// ------------------------------------------------------------------------------------------------
// Stop profiling
// ------------------------------------------------------------------------------------------------
void prProfileManager::Stop(u32 index)
{
    PRASSERT(index < PROFILE_MAX_ENTRIES);

    // Update allowed?
    if (m_update && m_enabled)
    {
        prProfileEntry* entry = m_entries[index];
        PRASSERT(entry);
        if (entry)
        {
            entry->Stop();
        }
    }
}


// ------------------------------------------------------------------------------------------------
// Returns the number of visible entries.
// ------------------------------------------------------------------------------------------------
s32 prProfileManager::Count() const
{
    s32 count = 0;

    // Count the entries.
    for (u32 i=0; i<PROFILE_MAX_ENTRIES; i++)
    {
        prProfileEntry* entry = m_entries[i];

        if (entry && entry->m_hits > 0)
        {
            count++;
        }
    }

    return count;
}

