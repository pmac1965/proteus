// File: prIosFacebook.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "../prConfig.h"


#if defined(PLATFORM_IOS)


class prFacebookCallbacks;
class prFacebook_Ios;


// Function: prFB_Initialise
//      Initialises facebook
void prFB_Initialise(prFacebookCallbacks *pCallback);

// Function: prFB_OpenSession
//      Opens a facebook session
void prFB_OpenSession(prFacebookCallbacks *pCallback, bool &mIsLoggedIn);

// Function: prFB_Login
//      Login to facebook
void prFB_Login(prFacebookCallbacks *pCallback, bool &mIsLoggedIn);

// Function: prFB_FetchUserDetails
//      Fetches the users details
void prFB_FetchUserDetails(prFacebookCallbacks *pCallback, prFacebook_Ios *pFacebook);

// Function: prFB_Brag
//      Brags to facebook
void prFB_Brag(prFacebookCallbacks *pCallback, Proteus::Core::u64 fbid);


#endif//PLATFORM_IOS

