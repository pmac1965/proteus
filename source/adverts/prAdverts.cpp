/**
 * prAdverts.cpp
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
#include "prAdverts.h"
#include "prAdvertProvider.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"
#include <list>


#if defined(PLATFORM_ANDROID)
#include <android/log.h>
#endif


/// ---------------------------------------------------------------------------
/// Implementation
/// ---------------------------------------------------------------------------
typedef struct AdvertsImplementation
{
    std::list<prAdvertProvider *>   list;

} AdvertsImplementation;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prAdverts::prAdverts() : pImpl (new AdvertsImplementation())
                       , imp   (*pImpl)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAdverts::~prAdverts()
{
    PRASSERT(pImpl);

    std::list<prAdvertProvider *>::iterator itr = imp.list.begin();
    std::list<prAdvertProvider *>::iterator end = imp.list.end();
    for( ; itr != end; ++itr)
    {
        PRSAFE_DELETE(*itr);
    }

    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Adds a provider
/// ---------------------------------------------------------------------------
void prAdverts::Add(prAdvertProvider *pProvider)
{
    PRASSERT(pImpl);
    PRASSERT(pProvider);

    if (pProvider)
    {
        imp.list.push_back(pProvider);
    }
}


/// ---------------------------------------------------------------------------
/// Updates all providers
/// ---------------------------------------------------------------------------
void prAdverts::Update(f32 dt)
{
    PRASSERT(pImpl);
    PRUNUSED(dt);
}


/// ---------------------------------------------------------------------------
/// Draws all providers
/// ---------------------------------------------------------------------------
void prAdverts::Draw()
{
    PRASSERT(pImpl);
}


/// ---------------------------------------------------------------------------
/// Shows ad 
/// ---------------------------------------------------------------------------
void prAdverts::ShowAd(int type)
{
    PRASSERT(pImpl);

    std::list<prAdvertProvider *>::iterator itr = imp.list.begin();
    std::list<prAdvertProvider *>::iterator end = imp.list.end();
    
    for( ; itr != end; ++itr)
    {
        (*itr)->Show(type);
    }
}


/// ---------------------------------------------------------------------------
/// Hides ad 
/// ---------------------------------------------------------------------------
void prAdverts::HideAd()
{
    PRASSERT(pImpl);

    std::list<prAdvertProvider *>::iterator itr = imp.list.begin();
    std::list<prAdvertProvider *>::iterator end = imp.list.end();
    
    for( ; itr != end; ++itr)
    {
        (*itr)->Hide();
    }
}
