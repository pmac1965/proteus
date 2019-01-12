// File: prSoundManager_Linux.h
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


#pragma once


#include "../prConfig.h"


#if defined(PLATFORM_LINUX)


#include "prSoundManager.h"
#include <AL/al.h>
#include <AL/alc.h>
#include "prWaves.h"
#include "../core/prTypes.h"


// Class: prSoundManager_Linux
//      Sound system controller for linux.
class prSoundManager_Linux : public prSoundManager
{
public:
    // Method: prSoundManager_Linux
    //      Ctor
    prSoundManager_Linux();

    // Method: ~prSoundManager_Linux
    //      Dtor
    ~prSoundManager_Linux();

    // Method: Initialise
    //      Initialises the sound system.
    bool Initialise();

    // Method: Release
    //      Releases all sound system assets.
    void Release();

    // Method: Update
    //      Updates the sound system.
    void Update(f32 dt);

    // Method: LoadSFX
    //      Loads the sound effects into sound memory.
    void LoadSFX(const prSFXInfo *sfx, s32 count);

    // Method: SongPlayByName
    //      Starts playing a song.
    void SongPlayByName(const char *filename);

    // Method: SongStop
    //      Stops the current song.
    void SongStop(f32 time = 0.0f);

    // Method: SongPause
    //      Pauses the active song.
    void SongPause(bool pause);

    // Method: SongGetPaused
    //      Determines if the current song is paused.
    bool SongGetPaused() const;

    // Method: SongSetVolume
    //      Sets the volume of the current song.
    void SongSetVolume(f32 volume);

    // Method: SFXPlay
    //      Plays a sound effect.
    s32 SFXPlay(s32 index, f32 volume = 1.0f, bool loop = false);

    // Method: SFXStop
    //      Stops the specified effect.
    void SFXStop(s32 id);

    // Method: SFXStop
    //      Stops the specified effect.
    void SFXStop(const char *name);

    // Method: SFXStopAll
    //      Stops all the sound effects.
    void SFXStopAll();

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    bool SFXIsPlaying(s32 index) const;

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    bool SFXIsPlaying(const char *name) const;

    // Method: SFXPause
    //      Pauses a specific sound effect
    void SFXPause(s32 index, bool state);

    // Method: SFXPause
    //      Pauses a specific sound effect
    void SFXPause(const char *name, bool state);

    // Method: SFXPauseAll
    //      Pauses all the sound effects
    void SFXPauseAll(bool state);

    // Method: SFXGetPaused
    //      Determines if all the sound effects are paused. This applies for all effects, not individual effects.
    bool SFXGetPaused() const;

    // Method: SFXGetActive
    //      Returns the number of active sound effects
    s32 SFXGetActive() const;

    // Method: SFXSetVolume
    //      Sets the volume of the specified effect.
    void SFXSetVolume(s32 id, f32 volume);

    // Method: SFXSetPosition
    //      Sets the position of a sound effect.
    void SFXSetPosition(const char *name, f32 x, f32 y, f32 z);


private:
    ALCdevice          *device;
    ALCcontext         *context;
    prWaves             waves;
    u32	songCurr;
};


#endif//PLATFORM_LINUX
