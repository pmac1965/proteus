// File: prSoundManager_PC.h
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


#ifndef __PRSOUNDMANAGER_PC_H
#define __PRSOUNDMANAGER_PC_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <Windows.h>
#include <al.h>
#include <alc.h>
#include "prSoundManager.h"
#include "prWaves.h"
#include "external/vorbisfile.h"


// typedefs
typedef long            (*LPOVREAD)         (OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
typedef int             (*LPOVOPENCALLBACKS)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);
typedef vorbis_info    *(*LPOVINFO)         (OggVorbis_File *vf,int link);
typedef int             (*LPOVCLEAR)        (OggVorbis_File *vf);


// Class: prSoundManager_PC
//      Sound system controller for the PC.
class prSoundManager_PC : public prSoundManager
{
public:
    // Method: prSoundManager_PC
    //      Ctor
    prSoundManager_PC();

    // Method: ~prSoundManager_PC
    //      Dtor
    ~prSoundManager_PC();

    // Method: Initialise
    //      Initialises the sound system.
    bool Initialise();

    // Method: Release
    //      Releases all sound system assets.
    void Release();

    // Method: Update
    //      Updates the sound system.
    //
    // Parameters:
    //      dt - Delta time
    //
    // Notes:
    //      This is handled by the engine. *Do_not_call*
    void Update(f32 dt);

    // Method: LoadSongs
    //      Registers a song list, so we can use play by index.
    //
    // Parameters:
    //      filenames - A list of the songs
    //      count     - List size
    //void LoadSongs(const char **filenames, s32 count);

    // Method: LoadSFX
    //      Loads the sound effects into sound memory.
    //
    // Parameters:
    //      sfc    - A list of the effects
    //      count  - List size
    //
    // See Also:
    //      <prSFXInfo>
    void LoadSFX(const prSFXInfo *sfx, s32 count);

    // Method: SongPlay
    //      Starts playing a song.
    //
    // Parameters:
    //      index - The song to play
    //void SongPlay(s32 index);

    // Method: SongPlayByName
    //      Starts playing a song.
    //
    // Parameters:
    //      filename - The song to play
    void SongPlayByName(const char *filename);

    // Method: SongStop
    //      Stops the current song.
    //
    // Parameters:
    //      time - The fade time.
    /*void SongStop(f32 time = 0.0f);

    // Method: SongGetCurrent
    //      Gets the current songs index.
    s32 SongGetCurrent() const;

    // Method: SongIsPlaying
    //      Is a song playing
    bool SongIsPlaying() const;

    // Method: SongSetMasterVolume
    //      Sets the master volume for the currently playing song.
    void SongSetMasterVolume(f32 volume);

    // Method: SongGetMasterVolume
    //      Gets the master volume for the currently playing song.
    f32 SongGetMasterVolume() const;

    // Method: SongPause
    //      Pauses the active song.
    void SongPause(bool pause);

    // Method: SongPause
    //      Determines if the current song is paused.
    bool SongGetPaused() const;//*/

    // Method: SongSetVolume
    //      Sets the volume of the current song.
    void SongSetVolume(f32 volume);

    // Method: SFXPlay
    //      Plays a sound effect.
    s32 SFXPlay(s32 index, f32 volume = 1.0f, bool loop = false);

    // Method: SFXPlay
    //      Plays a sound effect.
    //s32 SFXPlay(const char *name, f32 volume = 1.0f, bool loop = false);

    // Method: SFXStop
    //      Stops the specified effect.
    void SFXStop(s32 index);

    // Method: SFXStop
    //      Stops the specified effect.
    void SFXStop(const char *name);

    // Method: SFXStopAll
    //      Stops all the sound effects.
    void SFXStopAll();

    // Method: SFXSetMasterVolume
    //      Sets the master volume for all the effects.
/*    void SFXSetMasterVolume(f32 volume);

    // Method: SFXGetMasterVolume
    //      Gets the master volume for all the effects.
    f32 SFXGetMasterVolume() const;

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

    // Method: SFXPause
    //      Pauses all the sound effects
    void SFXPauseAll(bool state);

    // Method: SFXPause
    //      Determines if all the sound effects are paused. This applies for all effects, not individual effects.
    bool SFXGetPaused() const;

    // Method: SFXGetActive
    //      Returns the number of active sound effects
    s32 SFXGetActive() const;

    // Method: SFXSetVolume
    //      Sets the volume of the specified effect.
    void SFXSetVolume(s32 index, f32 volume);

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
    bool IsMuted() const;//*/


private:
    ALCdevice          *device;
    ALCcontext         *context;
    HINSTANCE           dll;
    LPOVOPENCALLBACKS   ov_open_callbacks;
    LPOVINFO            ov_info;
    LPOVCLEAR           ov_clear;
    LPOVREAD            ov_read;
    prWaves             waves;
};


#endif//PLATFORM_PC


#endif//__PRSOUNDMANAGER_PC_H
