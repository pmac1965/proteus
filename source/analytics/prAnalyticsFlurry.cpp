/**
 * prAnalyticsFlurry.cpp
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


#include "../prConfig.h"


#include "prAnalyticsFlurry.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"


#if defined(PLATFORM_IOS)
#include "../ios/prIosFlurry.h"
#endif


#if defined (PLATFORM_ANDROID)
#include "../android/prJNIAnalytics.h"
#endif


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prAnalyticsFlurry::prAnalyticsFlurry()
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prAnalyticsFlurry::~prAnalyticsFlurry()
{
}


/// ---------------------------------------------------------------------------
/// Construct the provider.
/// ---------------------------------------------------------------------------
void prAnalyticsFlurry::Construct(const char *appId, const char *secret)
{
#if defined (PLATFORM_IOS)
    
    PRUNUSED(secret);
    
    if (appId && *appId)
    {
        prIosFlurryConstruct(appId);
    }
    
#elif defined (PLATFORM_ANDROID)

    PRUNUSED(appId);
    PRUNUSED(secret);
    
    // the android version is constructed on app startup in the java code.
    // And uses onStart/onStop
    
#else
    
    PRUNUSED(appId);
    PRUNUSED(secret);
    
#endif
}


/// ---------------------------------------------------------------------------
/// Submits to the provider.
/// ---------------------------------------------------------------------------
bool prAnalyticsFlurry::Submit(const char *name, const char *value)
{
    bool result = false;
    
#if defined (PLATFORM_IOS)
    
    if (name && *name)
    {
        prIosFlurrySubmit(name);
        prTrace(LogError, "Logging event %s\n", name);
    }
    
#elif defined (PLATFORM_ANDROID)

    if (name && *name)
    {
        if (value && *value)
        {
            //JNI_SubmitFlurry(name, value);
            prTrace(LogError, "PROT:(2) Logging event %s\n", name);
        }
        else
        {
            prTrace(LogError, "PROT:(1) Logging event %s\n", name);
            prJNI_Submit(name);
        }
    }

#else

    PRUNUSED(name);
    PRUNUSED(value);

#endif
    
    return result;
}


/// ---------------------------------------------------------------------------
/// Commits the submitted key/values to the provider.
/// ---------------------------------------------------------------------------
void prAnalyticsFlurry::Commit()
{
}
