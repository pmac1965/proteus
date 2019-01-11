/**
 * prFacebook.cpp
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


#include "prFacebook.h"
#include "prFacebookBase.h"
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"
#include "../../debug/prTrace.h"


#if defined(PLATFORM_ANDROID)
#include "prFacebook_Android.h"
#endif


#if defined(PLATFORM_IOS)
#include "prFacebook_ios.h"
#endif


// Namespaces
namespace Proteus {
namespace Social {


//using namespace Proteus::Core;


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prFacebook::prFacebook(prFacebookCallbacks *pcb)
{
#if defined(PLATFORM_ANDROID)
    mpFacebook = new prFacebook_Android(pcb);

#elif defined(PLATFORM_IOS)
//#if 0
    mpFacebook = nullptr;//new prFacebook_Ios(pcb);
//#endif
    
#else
    mpFacebook = nullptr;

#endif
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prFacebook::~prFacebook()
{
    PRSAFE_DELETE(mpFacebook);
}


/// ----------------------------------------------------------------------------
/// Returns the logged in state
/// ----------------------------------------------------------------------------
bool prFacebook::IsLoggedIn() const
{ 
    bool result = false;

    if (mpFacebook)
    {
        result = mpFacebook->IsLoggedIn();
    }

    return result;
}


/// ----------------------------------------------------------------------------
/// Returns the players facebook ID
/// ----------------------------------------------------------------------------
u64 prFacebook::GetPlayerID() const
{
    u64 result = 0LL;

    if (mpFacebook)
    {
        result = mpFacebook->GetPlayerID();
    }

    return result;
}


/// ----------------------------------------------------------------------------
/// Returns the facebook players name
/// ----------------------------------------------------------------------------
const char *prFacebook::GetPlayerName() const
{
    const char *result = nullptr;

    if (mpFacebook)
    {
        result = mpFacebook->GetPlayerName();
    }

    return result;
}


/// ----------------------------------------------------------------------------
/// Initialise facebook
/// ----------------------------------------------------------------------------
bool prFacebook::Initialise()
{
    bool result = false;

    if (mpFacebook)
    {
        result = mpFacebook->Initialise();
    }

    return result;
}


/// ----------------------------------------------------------------------------
/// Allows facebook to set the user details
/// ----------------------------------------------------------------------------
void prFacebook::SetDetails(const char *name, u64 fbid)
{
    //mPlayerFBID = fbid;
    //mIsLoggedIn = true;         // Set logged in - only when logged in to facebook
    //                            //                 can we set the details    
    //if (name && *name)
    //{
    //    mPlayerName.Set(name);
    //}

    //prTrace(LogError, "FB SetDetails: %s\n", mPlayerName.Text());
    //prTrace(LogError, "FB SetDetails: %llu\n", fbid);
}


}}// Namespaces
