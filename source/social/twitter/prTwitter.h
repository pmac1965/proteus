// File: prTwitter.h
//      Cross platform format twitter access
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


// Namespaces
namespace Proteus {
namespace Social {


// Forward declarations
class prTwitterBase;
class prTwitterCallbacks;


// Class: prTwitter
//      Cross platform format twitter access
class prTwitter
{
public:
    // Method: prTwitter
    //      Ctor
    prTwitter();

    // Method: ~prTwitter
    //      Ctor
    virtual ~prTwitter();

    // Method: RegisterCallbackHandler
    //      Allows twitter to call us back
    //
    // Parameters:
    //      A pointer to the callback class
    //
    // See Also:
    //      <prTwitterCallbacks>
    void RegisterCallbackHandler(prTwitterCallbacks *pcb);

    // Method: Initialise
    //      Initialise twitter
    bool Initialise();

    // Method: Update
    //      Update twitter
    bool Update();

    // Method: ShowTweetSheet
    //      Show the tweet sheet
    //
    // Parameters:
    //      Initial text message for the tweet sheet.
    bool ShowTweetSheet(const char *initialText);


protected:
    prTwitterBase   *mpTwitter;
};


}}// Namespaces
