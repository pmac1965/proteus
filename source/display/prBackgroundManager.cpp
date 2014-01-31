/**
 * prBackgroundManager.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"


#include "prBackgroundManager.h"
#include "prBackground.h"
#include "../core/prMacros.h"
#include "../core/prResourceManager.h"
#include "../core/prCore.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"


/// ---------------------------------------------------------------------------
/// Constructs the background manager.
/// ---------------------------------------------------------------------------
prBackgroundManager::prBackgroundManager() : prCoreSystem(PRSYSTEM_BACKGROUNDMANAGER, "prBackgroundManager")
{
    for (int i=0; i<MAX_BACKGROUNDS; i++)
    {
        backgrounds[i] = NULL;
    }
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prBackgroundManager::~prBackgroundManager()
{
    ReleaseAll();
}


/// ---------------------------------------------------------------------------
/// Creates a background.
/// ---------------------------------------------------------------------------
prBackground *prBackgroundManager::Create(const char *filename)
{
    PRASSERT(filename && *filename);

    for (int i=0; i<MAX_BACKGROUNDS; i++)
    {
        if (backgrounds[i] == NULL)
        {
            #if !defined(REMOVE_EXCEPTIONS)
            try
            #endif
            {
                backgrounds[i] = new prBackground(filename);
                return backgrounds[i];
            }
            #if !defined(REMOVE_EXCEPTIONS)
            catch(...)
            {
                WARN("Failed to create background: ", filename);
            }
            #endif
        }
    }

    return NULL;
}


/// ---------------------------------------------------------------------------
/// Releases a background and any asssociated assets.
/// ---------------------------------------------------------------------------
void prBackgroundManager::Release(prBackground *bg)
{
    PRASSERT(bg);

    for (int i=0; i<MAX_BACKGROUNDS; i++)
    {
        if (backgrounds[i] == bg)
        {
            PRSAFE_DELETE(backgrounds[i]);
            return;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Releases all backgrounds and their asssociated assets. 
/// ---------------------------------------------------------------------------
void prBackgroundManager::ReleaseAll()
{
    for (int i=0; i<MAX_BACKGROUNDS; i++)
    {
        PRSAFE_DELETE(backgrounds[i]);
    }
}


/// ---------------------------------------------------------------------------
/// Shows all the assets being used by the background manager.
/// ---------------------------------------------------------------------------
void prBackgroundManager::DisplayUsage()
{
    TODO("Fix")

#if defined(_DEBUG) || defined(DEBUG)

    //prSystemResourceManager::Get()->DisplayUsage();//RMT_Background)->DisplayUsage();

#endif
}
