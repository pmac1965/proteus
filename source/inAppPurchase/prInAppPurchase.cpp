/**
 * prInAppPurchase.cpp
 *
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


#include "../prConfig.h"
#include "prInAppPurchase.h"
#include "prTransactionResult.h"
#include "prStore.h"
#include "../core/prMacros.h"
#include"../core/prString.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../math/prRandom.h"


// Platform specifics.
#if defined(PLATFORM_PC)
    #include "prStore_pc.h"

#elif defined(PLATFORM_ANDROID)
    #include "prStore_android.h"

#elif defined(PLATFORM_IOS)
    #include "prStore_ios.h"

#elif defined(PLATFORM_MAC)
    #include "prStore_mac.h"

#elif defined(PLATFORM_LINUX)
    #include "prStore_linux.h"

#else
    #error No platform defined.

#endif


using namespace Proteus::Math;
//using namespace Proteus::Core;


#define IAP_TEST_MIN_TIME   2
#define IAP_TEST_MAX_TIME   4


// Namespaces
namespace Proteus {
namespace IAP {


/// ---------------------------------------------------------------------------
/// Implementation data
/// ---------------------------------------------------------------------------
typedef struct prInAppPurchaseImplementation
{
    prInAppPurchaseImplementation()
    {
    #if defined(PROTEUS_IAP_TEST)
        connected   = false;
        testMode    = false;
        exp1        = false;
        exp0        = false;    
        testTimer   = 0.0f;
        testEvent   = IAPTEST_NONE;
        testStep    = 0;

    #else
        connected   = false;
        exp2        = false;
        exp1        = false;
        exp0        = false;    

    #endif
	}

#if defined(PROTEUS_IAP_TEST)
    bool        connected;
    bool        testMode;
    bool        exp1;
    bool        exp0;
    f32         testTimer;
    s32         testEvent;
    s32         testStep;
    prString    purchaseID;

#else
    bool        connected;
    bool        exp2;
    bool        exp1;
    bool        exp0;

#endif

} prInAppPurchaseImplementation;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prInAppPurchase::prInAppPurchase() : pImpl                     (new prInAppPurchaseImplementation())
                                   , imp                       (*pImpl)
                                   , pStore                    (nullptr)
                                   , pTransactionResultHandler (nullptr)
{
    PRASSERT(pImpl);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prInAppPurchase::~prInAppPurchase()
{
    PRSAFE_DELETE(pImpl);
    PRSAFE_DELETE(pStore);
    pTransactionResultHandler = nullptr;
}


/// ---------------------------------------------------------------------------
/// Register callback handler.
/// ---------------------------------------------------------------------------
void prInAppPurchase::Register(Proteus::IAP::prTransactionResult *handler)
{
    PRASSERT(pImpl);
    pTransactionResultHandler = handler;
}


/// ---------------------------------------------------------------------------
/// Initialisation call.
/// ---------------------------------------------------------------------------
void prInAppPurchase::Init()
{
    if (pStore == nullptr)
    {
#if defined(PLATFORM_PC)
        pStore = new prStore_pc(*this);
        pStore->Init();

#elif defined(PLATFORM_ANDROID)
        pStore = new prStore_android(*this);
        pStore->Init();

#elif defined(PLATFORM_IOS)
        pStore = new prStore_ios(*this);
        pStore->Init();

#elif defined(PLATFORM_LINUX)
        pStore = new prStore_linux(*this);
        pStore->Init();
        
#elif defined(PLATFORM_MAC)
        pStore = new prStore_mac(*this);
        pStore->Init();

#else
    #error No platform defined.

#endif
    }
}


/// ---------------------------------------------------------------------------
/// Updates.
/// ---------------------------------------------------------------------------
void prInAppPurchase::Update(f32 dt)
{
#if defined(PROTEUS_IAP_TEST)
    if (imp.testMode)
    {
        if (imp.testTimer > 0.0f)
        {
            imp.testTimer -= dt;

            if (imp.testTimer < 0.0f)
            {
                // Test purchase.
                if (imp.testEvent == IAPTEST_PURCHASE)
                {
                    switch(imp.testStep++)
                    {
                    // Fire off purchase start
                    case 0:
                        PRASSERT(pTransactionResultHandler);
                        pTransactionResultHandler->TransactionResult(TRANSACTION_PURCHASING, imp.purchaseID.Text());

                        // Buy time to random value
                        imp.testTimer = (float)(prRandomNumber(IAP_TEST_MIN_TIME, IAP_TEST_MAX_TIME) * 1000);
                        break;

                    // Fire off purchased
                    case 1:
                        PRASSERT(pTransactionResultHandler);
                        pTransactionResultHandler->TransactionResult(TRANSACTION_PURCHASED, imp.purchaseID.Text());

                        imp.testEvent = IAPTEST_NONE;
                        imp.testTimer = 0.0f;
                        imp.testMode  = false;
                        imp.testStep  = 0;
                        break;
                    }
                }

                // Test connection failed
                else if (imp.testEvent == IAPTEST_CONNECTTION_FAILED)
                {
                    if (pTransactionResultHandler)
                        pTransactionResultHandler->ConnectionStateChanged(false);

                    imp.testEvent = IAPTEST_NONE;
                    imp.testTimer = 0.0f;
                    imp.testMode  = false;
                    imp.testStep  = 0;
                }
            }
        }
        return;
    }
#endif


    // Update the connection status to the store.
    if (pStore)
    {
        // Update
        bool connected = pStore->Update(dt);

        // Update connection status.
        if (connected != imp.connected)
        {
            imp.connected = connected;

            PRASSERT(pTransactionResultHandler);
            pTransactionResultHandler->ConnectionStateChanged(connected);
        }

        // Show the connection state.
        PRASSERT(pTransactionResultHandler);
        pTransactionResultHandler->UpdateConnection(connected);
    }
}


/// ---------------------------------------------------------------------------
/// Begins a purchase
/// ---------------------------------------------------------------------------
bool prInAppPurchase::BeginPurchase(const char *name, u32 type)
{
    PRASSERT(pImpl);
    PRASSERT(name && *name);

    // Store the purchase ID. Used by test code
    #if defined(PROTEUS_IAP_TEST)
    imp.purchaseID.Set(name);
    #endif

    if (pStore)
    {
        // Inline purchase?
        if ((type & IAPCONFIG_INLINE) == IAPCONFIG_INLINE)
        {
            PRASSERT(pTransactionResultHandler);
            pTransactionResultHandler->TransactionResult(TRANSACTION_PURCHASING, name);
            pTransactionResultHandler->TransactionResult(TRANSACTION_PURCHASED, name);
        }

        else if ((type & IAPCONFIG_ONLINE) == IAPCONFIG_ONLINE)
        {
            pStore->BeginPurchase(name);
        }
    }

    return true;
}


/// ---------------------------------------------------------------------------
/// Is class in test mode
/// ---------------------------------------------------------------------------
bool prInAppPurchase::GetTestMode() const
{
#if defined(PROTEUS_IAP_TEST)
    return imp.testMode;

#else
    return false;

#endif
}


/// ---------------------------------------------------------------------------
/// Set test mode
/// ---------------------------------------------------------------------------
void prInAppPurchase::SetTestMode(u32 testEvent)
{
#if defined(PROTEUS_IAP_TEST)
    // Running a test?
    if (imp.testMode == true)
    {
        prTrace(LogError, "IAP test running already\n");
        return;
    }

    switch(testEvent)
    {
    case IAPTEST_CONNECTTION_FAILED:
        imp.testMode  = true;
        imp.testEvent = IAPTEST_CONNECTTION_FAILED;
        imp.testTimer = (float)(prRandomNumber(IAP_TEST_MIN_TIME, IAP_TEST_MAX_TIME) * 1000);
        imp.testStep  = 0;
        break;

    case IAPTEST_PURCHASE:
        imp.testMode  = true;
        imp.testEvent = IAPTEST_PURCHASE;
        imp.testTimer = (float)(prRandomNumber(IAP_TEST_MIN_TIME, IAP_TEST_MAX_TIME) * 1000);
        imp.testStep  = 0;
        break;

    default:
        imp.testMode  = false;
        imp.testEvent = IAPTEST_NONE;
        imp.testTimer = 0.0f;
        imp.testStep  = 0;
        prTrace(prLogLevel::LogError, "Invalid test mode\n");
        break;
    }

#else
    PRUNUSED(testEvent);
    prTrace(prLogLevel::LogError, "IAP test code is not enabled in engine\n");

#endif
}

    
/// ---------------------------------------------------------------------------
/// Callback from the game.
/// ---------------------------------------------------------------------------
void prInAppPurchase::EventNotify(s32 type, const char *id)
{
    PRASSERT(pTransactionResultHandler);

    // Okay store has sent a message. Let the game know
    if (pTransactionResultHandler)
    {
        switch(type)
        {
        case TRANSACTION_PURCHASING:            
        case TRANSACTION_PURCHASED:            
        case TRANSACTION_FAILED:
        case TRANSACTION_CANCELLED:
        case TRANSACTION_REFUNDED:
        case TRANSACTION_RESTORED:
            pTransactionResultHandler->TransactionResult(type, id);
            break;

        default:
            prTrace(prLogLevel::LogError, "invalid transaction result\n");
            break;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Find a products price.
/// ---------------------------------------------------------------------------
const char *prInAppPurchase::FindProductPrice(const char *name)
{
    PRASSERT(pStore);
    return pStore->FindPrice(name);
}


}}// Namespaces
