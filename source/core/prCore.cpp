/**
 * prCore.cpp
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
#include "../audio/prSoundManager_PC.h"
#include "../input/prTouch.h"


#if defined(PLATFORM_PC)
#include "../input/prMouse.h"
#endif


/// ---------------------------------------------------------------------------
/// Local data.
/// ---------------------------------------------------------------------------
namespace
{
    PRBOOL          init   = PRFALSE;
    prCoreSystem   *systems[PRSYSTEM_MAX];
}


/// ---------------------------------------------------------------------------
/// Creates the engine core components
/// ---------------------------------------------------------------------------
PRBOOL prCoreCreate(prRendererType rendererType, prVerType version)
{
    PRBOOL result = PRFALSE;

    if (init == PRFALSE)
    {
        memset(systems, 0, sizeof(systems));

        init   = PRTRUE;
        result = PRTRUE;

        // Create the core systems
        systems[PRSYSTEM_RESOURCEMANAGER]   = new prResourceManager();
        systems[PRSYSTEM_TOUCH]             = new prTouch();
        systems[PRSYSTEM_MESSAGEMANAGER]    = new prMessageManager();
        systems[PRSYSTEM_REGISTRY]          = new prRegistry();
        systems[PRSYSTEM_FILEMANAGER]       = new prFileManager();

        // Platform specific initialisation
        #if defined(PLATFORM_PC)
            // Check renderer type value.
            PRASSERT(rendererType == PRRENDERER_OPENGL || rendererType == PRRENDERER_DIRECTX);

            // Check version numbers
            if (rendererType == PRRENDERER_OPENGL)
            {
                PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);
                switch (version)
                {
                case PRGLVER_11:
                    systems[PRSYSTEM_RENDERER] = new prRenderer_GL11();
                    break;

                case PRGLVER_20:
                    PRPANIC("Under construction");
                    break;

                case PRGLVER_30:
                    PRPANIC("Under construction");
                    break;
                }
            }
            else if (rendererType == PRRENDERER_DIRECTX)
            {
                PRASSERT(version == PRDXVER_9 || version == PRDXVER_10 || version == PRDXVER_11);
            }

        #elif defined(PLATFORM_ANDROID)
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20);

        #elif defined(PLATFORM_IOS)
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20);

        #elif defined(PLATFORM_BADA)
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20);

        #elif defined(PLATFORM_LINUX)
            PRASSERT(rendererType == PRRENDERER_OPENGL);
            PRASSERT(version == PRGLVER_11 || version == PRGLVER_20 || version == PRGLVER_30);

        #else
            #error Unsupported platform

        #endif


        #if defined(PLATFORM_PC)
        systems[PRSYSTEM_MOUSE]     = new prMouse();
        systems[PRSYSTEM_KEYBOARD]  = NULL;
        #endif
    }
    else
    {
        prTrace("Core engine systems cannot be created twice.\n");
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
                if (systems[id] == NULL)
                {
                    systems[id] = new prBackgroundManager();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", systems[id]->Name());
                }
                break;

            // The sprite manager.
            case PRSYSTEM_SPRITEMANAGER:
                if (systems[id] == NULL)
                {
                    systems[id] = new prSpriteManager();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", systems[id]->Name());
                }
                break;

            // The on screen logger.
            case PRSYSTEM_ONSCREENLOGGER:
                if (systems[id] == NULL)
                {
                    systems[id] = new prOnScreenLogger();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", systems[id]->Name());
                }
                break;

            // The sound manager
            case PRSYSTEM_AUDIO:
                if (systems[id] == NULL)
                {
                #if defined(PLATFORM_PC)
                    systems[id] = new prSoundManager_PC();
                #elif defined(PLATFORM_ANDROID)
                    systems[id] = NULL;
                #elif defined(PLATFORM_IOS)
                    systems[id] = NULL;
                #elif defined(PLATFORM_LINUX)
                    systems[id] = NULL;
                #elif defined(PLATFORM_BADA)
                    systems[id] = NULL;
                #else
                    #error Platform undefined
                #endif
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", systems[id]->Name());
                }
                break;

            // FPS
            case PRSYSTEM_FPS:
                if (systems[id] == NULL)
                {
                #if defined(PLATFORM_PC)
                    systems[id] = new prFps_PC();
                #elif defined(PLATFORM_ANDROID)
                    systems[id] = NULL;
                #elif defined(PLATFORM_IOS)
                    systems[id] = NULL;
                #elif defined(PLATFORM_LINUX)
                    systems[id] = NULL;
                #elif defined(PLATFORM_BADA)
                    systems[id] = NULL;
                #else
                    #error Platform undefined
                #endif
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", systems[id]->Name());
                }
                break;

            // The fade manager
            case PRSYSTEM_FADEMANAGER:
                if (systems[id] == NULL)
                {
                    systems[id] = new prFadeManager();
                }
                else
                {
                    prTrace("Engine system '%s' already exists\n", systems[id]->Name());
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
        if (systems[i])
        {
            prTrace("Destroying %s - %i\n", systems[i]->Name(), systems[i]->ID());
            PRSAFE_DELETE(systems[i]);
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
            result = (systems[systemID] != NULL);
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
            return systems[systemID];
        }
        else
        {
            prTrace("prCoreGetComponent: Invalid system ID\n");
        }
    }

    return NULL;
}
