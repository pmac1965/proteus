/**
 * prTwitter.cpp
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


#include "prTwitter.h"
#include "prTwitterBase.h"
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"


#if defined(PLATFORM_ANDROID)
#include "prTwitter_Android.h"
#endif


#if defined(PLATFORM_IOS)
#include "prTwitter_ios.h"
#endif


// Namespaces
namespace Proteus {
namespace Social {


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prTwitter::prTwitter()
{
#if defined(PLATFORM_ANDROID)
    mpTwitter = new prTwitter_Android();
    //Initialise();

#elif defined(PLATFORM_IOS)
    mpTwitter = new prTwitter_Ios();
    //Initialise();

#else
    mpTwitter = nullptr;

#endif
}


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prTwitter::~prTwitter()
{
    PRSAFE_DELETE(mpTwitter);
}


/// ----------------------------------------------------------------------------
/// Allows twitter to call us
/// ----------------------------------------------------------------------------
void prTwitter::RegisterCallbackHandler(prTwitterCallbacks *pcb)
{
    if (mpTwitter)
    {
        mpTwitter->RegisterCallbackHandler(pcb);
    }
}


/// ----------------------------------------------------------------------------
/// Initialise twitter for a specific platform
/// ----------------------------------------------------------------------------
bool prTwitter::Initialise()
{
    bool result = false;

    if (mpTwitter)
    {
        result = mpTwitter->Initialise();
    }

    return result;
}


/// ----------------------------------------------------------------------------
/// Update twitter for a specific platform
/// ----------------------------------------------------------------------------
bool prTwitter::Update()
{
    bool result = false;

    if (mpTwitter)
    {
        result = mpTwitter->Update();
    }

    return result;
}


/// ----------------------------------------------------------------------------
/// Show the tweet sheet
/// ----------------------------------------------------------------------------
bool prTwitter::ShowTweetSheet(const char *initialText)
{
    bool result = false;

    if (mpTwitter)
    {
        result = mpTwitter->ShowTweetSheet(initialText);
    }

    return result;
}


}}// Namespaces
