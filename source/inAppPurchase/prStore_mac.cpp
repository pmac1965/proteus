/**
 * prStore_mac.cpp
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


#if defined(PLATFORM_MAC)


#include "prStore_mac.h"
#include "prInAppPurchase.h"
#include "../core/prMacros.h"


using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace IAP {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prStore_mac::prStore_mac(prInAppPurchase &iap) : prStore(iap)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prStore_mac::~prStore_mac()
{
}


/// ---------------------------------------------------------------------------
/// Callback from the game.
/// ---------------------------------------------------------------------------
void prStore_mac::EventNotify(s32 type, const char *id)
{
    m_prInAppPurchase.EventNotify(type, id);
}


/// ---------------------------------------------------------------------------
/// Purchase an item.
/// ---------------------------------------------------------------------------
void prStore_mac::BeginPurchase(const char *name, int id)
{
    PRUNUSED(name);
    PRUNUSED(id);
}


}}// Namespaces


#endif//PLATFORM_MAC
