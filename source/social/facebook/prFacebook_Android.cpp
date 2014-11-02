/**
 * prFacebook_Android.cpp
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


#if defined(PLATFORM_ANDROID)


#include "prFacebook_Android.h"
#include "../../core/prTypes.h"
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"
#include "../../android/prJNIFacebook.h"


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prFacebook_Android::prFacebook_Android()
{
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prFacebook_Android::~prFacebook_Android()
{
}


/// ----------------------------------------------------------------------------
/// Initialise facebook for a specific platform
/// ----------------------------------------------------------------------------
bool prFacebook_Android::Initialise()
{
    return true;
}


/// ----------------------------------------------------------------------------
/// Update facebook for a specific platform
/// ----------------------------------------------------------------------------
bool prFacebook_Android::Update()
{
    return true;
}


/// ----------------------------------------------------------------------------
/// Open a session
/// ----------------------------------------------------------------------------
bool prFacebook_Android::OpenSession()
{
    prJNI_FacebookOpenSession();
    return mIsLoggedIn;
}


/// ----------------------------------------------------------------------------
/// login to facebook
/// ----------------------------------------------------------------------------
bool prFacebook_Android::Login()
{
    prJNI_FacebookLogin();
    return mIsLoggedIn;
}


/// ----------------------------------------------------------------------------
/// logout of facebook
/// ----------------------------------------------------------------------------
bool prFacebook_Android::Logout()
{
    //prJNI_FacebookLogin();
    return mIsLoggedIn;
}

            
/// ----------------------------------------------------------------------------
/// Fetches the users details
/// ----------------------------------------------------------------------------
void prFacebook_Android::FetchUserDetails()
{
    //prFB_FetchUserDetails(mpCallbacks, this);
}
    
    
/// ----------------------------------------------------------------------------
/// Send a brag to facebook
/// ----------------------------------------------------------------------------
void prFacebook_Android::Brag(const char *initialText)
{
    prJNI_FacebookBrag();
    //prFB_Brag(mpCallbacks, mPlayerFBID);
}


#endif
