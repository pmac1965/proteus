/**
 * prIosTwitter.mm
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


#import <Social/Social.h>


#include "prTwitter.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Keep the interface here as its easier to compile
/// ---------------------------------------------------------------------------
@interface prIOSTwitter : NSObject
{
}

+ (prIOSTwitter *) instance;

- (bool) openTweetSheet:(const char *)initialText;

@end


/// ---------------------------------------------------------------------------
/// IMPLEMENTATION
/// ---------------------------------------------------------------------------
@implementation prIOSTwitter
    {
}


static prIOSTwitter *sharedHelper = nil;
    

/// ---------------------------------------------------------------------------
/// Construct a singleton
/// ---------------------------------------------------------------------------
+ (prIOSTwitter *) instance
{
    if (!sharedHelper)
    {
        sharedHelper = [[prIOSTwitter alloc] init];
    }
    
    return sharedHelper;
    }


/// ---------------------------------------------------------------------------
/// Opens the tweet sheet for basic tweeting
/// ---------------------------------------------------------------------------
- (bool) openTweetSheet:(const char *)initialText
{
    // The Tweet Function should be in the view controller
    extern bool Tweet(const char *initialText);
    return Tweet(initialText);
}


/// ---------------------------------------------------------------------------
/// The constructor
/// ---------------------------------------------------------------------------
- (id) init
{
    if ((self = [super init]))
    {
        prTrace("Construct prIOStwitter\n");
    }
    
    return self;
}


@end


/// ---------------------------------------------------------------------------
/// Opens the tweet sheet for basic tweeting
/// ---------------------------------------------------------------------------
bool prIosTwitterOpenTweetSheet(const char *initialText)
{
    bool result = false;
    
    if (initialText && *initialText)
    {
        result = [[prIOSTwitter instance] openTweetSheet:initialText];
    }

    return result;
}


#endif
