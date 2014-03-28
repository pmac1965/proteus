// File: prAdverts.h
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


#ifndef __PRADVERTS_H
#define __PRADVERTS_H


#include "../core/prTypes.h"


// Forward declarations.
class prAdvertProvider;
struct AdvertsImplementation;


// Class: prAdverts
//      In game advert manager.
class prAdverts
{
public:

    // Advert type.
    enum
    {
        FullScreen,         // Fullscreen advert.
        Banner              // Banner advert.
    };


public:
    // Method: prAdverts
    //      Ctor
    prAdverts();

    // Method: ~prAdverts
    //      Dtor
    ~prAdverts();

    // Method: Add
    //      Adds a provider
    //
    // Parameters:
    //      pProvider - An ad provider
    void Add(prAdvertProvider *pProvider);

    // Method: Update
    //      Updates all providers
    void Update(f32 dt);

    // Method: Draw
    //      Draws all providers
    void Draw();

    // Method: ShowAd
    //      Shows ad
    void ShowAd(s32 type);

    // Method: HideAd
    //      hides ads
    void HideAd();


private:

    // Don't change order.
    AdvertsImplementation  *pImpl;
    AdvertsImplementation  &imp;

    // Stops passing by value and assignment.
    prAdverts(const prAdverts&);
    const prAdverts& operator = (const prAdverts&);
};


#endif//__PRADVERTS_H
