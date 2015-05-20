// File: prGameSessionProvider.h
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


#ifndef __PRGAMESESSIONPROVIDER_H
#define __PRGAMESESSIONPROVIDER_H


#include "../core/prTypes.h"


// Class: prGameSessionProvider
//      Base class for game session providers
class prGameSessionProvider
{
public:
    // Method: prGameSessionProvider
    //      Ctor
    prGameSessionProvider(Proteus::Core::s32 type);

    // Method: ~prGameSessionProvider
    //      Dtor
    virtual ~prGameSessionProvider();

    // Method: GetProviderType
    //      Gets the game session provider type.
    Proteus::Core::s32 GetProviderType() const { return mType; }

    // Method: Initialise
    //      Initialises the game session provider
    virtual void Initialise() = 0;

    // Method: Update
    //      Updates the game session provider
    //
    // Returns:
    //      Returns true if still connected
    virtual bool Update() = 0;
    
    // Method: Disconnect
    //      Disconnects a game session
    virtual void Disconnect() = 0;


protected:
    Proteus::Core::s32     mType;
};


#endif//__PRGAMESESSIONPROVIDER_H
