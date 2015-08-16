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


#pragma once


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace IAP {


// Enum: IapTestEvent
//      IAP test events
//
// IAPTEST_NONE                 - Default setting
// IAPTEST_CONNECTTION_FAILED   - Generates a connection failed event
// IAPTEST_PURCHASE             - Generates a purchase event
// IAPTEST_REFUND               - Generates a refund event
enum IapTestEvent
{
    IAPTEST_NONE                = 0x00000000,
    IAPTEST_CONNECTTION_FAILED  = 0x20040001,
    IAPTEST_PURCHASE            = 0x20040002,
    IAPTEST_REFUND              = 0x20040004,
};


// Typedef: IapItemStatus
//      IAP items status
//
// IAPITEMSTATUS_AVAILABLE     -  Item is available to be bought
// IAPITEMSTATUS_PURCHASED     -  Item has been purchased
// IAPITEMSTATUS_REPEAT        -  Item can be bought more than once (Like a coin pack)
// IAPITEMSTATUS_ONCE          -  Item can only be bought once (Like a special weapon)
typedef enum IapItemStatus
{
    IAPITEMSTATUS_AVAILABLE     = 0x00000001,
    IAPITEMSTATUS_PURCHASED     = 0x00000002,
    IAPITEMSTATUS_REPEAT        = 0x00020000,
    IAPITEMSTATUS_ONCE          = 0x00040000,

} IapItemStatus;


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
    //      The handler receives *prTransactionResult* messages
    //
    // See Also:
    //      <prTransactionResult>
    void Register(Proteus::IAP::prTransactionResult *handler);

    // Method: Init
    //      Initialisation call.
    void Init();

    // Method: Update
    //      Updates the in app purchase manager.
    void Update(Proteus::Core::f32 dt);

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
    //
    // Notes:
    //      To test purchases on PC call <BeginPurchase>, then call
    //      <SetTestMode> with *IAPTEST_PURCHASE*
    //
    // See also:
    //      <IapTestEvent>
    void SetTestMode(Proteus::Core::u32 testEvent);

    // Method: EventNotify
    //      Callback from the game. This is then passed on to the appropriate handler
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
    void EventNotify(Proteus::Core::s32 type, const char *id);

    // Method: FindProductPrice
    //      Find a products price.
    const char *FindProductPrice(const char *name);


private:
    // Stops passing by value and assignment.
    prInAppPurchase(const prInAppPurchase&);
    const prInAppPurchase& operator = (const prInAppPurchase&);


private:
    // Don't change order.
	prInAppPurchaseImplementation  *pImpl;
	prInAppPurchaseImplementation  &imp;
    prStore                        *pStore;
    prTransactionResult            *pTransactionResultHandler;
};


}}// Namespaces