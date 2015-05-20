/**
 * prStore_ios.cpp
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


#if defined(PLATFORM_IOS)


#include "prStore_ios.h"
#include "prInAppPurchase.h"
#include "../core/prMacros.h"


using namespace Proteus::Core;


// Externs found in the iOS code
extern void IAPInit(prStore_ios *pStore);
extern void IAPBeginPurchase(const char *name);
extern bool IAPFindPrice(const char *name, char *buffer);
extern bool DoWeHaveAConnection();


/// ---------------------------------------------------------------------------
/// Ctor.
/// ---------------------------------------------------------------------------
prStore_ios::prStore_ios(prInAppPurchase &iap) : prStore(iap)
{
}


/// ---------------------------------------------------------------------------
/// Dtor.
/// ---------------------------------------------------------------------------
prStore_ios::~prStore_ios()
{
}


/// ---------------------------------------------------------------------------
/// Initialise the iphone store code in the game.
/// ---------------------------------------------------------------------------
void prStore_ios::Init()
{
    IAPInit(this);
}


/// ---------------------------------------------------------------------------
/// Perform store specific updates.
/// ---------------------------------------------------------------------------
bool prStore_ios::Update(f32 dt)
{
    return DoWeHaveAConnection();
}


/// ---------------------------------------------------------------------------
/// Callback from the game.
/// ---------------------------------------------------------------------------
void prStore_ios::EventNotify(s32 type, const char *id)
{
    m_prInAppPurchase.EventNotify(type, id);
}


/// ---------------------------------------------------------------------------
/// Purchase an item.
/// ---------------------------------------------------------------------------
void prStore_ios::BeginPurchase(const char *name, int id)
{
    PRASSERT(name && *name);
    PRUNUSED(id);

    if (name && *name)
    {
        IAPBeginPurchase(name);
    }
}


/// ----------------------------------------------------------------------------
/// Find an items price.
/// ----------------------------------------------------------------------------
const char *prStore_ios::FindPrice(const char *name, int id)
{
    PRASSERT(name && *name);
    PRUNUSED(id);
    
    if (name && *name)
    {
        static char buffer[64];
        
        if (IAPFindPrice(name, buffer))
        {
            return buffer;
        }
    }

    return NULL;
}


#endif//PLATFORM_IOS
