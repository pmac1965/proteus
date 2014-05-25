// File: prGameSessionIos_BT.h
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


#ifndef __PRGAMESESSIONIOS_BT_H
#define __PRGAMESESSIONIOS_BT_H


#include "../core/prTypes.h"
#include "prGameSessionProvider.h"


// Class: prGameSessionIos_BT
//      Game session provider for ios via bluetooth
class prGameSessionIos_BT : public prGameSessionProvider
{
public:
    // Method: prGameSessionIos_BT
    //      Ctor
    prGameSessionIos_BT();

    // Method: ~prGameSessionIos_BT
    //      Dtor
    ~prGameSessionIos_BT();

    // Method: Initialise
    //      Initialises the game session provider
    void Initialise();

    // Method: Update
    //      Updates the game session provider
    bool Update();
    
    // Method: Disconnect
    //      Disconnects a game session
    void Disconnect();
};


#endif//__PRGAMESESSIONIOS_BT_H
