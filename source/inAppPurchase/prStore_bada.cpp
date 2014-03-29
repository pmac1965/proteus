/**
 * prStore_bada.cpp
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
 *
 */


#include "../prConfig.h"


#if defined(PLATFORM_BADA)


#include "prStore_bada.h"
#include "prInAppPurchase.h"
#include "../core/prMacros.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prStore_bada::prStore_bada(prInAppPurchase &iap) : prStore(iap)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prStore_bada::~prStore_bada()
{
}


/// ---------------------------------------------------------------------------
/// Callback from the game.
/// ---------------------------------------------------------------------------
void prStore_bada::EventNotify(s32 type, const char *id)
{
    m_prInAppPurchase.EventNotify(type, id);
}


/// ---------------------------------------------------------------------------
/// Purchase an item.
/// ---------------------------------------------------------------------------
void prStore_bada::BeginPurchase(const char *name, int id)
{
    PRUNUSED(name);
    PRUNUSED(id);
}


#endif//PLATFORM_BADA