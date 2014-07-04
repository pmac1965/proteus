// File: prInAppPurchase.h
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


#ifndef __PRINAPPPURCHASE_H
#define __PRINAPPPURCHASE_H


#include "../core/prTypes.h"


// Tests
enum IapTestEvent
{
    IAPTEST_NONE                = 0x00000000,
    IAPTEST_CONNECTTION_FAILED  = 0x00000001,
    IAPTEST_PURCHASE            = 0x00000002,
};


// Forward declarations
class  prTransactionResult;
class  prStore;
struct prInAppPurchaseImplementation;


// Class: prInAppPurchase
//      Cross platform In App Purchase wrapper class
class prInAppPurchase
{
public:
    // Method: prInAppPurchase
    //      Ctor
    prInAppPurchase();

    // Method: ~prInAppPurchase
    //      Dtor
    ~prInAppPurchase();

    // Method: Register
    //      Register callback handler.
    //
    // See Also:
    //      <prTransactionResult>
    void Register(prTransactionResult *handler);

    // Method: Init
    //      Initialisation call.
    void Init();

    // Method: Update
    //      Updates the in app purchase manager.
    void Update(f32 dt);

    // Method: BeginPurchase
    //      Begins a purchase
    bool BeginPurchase(const char *name);

    // Method: GetTestMode
    //      Is class in test mode
    bool GetTestMode() const;

    // Method: SetTestMode
    //      Set test mode
    //
    // Parameters:
    //      testEvent - Which event to test.
    void SetTestMode(u32 testEvent);

    // Method: EventNotify
    //      Callback from the game. This is then passed on to the appropriate handler
    void EventNotify(s32 type, const char *id);

    // Method: FindProductPrice
    //      Find a products price.
    const char *FindProductPrice(const char *name);


private:

    // Stops passing by value and assignment.
    prInAppPurchase(const prInAppPurchase&);
    const prInAppPurchase& operator = (const prInAppPurchase&);


private:

    // Don't change order.
	prInAppPurchaseImplementation	*pImpl;
	prInAppPurchaseImplementation	&imp;
    prStore                         *pStore;
    prTransactionResult             *pTransactionResultHandler;
};


#endif//__PRINAPPPURCHASE_H
