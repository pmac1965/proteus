// File: prStore_bada.h
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


#ifndef __PRSTORE_BADA_H
#define __PRSTORE_BADA_H


#include "prStore.h"


// Forward declarations
class prInAppPurchase;


// Class: prStore_bada
//      Bada store class
class prStore_bada : public prStore
{
public:
    // Method: prStore_bada
    //      Ctor
    //
    // Parameters:
    //      iap - The parent iap creator class
    prStore_bada(prInAppPurchase &iap);

    // Method: ~prStore_bada
    //      Dtor
    ~prStore_bada();

    // Method: EventNotify
    //      Callback from the game.
    void EventNotify(s32 type, const char *id);

    // Method: BeginPurchase
    //      Purchase an item.
    void BeginPurchase(const char *name, int id);
};


#endif//__PRSTORE_BADA_H
