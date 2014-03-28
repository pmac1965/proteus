// File: prSave_pc.h
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


#ifndef __PRSAVEPC_H
#define __PRSAVEPC_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include "prSaveBase.h"


// Forward declarations
struct SavePCImplementation;


// Class: prSavePC
//      PC Save code.
class prSavePC : public prSaveBase
{
public:
    // Method: prSavePC
    //      Ctor
    prSavePC();

    // Method: ~prSavePC
    //      Dtor
    ~prSavePC();

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
    prSavePC(const prSavePC&);
    const prSavePC& operator = (const prSavePC&);

private:

    // Don't change order.
    SavePCImplementation  *pImpl;
    SavePCImplementation  &imp;
};


#endif


#endif//__PRSAVEPC_H
