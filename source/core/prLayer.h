// File: prLayer.h
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


#include "prString.h"


// Namespaces
namespace Proteus {
namespace Core {


// Class: prLayer
//      A class used to sort game objects like actors
//
// Notes:
//      Layers are sorted by index with 0 having the highest priority.
//      Layer priority decreases the higher the index
class prLayer
{
public:
    // Method: prLayer
    //      Ctor
    //
    // Parameters:
    //      layerName  - The layers name
    //      cb         - Objects index in that layer
    prLayer(const char *layerName, s32 index);

    // Method: Registerfactory
    //      Registers a function which creates the games actors.
    s32 GetIndex(){ return mIndex;}

    // Method: GetName
    //      Gets this layers name
    const char *GetName() { return mName.Text(); }

    // Method: SetName
    //      Sets this layers name
    void SetName(const char *name) { mName.Set(name); }

private:
    prString    mName;
    s32         mIndex;
};


}}// Namespaces
