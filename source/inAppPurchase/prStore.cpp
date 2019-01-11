/**
 * prStore.cpp
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


#include "prStore.h"
#include "../core/prMacros.h"


//using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace IAP {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prStore::prStore(prInAppPurchase &iap) : m_prInAppPurchase(iap)
{
}


/// ---------------------------------------------------------------------------
/// Perform store specific updates.
/// ---------------------------------------------------------------------------
bool prStore::Update(f32 dt)
{
    PRUNUSED(dt);
    return true;
}


/// ---------------------------------------------------------------------------
/// Callback from the game.
/// ---------------------------------------------------------------------------
void prStore::EventNotify(s32 type, const char *id)
{
    PRUNUSED(type);
    PRUNUSED(id);
}


/// ---------------------------------------------------------------------------
/// Purchase an item.
/// ---------------------------------------------------------------------------
void prStore::BeginPurchase(const char *id)
{
    PRUNUSED(id);
}


/// ---------------------------------------------------------------------------
/// Find an items price.
/// ---------------------------------------------------------------------------
const char *prStore::FindPrice(const char *id)
{
    PRUNUSED(id);
    return "$0.99";
}


}}// Namespaces
