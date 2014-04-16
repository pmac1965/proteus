/**
 * prIosAudio.mm
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


#import <UIKit/UIKit.h>
#import <AVFoundation/AVAudioPlayer.h>
#include "prIosAudio.h"
#include "../debug/prTrace.h"


namespace
{
    AVAudioPlayer *audioPlayer = nil;
}


/// ---------------------------------------------------------------------------
/// Plays a song
/// ---------------------------------------------------------------------------
void prIosSongPlay(const char *filename)
{
    prIosSongStop();
    
    if (filename && *filename)
    {
        NSString *path = [NSString stringWithCString:filename encoding:NSASCIIStringEncoding];
        NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle]
                                              pathForResource:path
                                              ofType:@"mp3"]];
        
        NSError *err = nil;
        audioPlayer = [[AVAudioPlayer alloc]
                        initWithContentsOfURL:url
                        error:&err];
        
        if (audioPlayer == nil)
        {
            NSLog(@"%@", [err description]);
        }
        else
        {
            // Set to repeat
            audioPlayer.numberOfLoops = -1;
            
            [audioPlayer play];
        }
        
        // Clean up
        //[url release];
        //[path release];
    }
}


/// ---------------------------------------------------------------------------
/// Stops a song
/// ---------------------------------------------------------------------------
void prIosSongStop()
{
    if (audioPlayer)
    {
        [audioPlayer stop];
        [audioPlayer release];
        audioPlayer = NULL;
    }
}


/// ---------------------------------------------------------------------------
/// Pause or unpause a song.
/// ---------------------------------------------------------------------------
void prIosSongPause(bool pause)
{
    if (audioPlayer)
    {
        if (pause)
        {
            [audioPlayer pause];
        }
        else
        {
            [audioPlayer play];
        }
    }
    else
    {
        NSLog(@"pause: No player instance");
    }
}


/// ---------------------------------------------------------------------------
/// Sets the song volume
/// ---------------------------------------------------------------------------
void prIosSongSetVolume(float volume)
{
    if (audioPlayer)
    {
        audioPlayer.volume = volume;
    }
    else
    {
        NSLog(@"SetVolume: No player instance");
    }
}


#endif
