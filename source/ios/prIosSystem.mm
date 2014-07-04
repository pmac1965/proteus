/**
 * prIosSystem.mm
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


#include "prIosTwitter.h"


#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>


// Debug
//#define DEBUG_IOS_SYSTEM



/// ---------------------------------------------------------------------------
/// Does simple vibration
/// ---------------------------------------------------------------------------
void prIosSystemVibrate()
{
    AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
}


/// ---------------------------------------------------------------------------
/// Enables or disables the devices sleep mode
/// ---------------------------------------------------------------------------
void prIosSystemSleepEnable(bool state)
{
    if (state)
    {
        #if defined(DEBUG_SLEEP) && defined(DEBUG)
        NSLog(@"setIdleTimerDisabled:YES");
        #endif
        
        [[UIApplication sharedApplication] setIdleTimerDisabled:YES];
    }
    else
    {
        #if defined(DEBUG_SLEEP) && defined(DEBUG)
        NSLog(@"setIdleTimerDisabled:NO");
        #endif
        
        [[UIApplication sharedApplication] setIdleTimerDisabled:NO];
    }
 
    #if defined(DEBUG_SLEEP) && defined(DEBUG)
    if ([UIApplication sharedApplication].idleTimerDisabled == YES)
    {
        NSLog(@"idleTimerDisabled == YES");
    }
    else
    {
        NSLog(@"idleTimerDisabled == NO");
    }
    #endif
}


#endif
