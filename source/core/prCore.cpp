/**
 * prCore.cpp
 * Copyright Paul Michael McNab. All rights reserved.
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
#include "../debug/prTrace.h"
#include "../display/prRenderer_GL11.h"
#include "../display/prRenderer_GL20.h"
#include "../display/prRenderer_DX9.h"
#include "../display/prBackgroundManager.h"
#include "../display/prSpriteManager.h"


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
        systems[PRSYSTEM_MESSAGEMANAGER]    = new prMessageManager();
        systems[PRSYSTEM_REGISTRY]          = new prRegistry();
        systems[PRSYSTEM_RESOURCEMANAGER]   = new prResourceManager();
        systems[PRSYSTEM_FILEMANAGER]       = new prFileManager();

        #if defined(PLATFORM_PC)
        systems[PRSYSTEM_MOUSE]     = new prMouse();
        systems[PRSYSTEM_KEYBOARD]  = NULL;
        #endif

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
void prCoreCreateOptional(s32 *optionalSystems)
{
    PRASSERT(optionalSystems);
    if (optionalSystems)
    {
        s32 index = 0;
        for (;;)
        {
            s32 id = optionalSystems[index++];
            if (id == PRSYSTEM_LISTEND)
            {
                break;
            }
            else
            {
                switch (id)
                {
                case PRSYSTEM_BACKGROUNDMANAGER:
                    systems[id] = new prBackgroundManager();
                    break;

                case PRSYSTEM_SPRITEMANAGER:
                    systems[id] = new prSpriteManager();
                    break;

                default:
                    prTrace("Yet to implement %i\n", id);
                    break;
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Destroys the engine core.
/// ---------------------------------------------------------------------------
void prCoreDestroy()
{
    for (s32 i = 0; i < PRSYSTEM_MAX; i++)
    {
        if (systems[i])
        {
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
