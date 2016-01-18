// File: prAchievement_base.h
//      Achievements base class.
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
#include "../core/prMacros.h"


// Enum: prAchievementProvider
//      A enumeration of all the providers by platform
//
// Engine       - All platforms
// Steam        - PC
// GooglePlay   - Android, iOS
enum prAchievementProvider
{
#if defined(PLATFORM_PC)
    Engine,
    Steam

#elif defined(PLATFORM_ANDROID)
    Engine,
    GooglePlay

#elif defined(PLATFORM_IOS)
    Engine,
    GooglePlay

#elif defined(PLATFORM_MAC)
    Engine,

#elif defined(PLATFORM_LINUX)
    Engine,

#else
    Engine,

#endif
};


// Class: prAchievementBase
//      Achievements provider base class.
class prAchievementBase
{
public:
    enum prAchievementProviderStatus
    {
        None,
        InitialisingStart,              // Achievements initialisation has started
        InitialisingSuccess,            // Achievements initialisation has succeeded
        InitialisingFailed,             // Achievements initialisation has faileded
        AwardingStart,                  // An award has started
        AwardingPending,                // An award is pending
        AwardingSuccess,                // An award has been given
        AwardingFailure,                // An award has failed
    };


public:
    // Method: prAchievementBase
    //      Ctor
    //
    // Parameters:
    //      provider - The provider to use
    //
    // See Also:
    //      <prAchievementProvider>
    explicit prAchievementBase(prAchievementProvider provider) : mAchievementProvider(provider)
                                                               , mReady              (false)
                                                               , mExp0               (false)
                                                               , mExp1               (false)
                                                               , mExp2               (false)
                                                               , mPrevStatus         (None)
                                                               , mCurrStatus         (None)
    {}

    // Method: ~prAchievementBase
    //      Dtor
    virtual ~prAchievementBase() {}

    // Method: Initialise
    //      Initialises the achievement system for a specific platform
    virtual void Initialise() = 0;

    // Method: StatusUpdate
    //      Called when the status has changed.
    virtual void StatusUpdate(prAchievementProviderStatus status) = 0;

    // Method: Update
    //      Updates the achievement system for a specific platform
    virtual void Update(Proteus::Core::f32 dt);

    // Method: Award
    //      Award an achievement.
    virtual void Award(const char *name, Proteus::Core::s32 id) = 0;

    // Method: IsAwarded
    //      Checks if an achievement has been awarded.
    virtual bool IsAwarded(const char *name, Proteus::Core::s32 id)
    {
        PRUNUSED(name);
        PRUNUSED(id);
        return false;
    }

    // Method: IsReady
    //      Checks if the provider is ready to be used
    bool IsReady() const { return mReady; }

    // Method: SetReady
    //      Sets the ready state
    void SetReady(bool state) { mReady = state; }

    // Method: GetProvider
    //      Gets the provider that is being used
    //
    // See Also:
    //      <prAchievementProvider>
    const prAchievementProvider GetProvider() const { return mAchievementProvider; }

    // Method: SetStatus
    //      Sets the status of the provider
    void SetStatus(prAchievementProviderStatus status) { mCurrStatus = status; }


private:
    const prAchievementProvider mAchievementProvider;           // The provider (Engine, Steam, etc)
    bool                        mReady;                         // Is the provider ready (Fully initialised)
    bool                        mExp0;                          // Expansion purposes
    bool                        mExp1;                          // Expansion purposes
    bool                        mExp2;                          // Expansion purposes

protected:
    prAchievementProviderStatus mPrevStatus;                    // Previous Provider status
    prAchievementProviderStatus mCurrStatus;                    // Current Provider status
};
