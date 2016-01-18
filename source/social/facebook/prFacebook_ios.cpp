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


// Defines
#ifndef METHOD_SHARE_SHEET                      // Just in case we define it externally
#define METHOD_SHARE_SHEET                      // This method uses the share sheet
#endif


#include "prFacebook_ios.h"
#include "../../core/prTypes.h"
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"
#include "../../debug/prTrace.h"
#include "../../ios/prIosFacebook.h"


// Namespaces
namespace Proteus {
namespace Social {


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prFacebook_Ios::prFacebook_Ios(prFacebookCallbacks *pcb) : prFacebookBase(pcb)
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
#if 0
    prFB_Initialise(mpCallbacks);
#endif
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
#if 0
    prFB_OpenSession(mpCallbacks, mIsLoggedIn);
#endif
    return mIsLoggedIn;
}


/// ----------------------------------------------------------------------------
/// login to facebook
/// ----------------------------------------------------------------------------
bool prFacebook_Ios::Login()
{
#if 0
    prFB_Login(mpCallbacks, mIsLoggedIn);
#endif
    return mIsLoggedIn;
}

            
/// ----------------------------------------------------------------------------
/// Fetches the users details
/// ----------------------------------------------------------------------------
void prFacebook_Ios::FetchUserDetails()
{
#if 0
    prFB_FetchUserDetails(mpCallbacks, this);
#endif
}
    
    
/// ----------------------------------------------------------------------------
/// Send a brag to facebook
/// ----------------------------------------------------------------------------
void prFacebook_Ios::Brag(const char *initialText)
{
#if defined(METHOD_SHARE_SHEET)

    extern bool Tweet(const char *initialText);
    Tweet(initialText);

#else

    PRUNUSED(initialText);
    prFB_Brag(mpCallbacks, mPlayerFBID);
    
#endif
}


}}// Namespaces


#endif
