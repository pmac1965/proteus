/**
 * prTwitter_Android.cpp
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


#include "prTwitter_Android.h"
#include "../../android/prJNITwitter.h"
#include "../../core/prTypes.h"
#include "../../core/prDefines.h"
#include "../../core/prMacros.h"
#include "../../debug/prTrace.h"


// Namespaces
namespace Proteus {
namespace Social {


/// ----------------------------------------------------------------------------
/// Ctor
/// ----------------------------------------------------------------------------
prTwitter_Android::prTwitter_Android()
{
}


/// ----------------------------------------------------------------------------
/// Dtor
/// ----------------------------------------------------------------------------
prTwitter_Android::~prTwitter_Android()
{
}


/// ----------------------------------------------------------------------------
/// Initialise twitter for a specific platform
/// ----------------------------------------------------------------------------
bool prTwitter_Android::Initialise()
{
    return true;
}


/// ----------------------------------------------------------------------------
/// Update twitter for a specific platform
/// ----------------------------------------------------------------------------
bool prTwitter_Android::Update()
{
    return true;
}


/// ----------------------------------------------------------------------------
/// Show the tweet sheet
/// ----------------------------------------------------------------------------
bool prTwitter_Android::ShowTweetSheet(const char *initialText)
{
    prJNI_ShowTweet(initialText);
}


}}// Namespaces


#endif
