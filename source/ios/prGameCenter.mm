/**
 * prGameCenter.mm
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


#if defined(PLATFORM_IOS)


#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>
#include "prGameCenter.h"


/// ---------------------------------------------------------------------------
/// Keep the interface here as its easier to compile
/// ---------------------------------------------------------------------------
@interface prGameCenter : NSObject
{
    BOOL gameCenterAvailable;
    BOOL userAuthenticated;
}

@property (assign, readonly) BOOL gameCenterAvailable;

+ (prGameCenter *) instance;

- (void)authenticateLocalUser;

@end



/// ---------------------------------------------------------------------------
/// IMPLEMENTATION
/// ---------------------------------------------------------------------------
@implementation prGameCenter
{
}


@synthesize gameCenterAvailable;


static prGameCenter *sharedHelper = nil;


/// ---------------------------------------------------------------------------
/// Construct a singleton
/// ---------------------------------------------------------------------------
+ (prGameCenter *) instance
{
    if (!sharedHelper)
    {
        sharedHelper = [[prGameCenter alloc] init];
    }
    
    return sharedHelper;
}


/// ---------------------------------------------------------------------------
/// Determine if game center is available
/// ---------------------------------------------------------------------------
- (BOOL) isGameCenterAvailable
{
    // Check for presence of GKLocalPlayer API
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    
    // Check if the device is running iOS 4.1 or later
    NSString *reqSysVer     = @"4.1";
    NSString *currSysVer    = [[UIDevice currentDevice] systemVersion];
    
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}


/// ---------------------------------------------------------------------------
/// The constructor
/// ---------------------------------------------------------------------------
- (id) init
{
    if ((self = [super init]))
    {
        gameCenterAvailable = [self isGameCenterAvailable];
        if (gameCenterAvailable)
        {
            NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
            
            [nc addObserver:self
                   selector:@selector(authenticationChanged)
                       name:GKPlayerAuthenticationDidChangeNotificationName
                     object:nil];
        }
    }
    
    return self;
}


/// ---------------------------------------------------------------------------
///
/// ---------------------------------------------------------------------------
- (void) authenticationChanged
{
    if ([GKLocalPlayer localPlayer].isAuthenticated && !userAuthenticated)
    {
        NSLog(@"Authentication changed: player authenticated.");
        userAuthenticated = TRUE;
    }
    else if (![GKLocalPlayer localPlayer].isAuthenticated && userAuthenticated)
    {
        NSLog(@"Authentication changed: player not authenticated");
        userAuthenticated = FALSE;
    }    
}


/// ---------------------------------------------------------------------------
///
/// ---------------------------------------------------------------------------
- (void) authenticateLocalUser
{    
    if (!gameCenterAvailable)
        return;
    
    NSLog(@"Authenticating local user...");
    
    if ([GKLocalPlayer localPlayer].authenticated == NO)
    {
        [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:nil];
    }
    else
    {
        NSLog(@"Already authenticated!");
    }
}

@end



/// ---------------------------------------------------------------------------
/// Initialises the game center if its available
/// ---------------------------------------------------------------------------
void prGameCenterInit()
{
    [[prGameCenter instance] authenticateLocalUser];
}


#endif
