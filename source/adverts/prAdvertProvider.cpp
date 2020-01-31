/**
 * prAdvertProvider.cpp
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


#include "prAdvertProvider.h"
#include "../debug/prDebug.h"
#include "../debug/prAssert.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prAdvertProvider::prAdvertProvider(prAdvertProviderDetails *pDetails) : mDetails(pDetails)
{
    PRASSERT(mDetails);
    mAdvertLoaded  = false;
    mAdvertLoading = false;
    mVisible       = false;
    mInitialised   = false;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAdvertProvider::~prAdvertProvider()
{
}


/// ---------------------------------------------------------------------------
/// Returns the provider type.
/// ---------------------------------------------------------------------------
s32 prAdvertProvider::ProviderType() const
{
    PRASSERT(mDetails);
    return mDetails->type;
}


/// ---------------------------------------------------------------------------
/// Returns the provider name.
/// ---------------------------------------------------------------------------
const char *prAdvertProvider::ProviderName() const
{
    PRASSERT(mDetails);
    return mDetails->name;
}


/// ---------------------------------------------------------------------------
/// Checks the provider for the specified ability
/// ---------------------------------------------------------------------------
bool prAdvertProvider::ProviderHasCapability(u32 ability) const
{
    PRASSERT(mDetails);
    return ((mDetails->caps & ability) == ability);
}
