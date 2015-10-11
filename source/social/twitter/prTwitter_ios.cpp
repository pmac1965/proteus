/**
 * prTwitter_ios.cpp
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


#include "prTwitter_ios.h"
#include "../../core/prTypes.h"
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"
#include "../../debug/prTrace.h"
#include "../../ios/prIosTwitter.h"


// Namespaces
namespace Proteus {
namespace Social {


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prTwitter_Ios::prTwitter_Ios(prTwitterCallbacks *pcb) : prTwitterBase(pcb)
{
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prTwitter_Ios::~prTwitter_Ios()
{
}


/// ----------------------------------------------------------------------------
/// Initialise twitter for a specific platform
/// ----------------------------------------------------------------------------
bool prTwitter_Ios::Initialise()
{
    return true;
}


/// ----------------------------------------------------------------------------
/// Update twitter for a specific platform
/// ----------------------------------------------------------------------------
bool prTwitter_Ios::Update()
{
    return true;
}


/// ----------------------------------------------------------------------------
/// Show the tweet sheet
/// ----------------------------------------------------------------------------
bool prTwitter_Ios::ShowTweetSheet(const char *initialText)
{
    if (initialText && *initialText)
    {
        prIosTwitterOpenTweetSheet(initialText);
    }
    
    return true;
}


}}// Namespaces


#endif
