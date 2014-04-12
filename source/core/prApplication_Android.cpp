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
 *
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
static const char *embedded[] = 
{
    "The Proteus Game Engine. ",
    "Author Paul Michael McNab. ",
    "Copyright Paul Michael McNab. All rights reserved.",
};


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prApplication_Android::prApplication_Android() : prApplication()
{
#if defined(_DEBUG) || defined(DEBUG)
    // Write startup info.
    prRegistry *reg = (prRegistry *)prCoreGetComponent(PRSYSTEM_REGISTRY);
    if (reg)
    {
        if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
        {
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Engine version : %s\n", prGetVersionAsString());
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Components:\n");
            prTrace("Lua            : 5.2.3 - Copyright (C) 1994-2013 Lua.org, PUC-Rio.\n");
            prTrace("Box2D          : 2.3.0 - Copyright (c) 2006-2013 Erin Catto\n");
            prTrace("TinyXML        : 2.6.2 - Copyright (c) 2000-2006 Lee Thomason.\n");
            prTrace("Zlib           : 1.2.8 - Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler.\n");
            prTrace("Ogg            : 1.2.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
            prTrace("Vorbis         : 1.3.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
            prTrace("AntTweakBar    : 1.16  - Copyright (C) 2005-2013 Philippe Decaudin.\n");        
            prTrace("libutf8proc    : 1.1.5 - Copyright (C) 2009 Public Software Group e. V.,\n");
            prTrace("                                                         Berlin, Germany\n");
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Config         : %s - Android - Debug\n", BuildType());
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Options        : Verbose      %s\n", reg->GetValue("Verbose"));
            prTrace("               : Use archives %s\n", reg->GetValue("UseArchives"));
            prTrace("               : Log to file  %s\n", reg->GetValue("LogToFile"));
            prTrace("               : Help         %s\n", reg->GetValue("Help"));
            prTrace("-------------------------------------------------------------------------------\n");
            prTrace("Build number: %i\n", BUILD_NUMBER);
            prTrace("-------------------------------------------------------------------------------\n");
        }
    }
#else
    // Write startup info.
    prTrace("-------------------------------------------------------------------------------\n");
    prTrace("Build number: %i\n", BUILD_NUMBER);
    prTrace("-------------------------------------------------------------------------------\n");

#endif


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


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prApplication_Android::~prApplication_Android()
{
}


/// ---------------------------------------------------------------------------
/// Creates the application display.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_Android::DisplayCreate(u32 width, u32 height, const char *pWindowName)
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
            prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (reg)
            {
                if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
                {
                   reg->ShowKeyValuePairs();
                }
            }
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
      while (1)
      {
          //prLinuxLoop();

          // Get systems
          //prMouse         *pMouse = static_cast<prMouse *>       (prCoreGetComponent(PRSYSTEM_MOUSE));
          //prSoundManager  *pSound = static_cast<prSoundManager *>(prCoreGetComponent(PRSYSTEM_AUDIO));
          prTouch         *pTouch = static_cast<prTouch *>       (prCoreGetComponent(PRSYSTEM_TOUCH));
//          prFps           *pFps   = static_cast<prFps *>         (prCoreGetComponent(PRSYSTEM_FPS));


//            GameTime::GetInstance()->Update();
          //static int c=0;
          //prTrace("Update: %i\n", c++);

          // Update game
          if (m_pWindow && m_pWindow->GetActive())
          {
//                float dt = GameTime::GetInstance()->ElapsedTime();

              // System updates
              //if (pMouse) { pMouse->Update(); }
              //if (pSound) { pSound->Update(16.0f); }
              if (pTouch) { pTouch->Update(); }
//              if (pFps)   { pFps->Begin(); }

              // Update and draw the game
              Update(16.0f);// dt);
              Draw();
          }

          // Needs modified to track game speed!
//          Sleep(14);
//          TODO("Needs modified to track game speed!")
          //Sleep(1);

//          if (pFps)   { pFps->End(); }

/*      if (recalcModelView)
        {
          glMatrixMode(GL_MODELVIEW);

          // reset modelview matrix to the identity matrix
          glLoadIdentity();

          // move the camera back three units
          glTranslatef(0.0, 0.0, -3.0);

          // rotate by X, Y, and Z angles
          glRotatef(xAngle, 0.1, 0.0, 0.0);
          glRotatef(yAngle, 0.0, 0.1, 0.0);
          glRotatef(zAngle, 0.0, 0.0, 1.0);

          recalcModelView = GL_FALSE;
          needRedraw = GL_TRUE;
        }
        if (needRedraw)
        {
          redraw();
          needRedraw = GL_FALSE;
        }//*/
      }

    return PRFALSE;
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


#endif//PLATFORM_ANDROID
