/**
 * prFacebook_ios.cpp
 *
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


#include "../../prConfig.h"


#if defined(PLATFORM_IOS)


#include "prFacebook_ios.h"
#include "../../core/prTypes.h"
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"
#include "../../debug/prTrace.h"
#include "../../ios/prIosFacebook.h"


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prFacebook_Ios::prFacebook_Ios()
{
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prFacebook_Ios::~prFacebook_Ios()
{
}


/// ----------------------------------------------------------------------------
/// Initialise facebook for a specific platform
/// ----------------------------------------------------------------------------
bool prFacebook_Ios::Initialise()
{
    prFB_Initialise(mpCallbacks);
    return true;
}


/// ----------------------------------------------------------------------------
/// Update facebook for a specific platform
/// ----------------------------------------------------------------------------
bool prFacebook_Ios::Update()
{
    return true;
}


/// ----------------------------------------------------------------------------
/// Open a session
/// ----------------------------------------------------------------------------
bool prFacebook_Ios::OpenSession()
{
    prFB_OpenSession(mpCallbacks, mIsLoggedIn);
    return mIsLoggedIn;
}


/// ----------------------------------------------------------------------------
/// login to facebook
/// ----------------------------------------------------------------------------
bool prFacebook_Ios::Login()
{
    prFB_Login(mpCallbacks, mIsLoggedIn);
    return mIsLoggedIn;
}

            
/// ----------------------------------------------------------------------------
/// Fetches the users details
/// ----------------------------------------------------------------------------
void prFacebook_Ios::FetchUserDetails()
{
    prFB_FetchUserDetails(mpCallbacks, this);
}
    
    
/// ----------------------------------------------------------------------------
/// Send a brag to facebook
/// ----------------------------------------------------------------------------
void prFacebook_Ios::Brag()
{
    prFB_Brag(mpCallbacks, mPlayerFBID);
}


#endif
