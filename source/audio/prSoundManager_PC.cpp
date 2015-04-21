/**
 * prSoundManager_PC.cpp
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


// http://blog.tbam.com.ar/2009/05/sound-spatiaiization-for-2d-games-in.html


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <Windows.h>
#include <al.h>
#include <alc.h>
#include "prSoundManager_PC.h"
#include "prSoundManagerShared.h"
#include "prOpenALErrors.h"
#include "prOpenALDeviceList.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "external/vorbisfile.h"


// Defines.
#define SOUND_DEBUG
#define SOUND_SHOW_FAILS
#define SONG_BUFFER_SIZE    PRKB(128)


// Locals
namespace
{
    ov_callbacks callbacks;


    // OggVorbis callback
    size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
    {
        return fread(ptr, size, nmemb, (FILE*)datasource);
    }

    // OggVorbis callback
    int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
    {
        return fseek((FILE*)datasource, (long)offset, whence);
    }

    // OggVorbis callback
    int ov_close_func(void *datasource)
    {
       return fclose((FILE*)datasource);
    }

    // OggVorbis callback
    long ov_tell_func(void *datasource)
    {
        return ftell((FILE*)datasource);
    }
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSoundManager_PC::prSoundManager_PC()
{
#ifdef SOUND_ALLOW

    pPCMBuffer          = new u8[SONG_BUFFER_SIZE];
    device              = NULL;
    context             = NULL;
    ov_open_callbacks   = NULL;
    ov_info             = NULL;
    ov_clear            = NULL;
    ov_read             = NULL;
    dll                 = NULL;
    frequency           = 0;
    channels            = 0;
    format              = 0;
    songSource          = 0;
    songBuffers[0]      = 0xFFFFFFFF;
    songBuffers[1]      = 0xFFFFFFFF;
    pOggFile            = NULL;


    memset(&oggStream, 0, sizeof(oggStream));


    // Try and load Vorbis DLLs. vorbisfile.dll will load ogg.dll and vorbis.dll
    dll = LoadLibrary(TEXT("vorbisfile.dll"));
    if (dll)
    {
        ov_open_callbacks = (LPOVOPENCALLBACKS)GetProcAddress(dll, "ov_open_callbacks");
        PRASSERT(ov_open_callbacks);

        ov_info = (LPOVINFO)GetProcAddress(dll, "ov_info");
        PRASSERT(ov_info);

        ov_clear = (LPOVCLEAR)GetProcAddress(dll, "ov_clear");
        PRASSERT(ov_clear);

        ov_read = (LPOVREAD)GetProcAddress(dll, "ov_read");
        PRASSERT(ov_read);
    }
    else
    {
        prTrace("Failed to load OggVorbis dll's\n");
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSoundManager_PC::~prSoundManager_PC()
{
#ifdef SOUND_ALLOW
    // Stop the music
    //SongStop();
        
    // Delete the SFX sources
    for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
    {
        if (soundEffects[i].uiSource != 0xFFFFFFFF)
        {
            alSourceStop(soundEffects[i].uiSource);
            AL_ERROR_CHECK()

            alDeleteSources(1, &soundEffects[i].uiSource);
            AL_ERROR_CHECK()

            soundEffects[i].uiSource = 0xFFFFFFFF;
        }
    }

    // Release OpenAl
    Release();

    // Unload dll
    if (dll)
    {
        FreeLibrary(dll);
        dll = NULL;
    }

    PRSAFE_DELETE_ARRAY(pPCMBuffer);
#endif
}


/// ---------------------------------------------------------------------------
/// Initialises the sound system.
/// ---------------------------------------------------------------------------
bool prSoundManager_PC::Initialise()
{
#ifdef SOUND_ALLOW
    
    // Already initialised?
    if (initialised)
    {
        prTrace("The sound system is already initialised.\n");
        return true;
    }


    // Get device list
    prOpenALDeviceList *pDeviceList = new prOpenALDeviceList();
    bool result = false;
    if (pDeviceList)
    {    
        s32 count = pDeviceList->GetNumDevices();
        if (count > 0)
        {
            for (int i=0; i<count; i++)
            {
                #if defined(SOUND_DEBUG)
                prTrace("Trying device: %s\n", pDeviceList->GetDeviceName(i));
                #endif

                // Open default OpenAL device
                device = alcOpenDevice(pDeviceList->GetDeviceName(i));
                ALC_ErrorCheck(device);
                if (device)
                {
                    // Create a Context
                    context = alcCreateContext(device, NULL);
                    ALC_ErrorCheck(device);
                    if (context)
                    {
                        // Make the context current active
                        alcMakeContextCurrent(context);
                        if (ALC_ErrorCheck(device) == ALC_NO_ERROR)
                        {
                            result = true;

                            // Generate the playback sources
                            for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
                            {
                                // Generate a source to playback a buffer
                                alGenSources(1, &soundEffects[i].uiSource);
                                if (AL_ErrorCheck() != AL_NO_ERROR)
                                {
                                    prTrace("Failed to allocate source %i\n", i);
                                    soundEffects[i].state    = SFX_STATE_UNAVAILABLE;
                                    soundEffects[i].uiSource = 0xFFFFFFFF;
                                    result = false;
                                }
                                else
                                {
                                    soundEffects[i].state = SFX_STATE_FREE;
                                }
                            }
                        }
                        else
                        {
                            prTrace("Failed to make OpenAL context current\n");
                        }
                    }

                    // Generated sources?
                    if (!result)
                    {
                        #if defined(SOUND_DEBUG)
                        prTrace("Device failed: %s\n", pDeviceList->GetDeviceName(i));
                        #endif

                        if (context)
                        {
                            // Remove context.
                            alcMakeContextCurrent(NULL);
                            ALC_ErrorCheck(device);

                            // Destroy context
                            alcDestroyContext(context);
                            ALC_ErrorCheck(device);
                        }

                        alcCloseDevice(device);
                        context = NULL;
                        device  = NULL;
                    }
                    else
                    {
                        #if defined(SOUND_DEBUG)
                        prTrace("Device succeeded: %s\n", pDeviceList->GetDeviceName(i));
                        #endif
                        break;
                    }
                }
                else
                {
                    prTrace("Failed to open audio device: %s\n", pDeviceList->GetDeviceName(i));
                }
            }
        }

        PRSAFE_DELETE(pDeviceList);
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
void prSoundManager_PC::Release()
{
#ifdef SOUND_ALLOW
    ALCcontext *pContext = alcGetCurrentContext();
    ALCdevice  *pDevice  = alcGetContextsDevice(pContext);

    if (pContext)
    {
        // Release the current context.
        if (!alcMakeContextCurrent(NULL))
        {
            prTrace("OpenAL shutdown error\n");
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
                prTrace("OpenAL shutdown error\n");
            }

            // Destroy device
            if (!alcCloseDevice(pDevice))
            {
                prTrace("OpenAL shutdown error\n");
            }
        }
    }
#endif
}


/// ---------------------------------------------------------------------------
/// Updates the sound system.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::Update(f32 dt)
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

        // Update music
        if (songPlaying)
        {
            // Loop song?
            if (!SongUpdate())
            {
                int index = songIndex;
                SongStop();
                SongPlay(index);
                SongSetVolume(songVolume);
                return;
            }

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
    }

#else

    PRUNUSED(dt);

#endif
}


/// ---------------------------------------------------------------------------
/// Loads the sound effects into sound memory.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::LoadSFX(const prSFXInfo *sfx, s32 count)
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

        for (int i=0; i<count; i++)
        {
            prWaveID id = -1;
            waves.LoadWaveFile(sfx[i].filename, &id);
            //prTrace("Load %s\n",sfx[i].filename);

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
                }
                else
                {
                    prTrace("Sound effect: %s\n", sfx[i].filename);
                    prTrace("Failed to create effect\n");

                    #if defined(SOUND_SHOW_FAILS) && (defined(_DEBUG) || defined(DEBUG))
                    prTrace("GetWaveSize           == %i\n", waves.GetWaveSize(id, (u32*)&iDataSize)                 == WR_OK);
                    prTrace("GetWaveData           == %i\n", waves.GetWaveData(id, (void**)&pData)                   == WR_OK);
                    prTrace("GetWaveFrequency      == %i\n", waves.GetWaveFrequency(id, (u32*)&iFrequency)           == WR_OK);
                    prTrace("GetWaveALBufferFormat == %i\n", waves.GetWaveALBufferFormat(id, (u32*)&eBufferFormat)   == WR_OK);
                    #endif
                }
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
void prSoundManager_PC::SongPlayByName(const char *filename)
{
#ifdef SOUND_ALLOW

    SongStop();
    
    // Open file.
    PRASSERT(filename && *filename);
    pOggFile = fopen(filename, "rb");
    if (pOggFile == NULL)
    {
        prTrace("Couldn't open .ogg file: %s\n", filename);
        return;
    }

    // Find track index
    PRASSERT(numTracks > 0);
    s32 index = -1;
    for (s32 i = 0; i < numTracks; i++)
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


    // Ogg stream support
    callbacks.read_func  = ov_read_func;
    callbacks.seek_func  = ov_seek_func;
    callbacks.close_func = ov_close_func;
    callbacks.tell_func  = ov_tell_func;

    // Reset
    frequency      = 0;
    channels       = 0;
    format         = 0;
    songSource     = 0;
    songBuffers[0] = 0xFFFFFFFF;
    songBuffers[1] = 0xFFFFFFFF;
    songPlaying    = false;
    songState      = SONG_STATE_FREE;
    songIndex      = -1;
    songFade       = 0.0f;
    songTime       = 0.0f;


    // Create an OggVorbis file stream
    PRASSERT(ov_open_callbacks);
    if (ov_open_callbacks(pOggFile , &oggStream, NULL, 0, callbacks) == 0)
    {
        // Get some information about the file (Channels, Format, and Frequency)
        PRASSERT(ov_info);
        vorbis_info *pVorbisInfo = ov_info(&oggStream, -1);
        if (pVorbisInfo)
        {
            frequency = pVorbisInfo->rate;
            channels  = pVorbisInfo->channels;

            if (pVorbisInfo->channels == 1)
            {
                format = AL_FORMAT_MONO16;
            }
            else if (pVorbisInfo->channels == 2)
            {
                format = AL_FORMAT_STEREO16;
            }
            else if (pVorbisInfo->channels == 4)
            {
                format = alGetEnumValue("AL_FORMAT_QUAD16");
            }
            else if (pVorbisInfo->channels == 6)
            {
                format = alGetEnumValue("AL_FORMAT_51CHN16");
            }
            else
            {
                // Close OggVorbis stream
                PRASSERT(ov_clear);
                ov_clear(&oggStream);
                prTrace("Unknown channel count from .ogg file: %s\n", filename);
                return;
            }
        }
        else
        {
            // Close OggVorbis stream
            PRASSERT(ov_clear);
            ov_clear(&oggStream);
            prTrace("Failed to get info from .ogg file: %s\n", filename);
            return;
        }


        alGenBuffers(1, &songBuffers[0]);
        AL_ERROR_CHECK()

        alGenBuffers(1, &songBuffers[1]);
        AL_ERROR_CHECK()

        alGenSources(1, &songSource);
        AL_ERROR_CHECK()
    
        alSource3f(songSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
        AL_ERROR_CHECK()

        alSource3f(songSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        AL_ERROR_CHECK()

        alSource3f(songSource, AL_DIRECTION, 0.0f, 0.0f, 0.0f);
        AL_ERROR_CHECK()

        alSourcef(songSource, AL_ROLLOFF_FACTOR, 0.0f);
        AL_ERROR_CHECK()

        alSourcei(songSource, AL_SOURCE_RELATIVE, AL_TRUE);
        AL_ERROR_CHECK()

        songPlaying = true;
        songIndex   = index;
        songState   = SONG_STATE_PLAYING;


        if (!SongStream((unsigned int)songBuffers[0]))
        {
            prTrace("stream failed 1\n");
        }

        if (!SongStream((unsigned int)songBuffers[1]))
        {
            prTrace("stream failed 2\n");
        }
    
        alSourceQueueBuffers(songSource, 2, songBuffers);
        AL_ERROR_CHECK()

        alSourcePlay(songSource);
        AL_ERROR_CHECK()

        //prTrace("Playing song: %s\n", filename);
        SongSetVolume(songVolume);
    }
    else
    {
        prTrace("Failed to open .ogg file: %s\n", filename);
    }

#else
    
    PRUNUSED(filename);
    
#endif
}


/// ---------------------------------------------------------------------------
/// Stops the current song.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SongStop(float time)
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

        alSourceStop(songSource);
        AL_ERROR_CHECK()

        Empty();

        alDeleteSources(1, &songSource);
        AL_ERROR_CHECK()

        alDeleteBuffers(2, songBuffers);
        AL_ERROR_CHECK()

        ov_clear(&oggStream);

        // Reset
        frequency      = 0;
        channels       = 0;
        format         = 0;
        songSource     = 0;
        songBuffers[0] = 0;
        songBuffers[1] = 0;
        songPlaying    = false;
        songState      = SONG_STATE_FREE;
        songIndex      = -1;
        songFade       = 0.0f;
        songTime       = 0.0f;
    }
    
#endif
}


/// ---------------------------------------------------------------------------
/// Pauses the active song.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SongPause(bool pause)
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
                        alSourcePause(songSource);
                        AL_ERROR_CHECK()
                    }
                }
                else
                {
                    if (songState == SONG_STATE_PAUSED)
                    {
                        songState = SONG_STATE_PLAYING;
                        alSourcePlay(songSource);
                        AL_ERROR_CHECK()
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
bool prSoundManager_PC::SongGetPaused() const
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
void prSoundManager_PC::SongSetVolume(f32 volume)
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
                    songVolume = vol;
                    vol *= masterMusVolume;
                    alSourcef(songSource, AL_GAIN, vol);
                    AL_ERROR_CHECK()
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
s32 prSoundManager_PC::SFXPlay(s32 index, f32 volume, bool loop)
{
    s32 handle = -1;

#ifdef SOUND_ALLOW

    if (initialised)
    {
        if (numEffects > 0)
        {
            PRASSERT(index >= 0);
            PRASSERT(index < numEffects);

            for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
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
void prSoundManager_PC::SFXStop(s32 id)
{
#ifdef SOUND_ALLOW

    if (initialised)
    {
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)id)
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

    PRUNUSED(id);

#endif
}


/// ---------------------------------------------------------------------------
/// Stops the specified effect.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SFXStop(const char *name)
{
#ifdef SOUND_ALLOW
        
    if (initialised && name && *name)
    {
        u32 hash = prStringHash(name);

        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].hash == hash)
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

    PRUNUSED(name);

#endif
}


/// ---------------------------------------------------------------------------
/// Stops all the sound effects.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SFXStopAll()
{
#ifdef SOUND_ALLOW

    if (initialised)
    {
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].state == SFX_STATE_PLAYING ||
                soundEffects[i].state == SFX_STATE_PAUSED)
            {
                soundEffects[i].state = SFX_STATE_FREE;
                soundEffects[i].id    = 0;
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
bool prSoundManager_PC::SFXIsPlaying(int id) const
{
    bool result = false;

#ifdef SOUND_ALLOW
    
    if (initialised)
    {
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)id)
            {
                if (soundEffects[i].state == SFX_STATE_PLAYING)
                {
                    result = true;
                    break;
                }
            }
        }
    }

#else

    PRUNUSED(id);

#endif

    return result;
}
    

/// ---------------------------------------------------------------------------
/// Determines if a particular sound effect is playing.
/// ---------------------------------------------------------------------------
bool prSoundManager_PC::SFXIsPlaying(const char *name) const
{
    bool result = false;

#ifdef SOUND_ALLOW

    PRASSERT(name && *name);

    if (initialised && name && *name)
    {
        u32 hash = prStringHash(name);

        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].hash == hash)
            {
                if (soundEffects[i].state == SFX_STATE_PLAYING)
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
void prSoundManager_PC::SFXPause(int id, bool state)
{
#ifdef SOUND_ALLOW

    if (initialised)
    {
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)id)
            {
                if (state)
                {
                    if (soundEffects[i].state == SFX_STATE_PLAYING)
                    {
                        soundEffects[i].state = SFX_STATE_PAUSED;
                        alSourcePause(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                    }
                }
                else
                {
                    if (soundEffects[i].state == SFX_STATE_PAUSED)
                    {
                        soundEffects[i].state = SFX_STATE_PLAYING;
                        alSourcePlay(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                    }
                }

                break;
            }
        }
    }

#else

    PRUNUSED(id);
    PRUNUSED(state);

#endif
}


/// ---------------------------------------------------------------------------
/// Pauses a specific sound effect
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SFXPause(const char *name, bool state)
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
                if (state)
                {
                    if (soundEffects[i].state == SFX_STATE_PLAYING)
                    {
                        soundEffects[i].state = SFX_STATE_PAUSED;
                        alSourcePause(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                    }
                }
                else
                {
                    if (soundEffects[i].state == SFX_STATE_PAUSED)
                    {
                        soundEffects[i].state = SFX_STATE_PLAYING;
                        alSourcePlay(soundEffects[i].uiSource);
                        AL_ERROR_CHECK()
                    }
                }

                break;
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
void prSoundManager_PC::SFXPauseAll(bool state)
{
#ifdef SOUND_ALLOW

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


/// ---------------------------------------------------------------------------
/// Determines if all the sound effects are paused.
/// ---------------------------------------------------------------------------
bool prSoundManager_PC::SFXGetPaused() const
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
s32 prSoundManager_PC::SFXGetActive() const
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
void prSoundManager_PC::SFXSetVolume(int id, f32 volume)
{
#ifdef SOUND_ALLOW
    
    if (initialised)
    {
        for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
        {
            if (soundEffects[i].id == (u32)id)
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

    PRUNUSED(id);
    PRUNUSED(volume);

#endif
}


/// ---------------------------------------------------------------------------
/// It's best if a sound effect is mono, or openal will not set the position
/// correctly
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SFXSetPosition(const char *name, f32 x, f32 y, f32 z)
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
/// Updates the current song
/// ---------------------------------------------------------------------------
bool prSoundManager_PC::SongUpdate()
{
    bool active = true;

#ifdef SOUND_ALLOW

    int processed;
    alGetSourcei(songSource, AL_BUFFERS_PROCESSED, &processed);
    AL_ERROR_CHECK()

    if (processed > 0)
    {
        while(processed--)
        {
            unsigned int buffer;
        
            alSourceUnqueueBuffers(songSource, 1, &buffer);
            AL_ERROR_CHECK()

            active = SongStream(buffer);

            alSourceQueueBuffers(songSource, 1, &buffer);
            AL_ERROR_CHECK()
        }
    }

#endif

    return active;
}


/// ---------------------------------------------------------------------------
/// Streams the song data
/// ---------------------------------------------------------------------------
bool prSoundManager_PC::SongStream(unsigned int buffer)
{
#ifdef SOUND_ALLOW

    //TODO("Put pcm on the heap")
    //char pcm[SONG_BUFFER_SIZE];
    int  size = 0;
    int  section;

    while(size < SONG_BUFFER_SIZE)
    {
        int result = ov_read(&oggStream, (char*)(pPCMBuffer + size), SONG_BUFFER_SIZE - size, 0, 2, 1, &section);    
        if (result > 0)
        {
            size += result;
        }
        else
        {
            if (result < 0)
            {
                prTrace("Stream error\n");
            }
            else
            {
                break;
            }
        }
    }
    
    if (size == 0)
    {
        return false;
    }
        
    //Trace("buffer %i\n", size);
    alBufferData(buffer, format, pPCMBuffer, size, frequency);
    AL_ERROR_CHECK()

#else

    PRUNUSED(buffer);

#endif

    return true;
}


/// ---------------------------------------------------------------------------
/// Empties the songs unused buffers.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::Empty()
{
#ifdef SOUND_ALLOW

    int queued;
    
    alGetSourcei(songSource, AL_BUFFERS_QUEUED, &queued);
    AL_ERROR_CHECK()
    
    if (queued > 0)
    {
        while(queued--)
        {
            ALuint buffer;    
            alSourceUnqueueBuffers(songSource, 1, &buffer);
            AL_ERROR_CHECK()
        }
    }

#endif
}


#endif//PLATFORM_PC
