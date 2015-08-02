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


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Implementation
/// ---------------------------------------------------------------------------
typedef struct AdvertsImplementation
{
    AdvertsImplementation()
    {
        pProvider = nullptr;
    }

    ~AdvertsImplementation()
    {
        PRSAFE_DELETE(pProvider);
    }

    prAdvertProvider *pProvider;

} AdvertsImplementation;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prAdverts::prAdverts() : pImpl (new AdvertsImplementation())
                       , imp   (*pImpl)
{
    PRASSERT(pImpl);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAdverts::~prAdverts()
{
    PRASSERT(pImpl);
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// So you can determine when the provider is initialised,
/// as you usually don't want ads appearing during loading.
/// ---------------------------------------------------------------------------
void prAdverts::Init()
{
    PRASSERT(pImpl);
    if (imp.pProvider)
    {
        imp.pProvider->Init();
    }
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
        imp.pProvider = pProvider;
    }
}


/// ---------------------------------------------------------------------------
/// Updates the provider
/// ---------------------------------------------------------------------------
void prAdverts::Update(f32 dt)
{
    PRASSERT(pImpl);
    PRUNUSED(dt);
    if (imp.pProvider)
    {
        imp.pProvider->Update();
    }
}


/// ---------------------------------------------------------------------------
/// Draws the provider
/// ---------------------------------------------------------------------------
void prAdverts::Draw()
{
    PRASSERT(pImpl);
    if (imp.pProvider)
    {
        imp.pProvider->Draw();
    }
}


/// ---------------------------------------------------------------------------
/// Shows ad 
/// ---------------------------------------------------------------------------
void prAdverts::ShowAd(int type)
{
    PRASSERT(pImpl);
    if (imp.pProvider)
    {
        imp.pProvider->Show(type);
    }
}


/// ---------------------------------------------------------------------------
/// Hides ad 
/// ---------------------------------------------------------------------------
void prAdverts::HideAd()
{
    PRASSERT(pImpl);
    if (imp.pProvider)
    {
        imp.pProvider->Hide();
    }
}


/// ---------------------------------------------------------------------------
/// Loads an advert if possible
/// ---------------------------------------------------------------------------
void prAdverts::LoadAd()
{
    if (IsAdvertLoaded()  == false &&
        IsAdvertLoading() == false)
    {
        prTrace(LogError, "Start loading advert\n");
        if (imp.pProvider)
        {
            imp.pProvider->Load();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Is an advert loading?
/// ---------------------------------------------------------------------------
bool prAdverts::IsAdvertLoading() const
{
    PRASSERT(pImpl);    

    bool result = false;

    if (imp.pProvider)
    {
        result = imp.pProvider->IsAdvertLoading();
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Has an advert loaded?
/// ---------------------------------------------------------------------------
bool prAdverts::IsAdvertLoaded() const
{
    PRASSERT(pImpl);    

    bool result = false;

    if (imp.pProvider)
    {
        result = imp.pProvider->IsAdvertLoaded();
    }

    return result;
}
