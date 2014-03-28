// File: prSave_android.h
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


#ifndef __PRSAVEANDROID_H
#define __PRSAVEANDROID_H


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)


#include "prSaveBase.h"


// Forward declarations
struct SaveAndroidImplementation;


// Class: prSaveAndroid
//      Android save code.
class prSaveAndroid : public prSaveBase
{
public:
    // Method: prSaveAndroid
    //      Ctor
    prSaveAndroid();

    // Method: ~prSaveAndroid
    //      Dtor
    ~prSaveAndroid();

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
    prSaveAndroid(const prSaveAndroid&);
    const prSaveAndroid& operator = (const prSaveAndroid&);

private:

    // Don't change order.
    SaveAndroidImplementation  *pImpl;
    SaveAndroidImplementation  &imp;
};


#endif


#endif//__PRSAVEANDROID_H
