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
//      IAP test events for the desktop allow testing of the purchase
//      code. Specifically rare events like failure and refunds
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


// Typedef: IapConfig
//      IAP item configuration data.
//
// IAPCONFIG_INLINE     -  Item is bought by inline a consumeable item, like coins
// IAPCONFIG_ONLINE     -  Item is bought by cash via online store
typedef enum IapConfig
{
    IAPCONFIG_INLINE    = 0x00000001,
    IAPCONFIG_ONLINE    = 0x00000002,

} IapConfig;


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
    // Parameters:
    //      handler - The results handler
    //
    // See Also:
    //      <prTransactionResult>
    void Register(Proteus::IAP::prTransactionResult *handler);

    // Method: Init
    //      Initialisation call.
    //
    // Notes:
    //      Initialisation is performed separately in case its problematic
    void Init();

    // Method: Update
    //      Updates the in app purchase manager.
    //
    // Parameters:
    //      dt - Delta time
    void Update(f32 dt);

    // Method: BeginPurchase
    //      Begins a purchase
    //
    // Parameters:
    //      name - Identifier of item to purchase
    //      type - The type of purchase (inline, online)
    //
    // See Also:
    //      <IapConfig>
    bool BeginPurchase(const char *name, u32 type);

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
    void SetTestMode(u32 testEvent);

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
	prInAppPurchaseImplementation  *pImpl;
	prInAppPurchaseImplementation  &imp;
    prStore                        *pStore;
    prTransactionResult            *pTransactionResultHandler;
};


}}// Namespaces