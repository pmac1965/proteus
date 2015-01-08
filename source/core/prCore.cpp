/**
 * prCore.cpp
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


#include <cstring>
#include "prCore.h"
#include "prCoreSystem.h"
#include "prMessageManager.h"
#include "prMacros.h"
#include "prRegistry.h"
#include "prResourceManager.h"
#include "../file/prFileManager.h"
#include "../display/prRenderer_GL11.h"
#include "../display/prRenderer_GL20.h"
#include "../display/prRenderer_DX9.h"
#include "../display/prBackgroundManager.h"
#include "../display/prSpriteManager.h"
#include "../display/prFadeManager.h"
#include "../debug/prOnScreenLogger.h"
#include "../debug/prTrace.h"
#include "../debug/prFps_PC.h"
#include "../debug/prFps_Android.h"
#include "../debug/prFps_ios.h"
#include "../audio/prSoundManager_PC.h"
#include "../audio/prSoundManager_Ios.h"
#include "../audio/prSoundManager_Android.h"
#include "../input/prTouch.h"
#include "../input/prAccelerometer.h"


#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)
#include "../input/prMouse.h"
#endif


#if defined(PLATFORM_PC)
#include "../input/prKeyboard_PC.h"
#endif


/// ---------------------------------------------------------------------------
/// Local data.
/// ---------------------------------------------------------------------------
namespace
{
    PRBOOL          init   = PRFALSE;
    prCoreSystem   *pSystems[PRSYSTEM_MAX];
}


/// ---------------------------------------------------------------------------
/// Creates the engine core components
/// ---------------------------------------------------------------------------
void prCoreInit()
{
    if (init == PRFALSE)
    {
        // Set core initialised
        init = PRTRUE;

        // Clear the systems
        memset(pSystems, 0, sizeof(prCoreSystem*) * PRSYSTEM_MAX);

        // Create the core systems
        pSystems[PRSYSTEM_RESOURCEMANAGER]  = new prResourceManager();
        pSystems[PRSYSTEM_TOUCH]            = new prTouch();
        pSystems[PRSYSTEM_MESSAGEMANAGER]   = new prMessageManager();
        pSystems[PRSYSTEM_REGISTRY]         = new prRegistry();
        pSystems[PRSYSTEM_FILEMANAGER]      = new prFileManager();

        #if defined(PLATFORM_PC)
        pSystems[PRSYSTEM_MOUSE]     = new prMouse();
        pSystems[PRSYSTEM_KEYBOARD]  = new prKeyboard_PC();
        #endif


        #if defined(PLATFORM_LINUX)
        pSystems[PRSYSTEM_MOUSE]     = new prMouse();
        #endif

    }
    else
    {
        PRWARN("Cannot initialise the core twice");
    }
}


/// ---------------------------------------------------------------------------
/// Sets the engine renderer
/// ---------------------------------------------------------------------------
PRBOOL prCoreSetRenderer(prRendererType rendererType, prVerType version)
{
    PRBOOL result = PRFALSE;

    if (init == PRTRUE)
    {
        result = PRTRUE;


        // Platform specific initialisation
        #if defined(PLATFORM_PC)
            // Check renderer type value.
            PRASSERT(rendererType == PRRENDERER_OPENGL || rendererType == PRRENDERER_DIRECTX);

            if (rendererType == PRRENDERER_OPENGL)
            {
                // Check version numbers
                PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
                switch (version)
                {
                case PRGLVER_11:
                    pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL11();
                    break;

                case PRGLVER_20:
                    pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL20();
                    break;

                case PRGLVER_30:
                    PRPANIC("Under construction");
                    break;
                }
            }
            else if (rendererType == PRRENDERER_DIRECTX)
            {
                // Check version numbers
                PRASSERT(version == PRDXVER_9 || version == PRDXVER_10 || version == PRDXVER_11);
                switch(version)
                {
                case PRDXVER_9:
                    PRPANIC("Under construction");
                    break;

                case PRDXVER_10:
                    PRPANIC("Under construction");
                    break;

                case PRDXVER_11:
                    PRPANIC("Under construction");
                    break;
                }
            }

        #elif defined(PLATFORM_ANDROID)
            // Check renderer type value.
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20);

            // Check version numbers
            if (rendererType == PRRENDERER_OPENGL)
            {
                PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
                switch (version)
                {
                    case PRGLVER_11:
                        pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL11();
                        break;
                    
                    case PRGLVER_20:
                        pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL20();
                        break;
                    
                    case PRGLVER_30:
                        PRPANIC("Under construction");
                        break;
                }
            }
        
        #elif defined(PLATFORM_IOS)
            // Check renderer type value.
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20);

            // Check version numbers
            if (rendererType == PRRENDERER_OPENGL)
            {
                PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
                switch (version)
                {
                    case PRGLVER_11:
                        pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL11();
                        break;
                    
                    case PRGLVER_20:
                        pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL20();
                        break;
                    
                    case PRGLVER_30:
                        PRPANIC("Under construction");
                        break;
                }
            }

        #elif defined(PLATFORM_BADA)
            // Check renderer type value.
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20);

            // Check version numbers
            if (rendererType == PRRENDERER_OPENGL)
            {
                PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
                switch (version)
                {
                    case PRGLVER_11:
                        pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL11();
                        break;
                    
                    case PRGLVER_20:
                        pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL20();
                        break;
                    
                    case PRGLVER_30:
                        PRPANIC("Under construction");
                        break;
                }
            }

        #elif defined(PLATFORM_LINUX)
            // Check renderer type value.
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);

            // Check version numbers
            if (rendererType == PRRENDERER_OPENGL)
            {
                PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
                switch (version)
                {
                case PRGLVER_11:
                    pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL11();
                    break;

                case PRGLVER_20:
                    pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL20();
                    break;

                case PRGLVER_30:
                    PRPANIC("Under construction");
                    break;
                }
            }

        
        #elif defined(PLATFORM_MAC)
            // Check renderer type value.
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
        
            // Check version numbers
            if (rendererType == PRRENDERER_OPENGL)
            {
                PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
                switch (version)
                {
                case PRGLVER_11:
                    pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL11();
                    break;
                    
                case PRGLVER_20:
                    pSystems[PRSYSTEM_RENDERER] = new prRenderer_GL20();
                    break;
                    
                case PRGLVER_30:
                    PRPANIC("Under construction");
                    break;
                }
            }
        
        #else
            #error Unsupported platform

        #endif


    }
    else
    {
        PRWARN("Core not initialised");
    }


    return result;
}


/// ---------------------------------------------------------------------------
/// Creates optional engine core components.
/// ---------------------------------------------------------------------------
void prCoreCreateOptional(s32 *optionalSystems, u32 count)
{
    PRASSERT(optionalSystems);
    PRASSERT(count > 0);
    if (optionalSystems)
    {
        for (u32 index = 0; index < count; index++)
        {
            s32 id = optionalSystems[index];
            switch (id)
            {
            // The background manager.
            case PRSYSTEM_BACKGROUNDMANAGER:
                if (pSystems[id] == NULL)
                {
                    pSystems[id] = new prBackgroundManager();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", pSystems[id]->Name());
                }
                break;

            // The sprite manager.
            case PRSYSTEM_SPRITEMANAGER:
                if (pSystems[id] == NULL)
                {
                    pSystems[id] = new prSpriteManager();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", pSystems[id]->Name());
                }
                break;

            // The on screen logger.
            case PRSYSTEM_ONSCREENLOGGER:
                if (pSystems[id] == NULL)
                {
                    pSystems[id] = new prOnScreenLogger();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", pSystems[id]->Name());
                }
                break;

            // The sound manager
            case PRSYSTEM_AUDIO:
                if (pSystems[id] == NULL)
                {
                #if defined(PLATFORM_PC)
                    pSystems[id] = new prSoundManager_PC();
                #elif defined(PLATFORM_ANDROID)
                    pSystems[id] = new prSoundManager_Android();
                #elif defined(PLATFORM_IOS)
                    pSystems[id] = new prSoundManager_Ios();
                #elif defined(PLATFORM_MAC)
                    pSystems[id] = NULL;
                #elif defined(PLATFORM_LINUX)
                    pSystems[id] = NULL;
                #elif defined(PLATFORM_BADA)
                    pSystems[id] = NULL;
                #else
                    #error Platform undefined
                #endif
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", pSystems[id]->Name());
                }
                break;

            // FPS
            case PRSYSTEM_FPS:
                if (pSystems[id] == NULL)
                {
                #if defined(PLATFORM_PC)
                    pSystems[id] = new prFps_PC();
                #elif defined(PLATFORM_ANDROID)
                    pSystems[id] = new prFps_Android();
                #elif defined(PLATFORM_IOS)
                    pSystems[id] = new prFps_ios();
                #elif defined(PLATFORM_MAC)
                    pSystems[id] = NULL;
                #elif defined(PLATFORM_LINUX)
                    pSystems[id] = NULL;
                #elif defined(PLATFORM_BADA)
                    pSystems[id] = NULL;
                #else
                    #error Platform undefined
                #endif
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", pSystems[id]->Name());
                }
                break;

            // The fade manager
            case PRSYSTEM_FADEMANAGER:
                if (pSystems[id] == NULL)
                {
                    pSystems[id] = new prFadeManager();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", pSystems[id]->Name());
                }
                break;

            // Accelerometer
            case PRSYSTEM_ACCELEROMETER:
                if (pSystems[id] == NULL)
                {
                    pSystems[id] = new prAccelerometer();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", pSystems[id]->Name());
                }
                break;

            // Unknown?
            default:
                if (id < 0 || id >= PRSYSTEM_MAX)
                {
                    prTrace("Invalid ID: %i\n", id);
                }
                else
                {
                    prTrace("Yet to implement %i\n", id);
                }
                break;
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Destroys the engine core.
/// ---------------------------------------------------------------------------
void prCoreDestroy()
{
    // Release in reverse, so systems which use textures
    // can get to release them first.
    for (s32 i = (PRSYSTEM_MAX - 1); i >= 0; i--)
    {
        if (pSystems[i])
        {
            prTrace("Destroying %s - %i\n", pSystems[i]->Name(), pSystems[i]->ID());
            PRSAFE_DELETE(pSystems[i]);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Does the core exist.
/// ---------------------------------------------------------------------------
PRBOOL prCoreExist()
{
    return init;
}


/// ---------------------------------------------------------------------------
/// Does a core component exist
/// ---------------------------------------------------------------------------
PRBOOL prCoreComponentExist(s32 systemID)
{
    PRBOOL result = PRFALSE;

    if (prCoreExist())
    {
        if (PRBETWEEN(systemID, 0, PRSYSTEM_MAX - 1))
        {
            result = (pSystems[systemID] != NULL);
        }
        else
        {
            prTrace("prCoreOptionalExist: Invalid system ID\n");
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Fetches a core system component pointer, Includes optional 
/// components.
/// ---------------------------------------------------------------------------
prCoreSystem *prCoreGetComponent(u32 systemID)
{
    if (prCoreExist())
    {
        if (PRBETWEEN(systemID, 0, PRSYSTEM_MAX - 1))
        {
            return pSystems[systemID];
        }
        else
        {
            prTrace("prCoreGetComponent: Invalid system ID\n");
        }
    }

    return NULL;
}
