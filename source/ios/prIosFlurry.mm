/**
 * prIosFlurry.mm
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


#if defined(PLATFORM_IOS)


#import <UIKit/UIKit.h>
#import "Flurry.h"
#include "../debug/prDebug.h"


/// ---------------------------------------------------------------------------
/// Constructs flurry
/// ---------------------------------------------------------------------------
void prIosFlurryConstruct(const char *appID)
{
    NSString *thisAppID = [[NSString alloc] initWithUTF8String:appID];

    [Flurry startSession:thisAppID];
    
#if !__has_feature(objc_arc)
    [thisAppID release];
#else
    // Do nothing
#endif

    DBG(@"%@", [Flurry getFlurryAgentVersion]);
}


/// ---------------------------------------------------------------------------
/// Submits an event to flurry
/// ---------------------------------------------------------------------------
void prIosFlurrySubmit(const char *event)
{
    NSString *thisEvent = [[NSString alloc] initWithUTF8String:event];
    
    [Flurry logEvent:thisEvent];
    
#if !__has_feature(objc_arc)
    [thisEvent release];
#else
    // Do nothing
#endif
}


#endif
