// File: prLayerManager.h
//      A class used to sort game objects into layers.
/**
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


#pragma once


#include "prCoreSystem.h"


int LAYER_MAX_LAYERS    = 32;


// Namespaces
namespace Proteus {
namespace Core {


// Forward declarations
class prLayer;


class prLayerManager : public prCoreSystem
{
public:
    // Method: prLayerManager
    //      Constructor
    prLayerManager();

    // Method: ~prLayerManager
    //      Destructor
    ~prLayerManager();

    // Method: GetLayerByIndex
    //      Gets a rendering layer by index.
    //
    // Parameters:
    //      index - integer between 0 and (LAYER_MAX_LAYERS - 1)
    //
    // Returns:
    //      The layer or nullptr if index is incorrect.
    prLayer *GetLayerByIndex(Proteus::Core::s32 index);

private:
    prLayer  **mpLayers;
};


}}// Namespaces
