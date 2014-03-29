// File: prStore_android.h
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


#ifndef __PRSTORE_ANDROID_H
#define __PRSTORE_ANDROID_H


#include "prStore.h"


// Forward declarations
class prInAppPurchase;


// Class: prStore_android
//      Android store class
class prStore_android : public prStore
{
public:
    // Method: prStore_android
    //      Ctor
    //
    // Parameters:
    //      iap - The parent iap creator class
    prStore_android(prInAppPurchase &iap);

    // Method: ~prStore_android
    //      Dtor
    ~prStore_android();

    // Method: EventNotify
    //      Callback from the game.
    void EventNotify(s32 type, const char *id);

    // Method: BeginPurchase
    //      Purchase an item.
    void BeginPurchase(const char *name, int id);
};


#endif//__PRSTORE_ANDROID_H
