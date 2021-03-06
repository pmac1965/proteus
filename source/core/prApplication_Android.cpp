/**
 * prApplication_Android.cpp
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


#if defined(PLATFORM_ANDROID)

 
#include "prApplication_Android.h"
#include "prWindow_Android.h"
#include "prCore.h"
#include "prRegistry.h"
#include "prVersion.h"
#include "prMacros.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../debug/prOnScreenLogger.h"
#include "../debug/prFps.h"
#include "../input/prTouch.h"
#include "../display/prRenderer.h"
#include "../core/prStringUtil.h"
#include "../audio/prSoundManager.h"
#include "../prVerNum.h"


// ----------------------------------------------------------------------------
// Static data
// ----------------------------------------------------------------------------
namespace 
{
    const char *embedded[] = 
    {
        "The Proteus Game Engine. ",
        "Author Paul Michael McNab. ",
        "Copyright Paul Michael McNab. All rights reserved.",
    };
}


// Namespaces
namespace Proteus {
namespace Core {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prApplication_Android::prApplication_Android() : prApplication()
{
    prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
    if (reg)
    {
        prTrace(LogInformation, "-------------------------------------------------------------------------------\n");
        prTrace(LogInformation, "Engine version : %s\n", prGetVersionAsString());
        prTrace(LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(LogVerbose, "Components:\n");
        prTrace(LogVerbose, "Lua              : 5.2.3 - Copyright (C) 1994-2013 Lua.org, PUC-Rio.\n");
        prTrace(LogVerbose, "Box2D            : 2.3.0 - Copyright (C) 2006-2013 Erin Catto\n");
        prTrace(LogVerbose, "TinyXML          : 2.6.2 - Copyright (C) 2000-2006 Lee Thomason.\n");
        prTrace(LogVerbose, "Zlib             : 1.2.8 - Copyright (C) 1995-2013 Jean-loup Gailly and\n");
        prTrace(LogVerbose, "                           Mark Adler.\n");
        prTrace(LogVerbose, "Ogg              : 1.2.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
        prTrace(LogVerbose, "Vorbis           : 1.3.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
        prTrace(LogVerbose, "AntTweakBar      : 1.16  - Copyright (C) 2005-2013 Philippe Decaudin.\n");        
        prTrace(LogVerbose, "Twitter4j        : 4.0.2 - Copyright 2007 Yusuke Yamamoto\n");        
        prTrace(LogVerbose, "libutf8proc      : 1.1.5 - Copyright (C) 2009 Public Software Group e. V.,\n");
        prTrace(LogVerbose, "                                                         Berlin, Germany\n");
        prTrace(LogVerbose, "Mersenne Twister : 1.1   - Copyright (C) 1997 - 2002, Makoto Matsumoto and\n");        
        prTrace(LogVerbose, "                           Takuji Nishimura,\n");        
        prTrace(LogVerbose, "                           Copyright (C) 2000 - 2009, Richard J. Wagner\n");        
        prTrace(LogVerbose, "Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus\n");        
        prTrace(LogVerbose, "Richard J. Wagner  v1.1  28 September 2009  wagnerr@umich.edu\n");        
        prTrace(LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(LogVerbose, "Config         : %s - Android - Debug\n", BuildType());
        prTrace(LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(LogVerbose, "Options        : Use archives %s\n", reg->GetValue("UseArchives"));
        prTrace(LogVerbose, "               : Log to file  %s\n", reg->GetValue("LogToFile"));
        prTrace(LogVerbose, "               : Help         %s\n", reg->GetValue("Help"));
        prTrace(LogInformation, "-------------------------------------------------------------------------------\n");
        prTrace(LogInformation, "Build number   : %i\n", BUILD_NUMBER);
        prTrace(LogInformation, "-------------------------------------------------------------------------------\n");
    }

    // Access the embedded data, so it'll stay linked into the game.
    for (u32 j=0; j<PRARRAY_SIZE(embedded); j++)
    {
        const char *text = embedded[j];

        // Just compare the first 10 characters.
        for (int k=0; k<10; k++)
        {
            if (text[k] == 0)
            {
            }
        }
    }
}


///// ---------------------------------------------------------------------------
///// Dtor
///// ---------------------------------------------------------------------------
//prApplication_Android::~prApplication_Android()
//{
//}


/// ---------------------------------------------------------------------------
/// Creates the application display.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_Android::DisplayCreate(u32 width, u32 height, const char *pWindowName, bool fullscreen)
{
    PRBOOL result = PRFALSE;


    // Kill old.
    PRSAFE_DELETE(m_pWindow);
    m_pWindow = new prWindow_Android();


    // Create the window
    bool success = static_cast<prWindow_Android *>(m_pWindow)->Create(width, height, 32, pWindowName);
    if (success)
    {
        m_running = PRTRUE;
        result    = PRTRUE;

        // Set window.
        prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
        if (pRenderer)
        {
            pRenderer->SetWindow(m_pWindow);
            pRenderer->Init();
        }

        // Set registry
        prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        if (reg)
        {
            // Set title here
            static_cast<prWindow_Android *>(m_pWindow)->SetTitle(pWindowName);

            reg->SetValue("WindowName", pWindowName);
            reg->SetValue("ScreenWidth", width);
            reg->SetValue("ScreenHeight", height);

            // Show startup entries
            reg->ShowKeyValuePairs();
        }
    }
    else
    {
        // Exit app.
        m_running = PRFALSE;
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Run the game.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_Android::Run()
{
    if (m_running)
    {
        // Get systems
        prSoundManager  *pSound = static_cast<prSoundManager *>(prCoreGetComponent(PRSYSTEM_AUDIO));
        prTouch         *pTouch = static_cast<prTouch *>       (prCoreGetComponent(PRSYSTEM_TOUCH));
        prFps           *pFps   = static_cast<prFps *>         (prCoreGetComponent(PRSYSTEM_FPS));


        // Update game
        if (m_pWindow && m_pWindow->GetActive())
        {
            // System updates
            if (pSound) { pSound->Update(16.0f); }
            if (pTouch) { pTouch->Update(); }
            if (pFps)   { pFps->Begin(); }

            // Update and draw the game
            Update(16.0f);
            Draw();
        }
    }

    return m_running;
}


/// ---------------------------------------------------------------------------
/// Returns the build
/// ---------------------------------------------------------------------------
const char *prApplication_Android::BuildType()
{
    #if defined(PROTEUS_TOOL)
        return "Tool";
    #else
        return "Game";
    #endif
}


}}// Namespaces


#endif//PLATFORM_ANDROID
