// File: prTwitter_ios.h
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


#pragma once


#include "prTwitterBase.h"


// Namespaces
namespace Proteus {
namespace Social {


// Class: prTwitter_Ios
//      A base class for abstracting application specific twitter
//      functionality into a cross platform format
class prTwitter_Ios : public prTwitterBase
{
public:
    // Method: prTwitter_Ios
    //      Ctor
    prTwitter_Ios();

    // Method: ~prTwitter_Ios
    //      Ctor
    ~prTwitter_Ios();

    // Method: Initialise
    //      Initialise twitter for a specific platform
    bool Initialise() override;

    // Method: Update
    //      Update twitter for a specific platform
    bool Update() override;

    // Method: ShowTweetSheet
    //      Show the tweet sheet
    bool ShowTweetSheet(const char *initialText) override;
};


}}// Namespaces
