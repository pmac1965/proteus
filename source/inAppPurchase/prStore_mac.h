// File: prStore_mac.h
/**
 *  Copyright 2014 Paul Michael McNab
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


#ifndef __PRSTORE_MAC_H
#define __PRSTORE_MAC_H


#include "prStore.h"
#include "../core/prTypes.h"


// Forward declarations
class prInAppPurchase;


// Class: prStore_mac
//      Linux store class
class prStore_mac : public prStore
{
public:
    // Method: prStore_mac
    //      Ctor
    //
    // Parameters:
    //      iap - The parent iap creator class
    prStore_mac(prInAppPurchase &iap);

    // Method: ~prStore_mac
    //      Dtor
    ~prStore_mac();

    // Method: EventNotify
    //      Callback from the game.
    void EventNotify(Proteus::Core::s32 type, const char *id);

    // Method: BeginPurchase
    //      Purchase an item.
    void BeginPurchase(const char *name, int id);
};


#endif//__PRSTORE_MAC_H
