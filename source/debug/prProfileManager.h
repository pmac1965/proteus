/**
 *  @file       prProfileManager.h
 *  @brief      Contains a class used to manage profiling entries.
 *  @author     Paul McNab. Copyright 2008 Paul Michael McNab. All rights reserved.
 *  @version    Sep 06, 2008 - Initial creation.
 *  @note		This class is true singleton. You cannot create an instance.
 */


#ifndef __PRPROFILEMANAGER_H
#define __PRPROFILEMANAGER_H


#include "../prConfig.h"
#include "../core/prTypes.h"


// ------------------------------------------------------------------------------------------------
// Forward declarations
// ------------------------------------------------------------------------------------------------
class prFixedWidthFont;
class prProfileEntry;


// ------------------------------------------------------------------------------------------------
// Shortcut defines as you shouldn't add the profiling code without them
// ------------------------------------------------------------------------------------------------
#if (defined(PLATFORM_PC) || defined(PLATFORM_IOS) || defined(PLATFORM_ANDROID)) && defined(PROFILE)
    #define ProfileCreate(name)                     prProfileManager::GetInstance().Create((name))
    #define ProfileEnable(state)                    prProfileManager::GetInstance().Enable((state))
    #define ProfileBegin()                          prProfileManager::GetInstance().Begin()
    #define ProfileReset()                          prProfileManager::GetInstance().Reset()
    #define ProfileDestroy()                        prProfileManager::GetInstance().Destroy()
    #define ProfileStart(index)                     prProfileManager::GetInstance().Start((index))
    #define ProfileStop(index)                      prProfileManager::GetInstance().Stop((index))
    #define ProfileDisplay(x, y, fnt, spacing, idx) prProfileManager::GetInstance().Display((x), (y), (fnt), (spacing), (idx))
    #define ProfileCount()                          prProfileManager::GetInstance().Count()

#else
    #define ProfileCreate(name)
    #define ProfileEnable(state)
    #define ProfileBegin()
    #define ProfileReset()
    #define ProfileDestroy()
    #define ProfileStart(index)
    #define ProfileStop(index)
    #define ProfileDisplay(x, y, fnt, spacing, idx)
    #define ProfileCount()

#endif


// ------------------------------------------------------------------------------------------------
// Defines
// ------------------------------------------------------------------------------------------------
#define PROFILE_MAX_ENTRIES         128


// ------------------------------------------------------------------------------------------------
// Class used to manage profiling entries.
// ------------------------------------------------------------------------------------------------
class prProfileManager
{
public:

    // Returns a reference to the profile manager instance.
    static prProfileManager& GetInstance() { return m_instance; }

    // Resets all the profile entries to their initial state.
    void Reset();

    // Begins a profiling cycle.
    void Begin();

    // Creates a new profiling entry.
    s32 Create(const char *name);

    // Destroys all profiling entries.
    void Destroy();

    // Displays the profile timings.
    void Display(s32 x, s32 y, prFixedWidthFont* font, s32 spacing, u32 first);

    // Enables/disables profiling.
    void Enable(bool state) { m_update = state; }

    // Start profiling
    void Start(u32 index);

    // Stop profiling
    void Stop(u32 index);

    // Returns the number of visible entries.
    s32 Count();


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
