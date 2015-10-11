// File: prFacebook_Android.h
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


#include "prFacebookBase.h"


// Namespaces
namespace Proteus {
namespace Social {


// Forward declarations
class prFacebookCallbacks;


// Class: prFacebook_Android
//      A base class for abstracting application specific facebook
//      functionality into a cross platform format
class prFacebook_Android : public prFacebookBase
{
public:
    // Method: prFacebook_Android
    //      Ctor
    prFacebook_Android(prFacebookCallbacks *pcb);

    // Method: ~prFacebook_Android
    //      Ctor
    ~prFacebook_Android();

    // Method: Initialise
    //      Initialise facebook for a specific platform
    bool Initialise() override;

    // Method: Update
    //      Update facebook for a specific platform
    bool Update() override;

    // Method: OpenSession
    //      Open a session
    bool OpenSession() override;

    // Method: Login
    //      login to facebook
    bool Login() override;

    // Method: Logout
    //      logout of facebook
    bool Logout() override;
    
    // Method: FetchUserDetails
    //      login to facebook
    void FetchUserDetails() override;
    
    // Method: Brag
    //      Send a brag to facebook
    void Brag(const char *initialText) override;
};


}}// Namespaces
