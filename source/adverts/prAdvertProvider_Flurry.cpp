/**
 * prAdvertProvider_Flurry.cpp
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


#include "prAdvertProvider_Flurry.h"
#include "prAdvertProvider.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"


#if defined(PLATFORM_ANDROID)
#include "../android/prJNIAdverts.h"
#endif


/// ---------------------------------------------------------------------------
/// Set provider data. As its not const the data can be adjusted at runtime.
/// ---------------------------------------------------------------------------
namespace
{
    prAdvertProviderDetails details =
    {
        AD_PROVIDER_FLURRY,                                     // Type
        AD_PROVIDER_CAPABILITY_RENDER |                         // This provider renders the adverts.
        AD_PROVIDER_CAPABILITY_FULLSCREEN   |                   // This provider has full screen adverts.
        AD_PROVIDER_CAPABILITY_INTERSTITIAL |                   // This provider has interstitial adverts.
        AD_PROVIDER_CAPABILITY_BANNER,                          // This provider supports banner adverts.
        "Flurry"                                                // Name
    };
}


/// ---------------------------------------------------------------------------
/// Ctor 
/// ---------------------------------------------------------------------------
prAdvertProvider_Flurry::prAdvertProvider_Flurry() : prAdvertProvider(&details)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAdvertProvider_Flurry::~prAdvertProvider_Flurry()
{
}


/// ---------------------------------------------------------------------------
/// This is to allow the client device to callback the provider with
/// advert event notifications.
/// ---------------------------------------------------------------------------
void prAdvertProvider_Flurry::EventNotify(u32 eventType, u32 data1, u32 data2)
{
    switch(eventType)
    {
    case FLURRY_ADVERT_OPENED:
        prTrace("FLURRY_ADVERT_OPENED\n");
        break;

    case FLURRY_ADVERT_CLICKED:
        prTrace("FLURRY_ADVERT_CLICKED\n");
        break;

    case FLURRY_ADVERT_CLOSED:
        prTrace("FLURRY_ADVERT_CLOSED\n");
        break;

    case FLURRY_ADVERT_RECEIVED:
        prTrace("FLURRY_ADVERT_RECEIVED\n");
        break;

    case FLURRY_ADVERT_RECEIVE_FAILED:
        prTrace("FLURRY_ADVERT_RECEIVE_FAILED\n");
        break;

    case FLURRY_ADVERT_RENDERED:
        prTrace("FLURRY_ADVERT_RENDERED\n");
        break;

    case FLURRY_ADVERT_RENDER_FAILED:
        prTrace("FLURRY_ADVERT_RENDER_FAILED\n");
        break;

    case FLURRY_VIDEO_COMPLETED:
        prTrace("FLURRY_VIDEO_COMPLETED\n");
        break;

    default:
        PRPANIC("Unknown event type");
        break;
    }
}


/// ---------------------------------------------------------------------------
/// Initialise the provider.
/// ---------------------------------------------------------------------------
void prAdvertProvider_Flurry::Init()
{
    prTrace("prAdvertProvider_Flurry::Init()\n");
    mInitialised = true;
}


/// ---------------------------------------------------------------------------
/// Start loading an advert
/// ---------------------------------------------------------------------------
void prAdvertProvider_Flurry::Load()
{
    // Start load
    mAdvertLoaded   = false;
    mAdvertLoading  = true;

#if defined(PLATFORM_ANDROID)

    prJNI_FlurryLoadAdvert();

#elif defined(PLATFORM_IOS)

#endif
}


/// ---------------------------------------------------------------------------
/// Show advert
/// ---------------------------------------------------------------------------
void prAdvertProvider_Flurry::Show(s32 type)
{
    prTrace("prAdvertProvider_Flurry::Show()\n");

    PRUNUSED(type);

#if defined(PLATFORM_ANDROID)
    // Advert loaded?
    if (!mAdvertLoaded)
    {
        prTrace("prAdvertProvider_Flurry: Advert not loaded\n");
        return;
    }

    // Advert loading?
    if (mAdvertLoading)
    {
        prTrace("prAdvertProvider_Flurry: Advert is loading\n");
        return;
    }


    // Visible
    if (mVisible)
    {
        prTrace("prAdvertProvider_Flurry: Advert is visible\n");
        return;
    }

    prJNI_FlurryShowAdvert();


#elif defined(PLATFORM_IOS)

#endif
}


/// ---------------------------------------------------------------------------
/// Hide advert.
/// ---------------------------------------------------------------------------
void prAdvertProvider_Flurry::Hide()
{
    prTrace("prAdvertProvider_Flurry::Hide()\n");

    if (mVisible)
    {
        mVisible = false;

        prJNI_FlurryHideAdvert();
    }
}


/// ---------------------------------------------------------------------------
/// Update provider 
/// ---------------------------------------------------------------------------
void prAdvertProvider_Flurry::Update()
{
}


/// ---------------------------------------------------------------------------
/// Draw provider 
/// ---------------------------------------------------------------------------
void prAdvertProvider_Flurry::Draw()
{
}


#endif
