// File: prTwitter.h
//      Cross platform twitter access
//
// Notes:
//      A mix-in class is used to receive callbacks from the twitter
//      code.
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


// Class: prTwitterCallbacks
//      This mix-in class provides an easy way to get callbacks from the engine
class prTwitterCallbacks
{
public:
    // Method: Twitter_Initialise
    //      Twitter initialised
    virtual void Twitter_Initialise() = 0;
    
    // Method: Twitter_Sent
    //      Twitter update sent
    virtual void Twitter_Sent() = 0;

    // Method: Twitter_Cancel
    //      Twitter update cancelled
    virtual void Twitter_Cancel() = 0;
};


// Forward declarations
class prTwitterBase;


// Class: prTwitter
//      Class to provide cross platform twitter access
class prTwitter
{
public:
    // Method: prTwitter
    //      Ctor
    //
    // Parameters:
    //      pcb - A pointer to the class which handles callbacks
    //
    // See Also:
    //      <prTwitterCallbacks>
    explicit prTwitter(prTwitterCallbacks *pcb);

    // Method: ~prTwitter
    //      Dtor
    ~prTwitter();

    // Method: Initialise
    //      Initialise twitter
    //
    // Notes:
    //      Kept separate so initialisation can be performed when required,
    //      rather than at class creation. In case initialisation causes issues
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

    
private:
    // Stops passing by value and assignment.
    prTwitter(const prTwitter&);
    const prTwitter& operator = (const prTwitter&);
};


}}// Namespaces
