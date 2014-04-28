// File: prGameSessionAndroid_BT.h
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


#ifndef __PRGAMESESSIONANDROID_BT_H
#define __PRGAMESESSIONANDROID_BT_H


#include "../core/prTypes.h"
#include "prGameSessionProvider.h"


// Class: prGameSessionAndroid_BT
//      Game session provider for android via bluetooth
class prGameSessionAndroid_BT : public prGameSessionProvider
{
public:
    // Method: prGameSessionAndroid_BT
    //      Ctor
    prGameSessionAndroid_BT();

    // Method: ~prGameSessionAndroid_BT
    //      Dtor
    ~prGameSessionAndroid_BT();

    // Method: Initialise
    //      Initialises the game session provider
    void Initialise();

    // Method: Update
    //      Updates the game session provider
    void Update();
};


#endif//__PRGAMESESSIONANDROID_BT_H
