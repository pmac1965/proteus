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
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"
#include "../../debug/prTrace.h"


using namespace Proteus::Core;


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prFacebook::prFacebook()
{
    mpCallbacks = NULL;
    mIsLoggedIn = false;
    mPlayerFBID = 0LL;
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prFacebook::~prFacebook()
{
}


/// ----------------------------------------------------------------------------
/// Allows facebook to call us
/// ----------------------------------------------------------------------------
void prFacebook::RegisterCallbackHandler(prFacebookCallbacks *pcb)
{
    mpCallbacks = pcb;
}


/// ----------------------------------------------------------------------------
/// Allows facebook to set the user details
/// ----------------------------------------------------------------------------
void prFacebook::SetDetails(const char *name, u64 fbid)
{
    mPlayerFBID = fbid;
    mIsLoggedIn = true;         // Set logged in - only when logged in to facebook
                                //                 can we set the details    
    if (name && *name)
    {
        mPlayerName.Set(name);
    }

    prTrace(LogError, "FB SetDetails: %s\n", mPlayerName.Text());
    prTrace(LogError, "FB SetDetails: %llu\n", fbid);
}
