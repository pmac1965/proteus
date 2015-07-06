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
#include "../debug/prTrace.h"
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
using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Implementation data
/// ---------------------------------------------------------------------------
typedef struct prInAppPurchaseImplementation
{
    prInAppPurchaseImplementation()
    {
        connected   = false;
        testMode    = false;
        exp1        = false;
        exp0        = false;    
        testTimer   = 0.0f;
        testEvent   = IAPTEST_NONE;
        testStep    = 0;
	}

    bool    connected;
    bool    testMode;
    bool    exp1;
    bool    exp0;
    
    f32     testTimer;
    s32     testEvent;
    s32     testStep;

} prInAppPurchaseImplementation;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prInAppPurchase::prInAppPurchase() : pImpl                     (new prInAppPurchaseImplementation())
                                   , imp                       (*pImpl)
                                   , pStore                    (NULL)
                                   , pTransactionResultHandler (NULL)
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
    pTransactionResultHandler = NULL;
}


/// ---------------------------------------------------------------------------
/// Register callback handler.
/// ---------------------------------------------------------------------------
void prInAppPurchase::Register(prTransactionResult *handler)
{
    PRASSERT(pImpl);

    if (handler)
    {
        pTransactionResultHandler = handler;
    }
}


/// ---------------------------------------------------------------------------
/// Initialisation call.
/// ---------------------------------------------------------------------------
void prInAppPurchase::Init()
{
    if (pStore == NULL)
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
#if defined(_DEBUG) || defined(DEBUG)

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
                        if (pTransactionResultHandler)
                            pTransactionResultHandler->TransactionResult(TRANSACTION_PURCHASING, NULL);

                        // Buy time to random value
                        imp.testTimer = (float)(prRandomNumber(2, 8) * 1000);
                        break;

                    // Fire off purchased
                    case 1:
                        if (pTransactionResultHandler)
                            pTransactionResultHandler->TransactionResult(TRANSACTION_PURCHASED, NULL);

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

            if (pTransactionResultHandler)
                pTransactionResultHandler->ConnectionStateChanged(connected);
        }

        // Show the connection state.
        if (pTransactionResultHandler)
            pTransactionResultHandler->UpdateConnection(connected);
    }
}


/// ---------------------------------------------------------------------------
/// Begins a purchase
/// ---------------------------------------------------------------------------
bool prInAppPurchase::BeginPurchase(const char *name)
{
    PRASSERT(pImpl);
    PRASSERT(name && *name);

//#if defined(IAP_TEST)
//    UNUSED(name);
//    imp.testTimer = 1;
//    imp.testEvent = 0;
//
//#else
    if (pStore)
    {
        pStore->BeginPurchase(name, 0);
    }

//#endif

    return true;
}


/// ---------------------------------------------------------------------------
/// Is class in test mode
/// ---------------------------------------------------------------------------
bool prInAppPurchase::GetTestMode() const
{
    return imp.testMode;
}


/// ---------------------------------------------------------------------------
/// Set test mode
/// ---------------------------------------------------------------------------
void prInAppPurchase::SetTestMode(u32 testEvent)
{
#if defined(_DEBUG) || defined(DEBUG)
    // Running a test?
    if (imp.testMode == true)
    {
        prTrace(LogError, "IAP test running already\n");
        prTrace(LogError, "Please do one at a time\n");
        return;
    }

    switch(testEvent)
    {
    case IAPTEST_CONNECTTION_FAILED:
        imp.testMode  = true;
        imp.testEvent = IAPTEST_CONNECTTION_FAILED;
        imp.testTimer = (float)(prRandomNumber(2, 8) * 1000);
        imp.testStep  = 0;
        break;

    case IAPTEST_PURCHASE:
        imp.testMode  = true;
        imp.testEvent = IAPTEST_PURCHASE;
        imp.testTimer = (float)(prRandomNumber(2, 8) * 1000);
        imp.testStep  = 0;
        break;

    default:
        imp.testMode  = false;
        imp.testEvent = IAPTEST_NONE;
        imp.testTimer = 0.0f;
        imp.testStep  = 0;
        prTrace(LogError, "Invalid test mode\n");
        prTrace(LogError, "Tests turned off\n");
        break;
    }

#else

    PRUNUSED(testEvent);

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
            prTrace(LogError, "invalid transaction result\n");
            break;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Find a products price.
/// ---------------------------------------------------------------------------
const char *prInAppPurchase::FindProductPrice(const char *name)
{
    if (pStore)
    {
        return pStore->FindPrice(name, 0);
    }
    
    return NULL;
}
