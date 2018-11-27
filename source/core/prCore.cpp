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
#include <clocale>
#include "prCore.h"
#include "prCoreSystem.h"
#include "prMessageManager.h"
#include "prMacros.h"
#include "prRegistry.h"
#include "prResourceManager.h"
#include "../achievements/prLeaderboards.h"
#include "../file/prFileManager.h"
#include "../display/prRenderer_GL11.h"
#include "../display/prRenderer_GL20.h"
#include "../display/prBackgroundManager.h"
#include "../display/prSpriteManager.h"
#include "../display/prFadeManager.h"
#include "../debug/prOnScreenLogger.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../debug/prFps_PC.h"
#include "../debug/prFps_Android.h"
#include "../debug/prFps_ios.h"
#include "../debug/prFps_Linux.h"
#include "../audio/prSoundManager_PC.h"
#include "../audio/prSoundManager_Ios.h"
#include "../audio/prSoundManager_Android.h"
#include "../audio/prSoundManager_Linux.h"
#include "../input/prTouch.h"
#include "../input/prAccelerometer.h"


#if defined(PLATFORM_PC)
#include "../input/prMouse.h"
#include "../input/prKeyboard_PC.h"
#endif


#if defined(PLATFORM_LINUX)
#include "../input/prMouse.h"
#include "../input/prKeyboard_Linux.h"
#endif


using namespace Proteus::Core;
using namespace Proteus::Achievements;


/// ---------------------------------------------------------------------------
/// Local data.
/// ---------------------------------------------------------------------------
namespace
{
    PRBOOL          init   = PRFALSE;
    prCoreSystem   *pSystems[PRSYSTEM_MAX];


    // Debug assist data
    static struct CoreSystemName
    {
        s32 id;
        const char *name;
    }
    coreSystemNames[] = 
    {
        // Core systems
        { PRSYSTEM_RESOURCEMANAGER,     "PRSYSTEM_RESOURCEMANAGER"      },
        { PRSYSTEM_TOUCH,               "PRSYSTEM_TOUCH"                },
        { PRSYSTEM_MESSAGEMANAGER,      "PRSYSTEM_MESSAGEMANAGER"       },
        { PRSYSTEM_REGISTRY,            "PRSYSTEM_REGISTRY"             },
        { PRSYSTEM_FILEMANAGER,         "PRSYSTEM_FILEMANAGER"          },
        { PRSYSTEM_RENDERER,            "PRSYSTEM_RENDERER"             },

        // Desktop only
    #if defined(PLATFORM_PC)
        { PRSYSTEM_MOUSE,               "PRSYSTEM_MOUSE"                },
        { PRSYSTEM_KEYBOARD,            "PRSYSTEM_KEYBOARD"             },

        // Desktop only
    #elif defined(PLATFORM_LINUX)
        { PRSYSTEM_MOUSE,               "PRSYSTEM_MOUSE"                },
        { PRSYSTEM_KEYBOARD,            "PRSYSTEM_KEYBOARD"             },
    #endif

        // Optional systems
        { PRSYSTEM_FPS,                 "PRSYSTEM_FPS"                  },
        { PRSYSTEM_AUDIO,               "PRSYSTEM_AUDIO"                },
        { PRSYSTEM_ONSCREENLOGGER,      "PRSYSTEM_ONSCREENLOGGER"       },
        { PRSYSTEM_FADEMANAGER,         "PRSYSTEM_FADEMANAGER"          },
        { PRSYSTEM_GUIMANAGER,          "PRSYSTEM_GUIMANAGER"           },
        { PRSYSTEM_BACKGROUNDMANAGER,   "PRSYSTEM_BACKGROUNDMANAGER"    },
        { PRSYSTEM_SPRITEMANAGER,       "PRSYSTEM_SPRITEMANAGER"        },
        { PRSYSTEM_ACCELEROMETER,       "PRSYSTEM_ACCELEROMETER"        },
        { PRSYSTEM_RENDERER,            "PRSYSTEM_RENDERER"             },
        { PRSYSTEM_FONTMANAGER,         "PRSYSTEM_FONTMANAGER"          },
        { PRSYSTEM_LEADERBOARDS,        "PRSYSTEM_LEADERBOARDS"         },
    };


    /// -----------------------------------------------------------------------
    /// Shows the name of a system by its ID
    /// -----------------------------------------------------------------------
    const char *CoreGetSystemName(s32 id)
    {
        for(u32 i=0; i<PRARRAY_SIZE(coreSystemNames); i++)
        {
            if (coreSystemNames[i].id == id)
            {
                return coreSystemNames[i].name;
            }
        }

        return "Core system not found";
    }


    /// -----------------------------------------------------------------------
    /// Shows the name of a system by its ID
    /// -----------------------------------------------------------------------
    void CoreShowSystemExists(s32 id)
    {
        prTrace(prLogLevel::LogError, "Engine system '%s' already exists\n", pSystems[id]->Name());
    }
}


/// ---------------------------------------------------------------------------
/// Creates the engine core components
/// ---------------------------------------------------------------------------
void prCoreInit(const char *saveDataPath)
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
        pSystems[PRSYSTEM_FILEMANAGER]      = new prFileManager(saveDataPath);

        #if defined(PLATFORM_PC)
        pSystems[PRSYSTEM_MOUSE]     = new prMouse();
        pSystems[PRSYSTEM_KEYBOARD]  = new prKeyboard_PC();
        #endif

        #if defined(PLATFORM_LINUX)
        pSystems[PRSYSTEM_MOUSE]     = new prMouse();
        pSystems[PRSYSTEM_KEYBOARD]  = new prKeyboard_Linux();
        #endif

        // Set the locale to English.
        // This stops XML reading from screwing up because it can use the wrong locale to
        // extract the values. Example: English:'1.00f' - French:'1,00f'
        setlocale(LC_ALL, "en-GB");
    }
    else
    {
        PRWARN("Warning: You cannot initialise the core twice");
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
            PRASSERT(rendererType == PRRENDERER_OPENGL);
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
                if (pSystems[id] == nullptr)
                {
                    pSystems[id] = new prBackgroundManager();
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // The sprite manager.
            case PRSYSTEM_SPRITEMANAGER:
                if (pSystems[id] == nullptr)
                {
                    pSystems[id] = new prSpriteManager();
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // The on screen logger.
            case PRSYSTEM_ONSCREENLOGGER:
                if (pSystems[id] == nullptr)
                {
                    pSystems[id] = new prOnScreenLogger();
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // The sound manager
            case PRSYSTEM_AUDIO:
                if (pSystems[id] == nullptr)
                {
                #if defined(PLATFORM_PC)
                    pSystems[id] = new prSoundManager_PC();
                #elif defined(PLATFORM_ANDROID)
                    pSystems[id] = new prSoundManager_Android();
                #elif defined(PLATFORM_IOS)
                    pSystems[id] = new prSoundManager_Ios();
                #elif defined(PLATFORM_MAC)
                    pSystems[id] = nullptr;
                #elif defined(PLATFORM_LINUX)
                    pSystems[id] = new prSoundManager_Linux();
                #else
                    #error Platform undefined
                #endif
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // FPS
            case PRSYSTEM_FPS:
                if (pSystems[id] == nullptr)
                {
                #if defined(PLATFORM_PC)
                    pSystems[id] = new prFps_PC();
                #elif defined(PLATFORM_ANDROID)
                    pSystems[id] = new prFps_Android();
                #elif defined(PLATFORM_IOS)
                    pSystems[id] = new prFps_ios();
                #elif defined(PLATFORM_MAC)
                    pSystems[id] = nullptr;
                #elif defined(PLATFORM_LINUX)
                    pSystems[id] = new prFps_Linux();
                #else
                    #error Platform undefined
                #endif
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // The fade manager
            case PRSYSTEM_FADEMANAGER:
                if (pSystems[id] == nullptr)
                {
                    pSystems[id] = new prFadeManager();
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // Accelerometer
            case PRSYSTEM_ACCELEROMETER:
                if (pSystems[id] == nullptr)
                {
                    pSystems[id] = new prAccelerometer();
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // Leaderboards
            case PRSYSTEM_LEADERBOARDS:
                if (pSystems[id] == nullptr)
                {
                    pSystems[id] = new prLeaderboard();
                }
                else
                {
                    CoreShowSystemExists(id);
                }
                break;

            // Unknown?
            default:
                if (id < 0 || id >= PRSYSTEM_MAX)
                {
                    prTrace(prLogLevel::LogError, "Invalid core system ID: %i\n", id);
                }
                else
                {
                    prTrace(prLogLevel::LogError, "Yet to implement %i - %s\n", id, CoreGetSystemName(id));
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
            //PRLOGI("Destroying %s - %i\n", pSystems[i]->Name(), pSystems[i]->ID());
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
            result = (pSystems[systemID] != nullptr);
        }
        else
        {
            prTrace(prLogLevel::LogError, "prCoreOptionalExist: Invalid system ID\n");
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
            prTrace(prLogLevel::LogError, "prCoreGetComponent: Invalid system ID\n");
        }
    }

    return nullptr;
}
