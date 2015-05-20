// File: prAdvertProvider_Flurry.h
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


#ifndef __PRADVERTPROVIDER_FLURRY_H
#define __PRADVERTPROVIDER_FLURRY_H


#include "prAdvertProvider.h"
#include "../core/prTypes.h"


// Enum: FlurryEvents 
//      Flurry events
//
//  - FLURRY_ADVERT_OPENED
//  - FLURRY_ADVERT_CLICKED
//  - FLURRY_ADVERT_CLOSED
//  - FLURRY_ADVERT_RECEIVED
//  - FLURRY_ADVERT_RECEIVE_FAILED
//  - FLURRY_ADVERT_RENDERED
//  - FLURRY_ADVERT_RENDER_FAILED
//  - FLURRY_VIDEO_COMPLETED
enum FlurryEvents
{
    FLURRY_ADVERT_OPENED,
    FLURRY_ADVERT_CLICKED,
    FLURRY_ADVERT_CLOSED,
    FLURRY_ADVERT_RECEIVED,
    FLURRY_ADVERT_RECEIVE_FAILED,
    FLURRY_ADVERT_RENDERED,
    FLURRY_ADVERT_RENDER_FAILED,
    FLURRY_VIDEO_COMPLETED
};


// Class: prAdvertProvider_Flurry
//      Ad provider for flurry
class prAdvertProvider_Flurry : public prAdvertProvider
{
public:
    // Method: prAdvertProvider_Flurry
    //      Ctor
    prAdvertProvider_Flurry();

    // Method: ~prAdvertProvider_Flurry
    //      Dtor
    ~prAdvertProvider_Flurry();

    // Method: EventNotify
    //      This is to allow the client device to callback the provider with
    //      advert event notifications.
    //
    // Parameters:
    //      eventType   - The event type
    //      data1       - Event data (Optional)
    //      data2       - Event data (Optional)
    //
    // See Also:
    //      <FlurryEvents>
    void EventNotify(Proteus::Core::u32 eventType, Proteus::Core::u32 data1 = 0, Proteus::Core::u32 data2 = 0);

    // Method: Init
    //      Initialise the advert provider.
    void Init();

    // Method: Load
    //      Start loading an advert
    void Load();

    // Method: Show
    //      Show an advert
    void Show(Proteus::Core::s32 type);

    // Method: Hide
    //      Hide an advert.
    void Hide();

    // Method: Update
    //      Updates the provider
    void Update();

    // Method: Draw
    //      Draws the provider
    void Draw();
};


#endif//__PRADVERTPROVIDER_FLURRY_H
