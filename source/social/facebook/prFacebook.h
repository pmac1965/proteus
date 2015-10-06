// File: prFacebook.h
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


// Class: prFacebookCallbacks
//      Class provides an easy way to get callbacks from the engine
class prFacebookCallbacks
{
public:
    // Method: Facebook_Initialise
    //      Facebook initialisation callback
    virtual void Facebook_Initialise(bool state) = 0;

    // Method: Facebook_Login
    //      Facebook login callback
    virtual void Facebook_Login(bool state) = 0;

    // Method: Facebook_Logout
    //      Facebook logout callback
    virtual void Facebook_Logout(bool state) = 0;

    // Method: Facebook_FetchUserDetails
    //      Facebook fetch callback
    virtual void Facebook_FetchUserDetails(bool state) = 0;

    // Method: Facebook_OpenSession
    //      Facebook open session callback
    virtual void Facebook_OpenSession(bool state) = 0;
    
    // Method: Facebook_Brag
    //      Facebook brag callback
    virtual void Facebook_Brag(bool state) = 0;
    
    // Method: Facebook_PostSent
    //      Facebook callback
    virtual void Facebook_PostSent() = 0;
    
    // Method: Facebook_PostCancelled
    //      Facebook callback
    virtual void Facebook_PostCancelled() = 0;
    
    // Method: Facebook_PostError
    //      Facebook callback
    virtual void Facebook_PostError() = 0;
};


// Class: prFacebook
//      A base class for abstracting application specific facebook
//      functionality into a cross platform format
class prFacebook
{
public:
    // Method: prFacebook
    //      Ctor
    prFacebook();

    // Method: ~prFacebook
    //      Ctor
    virtual ~prFacebook();

    // Method: RegisterCallbackHandler
    //      Allows facebook to call us
    void RegisterCallbackHandler(prFacebookCallbacks *pcb);

    // Method: IsLoggedIn
    //      Returns the logged in state
    bool IsLoggedIn() const { return mIsLoggedIn; }

    // Method: SetLoggedIn
    //      Allows facebook to call us
    void SetLoggedIn(bool state) { mIsLoggedIn = state; }

    // Method: SetDetails
    //      Allows facebook to set the user details
    void SetDetails(const char *name, Proteus::Core::u64 fbid);
    
    // Method: GetPlayerID
    //      Returns the facebook players ID
    Proteus::Core::u64 GetPlayerID() const { return mPlayerFBID; }
    
    // Method: GetPlayerName
    //      Returns the facebook players name
    const char *GetPlayerName() const { return mPlayerName.Text(); }

    // Method: Initialise
    //      Initialise facebook for a specific platform
    virtual bool Initialise() { return false; }

    // Method: Update
    //      Update facebook for a specific platform
    virtual bool Update() { return false; }

    // Method: OpenSession
    //      Open a session
    virtual bool OpenSession() { return false; }

    // Method: Login
    //      login to facebook
    virtual bool Login() { return false; }

    // Method: Logout
    //      logout off facebook
    virtual bool Logout() { return false; }

    // Method: FetchUserDetails
    //      login to facebook
    virtual void FetchUserDetails() {}
        
    // Method: Brag
    //      Send a brag to facebook
    virtual void Brag(const char *initialText) { PRUNUSED(initialText); }


protected:
    prFacebookCallbacks    *mpCallbacks;
    prString                mPlayerName;
    Proteus::Core::u64      mPlayerFBID;
    bool                    mIsLoggedIn;
};
