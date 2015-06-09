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
#include "../android/AL/al.h"
#include "../android/AL/alc.h"
#include "prOpenALErrors.h"
#include <android/log.h>


using namespace Proteus::Core;


/// Defines.
#define SOUND_DEBUG


/// ---------------------------------------------------------------------------
/// Construct.
/// ---------------------------------------------------------------------------
prSoundManager_Android::prSoundManager_Android()
{
    songCurr                = 0xFFFFFFFF;
    device                  = NULL;
    context                 = NULL;
}


/// ---------------------------------------------------------------------------
/// Destruct.
/// ---------------------------------------------------------------------------
prSoundManager_Android::~prSoundManager_Android()
{
    PRSAFE_DELETE_ARRAY(pLoadedWaves);
}


/// ---------------------------------------------------------------------------
/// Initialises the sound system.
/// ---------------------------------------------------------------------------
bool prSoundManager_Android::Initialise()
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    // Already initialised?
    if (!initialised)
    {
        // Init effects
        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            soundEffects[i].state    = SFX_STATE_FREE;
            soundEffects[i].uiSource = 0xFFFFFFFF;
        }


	    // Open default OpenAL device
        device = alcOpenDevice(NULL);
        ALC_ErrorCheck(device);
        if (device)
        {
            // Create a Context
            context = alcCreateContext(device, NULL);
            ALC_ErrorCheck(device);
            if (context)
            {
                // Make the context current (active)
                alcMakeContextCurrent(context);
                if (ALC_ErrorCheck(device) != ALC_NO_ERROR)
                {
                    __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to make OpenAL context current\n");
                }

                // Generate the playback sources
                for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
                {
                    // Generate a source to playback a buffer
                    alGenSources(1, &soundEffects[i].uiSource);

                    if (AL_ErrorCheck() != AL_NO_ERROR)
                    {
                        soundEffects[i].state    = SFX_STATE_UNAVAILABLE;
                        soundEffects[i].uiSource = 0xFFFFFFFF;
                        __android_log_print(ANDROID_LOG_ERROR, "Proteus", "OpenAL failed to generate a source\n");
                    }
                    else
                    {
                        soundEffects[i].state    = SFX_STATE_FREE;
                    }
                }
            }
            else
            {
                __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to create OpenAL context\n");
                alcCloseDevice(device);
                context = NULL;
                device  = NULL;
            }
        }
        else
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to open default OpenAL device\n");
        }

        initialised = true;
    }
    else
    {
        prTrace(LogError, "The sound system is already initialised\n");
    }


    return initialised;

#else

    return true;

#endif
}


/// ---------------------------------------------------------------------------
/// Releases all sound system assets.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::Release()
{
#if defined(SOUND_ALLOW) && defined(USE_OPENAL)

    ALCcontext *pContext = alcGetCurrentContext();
    ALCdevice  *pDevice  = alcGetContextsDevice(pContext);

    if (pContext)
    {
        __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Cleaning OpenAL\n");

        // Release the current context.
        if (!alcMakeContextCurrent(NULL))
        {
            PRWARN("OpenAL shutdown error");
        }
		ALC_ErrorCheck(pDevice);

        // Destroy context
        alcDestroyContext(pContext);
        ALC_ErrorCheck(pDevice);

        // Destroy device.
        if (pDevice)
        {
            if (alcGetError(pDevice) != ALC_NO_ERROR)
            {
                PRWARN("OpenAL shutdown error");
            }

            // Destroy device
            if (!alcCloseDevice(pDevice))
            {
                PRWARN("OpenAL shutdown error");
            }
        }
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Updates the sound system.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::Update(f32 dt)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    if (initialised)
    {
        active = 0;

        // Free any stopped effects
        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].state == SFX_STATE_PLAYING)
            {
                active++;

                ALint state;            
                alGetSourcei(soundEffects[i].uiSource, AL_SOURCE_STATE, &state);
                AL_ERROR_CHECK()
            
                // Set free?
                if (state != AL_PLAYING && state != AL_PAUSED)
                {
                    soundEffects[i].state = SFX_STATE_FREE;
                    active--;
                }
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


/// ---------------------------------------------------------------------------
/// Loads the effects into sound memory.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::LoadSFX(const prSFXInfo *sfx, s32 count)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    PRASSERT(count > 0);    
    PRASSERT(count <= AUDIO_MAX_EFFECTS);
    PRASSERT(sfx);

    // Called twice?
    if (pLoadedWaves)
    {
        prTrace(LogError, "Attempted to load sfx twice\n");
        return;
    }    

    if (PRBETWEEN(count, 1, AUDIO_MAX_EFFECTS) && sfx)
    {
        pLoadedWaves = new prLoadedWave[count];
        numEffects   = count;

        for (s32 i=0; i<count; i++)
        {
            prWaveID id = -1;
            waves.LoadWaveFile(sfx[i].filename, &id);
            //__android_log_print(ANDROID_LOG_ERROR, "Proteus", "Loading %s\n", sfx[i].filename);

            if (id != -1)
            {
                prLoadedWave *entry = &pLoadedWaves[i];
                ALchar       *pData;
                ALint         iDataSize;
                ALint         iFrequency;
                ALenum        eBufferFormat;

                if (waves.GetWaveSize(id, (u32*)&iDataSize)                 == WR_OK &&
	                waves.GetWaveData(id, (void**)&pData)                   == WR_OK &&
	                waves.GetWaveFrequency(id, (u32*)&iFrequency)           == WR_OK &&
	                waves.GetWaveALBufferFormat(id, (u32*)&eBufferFormat)   == WR_OK
                   )
                {
                    // Generate a buffer
                	alGenBuffers(1, &entry->uiBuffer);
                    AL_ERROR_CHECK()

	                // Store data.
				    alBufferData(entry->uiBuffer, eBufferFormat, pData, iDataSize, iFrequency);
                    AL_ERROR_CHECK()

                    // Delete file as we no longer require it.
                    waves.DeleteWaveFile(id);

                    // Set the hash
                    entry->hash = sfx[i].hash;

                    //__android_log_print(ANDROID_LOG_ERROR, "Proteus", "Loaded %s\n", sfx[i].filename);
                }
                else
                {
                    __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Sound effect: %s\n", sfx[i].filename);
                    __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to create effect\n");

                    #if defined(SOUND_SHOW_FAILS) && (defined(_DEBUG) || defined(DEBUG))
                    __android_log_print(ANDROID_LOG_ERROR, "Proteus", "GetWaveSize           == %i\n", waves.GetWaveSize(id, (u32*)&iDataSize)                 == WR_OK);
	                __android_log_print(ANDROID_LOG_ERROR, "Proteus", "GetWaveData           == %i\n", waves.GetWaveData(id, (void**)&pData)                   == WR_OK);
	                __android_log_print(ANDROID_LOG_ERROR, "Proteus", "GetWaveFrequency      == %i\n", waves.GetWaveFrequency(id, (u32*)&iFrequency)           == WR_OK);
	                __android_log_print(ANDROID_LOG_ERROR, "Proteus", "GetWaveALBufferFormat == %i\n", waves.GetWaveALBufferFormat(id, (u32*)&eBufferFormat)   == WR_OK);
                    #endif
                }
            }
            else
            {
                __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to load %s\n", sfx[i].filename);
                __android_log_print(ANDROID_LOG_ERROR, "Proteus", "If the file is not missing, then check .wav format. Should be PCM\n");
            }
        }
    }

#else

    PRUNUSED(count);
    PRUNUSED(sfx);

#endif
}


/// ---------------------------------------------------------------------------
/// Starts playing a song.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SongPlayByName(const char *filename)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

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
        prTrace(LogError, "Failed to find registered music track %s\n", filename);
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
        prTrace(LogError, "SoundManager: Invalid audio filename.\n");
        return;
    }        

    // Create filename without 'data/'
    strcpy(android_filename, &android_filename[5]);    
    
    // Remove old .ogg extension and add .mp3
    i = prStringFindLastIndex(android_filename, '.');
    if (i == -1)
    {
        prTrace(LogError, "SoundManager: Invalid audio filename.\n");
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
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

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
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

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

#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

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
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

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
                    prTrace(LogError, "Tried to set the volume of a paused song.\n");
                }
            }
        }
    }

#else

    PRUNUSED(volume);

#endif
}


/// ---------------------------------------------------------------------------
/// Plays a sound effect.
/// ---------------------------------------------------------------------------
s32 prSoundManager_Android::SFXPlay(s32 index, f32 volume, bool loop)
{
    s32 handle = -1;

#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    if (initialised)
    {
        if (numEffects > 0)
        {
            PRASSERT(index >= 0);
            PRASSERT(index < numEffects);

            for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
            {
                if (soundEffects[i].state == SFX_STATE_FREE)
                {
                    prLoadedWave *entry = &pLoadedWaves[index];

                    // Attach Source to Buffer
                    alSourcei(soundEffects[i].uiSource, AL_BUFFER, entry->uiBuffer);
                    AL_ERROR_CHECK()

					// Set volume
                    float vol = PRCLAMP(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
                    vol *= masterSfxVolume;
                    alSourcef(soundEffects[i].uiSource, AL_GAIN, vol);
                    AL_ERROR_CHECK()

                    // Set looping
                    alSourcei(soundEffects[i].uiSource, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
                    AL_ERROR_CHECK()

                    // Play Source
                    alSourcePlay(soundEffects[i].uiSource);
                    AL_ERROR_CHECK()


                    // Set states
                    soundEffects[i].state = SFX_STATE_PLAYING;
                    soundEffects[i].hash  = entry->hash;
                    soundEffects[i].id    = effectId++;

                    // Set return value
                    handle = soundEffects[i].id;
                    break;
                }
            }
        }
    }

#else

    PRUNUSED(loop);
    PRUNUSED(volume);
    PRUNUSED(index);

#endif

    return handle;
}


/// ---------------------------------------------------------------------------
/// Stops the specified effect.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SFXStop(s32 index)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))
    
    if (initialised)
    {
        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)index)
            {
                if (soundEffects[i].state == SFX_STATE_PLAYING)
                {
                    soundEffects[i].state = SFX_STATE_FREE;
                    soundEffects[i].id    = 0;
                    alSourceStop(soundEffects[i].uiSource);
                    AL_ERROR_CHECK()
                }

                break;
            }
        }
    }

#else

    PRUNUSED(index);

#endif
}


/// ---------------------------------------------------------------------------
/// Stops the specified effect.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SFXStop(const char *name)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    if (initialised && name && *name)
    {
        u32 hash = prStringHash(name);

        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].hash == hash)
            {
                if (soundEffects[i].state == SFX_STATE_PLAYING)
                {
                    soundEffects[i].state = SFX_STATE_FREE;
                    alSourceStop(soundEffects[i].uiSource);
                    AL_ERROR_CHECK()
                    break;
                }
            }
        }
    }

#else

    PRUNUSED(name);

#endif
}


/// ---------------------------------------------------------------------------
/// Stops all the sound effects.
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SFXStopAll()
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    if (initialised)
    {
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].state == SFX_STATE_PLAYING ||
                soundEffects[i].state == SFX_STATE_PAUSED)
            {
                soundEffects[i].state = SFX_STATE_FREE;
                alSourceStop(soundEffects[i].uiSource);
                AL_ERROR_CHECK()
            }
        }

        sfxPaused = false;
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Determines if a particular sound effect is playing.
/// ---------------------------------------------------------------------------
bool prSoundManager_Android::SFXIsPlaying(s32 index) const
{
    bool result = false;

#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))
    
    if (initialised)
    {
        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].state == SFX_STATE_PLAYING)
            {
                if (soundEffects[i].id == (u32)index)
                {
                    result = true;
                    break;
                }
            }
        }
    }

#else

    PRUNUSED(index);

#endif

    return result;
}
    

/// ---------------------------------------------------------------------------
/// Determines if a particular sound effect is playing.
/// ---------------------------------------------------------------------------
bool prSoundManager_Android::SFXIsPlaying(const char *name) const
{
    bool result = false;

#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    PRASSERT(name && *name);

    if (initialised && name && *name)
    {
        u32 hash = prStringHash(name);

        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].state == SFX_STATE_PLAYING)
            {
                if (soundEffects[i].hash == hash)
                {
                    result = true;
                    break;
                }
            }
        }
    }

#else

    PRUNUSED(name);

#endif

    return result;
}


/// ---------------------------------------------------------------------------
/// Pauses a specific sound effect
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SFXPause(s32 index, bool state)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    if (initialised)
    {
        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)index)
            {
                if (state)
                {
                    if (soundEffects[i].state == SFX_STATE_PLAYING)
                    {
                        soundEffects[i].state = SFX_STATE_PAUSED;
                        alSourcePause(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                        break;
                    }
                }
                else
                {
                    if (soundEffects[i].state == SFX_STATE_PAUSED)
                    {
                        soundEffects[i].state = SFX_STATE_PLAYING;
                        alSourcePlay(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                        break;
                    }
                }
            }
        }
    }

#else

    PRUNUSED(index);
    PRUNUSED(state);

#endif
}


/// ---------------------------------------------------------------------------
/// Pauses a specific sound effect
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SFXPause(const char *name, bool state)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    if (initialised && name && *name)
    {
        u32 hash = prStringHash(name);

        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].hash == hash)
            {
                if (state)
                {
                    if (soundEffects[i].state == SFX_STATE_PLAYING)
                    {
                        soundEffects[i].state = SFX_STATE_PAUSED;
                        alSourcePause(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                        break;
                    }
                }
                else
                {
                    if (soundEffects[i].state == SFX_STATE_PAUSED)
                    {
                        soundEffects[i].state = SFX_STATE_PLAYING;
                        alSourcePlay(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                        break;
                    }
                }
            }
        }
    }

#else

    PRUNUSED(name);
    PRUNUSED(state);

#endif
}


// ----------------------------------------------------------------------------
// Pauses all the sound effects
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXPauseAll(bool state)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))
    
    if (initialised)
    {
        if (sfxPaused != state)
        {
            sfxPaused = state;

            if (state)
            {
                for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
                {
                    if (soundEffects[i].state == SFX_STATE_PLAYING)
                    {
                        soundEffects[i].state = SFX_STATE_PAUSED;
                        alSourcePause(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                    }
                }
            }
            else
            {
                for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
                {
                    if (soundEffects[i].state == SFX_STATE_PAUSED)
                    {
                        soundEffects[i].state = SFX_STATE_PLAYING;
                        alSourcePlay(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                    }
                }
            }
        }
    }

#else

    PRUNUSED(state);

#endif
}


// ----------------------------------------------------------------------------
// Determines if all the sound effects are paused.
// ----------------------------------------------------------------------------
bool prSoundManager_Android::SFXGetPaused() const
{
    bool result = false;

#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))
    
    if (initialised)
    {
        result = sfxPaused;
    }

#endif

    return result;
}


// ----------------------------------------------------------------------------
// Returns the number of active sound effects
// ----------------------------------------------------------------------------
s32 prSoundManager_Android::SFXGetActive() const
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))
    return active;
#else
    return 0;
#endif
}


// ----------------------------------------------------------------------------
// Sets the volume of the specified effect.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SFXSetVolume(s32 index, f32 volume)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))
    
    if (initialised)
    {
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)index)
            {
                if (soundEffects[i].state == SFX_STATE_PLAYING)
                {
                    // Set volume
                    float vol = PRCLAMP(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
                    vol *= masterSfxVolume;
                    alSourcef(soundEffects[i].uiSource, AL_GAIN, vol);
                    AL_ERROR_CHECK()
                    break;
                }
            }
        }
    }

#else

    PRUNUSED(index);
    PRUNUSED(volume);

#endif
}


/// ---------------------------------------------------------------------------
/// It's best if a sound effect is mono, or openal will not set the position
/// correctly
/// ---------------------------------------------------------------------------
void prSoundManager_Android::SFXSetPosition(const char *name, f32 x, f32 y, f32 z)
{
#if (defined(SOUND_ALLOW) && defined(USE_OPENAL))

    PRASSERT(name && *name);

    if (initialised && name && *name)
    {
        u32 hash = prStringHash(name);

        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].hash == hash)
            {
//                if (soundEffects[i].state == SFX_STATE_PLAYING)
                {
                    alSource3f(soundEffects[i].uiSource, AL_POSITION, x, y, z);
                    AL_ERROR_CHECK()
                }
                break;
            }
        }
    }

#else

    PRUNUSED(name);
    PRUNUSED(x);
    PRUNUSED(y);
    PRUNUSED(z);

#endif
}


#endif// PLATFORM_ANDROID
