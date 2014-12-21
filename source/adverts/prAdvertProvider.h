// File: prAdvertProvider.h
/**
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


#ifndef __PRADVERTPROVIDER_H
#define __PRADVERTPROVIDER_H


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prMacros.h"


// Current advert providers. By platform
enum
{
#if defined(PLATFORM_ANDROID)
    AD_PROVIDER_ADMOB,
    AD_PROVIDER_ADCLIX,
    AD_PROVIDER_FLURRY,

#elif defined(PLATFORM_IOS)
    AD_PROVIDER_IADS,
    //AD_PROVIDER_FLURRY,       - Add when we can test

#else
    AD_PROVIDER_NONE,

#endif
};


// Advert provider capabilities.
enum
{
    AD_PROVIDER_CAPABILITY_NONE         = 0x00000000,               // Default
    AD_PROVIDER_CAPABILITY_RENDER       = 0x00000001,               // Not all ad providers can draw the adverts.
    AD_PROVIDER_CAPABILITY_BANNER       = 0x00000002,               // Ad provider can draw banner adverts.
    AD_PROVIDER_CAPABILITY_FULLSCREEN   = 0x00000004,               // Ad provider can draw full screen adverts.
    AD_PROVIDER_CAPABILITY_INTERSTITIAL = 0x00000008,               // Ad provider interstitial adverts.
};


// Ad provider details
typedef struct prAdvertProviderDetails
{
    u32         type;               // Provider type. For example; AD_PROVIDER_ADMOB
    u32         caps;               // Provider capabilities.
    const char *name;               // Provider name

} prAdvertProviderDetails;


// Class: prAdvertProvider
//      Base class for an advert provider.
class prAdvertProvider
{
public:
    // Method: prAdvertProvider
    //      Ctor
    prAdvertProvider(prAdvertProviderDetails *);

    // Method: ~prAdvertProvider
    //      Dtor
    virtual ~prAdvertProvider();

    // Method: EventNotify
    //      This is to allow the client device to callback the provider with
    //      advert event notifications. These will be provider specific.
    //
    // Parameters:
    //      eventType   - The event type
    //      data1       - Event data (Optional)
    //      data2       - Event data (Optional)
    virtual void EventNotify(u32 eventType, u32 data1 = 0, u32 data2 = 0) { PRUNUSED(eventType); PRUNUSED(data1); PRUNUSED(data2); }

    // Method: Init
    //      So you can determine when the provider is initialised,
    //      as you usually don't want ads appearing during loading,
    //      some providers show ads almost straight away
    virtual void Init() {}

    // Method: Load
    //      So you can determine when the provider loads an advert
    virtual void Load() {}

    // Method: Show
    //      Show advert
    virtual void Show(s32 type) = 0;

    // Method: Hide
    //      Hide advert.
    virtual void Hide() = 0;

    // Method: Update
    //      Some ad systems need to be manually updated for time, etc.
    virtual void Update() = 0;

    // Method: Draw
    //      Some ad providers need to be drawn by the game
    virtual void Draw() = 0;

    // Method: ProviderType
    //      Returns the provider type.
    s32 ProviderType() const;
    
    // Method: ProviderName
    //      Returns the provider name.
    const char *ProviderName() const;

    // Method: ProviderHasCapability
    //      Checks the provider for the specified ability
    bool ProviderHasCapability(u32 ability) const;

    // Method: IsAdvertLoaded
    //      Checks if the provider has loaded
    bool IsAdvertLoaded() const { return mAdvertLoaded; }

    // Method: IsAdvertLoading
    //      Checks if the provider is loading an advert
    bool IsAdvertLoading() const { return mAdvertLoading; }


protected:

     prAdvertProviderDetails   *mDetails;
     bool                       mAdvertLoaded;
     bool                       mAdvertLoading;
     bool                       mVisible;
     bool                       mInitialised;
};


#endif//__PRADVERTPROVIDER_H
