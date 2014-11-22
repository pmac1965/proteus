/**
 * prBackgroundManager.cpp
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
#if defined(_DEBUG) || defined(DEBUG)

    prTrace("\nBackground Manager: =============================================================\n");

    for (s32 i=0; i<MAX_BACKGROUNDS; i++)
    {
        if (backgrounds[i])
        {
            prTrace("Slot %i occupied: W: %i, H: %i\n", i, backgrounds[i]->GetWidth(),
                                                           backgrounds[i]->GetHeight());
        }
        else
        {
            prTrace("Slot %i unoccupied\n", i);
        }
    }

#endif
}
