// File: prStore_ios.h
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


#pragma once


#include "prStore.h"


// Namespaces
namespace Proteus {
namespace IAP {


// Forward declarations
class prInAppPurchase;


// Class: prStore_ios
//      IOS store class
class prStore_ios : public prStore
{
public:
    // Method: prStore_ios
    //      Ctor
    //
    // Parameters:
    //      iap - The parent iap creator class
    explicit prStore_ios(prInAppPurchase &iap);

    // Method: ~prStore_ios
    //      Dtor
    ~prStore_ios();
    
    // Method: Init
    //      Perform store specific initialisation.
    void Init() override;
    
    // Method: Update
    //      Perform store specific updates.
    bool Update(Proteus::Core::f32 dt) override;

    // Method: EventNotify
    //      Callback from the game.
    //
    // Parameters:
    //      result - The result of a transaction
    //      id     - Identifier of the item involved
    //
    // See also:
    //      <prTransactionResultType>
    void EventNotify(Proteus::Core::s32 type, const char *id) override;

    // Method: BeginPurchase
    //      Purchase an item.
    void BeginPurchase(const char *name, int id) override;
    
    // Method: FindPrice
    //      Find an items price.
    const char *FindPrice(const char *name, int id);
};


}}// Namespace
