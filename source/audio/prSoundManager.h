// File: prSoundManager.h
//      The sound system base controller class. All audio systems derive
//      from this base class and support all functionality
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


#include "prSoundManagerShared.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations
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
    //
    // Notes:
    //      This is handled by the engine. *DO NOT CALL*
    virtual bool Initialise() = 0;

    // Method: Release
    //      Releases all sound system assets.
    //
    // Notes:
    //      This is handled by the engine. *DO NOT CALL*
    virtual void Release() = 0;

    // Method: Update
    //      Updates the sound system.
    //
    // Parameters:
    //      dt - Delta time
    //
    // Notes:
    //      This is handled by the engine. *DO NOT CALL*
    virtual void Update(Proteus::Core::f32 dt) = 0;

    // Method: LoadSongs
    //      Registers a song list, so we can use play by index.
    //
    // Parameters:
    //      filenames - A list of the songs
    //      count     - List size
    void LoadSongs(const char **filenames, Proteus::Core::s32 count);

    // Method: LoadSFX
    //      Loads the sound effects into sound memory.
    //
    // Parameters:
    //      sfc    - A list of the effects
    //      count  - List size
    //
    // See Also:
    //      <prSFXInfo>
    virtual void LoadSFX(const prSFXInfo *sfx, Proteus::Core::s32 count) = 0;

    // Method: SongPlay
    //      Starts playing a song.
    //
    // Parameters:
    //      index - The song to play
    void SongPlay(Proteus::Core::s32 index);

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
    virtual void SongStop(Proteus::Core::f32 time = 0.0f) = 0;

    // Method: SongGetCurrent
    //      Gets the current songs index.
    Proteus::Core::s32 SongGetCurrent() const { return songIndex; }

    // Method: SongIsPlaying
    //      Is a song playing
    bool SongIsPlaying() const { return songPlaying; }

    // Method: SongSetMasterVolume
    //      Sets the master volume for the currently playing song.
    //
    // Parameters:
    //      volume - The volume is a value between 0.0f and 1.0f
    void SongSetMasterVolume(Proteus::Core::f32 volume);

    // Method: SongGetMasterVolume
    //      Gets the master volume for the currently playing song.
    Proteus::Core::f32 SongGetMasterVolume() const { return masterMusVolume; }

    // Method: SongPause
    //      Pauses the active song.
    virtual void SongPause(bool pause) = 0;

    // Method: SongGetPaused
    //      Determines if the current song is paused.
    virtual bool SongGetPaused() const = 0;

    // Method: SongSetVolume
    //      Sets the volume of the current song.
    //
    // Parameters:
    //      volume - The volume is a value between 0.0f and 1.0f
    virtual void SongSetVolume(Proteus::Core::f32 volume) = 0;

    // Method: SFXPlay
    //      Plays a sound effect.
    //
    // Parameters:
    //      index  - The effects index
    //      volume - The volume is a value between 0.0f and 1.0f
    //      loop   - Is this a looping effect?
    //
    // Returns:
    //      Returns an ID for the effect
    virtual Proteus::Core::s32 SFXPlay(Proteus::Core::s32 index, Proteus::Core::f32 volume = 1.0f, bool loop = false) = 0;

    // Method: SFXPlay
    //      Plays a sound effect.
    //
    // Parameters:
    //      name   - The effects name
    //      volume - The volume is a value between 0.0f and 1.0f
    //      loop   - Is this a looping effect?
    //
    // Returns:
    //      Returns an ID for the effect
    Proteus::Core::s32 SFXPlay(const char *name, Proteus::Core::f32 volume = 1.0f, bool loop = false);

    // Method: SFXStop
    //      Stops the specified effect.
    //
    // Parameters:
    //      id   - The effects id
    virtual void SFXStop(Proteus::Core::s32 id) = 0;

    // Method: SFXStop
    //      Stops the specified effect.
    //
    // Parameters:
    //      name   - The effects name
    virtual void SFXStop(const char *name) = 0;

    // Method: SFXStopAll
    //      Stops all the sound effects.
    virtual void SFXStopAll() = 0;

    // Method: SFXSetMasterVolume
    //      Sets the master volume for all the effects.
    //
    // Parameters:
    //      volume - The volume is a value between 0.0f and 1.0f
    void SFXSetMasterVolume(Proteus::Core::f32 volume);

    // Method: SFXGetMasterVolume
    //      Gets the master volume for all the effects.
    Proteus::Core::f32 SFXGetMasterVolume() const { return masterSfxVolume; }

    // Method: SFXSetPosition
    //      Sets the position of a sound effect.
    //
    // Parameters:
    //      name - The effects name
    //      x    - X position
    //      y    - Y position
    //      z    - Z position
    //
    // Notes:
    //      It's required that the sound effect is *MONO*, or openal will not set the position
    //      correctly. Stereo effects come with positional data
    virtual void SFXSetPosition(const char *name, Proteus::Core::f32 x, Proteus::Core::f32 y, Proteus::Core::f32 z) = 0;

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    //
    // Parameters:
    //      id  - The effects id
    virtual bool SFXIsPlaying(Proteus::Core::s32 id) const = 0;

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    //
    // Parameters:
    //      name  - The effects name
    virtual bool SFXIsPlaying(const char *name) const = 0;

    // Method: SFXPause
    //      Pauses/unpauses a specific sound effect
    //
    // Parameters:
    //      id     - The effects id
    //      state  - true or false
    virtual void SFXPause(Proteus::Core::s32 id, bool state) = 0;

    // Method: SFXPause
    //      Pauses/unpauses a specific sound effect
    //
    // Parameters:
    //      name   - The effects name
    //      state  - true or false
    virtual void SFXPause(const char *name, bool state) = 0;

    // Method: SFXPauseAll
    //      Pauses/unpauses all the sound effects
    //
    // Parameters:
    //      state - true or false
    virtual void SFXPauseAll(bool state) = 0;

    // Method: SFXGetPaused
    //      Determines if all the sound effects are paused. This applies for all effects, not individual effects.
    virtual bool SFXGetPaused() const = 0;

    // Method: SFXGetActive
    //      Returns the number of active sound effects
    virtual Proteus::Core::s32 SFXGetActive() const = 0;

    // Method: SFXSetVolume
    //      Sets the volume of the specified effect.
    //
    // Parameters:
    //      id     - The effects id
    //      volume - The volume is a value between 0.0f and 1.0f
    virtual void SFXSetVolume(Proteus::Core::s32 id, Proteus::Core::f32 volume) = 0;

    // Method: DisplayUsage
    //      Displays debug information on the sound player.
    //
    // Notes:
    //      This will vary dependant on the OS
    virtual void DisplayUsage() const {}

    // Method: IsSoundAvailable
    //      Debug function which determines if the sound system is available.
    bool IsSoundAvailable() const;

    // Method: Mute
    //      Mute/unmute all sound
    //
    // Parameters:
    //      state - true or false
    void Mute(bool state);

    // Method: IsMuted
    //      Is sound muted?
    bool IsMuted() const;


protected:
    Proteus::Core::f32  masterMusVolume;
    Proteus::Core::f32  masterSfxVolume;
    Proteus::Core::f32  masterMusVolumeBackup;
    Proteus::Core::f32  masterSfxVolumeBackup;

    Proteus::Core::f32  songVolume;
    Proteus::Core::s32  songIndex;
    Proteus::Core::s32  songState;
    Proteus::Core::f32  songFade;
    Proteus::Core::f32  songTime;

    Proteus::Core::s32  active;
    Proteus::Core::u32  effectId;

    bool                songPlaying;
    bool                initialised;
    bool                sfxPaused;
    bool                mIsMuted;

    prLoadedWave       *pLoadedWaves;
    const char        **pMusicTracks;
    Proteus::Core::s32  numTracks;
    Proteus::Core::s32  numEffects;

    prSoundEffectEntry  soundEffects[AUDIO_MAX_ACTIVE];
};
