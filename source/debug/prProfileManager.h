// File: prProfileManager.h
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


#ifndef __PRPROFILEMANAGER_H
#define __PRPROFILEMANAGER_H


#include "../prConfig.h"
#include "../core/prTypes.h"


// Forward declarations
class prFixedWidthFont;
class prProfileEntry;


// Shortcut defines as you shouldn't add the profiling code without them
#if (defined(PLATFORM_PC) || defined(PLATFORM_IOS) || defined(PLATFORM_ANDROID)) && defined(PROFILE)
    #define ProfileCreate(name)                     prProfileManager::GetInstance().Create((name))
    #define ProfileEnable(state)                    prProfileManager::GetInstance().Enable((state))
    #define ProfileIsEnabled()                      prProfileManager::GetInstance().IsEnabled()
    #define ProfileBegin()                          prProfileManager::GetInstance().Begin()
    #define ProfileReset()                          prProfileManager::GetInstance().Reset()
    #define ProfileDestroy()                        prProfileManager::GetInstance().Destroy()
    #define ProfileStart(index)                     prProfileManager::GetInstance().Start((index))
    #define ProfileStop(index)                      prProfileManager::GetInstance().Stop((index))
    #define ProfileDisplay(x, y, fnt, spacing, idx) prProfileManager::GetInstance().Display((x), (y), (fnt), (spacing), (idx))
    #define ProfileCount()                          prProfileManager::GetInstance().Count()

    // Use these to declare data for the profiler
    #define PROF_VAR_DEC(var)                       s32 var = -1;
    #define PROF_VAR_SET(var)                       var = ProfileCreate(#var);

#else
    #define ProfileCreate(name)
    #define ProfileEnable(state)
    #define ProfileIsEnabled()                      false
    #define ProfileBegin()
    #define ProfileReset()
    #define ProfileDestroy()
    #define ProfileStart(index)
    #define ProfileStop(index)
    #define ProfileDisplay(x, y, fnt, spacing, idx)
    #define ProfileCount()

    // Use these to declare data for the profiler
    #define PROF_VAR_DEC(var)
    #define PROF_VAR_SET(var)

#endif


// Defines
#define PROFILE_MAX_ENTRIES         128


// Class: prProfileManager
//      Class used to manage profiling entries.
//
// Notes:
//      This class should only be used on platforms where no proper profiling
//      tools are available. SO LAST DITCH ONLY!!!
//
//      It does however have a few useful features, like hit count, so you 
//      know how often a function is getting hit
//
// Notes:
//      This class is a singleton
class prProfileManager
{
public:
    // Method: GetInstance
    //      Returns a reference to the profile manager instance.
    static prProfileManager& GetInstance() { return m_instance; }

    // Method: Reset
    //      Resets all the profile entries to their initial state.
    void Reset();

    // Method: Begin
    //      Begins a profiling cycle.
    void Begin();

    // Method: Create
    //      Creates a new profiling entry.
    Proteus::Core::s32 Create(const char *name);

    // Method: Destroy
    //      Destroys all profiling entries.
    void Destroy();

    // Method: Display
    //      Displays the profile timings.
    //
    // Parameters:
    //      x       - The screen x position
    //      y       - The screen y position
    //      font    - The font to draw with
    //      spacing - The spacing between the lines of text, so it can be tightly displayed
    //      first   - The first timing t0 display (Allows scrolling of the text)
    void Display(Proteus::Core::s32 x, Proteus::Core::s32 y, prFixedWidthFont* font, Proteus::Core::s32 spacing, Proteus::Core::u32 first);

    // Method: Enable
    //      Enables/disables profiling.
    void Enable(bool state) { m_update = state; }

    // Method: IsEnabled
    //      Determines if profiling is updating/drawing
    bool IsEnabled() const { return m_update; }

    // Method: Start
    //      Start profiling
    void Start(Proteus::Core::u32 index);

    // Method: Stop
    //      Stop profiling
    void Stop(Proteus::Core::u32 index);

    // Method: Count
    //      Returns the number of visible entries.
    Proteus::Core::s32 Count() const;


private:
    // This class is true singleton. You cannot create an instance.
    prProfileManager();
    ~prProfileManager();

    // Stop passing by value and assignment.
    prProfileManager(const prProfileManager&);
    const prProfileManager& operator = (const prProfileManager&);


private:
    static prProfileManager m_instance;
    prProfileEntry         *m_entries[PROFILE_MAX_ENTRIES];
    float                   m_frequency;
    bool                    m_update;
    bool                    m_enabled;
    bool                    m_exp0;
    bool                    m_exp1;
};


#endif//__PRPROFILEMANAGER_H
