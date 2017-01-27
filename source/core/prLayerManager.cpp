/**
 * prLayerManager.cpp
 *
 *  Copyright 2016 Paul Michael McNab
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


#include "prLayerManager.h"
#include "prLayer.h"
#include "prMacros.h"
#include "prCore.h"


// Namespaces
namespace Proteus {
namespace Core {


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prLayerManager::prLayerManager() : prCoreSystem(PRSYSTEM_LAYERMANAGER, "prLayerManager")
{
    // Create holder array
    mpLayers = new prLayer*[32];

    // Create the named default layers
    mpLayers[0] = new prLayer("Default",      0);
    mpLayers[1] = new prLayer("Background",   1);
    mpLayers[2] = new prLayer("Player",       2);
    mpLayers[3] = new prLayer("Foreground",   3);

    // Create the used layers
    for (int i=4; i<32;i++)
    {
        mpLayers[i] = new prLayer("Unused", i);
    }
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prLayerManager::~prLayerManager()
{
    // Delete the layers
    for (int i=0; i<32;i++)
    {
        PRSAFE_DELETE(mpLayers[i]);
    }

    // Delete holder array
    PRSAFE_DELETE_ARRAY(mpLayers);
}


}}// Namespaces
