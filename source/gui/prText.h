// File: prText.h
/**
 * Copyright 2016 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#pragma once


#include "prWidget.h"
//#include "../core/prTypes.h"
//#include "../core/prString.h"
//#include "../display/prColour.h"


// Forward declarations
class prSprite;


// Namespaces
namespace Proteus {
namespace Gui {


// Class: prText
class prText : public prWidget
{
private:
    // Friends
    friend class prGui;

    // Method: prPane
    //      Ctor
    //
    // Parameters:
    //      name           - The name of the pane
    //      pSpriteManager - A pointer to a sprite manager.
    prText(const char *name, prSpriteManager *pSpriteManager);

    // Method: ~prPane
    //      Dtor
    ~prText() {}
};


}} // Namespaces
