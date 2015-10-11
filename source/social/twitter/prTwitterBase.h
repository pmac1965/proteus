// File: prTwitterBase.h
//      Base class for the twitter classes
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


#include "../../core/prMacros.h"


// Namespaces
namespace Proteus {
namespace Social {


// Forward declarations
class prTwitterCallbacks;


// Class: prTwitterBase
//      The base class for twitter classes
class prTwitterBase
{
public:
    // Method: prTwitterBase
    //      Ctor
    prTwitterBase(prTwitterCallbacks *pcb) : mpCallbacks(pcb)
    {}

    // Method: ~prTwitterBase
    //      Ctor
    virtual ~prTwitterBase()
    {}

    // Method: Initialise
    //      Initialise twitter for a specific platform
    virtual bool Initialise() { return false; }

    // Method: Update
    //      Update twitter for a specific platform
    virtual bool Update() { return false; }

    // Method: ShowTweetSheet
    //      Show the tweet sheet
    //
    // Parameters:
    //      Initial text message for the tweet sheet.
    virtual bool ShowTweetSheet(const char *initialText) { PRUNUSED(initialText); return false; }


protected:
    prTwitterCallbacks    *mpCallbacks;
};


}}// Namespaces
