/**
 * prSoundManager_Ios.cpp
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


// Iphone
#if defined(PLATFORM_IOS)


#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#include <AudioToolbox/AudioServices.h>
#include "prSoundManager_Ios.h"
#include "prWaves.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"
#include "../core/prCoreSystem.h"
#include "../core/prStringUtil.h"
#include "../ios/prIosAudio.h"
#include "../ios/prIos.h"
#include "prOpenALErrors.h"


/// Defines.
#define SOUND_DEBUG


/// ---------------------------------------------------------------------------
/// Construct.
/// ---------------------------------------------------------------------------
prSoundManager_Ios::prSoundManager_Ios()
{
    /*masterMusVolume         = AUDIO_MUS_MAX_VOLUME;
    masterSfxVolume         = AUDIO_SFX_MAX_VOLUME;
    songVolume              = AUDIO_MUS_INITIAL_VOL;
    songIndex               = -1;
    songState               = SONG_STATE_FREE;
    songFade                = 0.0f;
    songTime                = 0.0f;
    songPlaying             = false;
    initialised             = false;
    sfxPaused               = false;
    exp2                    = false;
    pLoadedWaves            = NULL;
    pMusicTracks            = NULL;
    numTracks               = 0;
    numEffects              = 0;
    active                  = 0;
    effectId                = 1;//*/
    songCurr                = -1;
    device                  = NULL;
    context                 = NULL;

    // Set default states
    /*for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
    {
        soundEffects[i].state    = SFX_STATE_UNAVAILABLE;
        soundEffects[i].uiSource = 0xFFFFFFFF;
    }

    TODO("Add audio loop control")
    TODO("Add silent mode")*/
}


/// ---------------------------------------------------------------------------
/// Destruct.
/// ---------------------------------------------------------------------------
prSoundManager_Ios::~prSoundManager_Ios()
{
    PRSAFE_DELETE_ARRAY(pLoadedWaves);
}


/// ---------------------------------------------------------------------------
/// Initialises the sound system.
/// ---------------------------------------------------------------------------
bool prSoundManager_Ios::Initialise()
{
#ifdef SOUND_ALLOW
    
    // Already initialised?
    if (initialised)
    {
        PRWARN("The sound system is already initialised.");
        return true;
    }

    
    bool result = false;
    
    
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
                prTrace("Failed to make OpenAL context current\n");
            }

            result = true;

            // Generate the playback sources
            for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
            {
                // Generate a source to playback a buffer
                alGenSources(1, &soundEffects[i].uiSource);

                if (AL_ErrorCheck() != AL_NO_ERROR)
                {
                    soundEffects[i].state    = SFX_STATE_UNAVAILABLE;
                    soundEffects[i].uiSource = 0xFFFFFFFF;
                }
                else
                {
                    soundEffects[i].state    = SFX_STATE_FREE;
                }
            }
        }
        else
        {
            prTrace("Failed to create OpenAL context\n");
            alcCloseDevice(device);
            context = NULL;
            device  = NULL;
        }
    }
    else
    {
        prTrace("Failed to open default OpenAL device\n");
    }

    initialised = result;

    return result;

#else
    
    return true;

#endif
}


/// ---------------------------------------------------------------------------
/// Releases all sound system assets.
/// ---------------------------------------------------------------------------
void prSoundManager_Ios::Release()
{
#ifdef SOUND_ALLOW

    ALCcontext *pContext = alcGetCurrentContext();
    ALCdevice  *pDevice  = alcGetContextsDevice(pContext);

    if (pContext)
    {
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
void prSoundManager_Ios::Update(f32 dt)
{
#ifdef SOUND_ALLOW

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
            f32 v = (PRLERP(songFade, 0.0f, songTime) / songFade); 
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

#endif
}


/// ---------------------------------------------------------------------------
/// Loads the effects into sound memory.
/// ---------------------------------------------------------------------------
void prSoundManager_Ios::LoadSFX(const prSFXInfo *sfx, s32 count)
{
#ifdef SOUND_ALLOW

    PRASSERT(count > 0);    
    PRASSERT(count <= AUDIO_MAX_EFFECTS);
    PRASSERT(sfx);
  
    
    // Called twice?
    if (pLoadedWaves)
    {
        prTrace("Attempted to load sfx twice\n");
        return;
    }
    

    if (PRBETWEEN(count, 1, AUDIO_MAX_EFFECTS) && sfx)
    {
        pLoadedWaves = new prLoadedWave[count];
        numEffects   = count;

        for (s32 i=0; i<count; i++)
        {
            ALsizei iDataSize;
            ALsizei iFrequency;
            ALenum  eBufferFormat;

            void *pAudioData = prLoadAudioFile(sfx[i].filename, &iDataSize, &iFrequency, &eBufferFormat);
            if (pAudioData)
            {
                prLoadedWave  *entry = &pLoadedWaves[i];

                // Generate a buffer
                alGenBuffers(1, &entry->uiBuffer);
                AL_ERROR_CHECK()

	            // Store data.
				alBufferData(entry->uiBuffer, eBufferFormat, pAudioData, iDataSize, iFrequency);
                AL_ERROR_CHECK()

                // Set the hash
                entry->hash = sfx[i].hash;
            }
            else
            {
                prTrace("Failed to load %s\n", sfx[i].filename);
                prTrace("If the file is not missing, then check .wav format. Should be PCM\n");
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
void prSoundManager_Ios::SongPlayByName(const char *filename)
{
#ifdef SOUND_ALLOW

    PRASSERT(filename && *filename); 
    
    
    // Already playing song?
    s32 hash = prStringHash(filename);
    if (hash == songCurr)
    {
        prTrace("No replay song\n");
        return;
    }
    
    
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

    if (index == -1)
    {
        prTrace("Failed to find registered music track %s\n", filename);
        return;
    }
    

    // Reset
    songPlaying = false;
    songIndex   = -1;
    songState   = SONG_STATE_FREE;
    songFade    = 0.0f;
    songTime    = 0.0f;


    // First, remove path if present.
    char ios_filename[256];
    
    strcpy(ios_filename, filename);
    prStringReplaceChar(ios_filename, '\\', '/');
    prStringToLower(ios_filename);
    s32 idx = prStringFindLastIndex(ios_filename, '/');        
    if (idx == -1)
    {
        idx = 0;
    }
    else
    {
        idx++;
    }
    
    
    // Remove old .ogg extension.
    s32 i = prStringFindLastIndex(ios_filename, '.');
    if (i == -1)
    {
        prTrace("prSoundManager_Ios: Invalid audio filename.\n");
        return;
    }        
    ios_filename[i] = '\0';        
    
    
    // Set playing
    songPlaying = true;
    songIndex   = index;
    songState   = SONG_STATE_PLAYING;
    songCurr    = hash;
    
    prIosSongPlay(&ios_filename[idx]);
    SongSetVolume(1.0f);
    prTrace("IOS: Playing song: %s\n", &ios_filename[idx]);

#else
    
    PRUNUSED(filename);
    
#endif
}


/// ---------------------------------------------------------------------------
/// Stops the current song.
/// ---------------------------------------------------------------------------
void prSoundManager_Ios::SongStop(f32 time)
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

        prIosSongStop();

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
void prSoundManager_Ios::SongPause(bool pause)
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
                        prIosSongPause(pause);
                        
                        // Remove context
                        prTrace("Remove context\n");
                        alcMakeContextCurrent(NULL);
                    }
                }
                else
                {
                    if (songState == SONG_STATE_PAUSED)
                    {
                        songState = SONG_STATE_PLAYING;
                        prIosSongPause(pause);
                        
                        // Restore context
                        prTrace("Restore context\n");
                        alcMakeContextCurrent(context);
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
bool prSoundManager_Ios::SongGetPaused() const
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
void prSoundManager_Ios::SongSetVolume(f32 volume)
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
                    f32 vol = PRCLAMP(volume, AUDIO_MUS_MIN_VOLUME, AUDIO_MUS_MAX_VOLUME);
                    vol *= masterMusVolume;
                    prIosSongSetVolume(vol);
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


/// ---------------------------------------------------------------------------
/// Plays a sound effect.
/// ---------------------------------------------------------------------------
s32 prSoundManager_Ios::SFXPlay(s32 index, f32 volume, bool loop)
{
    s32 handle = -1;

#ifdef SOUND_ALLOW

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
                    f32 vol = PRCLAMP(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
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
                    handle = i;
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
void prSoundManager_Ios::SFXStop(s32 index)
{
#ifdef SOUND_ALLOW

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
                    break;
                }
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
void prSoundManager_Ios::SFXStop(const char *name)
{
#ifdef SOUND_ALLOW
        
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
void prSoundManager_Ios::SFXStopAll()
{
#ifdef SOUND_ALLOW

    if (initialised)
    {
        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].state == SFX_STATE_PLAYING || soundEffects[i].state == SFX_STATE_PAUSED)
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
bool prSoundManager_Ios::SFXIsPlaying(s32 index) const
{
    bool result = false;

#ifdef SOUND_ALLOW
    
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
bool prSoundManager_Ios::SFXIsPlaying(const char *name) const
{
    bool result = false;

#ifdef SOUND_ALLOW

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
void prSoundManager_Ios::SFXPause(s32 index, bool state)
{

#ifdef SOUND_ALLOW

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
void prSoundManager_Ios::SFXPause(const char *name, bool state)
{
#ifdef SOUND_ALLOW

    PRASSERT(name && *name);

    if (initialised && name && *name)
    {
        u32 hash = prStringHash(name);

        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
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


/// ---------------------------------------------------------------------------
/// Pauses all the sound effects
/// ---------------------------------------------------------------------------
void prSoundManager_Ios::SFXPauseAll(bool state)
{
#ifdef SOUND_ALLOW

    if (initialised)
    {
        if (sfxPaused != state)
        {
            sfxPaused = state;

            if (state)
            {
                for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
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
                for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
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


/// ---------------------------------------------------------------------------
/// Determines if all the sound effects are paused.
/// ---------------------------------------------------------------------------
bool prSoundManager_Ios::SFXGetPaused() const
{
    bool result = false;

#ifdef SOUND_ALLOW
    
    if (initialised)
    {
        result = sfxPaused;
    }

#endif

    return result;
}


/// ---------------------------------------------------------------------------
/// Returns the number of active sound effects
/// ---------------------------------------------------------------------------
s32 prSoundManager_Ios::SFXGetActive() const
{
#ifdef SOUND_ALLOW    
    return active;
#else
    return 0;
#endif
}


/// ---------------------------------------------------------------------------
/// Sets the volume of the specified effect.
/// ---------------------------------------------------------------------------
void prSoundManager_Ios::SFXSetVolume(s32 index, f32 volume)
{
#ifdef SOUND_ALLOW
    
    if (initialised)
    {
        for (s32 i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)index)
            {
                if (soundEffects[i].state == SFX_STATE_PLAYING)
                {
                    // Set volume
                    f32 vol = PRCLAMP(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
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
void prSoundManager_Ios::SFXSetPosition(const char *name, f32 x, f32 y, f32 z)
{
#ifdef SOUND_ALLOW

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


/// ---------------------------------------------------------------------------
/// Display debug information on the sound player.
/// ---------------------------------------------------------------------------
/*void prSoundManager_Ios::DisplayUsage() const
{
#ifdef SOUND_ALLOW
    prTrace("Stubbed call\n");
#endif
}*/


/// ---------------------------------------------------------------------------
/// Debug function which determines if the sound system is available.
/// ---------------------------------------------------------------------------
/*bool prSoundManager_Ios::IsSoundAvailable() const
{
#ifdef SOUND_ALLOW    
    return true;
#else    
    return false;
#endif
}*/


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
/*void prSoundManager_Ios::Mute(bool state)
{
#ifdef SOUND_ALLOW
    
    if (state)
    {
        if (!isMuted)
        {
            masterMusVolumeBackup = masterMusVolume;
            masterSfxVolumeBackup = masterSfxVolume;
            masterMusVolume       = 0.0f;
            masterSfxVolume       = 0.0f;
            isMuted               = true;
        }
    }
    else
    {
        if (isMuted)
        {
            masterMusVolume       = masterMusVolumeBackup;
            masterSfxVolume       = masterSfxVolumeBackup;
            masterMusVolumeBackup = 0.0f;
            masterSfxVolumeBackup = 0.0f;
            isMuted               = false;
        }
    }

#else

    PRUNUSED(state);

#endif
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
bool prSoundManager_Ios::IsMuted() const
{
#ifdef SOUND_ALLOW
    return isMuted;
#else
    return false;
#endif
}*/


#endif// PLATFORM_IOS
