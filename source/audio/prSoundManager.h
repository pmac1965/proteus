// File: prSoundManager.h
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


#ifndef __PRSOUNDMANAGER_H
#define __PRSOUNDMANAGER_H


#include "prSoundManagerShared.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations
//struct prSFXInfo;
class prLoadedWave;


// Class: prSoundManager
//      Sound system controller base class.
class prSoundManager : public prCoreSystem
{
public:
    // Method: prSoundManager
    //      Ctor
    prSoundManager();

    // Method: ~prSoundManager
    //      Dtor
    virtual ~prSoundManager();

    // Method: Initialise
    //      Initialises the sound system.
    virtual bool Initialise() = 0;

    // Method: Release
    //      Releases all sound system assets.
    virtual void Release() = 0;

    // Method: Update
    //      Updates the sound system.
    //
    // Parameters:
    //      dt - Delta time
    //
    // Notes:
    //      This is handled by the engine. *Do_not_call*
    virtual void Update(f32 dt) = 0;

    // Method: LoadSongs
    //      Registers a song list, so we can use play by index.
    //
    // Parameters:
    //      filenames - A list of the songs
    //      count     - List size
    void LoadSongs(const char **filenames, s32 count);

    // Method: LoadSFX
    //      Loads the sound effects into sound memory.
    //
    // Parameters:
    //      sfc    - A list of the effects
    //      count  - List size
    //
    // See Also:
    //      <prSFXInfo>
    virtual void LoadSFX(const prSFXInfo *sfx, s32 count) = 0;

    // Method: SongPlay
    //      Starts playing a song.
    //
    // Parameters:
    //      index - The song to play
    void SongPlay(s32 index);

    // Method: SongPlayByName
    //      Starts playing a song.
    //
    // Parameters:
    //      filename - The song to play
    virtual void SongPlayByName(const char *filename) = 0;

    // Method: SongStop
    //      Stops the current song.
    //
    // Parameters:
    //      time - The fade time.
    virtual void SongStop(f32 time = 0.0f) = 0;

    // Method: SongGetCurrent
    //      Gets the current songs index.
    s32 SongGetCurrent() const { return songIndex; }

    // Method: SongIsPlaying
    //      Is a song playing
    bool SongIsPlaying() const { return songPlaying; }

    // Method: SongSetMasterVolume
    //      Sets the master volume for the currently playing song.
    void SongSetMasterVolume(f32 volume);

    // Method: SongGetMasterVolume
    //      Gets the master volume for the currently playing song.
    f32 SongGetMasterVolume() const { return masterMusVolume; }

    // Method: SongPause
    //      Pauses the active song.
    virtual void SongPause(bool pause) = 0;

    // Method: SongPause
    //      Determines if the current song is paused.
    virtual bool SongGetPaused() const = 0;

    // Method: SongSetVolume
    //      Sets the volume of the current song.
    virtual void SongSetVolume(f32 volume) = 0;

    // Method: SFXPlay
    //      Plays a sound effect.
    virtual s32 SFXPlay(s32 index, f32 volume = 1.0f, bool loop = false) = 0;

    // Method: SFXPlay
    //      Plays a sound effect.
    s32 SFXPlay(const char *name, f32 volume = 1.0f, bool loop = false);

    // Method: SFXStop
    //      Stops the specified effect.
    virtual void SFXStop(s32 id) = 0;

    // Method: SFXStop
    //      Stops the specified effect.
    virtual void SFXStop(const char *name) = 0;

    // Method: SFXStopAll
    //      Stops all the sound effects.
    virtual void SFXStopAll() = 0;

    // Method: SFXSetMasterVolume
    //      Sets the master volume for all the effects.
    void SFXSetMasterVolume(f32 volume);

    // Method: SFXGetMasterVolume
    //      Gets the master volume for all the effects.
    f32 SFXGetMasterVolume() const { return masterSfxVolume; }

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    virtual bool SFXIsPlaying(s32 id) const = 0;

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    virtual bool SFXIsPlaying(const char *name) const = 0;

    // Method: SFXPause
    //      Pauses a specific sound effect
    virtual void SFXPause(s32 id, bool state) = 0;

    // Method: SFXPause
    //      Pauses a specific sound effect
    virtual void SFXPause(const char *name, bool state) = 0;

    // Method: SFXPause
    //      Pauses all the sound effects
    virtual void SFXPauseAll(bool state) = 0;

    // Method: SFXPause
    //      Determines if all the sound effects are paused. This applies for all effects, not individual effects.
    virtual bool SFXGetPaused() const = 0;

    // Method: SFXGetActive
    //      Returns the number of active sound effects
    virtual s32 SFXGetActive() const = 0;

    // Method: SFXSetVolume
    //      Sets the volume of the specified effect.
    virtual void SFXSetVolume(s32 id, f32 volume) = 0;

    // Method: DisplayUsage
    //      Displays debug information on the sound player.
    void DisplayUsage() const;

    // Method: IsSoundAvailable
    //      Debug function which determines if the sound system is available.
    bool IsSoundAvailable() const;

    // Method: Mute
    //      Mute all sound
    void Mute(bool state);

    // Method: IsMuted
    //      Is sound muted?
    bool IsMuted() const;


protected:
    f32                 masterMusVolume;
    f32                 masterSfxVolume;

    f32                 songVolume;
    s32                 songIndex;
    s32                 songState;
    f32                 songFade;
    f32                 songTime;

    s32                 active;
    u32                 effectId;

    bool                songPlaying;
    bool                initialised;
    bool                sfxPaused;
    bool                exp2;

    prLoadedWave       *pLoadedWaves;
    const char        **pMusicTracks;
    s32                 numTracks;
    s32                 numEffects;

    prSoundEffectEntry  soundEffects[AUDIO_MAX_ACTIVE];
};


#endif//__PRSOUNDMANAGER_H