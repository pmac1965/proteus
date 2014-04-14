/**
 * prSoundManager_Android.cpp
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


#if defined(PLATFORM_ANDROID)


#include <string.h>
#include "prsoundManager_Android.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../android/prJNIAudio.h"


/// Defines.
#define SOUND_DEBUG


/// ---------------------------------------------------------------------------
/// Construct.
/// ---------------------------------------------------------------------------
prSoundManager_Android::prSoundManager_Android()
{
    songCurr                = 0xFFFFFFFF;
}


/// ---------------------------------------------------------------------------
/// Destruct.
/// ---------------------------------------------------------------------------
prSoundManager_Android::~prSoundManager_Android()
{
}


/// ---------------------------------------------------------------------------
/// Initialises the sound system.
/// ---------------------------------------------------------------------------
bool prSoundManager_Android::Initialise()
{
#ifdef SOUND_ALLOW
    
    // Already initialised?
    if (!initialised)
    {
        // Init effects
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            soundEffects[i].state    = SFX_STATE_FREE;
            soundEffects[i].uiSource = 0xFFFFFFFF;
        }

        initialised = true;
    }

#endif

    return true;
}


/// ---------------------------------------------------------------------------
/// Releases all sound system assets.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::Release()
{
}


/// ---------------------------------------------------------------------------
/// Updates the sound system.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::Update(f32 dt)
{
#ifdef SOUND_ALLOW

    if (initialised)
    {
        active = 0;

        // Free any stopped effects
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].state == SFX_STATE_PLAYING)
            {
                active++;
            }
        }

        // Fade down?
        if (songFade > 0)
        {
            // PRLERP(runTime / emit_time_run, emit_time_max, emit_time_min);
            float v = (PRLERP(songFade, 0.0f, songTime) / songFade); 
            v = (v / songFade);
            songTime -= (dt / 1000.0f);
    
            if (v > 0.0f)
            {
                SongSetVolume(v);
            }
            else
            {
                SongStop();
            }
        }
    }

#else

    PRUNUSED(dt);

#endif
}


// ----------------------------------------------------------------------------
// Loads the effects into sound memory.
// ----------------------------------------------------------------------------
void prSoundManager_Android::LoadSFX(const prSFXInfo *sfx, s32 count)
{
}


/// ---------------------------------------------------------------------------
/// Starts playing a song.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SongPlayByName(const char *filename)
{
#ifdef SOUND_ALLOW

    PRASSERT(filename && *filename); 
    
    // Already playing song?
    s32 hash = prStringHash(filename);
    if (hash == songCurr)
    {
        return;
    }
        
    // Kill old.
    SongStop();

    // Find track index
    PRASSERT(numTracks > 0);
    s32 index = -1;
    for (s32 i=0; i<numTracks; i++)
    {
        if (strcmp(filename, pMusicTracks[i]) == 0)
        {
            index = i;
            break;
        }
    }

    // No track?
    if (index == -1)
    {
        prTrace("Failed to find registered music track %s\n", filename);
        return;
    }    

    // Name to android format.
    char android_filename[256];
    strcpy(android_filename, filename);
    prStringReplaceChar(android_filename, '\\', '/');
    prStringToLower(android_filename);        

    // Check valid?
    s32 i = prStringFindLastIndex(android_filename, '/');
    if (i == -1)
    {
        prTrace("SoundManager: Invalid audio filename.\n");
        return;
    }        

    // Create filename without 'data/'
    strcpy(android_filename, &android_filename[5]);    
    
    // Remove old .ogg extension and add .mp3
    i = prStringFindLastIndex(android_filename, '.');
    if (i == -1)
    {
        prTrace("SoundManager: Invalid audio filename.\n");
        return;
    }            
    android_filename[i] = '\0';
    strcat(android_filename, ".mp3");
    
    // And play
    prJNI_SongPlay(android_filename);
        
    // Set playing
    songPlaying = true;
    songIndex   = index;
    songState   = SONG_STATE_PLAYING;
    songCurr    = hash;
    
    // Set volume.
    SongSetVolume(1.0f);

#else
    
    PRUNUSED(filename);
    
#endif
}


/// ---------------------------------------------------------------------------
/// Stops the current song.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SongStop(f32 time)
{
#ifdef SOUND_ALLOW

    if (songPlaying)
    {
        if (time > 0)
        {
            songFade = PRCLAMP(time, 0.0f, 10.0f);
            songTime = songFade;
            return;
        }

        // Stop the song 
        prJNI_SongStop();

        songPlaying = false;
        songState   = SONG_STATE_FREE;
        songIndex   = -1;
        songFade    = 0.0f;
        songTime    = 0.0f;
        songCurr    = -1;
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Pauses the active song.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SongPause(bool pause)
{
#ifdef SOUND_ALLOW

    if (initialised)
    {
        if (numTracks > 0)
        {
            if (songPlaying)
            {
                if (pause)
                {
                    if (songState == SONG_STATE_PLAYING)
                    {
                        songState = SONG_STATE_PAUSED;
                        prJNI_SongPause();
                    }
                }
                else
                {
                    if (songState == SONG_STATE_PAUSED)
                    {
                        songState = SONG_STATE_PLAYING;
                        prJNI_SongResume();
                    }
                }
            }
        }
    }

#else

    PRUNUSED(pause);

#endif
}


/// ---------------------------------------------------------------------------
/// Determines if the current song is paused.
/// ---------------------------------------------------------------------------
bool prSoundManager_Android::SongGetPaused() const
{
    bool result = false;

#ifdef SOUND_ALLOW

    if (initialised)
    {
        if (numTracks > 0)
        {
            if (songPlaying)
            {
                if (songState == SONG_STATE_PAUSED)
                {
                    result = true;
                }
            }
        }
    }

#endif

    return result;
}


/// ---------------------------------------------------------------------------
/// Sets the volume of the current song.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SongSetVolume(f32 volume)
{
#ifdef SOUND_ALLOW
    if (initialised)
    {
        if (numTracks > 0)
        {
            if (songPlaying)
            {
                if (songState == SONG_STATE_PLAYING)
                {
                    // Set volume
                    float vol = PRCLAMP(volume, AUDIO_MUS_MIN_VOLUME, AUDIO_MUS_MAX_VOLUME);
                    vol *= masterMusVolume;
                    prJNI_SongSetVolume(vol);
                }
                else
                {
                    prTrace("Tried to set the volume of a paused song.\n");
                }
            }
        }
    }

#else

    PRUNUSED(volume);

#endif
}


// ----------------------------------------------------------------------------
// Plays a sound effect.
// ----------------------------------------------------------------------------
s32 prSoundManager_Android::SFXPlay(s32 index, f32 volume, bool loop)
{
    PRUNUSED(loop);
    PRUNUSED(volume);
    PRUNUSED(index);
    return -1;
}


// ----------------------------------------------------------------------------
// Stops the specified effect.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXStop(s32 index)
{
    PRUNUSED(index);
}


// ----------------------------------------------------------------------------
// Stops the specified effect.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXStop(const char *name)
{
    PRUNUSED(name);
}


// ----------------------------------------------------------------------------
// Stops all the sound effects.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXStopAll()
{
}


// ----------------------------------------------------------------------------
// Determines if a particular sound effect is playing.
// ----------------------------------------------------------------------------
bool prSoundManager_Android::SFXIsPlaying(s32 index) const
{
    bool result = false;
    PRUNUSED(index);
    return result;
}
    

// ----------------------------------------------------------------------------
// Determines if a particular sound effect is playing.
// ----------------------------------------------------------------------------
bool prSoundManager_Android::SFXIsPlaying(const char *name) const
{
    bool result = false;
    PRUNUSED(name);
    return result;
}


// ----------------------------------------------------------------------------
// Pauses a specific sound effect
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXPause(s32 index, bool state)
{
    PRUNUSED(index);
    PRUNUSED(state);
}


// ----------------------------------------------------------------------------
// Pauses a specific sound effect
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXPause(const char *name, bool state)
{
    PRUNUSED(name);
    PRUNUSED(state);
}


// ----------------------------------------------------------------------------
// Pauses all the sound effects
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXPauseAll(bool state)
{
    PRUNUSED(state);
}


// ----------------------------------------------------------------------------
// Determines if all the sound effects are paused.
// ----------------------------------------------------------------------------
bool prSoundManager_Android::SFXGetPaused() const
{
    bool result = false;
    return result;
}


// ----------------------------------------------------------------------------
// Returns the number of active sound effects
// ----------------------------------------------------------------------------
s32 prSoundManager_Android::SFXGetActive() const
{
    return 0;
}


// ----------------------------------------------------------------------------
// Sets the volume of the specified effect.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXSetVolume(s32 index, f32 volume)
{
    PRUNUSED(index);
    PRUNUSED(volume);
}


#endif// PLATFORM_ANDROID
