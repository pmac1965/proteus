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


#pragma once


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
    bool Initialise() override;

    // Method: Release
    //      Releases all sound system assets.
    void Release() override;

    // Method: Update
    //      Updates the sound system.
    void Update(f32 dt) override;

    // Method: LoadSFX
    //      Loads the sound effects into sound memory.
    void LoadSFX(const prSFXInfo *sfx, s32 count) override;

    // Method: SongPlayByName
    //      Starts playing a song.
    void SongPlayByName(const char *filename) override;

    // Method: SongStop
    //      Stops the current song.
    void SongStop(f32 time = 0.0f) override;

    // Method: SongPause
    //      Pauses the active song.
    void SongPause(bool pause) override;

    // Method: SongGetPaused
    //      Determines if the current song is paused.
    bool SongGetPaused() const override;

    // Method: SongSetVolume
    //      Sets the volume of the current song.
    void SongSetVolume(f32 volume) override;

    // Method: SFXPlay
    //      Plays a sound effect.
    s32 SFXPlay(s32 index, f32 volume = 1.0f, bool loop = false) override;

    // Method: SFXStop
    //      Stops the specified effect.
    void SFXStop(s32 id) override;

    // Method: SFXStop
    //      Stops the specified effect.
    void SFXStop(const char *name) override;

    // Method: SFXStopAll
    //      Stops all the sound effects.
    void SFXStopAll() override;

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    bool SFXIsPlaying(s32 index) const override;

    // Method: SFXIsPlaying
    //      Determines if a particular sound effect is playing.
    bool SFXIsPlaying(const char *name) const override;

    // Method: SFXPause
    //      Pauses a specific sound effect
    void SFXPause(s32 index, bool state) override;

    // Method: SFXPause
    //      Pauses a specific sound effect
    void SFXPause(const char *name, bool state) override;

    // Method: SFXPauseAll
    //      Pauses all the sound effects
    void SFXPauseAll(bool state) override;

    // Method: SFXGetPaused
    //      Determines if all the sound effects are paused. This applies for all effects, not individual effects.
    bool SFXGetPaused() const override;

    // Method: SFXGetActive
    //      Returns the number of active sound effects
    s32 SFXGetActive() const override;

    // Method: SFXSetVolume
    //      Sets the volume of the specified effect.
    void SFXSetVolume(s32 id, f32 volume) override;

    // Method: SFXSetPosition
    //      Sets the position of a sound effect.
    void SFXSetPosition(const char *name, f32 x, f32 y, f32 z) override;

    // Method: DisplayUsage
    //      Displays debug information on the sound player.
    void DisplayUsage() const override;


private:
    u8                 *pPCMBuffer;
    ALCdevice          *device;
    ALCcontext         *context;
    HINSTANCE           dll;
    LPOVOPENCALLBACKS   ov_open_callbacks;
    LPOVINFO            ov_info;
    LPOVCLEAR           ov_clear;
    LPOVREAD            ov_read;
    prWaves             waves;

    FILE               *pOggFile;
    OggVorbis_File      oggStream;
    ALuint              frequency;
    ALuint              channels;
    ALuint              format;
    ALuint              songBuffers[2];
    ALuint              songSource;


private:    
    // Method: SongUpdate
    //      Updates the current song
    bool SongUpdate();

    // Method: SongStream
    //      Streams the song data
    bool SongStream(unsigned int buffer);

    // Method: Empty
    //      Empties the songs unused buffers
    void Empty();
};


#endif//PLATFORM_PC
