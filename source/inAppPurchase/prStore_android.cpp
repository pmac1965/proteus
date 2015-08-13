/**
 * prStore_android.cpp
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


#if defined(PLATFORM_ANDROID)


#include "prStore_android.h"
#include "prInAppPurchase.h"
#include "../core/prMacros.h"
#include "../android/prJNIInAppPurchase.h"


using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace IAP {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prStore_android::prStore_android(prInAppPurchase &iap) : prStore(iap)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prStore_android::~prStore_android()
{
}


/// ---------------------------------------------------------------------------
/// Callback from the game.
/// ---------------------------------------------------------------------------
void prStore_android::EventNotify(s32 type, const char *id)
{
    m_prInAppPurchase.EventNotify(type, id);
}


/// ---------------------------------------------------------------------------
/// Purchase an item.
/// ---------------------------------------------------------------------------
void prStore_android::BeginPurchase(const char *name, int id)
{
    PRUNUSED(id);

    if (name && *name)
    {
        prJNI_BeginPurchase(name);
    }
}


}}// Namespaces


#endif//PLATFORM_ANDROID
