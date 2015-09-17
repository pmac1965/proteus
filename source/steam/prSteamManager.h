// File: prSteamManager.h
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


#include "../core/prTypes.h"


#ifdef STEAM_CEG
    // Steam DRM header file
    #include "cegclient.h"

#else
    #define Steamworks_InitCEGLibrary() (true)
    #define Steamworks_TermCEGLibrary()
    #define Steamworks_TestSecret()
    #define Steamworks_SelfCheck()

#endif


// Enum: SteamNotificationPosition
//      Steam notification dialog position.
//      Most people should use the default setting
//      because steam users will be comfortable with this
//
// - STEAM_NOTIFICATION_DEFAULT
// - STEAM_NOTIFICATION_TOP_LEFT
// - STEAM_NOTIFICATION_TOP_RIGHT
// - STEAM_NOTIFICATION_BOT_LEFT
// - STEAM_NOTIFICATION_BOT_RIGHT
enum SteamNotificationPosition
{
    STEAM_NOTIFICATION_DEFAULT,
    STEAM_NOTIFICATION_TOP_LEFT,
    STEAM_NOTIFICATION_TOP_RIGHT,
    STEAM_NOTIFICATION_BOT_LEFT,
    STEAM_NOTIFICATION_BOT_RIGHT
};


// Class: prSteamManager
//      Provides a cross platform interface to steam functionality
class prSteamManager
{
public:
    // Method: prSteamManager
    //      Ctor
    prSteamManager();

    // Method: ~prSteamManager
    //      Dtor
    ~prSteamManager();

    // Method: Enable
    //      Enable/disable steam
    //
    // Notes:
    //      Intended for tester use to disable steam on start-up
    //      via a command line option.
    //
    // Notes:
    //      Should not be used on a release build
    void Enable(bool state) { mEnabled = state; }

    // Method: IsEnabled
    //      determines if steam is enabled
    bool IsEnabled() const { return mEnabled; }

    // Method: IsAvailable
    //      Is steam available on this platform
    bool IsAvailable() const;

    // Method: Initialise
    //      Initialise the steam manager
    //
    // Parameters:
    //      notificationPosition - The position of the steam notification dialog
    //
    // See Also
    //      <SteamNotificationPosition>
    bool Initialise(Proteus::Core::u32 id, Proteus::Core::u32 notificationPosition = STEAM_NOTIFICATION_DEFAULT);

    // Method: Deinitialise
    //      Deinitialise the steam manager
    void Deinitialise();


private:
    bool    mEnabled;   // Steam is enabled.
    bool    mUseVR;     // Using VR
};
