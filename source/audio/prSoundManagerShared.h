// File: prSoundManagerShared.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __PRSOUNDMANAGERSHARED_H
#define __PRSOUNDMANAGERSHARED_H


#include "../core/prTypes.h"
#include "../debug/prTrace.h"


// Shared defines
#define AUDIO_MAX_EFFECTS       256                     // Maximum effects that can be loaded
#define AUDIO_MAX_ACTIVE        24                      // Maximum sound efffects that can be played simultaneously
#define AUDIO_SFX_MIN_VOLUME    0.0f                    // Min volume
#define AUDIO_SFX_MAX_VOLUME    1.0f                    // Max volume
#define AUDIO_MUS_MIN_VOLUME    0.0f                    // Min volume
#define AUDIO_MUS_MAX_VOLUME    1.0f                    // Max volume
#define AUDIO_MUS_INITIAL_VOL   0.8f                    // Initial song volume


// Enum: prSoundEffectEntryState
//      SFX states
//
//  - SFX_STATE_FREE
//  - SFX_STATE_PLAYING
//  - SFX_STATE_PAUSED
//  - SFX_STATE_UNAVAILABLE
enum prSoundEffectEntryState
{
    SFX_STATE_FREE,
    SFX_STATE_PLAYING,
    SFX_STATE_PAUSED,
    SFX_STATE_UNAVAILABLE
};


// Enum: prSongState
//      Song states
//
//  - SONG_STATE_FREE
//  - SONG_STATE_PLAYING
//  - SONG_STATE_PAUSED
enum prSongState
{
    SONG_STATE_FREE,
    SONG_STATE_PLAYING,
    SONG_STATE_PAUSED,
};


// Struct: prSFXInfo
//      Audio definiton list
//
//  u32          - hash;
//  const char * - filename;
typedef struct prSFXInfo
{
    Proteus::Core::u32   hash;
    const char          *filename;

} prSFXInfo;


// Class: prLoadedWave
//      Stores loaded wave data.
class prLoadedWave
{
public:
    // Method: prLoadedWave
    //      Ctor
    prLoadedWave();
    
    // Method: ~prLoadedWave
    //      Dtor
    ~prLoadedWave();

public:
    // Data
    Proteus::Core::u32  uiBuffer;
    Proteus::Core::u32  hash;
};


// Struct: prSoundEffectEntry
//      Stores data about all active effects.
//
//  u32 - uiSource;
//  u32 - state;
//  u32 - hash;
//  u32 - id;
typedef struct prSoundEffectEntry
{
    Proteus::Core::u32 uiSource;
    Proteus::Core::u32 state;
    Proteus::Core::u32 hash;
    Proteus::Core::u32 id;

} prSoundEffectEntry;


#endif//__PRSOUNDMANAGERSHARED_H
