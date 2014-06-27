// File: prTwitter.h
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


#ifndef __PRTWITTER_H
#define __PRTWITTER_H


#include "../../core/prTypes.h"
#include "../../core/prString.h"


// Class: prTwitterCallbacks
//      Class provides an easy way to get callbacks from the engine
class prTwitterCallbacks
{
public:
    // Method: Twitter_Initialise
    //      Twitter initialisation callback
    virtual void Twitter_Initialise(bool state) = 0;
    
    // Method: Twitter_Sent
    //      Twitter callback
    virtual void Twitter_Sent(bool state) = 0;

    // Method: Twitter_Cancel
    //      Twitter callback
    virtual void Twitter_Cancel(bool state) = 0;
};


// Class: prTwitter
//      A base class for abstracting application specific twitter
//      functionality into a cross platform format
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
    //      Allows twitter to call us
    void RegisterCallbackHandler(prTwitterCallbacks *pcb);

    // Method: Initialise
    //      Initialise twitter for a specific platform
    virtual bool Initialise() { return false; }

    // Method: Update
    //      Update twitter for a specific platform
    virtual bool Update() { return false; }

    // Method: ShowTweetSheet
    //      Show the tweet sheet
    virtual bool ShowTweetSheet(const char *initialText) { return false; }


protected:
    prTwitterCallbacks    *mpCallbacks;
};


#endif//__PRTWITTER_H
