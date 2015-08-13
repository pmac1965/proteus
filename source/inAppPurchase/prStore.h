// File: prStore.h
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


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace IAP {


// Forward declarations.
class prInAppPurchase;


// Class: prStore
//      Base store class
class prStore
{
public:
    // Method: prStore
    //      Ctor
    //
    // Parameters:
    //      iap - The parent iap creator class
    explicit prStore(prInAppPurchase &iap);

    // Method: ~prStore
    //      Dtor
    virtual ~prStore();

    // Method: Init
    //      Perform store specific initialisation.
    virtual void Init();

    // Method: Update
    //      Perform store specific updates.
    virtual bool Update(Proteus::Core::f32 dt);
    
    // Method: EventNotify
    //      Callback from the game.
    //
    // Parameters:
    //      result - The result of a transaction
    //      id     - Identifier of the item involved
    //
    // See also:
    //      <prTransactionResultType>
    //
    // See also:
    //      <prTransactionResult>
    virtual void EventNotify(Proteus::Core::s32 type, const char *id);

    // Method: BeginPurchase
    //      Purchase an item.
    //
    // Parameters:
    //      name - Name of the item to purchase, *OR*
    //      id   - ID of the item to purchase
    //
    // Notes:
    //      Allows for multiple platforms, hence the two
    //      parameters types
    virtual void BeginPurchase(const char *name, int id);
    
    // Method: FindPrice
    //      Find an items price.
    //
    // Parameters:
    //      name - Name of the item to purchase, *OR*
    //      id   - ID of the item to purchase
    //
    // Notes:
    //      Allows for multiple platforms, hence the two
    //      parameters types
    virtual const char *FindPrice(const char *name, int id);


private:
    // Stops passing by value and assignment.
    prStore(const prStore&);
    const prStore& operator = (const prStore&);


protected:
    prInAppPurchase &m_prInAppPurchase;
};


}}// Namespaces
