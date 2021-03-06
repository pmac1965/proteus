// File: prSave_ios.h
/**
 * Copyright 2014 Paul Michael McNab
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


#include "../prConfig.h"


#if defined(PLATFORM_IOS)


#include "prSaveBase.h"


// Forward declarations
struct SaveIosImplementation;


// Class: prSaveIos
//      IOS Save code.
class prSaveIos : public prSaveBase
{
public:
    // Method: prSaveIos
    //      Ctor
    prSaveIos();

    // Method: ~prSaveIos
    //      Dtor
    ~prSaveIos();

    // Method: SaveBegin
    //      Overridden method to start a save
    bool SaveBegin();

    // Method: SaveUpdate
    //      Overridden method to update a save
    bool SaveUpdate();

    // Method: SaveEnd
    //      Overridden method to finish a save
    bool SaveEnd();

    // Method: LoadBegin
    //      Overridden method to start loading
    bool LoadBegin();

    // Method: LoadUpdate
    //      Overridden method to update loading
    bool LoadUpdate();

    // Method: LoadEnd
    //      Overridden method to finish loading
    bool LoadEnd();

private:

    // Stops passing by value and assignment.
    prSaveIos(const prSaveIos&);
    const prSaveIos& operator = (const prSaveIos&);

private:

    // Don't change order.
    SaveIosImplementation  *pImpl;
    SaveIosImplementation  &imp;
};


#endif
