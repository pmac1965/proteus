/**
 * prAdvertProvider_iAds.cpp
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


#include "prAdvertProvider_iAds.h"
#include "prAdvertProvider.h"
#include "../core/prMacros.h"


/// ---------------------------------------------------------------------------
/// Set provider data. As its not const the data can be adjusted at runtime.
/// ---------------------------------------------------------------------------
namespace
{
    prAdvertProviderDetails details =
    {
        AD_PROVIDER_IADS,
        AD_PROVIDER_CAPABILITY_RENDER |                         // This provider renders the adverts.
        AD_PROVIDER_CAPABILITY_BANNER,                          // This provider supports banner adverts.
        "iAds"
    };
}


//extern void prADHide();
//extern void prADShow();


/// ---------------------------------------------------------------------------
/// Ctor 
/// ---------------------------------------------------------------------------
prAdvertProvider_iAds::prAdvertProvider_iAds() : prAdvertProvider(&details)
{
    m_visible   = false;
    m_exp0      = false;
    m_exp1      = false;
    m_exp2      = false;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAdvertProvider_iAds::~prAdvertProvider_iAds()
{
}


/// ---------------------------------------------------------------------------
/// Initialise the provider.
/// ---------------------------------------------------------------------------
void prAdvertProvider_iAds::Init()
{
}


/// ---------------------------------------------------------------------------
/// Show advert
/// ---------------------------------------------------------------------------
void prAdvertProvider_iAds::Show(s32 type)
{
    PRUNUSED(type);

    if (!m_visible)
    {
        //prADShow();
        m_visible = true;
    }
}


/// ---------------------------------------------------------------------------
/// Hide advert.
/// ---------------------------------------------------------------------------
void prAdvertProvider_iAds::Hide()
{
    if (m_visible)
    {
        //prADHide();
        m_visible = false;
    }
}


/// ---------------------------------------------------------------------------
/// Update provider 
/// ---------------------------------------------------------------------------
void prAdvertProvider_iAds::Update()
{
}


/// ---------------------------------------------------------------------------
/// Draw provider 
/// ---------------------------------------------------------------------------
void prAdvertProvider_iAds::Draw()
{
}


#endif
