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
 *
 */


#include "../prConfig.h"


#include "prAnalyticsFlurry.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"


#if defined (PLATFORM_ANDROID)
//#include "../android/JNIInterface.h"
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
    PRUNUSED(appId);
    PRUNUSED(secret);
}


/// ---------------------------------------------------------------------------
/// Submits to the provider.
/// ---------------------------------------------------------------------------
bool prAnalyticsFlurry::Submit(const char *name, const char *value)
{
    bool result = false;
    
#if defined (PLATFORM_IOS)
    
    //extern bool FLurry_LogEvent(const char *name, const char *value);
    //if (name && *name)
    //{
    //    //Trace("Logging event %s\n", name);
    //    result = FLurry_LogEvent(name, value);
    //}
    
#elif defined (PLATFORM_ANDROID)

    if (value && *value)
    {
        //JNI_SubmitFlurry(name, value);
    }
    else
    {
        //JNI_SubmitFlurry(name);
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
