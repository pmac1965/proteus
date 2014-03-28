// File: prSave_bada.h
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


#ifndef __PRSAVEBADA_H
#define __PRSAVEBADA_H


#include "../prConfig.h"


#if defined(PLATFORM_BADA)


#include "prSaveBase.h"


// Forward declarations
struct SaveBadaImplementation;


// Class: prSaveBada
//      Bada save code.
class prSaveBada : public prSaveBase
{
public:
    // Method: prSaveBada
    //      Ctor
    prSaveBada();

    // Method: ~prSaveBada
    //      Dtor
    ~prSaveBada();

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
    prSaveBada(const prSaveBada&);
    const prSaveBada& operator = (const prSaveBada&);

private:

    // Don't change order.
    SaveBadaImplementation  *pImpl;
    SaveBadaImplementation  &imp;
};


#endif


#endif//__PRSAVEBADA_H
