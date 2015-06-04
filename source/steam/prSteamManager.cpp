/**
 * prSteamManager.cpp
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


#include "prSteamManager.h"
#include "../prConfig.h"
#include "../debug/prAlert.h"
#include "../debug/prTrace.h"


using namespace Proteus::Core;


// __cdecl is PC specific
#if defined(PLATFORM_PC)
    #define PC_API  __cdecl
#else
    #define PC_API
#endif


// Steam allowed?
#if defined(ALLOW_STEAM)
    // PC  
    #if defined(PLATFORM_PC)
    #include <steam/steam_api.h>
    #endif

#endif


/// ---------------------------------------------------------------------------
/// Purpose: callback hook for debug text emitted from the Steam API
/// ---------------------------------------------------------------------------
extern "C" void PC_API SteamAPIDebugTextHook(int nSeverity, const char *pchDebugText)
{
    prTrace(LogError, pchDebugText);

	// if you're running in the debugger, only warnings (nSeverity >= 1) will be sent
	// if you add -debug_steamapi to the command-line, a lot of extra informational messages will also be sent
/*	::OutputDebugString( pchDebugText ); */

	if (nSeverity >= 1)
	{
		// place to set a breakpoint for catching API errors
		//int x = 3;
		//x = x;
	}//*/
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSteamManager::prSteamManager()
{
    mUseVR = false;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSteamManager::~prSteamManager()
{
}


/// ---------------------------------------------------------------------------
/// Is steam available on this platform
/// ---------------------------------------------------------------------------
bool prSteamManager::IsAvailable() const
{
#if defined(ALLOW_STEAM)

    // PC  
    #if defined(PLATFORM_PC)
    return true;
    
    #else
    return false;

    #endif

#else

    return false;

#endif
}


/// ---------------------------------------------------------------------------
/// Initialise steam if available on this platform
/// ---------------------------------------------------------------------------
bool prSteamManager::Initialise(u32 id, u32 notificationPosition)
{
    bool result = false;

#if (defined(ALLOW_STEAM) && defined(PLATFORM_PC))
    // Init data
    mUseVR = false;

    if (IsAvailable())
    {
	    if (SteamAPI_RestartAppIfNecessary(id))//k_uAppIdInvalid))
	    {
		    // if Steam is not running or the game wasn't started through Steam, SteamAPI_RestartAppIfNecessary starts the 
		    // local Steam client and also launches this game again.
		    //
		    // Once you get a public Steam AppID assigned for this game, you need to replace k_uAppIdInvalid with it and
		    // removed steam_appid.txt from the game depot.
            return result;
	    }

	    // Init Steam CEG
	    if (!Steamworks_InitCEGLibrary())
	    {
		    //OutputDebugString( "Steamworks_InitCEGLibrary() failed\n" );
		    prAlertShowError("Fatal Error", "Steam must be running to play this game (InitDrmLibrary() failed).");
            return result;
	    }

	    // Initialize SteamAPI, if this fails we bail out since we depend on Steam for lots of stuff.
	    // You don't necessarily have to though if you write your code to check whether all the Steam
	    // interfaces are NULL before using them and provide alternate paths when they are unavailable.
	    //
	    // This will also load the in-game steam overlay dll into your process. That dll is normally
	    // injected by steam when it launches games, but by calling this you cause it to always load,
	    // even when not launched via steam.
	    if (!SteamAPI_Init())
	    {
		    //OutputDebugString( "SteamAPI_Init() failed\n" );
		    prAlertShowError( "Fatal Error", "Steam must be running to play this game (SteamAPI_Init() failed).\n" );
            return result;
	    }

	    // Set our debug handler
	    SteamClient()->SetWarningMessageHook(&SteamAPIDebugTextHook);       

	    // Tell Steam where it's overlay should show notification dialogs, this can be top right, top left,
	    // bottom right, bottom left. The default position is the bottom left if you don't call this.  
	    // Generally you should use the default and not call this as users will be most comfortable with 
	    // the default position.  The API is provided in case the bottom right creates a serious conflict 
	    // with important UI in your game.
        switch(notificationPosition)
        {
        case STEAM_NOTIFICATION_TOP_LEFT:
            SteamUtils()->SetOverlayNotificationPosition(k_EPositionTopLeft);
            break;
        case STEAM_NOTIFICATION_TOP_RIGHT:
            SteamUtils()->SetOverlayNotificationPosition(k_EPositionTopRight);
            break;
        case STEAM_NOTIFICATION_BOT_LEFT:
            SteamUtils()->SetOverlayNotificationPosition(k_EPositionBottomLeft);
            break;
        case STEAM_NOTIFICATION_BOT_RIGHT:
            SteamUtils()->SetOverlayNotificationPosition(k_EPositionBottomRight);
            break;
        }

	    // Ensure that the user has logged into Steam. This will always return true if the game is launched
	    // from Steam, but if Steam is at the login prompt when you run your game from the debugger, it
	    // will return false.
	    if ( !SteamUser()->BLoggedOn() )
	    {
//		    OutputDebugString( "Steam user is not logged in\n" );
		    prAlertShowError("Fatal Error", "Steam user must be logged in to play this game (SteamUser()->BLoggedOn() returned false).\n" );
            return result;
	    }

        // VR Mode?
        mUseVR = SteamUtils()->IsSteamRunningInVR();
        
	    // do a DRM self check
	    Steamworks_SelfCheck();

        result = true;
    }

#endif

    return result;
}


/// ---------------------------------------------------------------------------
/// Deinitialise steam if available on this platform
/// ---------------------------------------------------------------------------
void prSteamManager::Deinitialise()
{
#if (defined(ALLOW_STEAM) && defined(PLATFORM_PC))

    // Shutdown the SteamAPI
    if (SteamController())
    {
	    SteamController()->Shutdown();
    }

	SteamAPI_Shutdown();

	// Shutdown Steam CEG
	Steamworks_TermCEGLibrary();

#endif

    prTrace(LogError, "prSteamManager::Deinitialise()\n");
}
