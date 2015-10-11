// File: prFacebookBase.h
//      Base class for the facebook classes
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


#include "../../core/prTypes.h"
#include "../../core/prString.h"
#include "../../core/prMacros.h"


// Namespaces
namespace Proteus {
namespace Social {


// Forward declarations
class prFacebookCallbacks;


// Class: prFacebookBase
//      The base class for facebook classes
class prFacebookBase
{
public:
    // Method: prFacebookBase
    //      Ctor
    prFacebookBase(prFacebookCallbacks *pcb) : mpCallbacks  (pcb)
                                             , mPlayerFBID  (0LL)
                                             , mIsLoggedIn  (false)
    {}

    // Method: ~prFacebookBase
    //      Ctor
    virtual ~prFacebookBase()
    {}

    // Method: IsLoggedIn
    //      Returns the logged in state
    bool IsLoggedIn() const { return mIsLoggedIn; }
    
    // Method: GetPlayerID
    //      Returns the players facebook ID
    Proteus::Core::u64 GetPlayerID() const { return mPlayerFBID; }
    
    // Method: GetPlayerName
    //      Returns the facebook players name
    const char *GetPlayerName() const { return mPlayerName.Text(); }

    // Method: Initialise
    //      Initialise facebook for a specific platform
    virtual bool Initialise() = 0;

    // Method: Update
    //      Update facebook for a specific platform
    virtual bool Update() = 0;

    // Method: OpenSession
    //      Open a session
    virtual bool OpenSession() = 0;

    // Method: Login
    //      login to facebook
    virtual bool Login() = 0;

    // Method: Logout
    //      logout of facebook
    virtual bool Logout() = 0;
    
    // Method: FetchUserDetails
    //      login to facebook
    virtual void FetchUserDetails() = 0;
    
    // Method: Brag
    //      Send a brag to facebook
    virtual  void Brag(const char *initialText) = 0;


protected:
    prString                mPlayerName;
    prFacebookCallbacks    *mpCallbacks;
    Proteus::Core::u64      mPlayerFBID;
    bool                    mIsLoggedIn;
};


}}// Namespaces
