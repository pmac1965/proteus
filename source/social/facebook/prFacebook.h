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


// Namespaces
namespace Proteus {
namespace Social {


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


// Forward declarations
class prFacebookBase;


// Class: prFacebook
//      Class for abstracting facebook functionality into a cross platform format
class prFacebook
{
public:
    // Method: prFacebook
    //      Ctor
    //
    // Parameters:
    //      pcb - A pointer to the class which handles callbacks
    //
    // See Also:
    //      <prFacebookCallbacks>
    explicit prFacebook(prFacebookCallbacks *pcb);

    // Method: ~prFacebook
    //      Ctor
    ~prFacebook();

    // Method: IsLoggedIn
    //      Returns the logged in state
    bool IsLoggedIn() const;
    
    // Method: GetPlayerID
    //      Returns the players facebook ID
    u64 GetPlayerID() const;
    
    // Method: GetPlayerName
    //      Returns the facebook players name
    //
    // Returns:
    //      May return nullptr
    const char *GetPlayerName() const;

    // Method: Initialise
    //      Initialise facebook for a specific platform
    //
    // Notes:
    //      Kept separate so initialisation can be performed when required,
    //      rather than at class creation. In case initialisation causes issues
    bool Initialise();

    // Method: Update
    //      Update facebook for a specific platform
    bool Update() { return false; }

    // Method: OpenSession
    //      Open a session
    bool OpenSession() { return false; }

    // Method: Login
    //      login to facebook
    bool Login() { return false; }

    // Method: Logout
    //      logout off facebook
    bool Logout() { return false; }

    // Method: FetchUserDetails
    //      login to facebook
    void FetchUserDetails() {}
        
    // Method: Brag
    //      Send a brag to facebook
    void Brag(const char *initialText) { PRUNUSED(initialText); }


private:
    // Allows facebook to set logged in state
    //void SetLoggedIn(bool state) { mIsLoggedIn = state; }

    // Allows facebook to set the user details
    void SetDetails(const char *name, u64 fbid);


private:
    prFacebookBase         *mpFacebook;
    //prString                mPlayerName;
    //u64      mPlayerFBID;
    //bool                    mIsLoggedIn;
    
    
private:
    // Stops passing by value and assignment.
    prFacebook(const prFacebook&);
    const prFacebook& operator = (const prFacebook&);
};


}}// Namespaces
