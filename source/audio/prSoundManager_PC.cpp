/**
 * prSoundManager_PC.cpp
 */


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
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "external/vorbisfile.h"


// Defines.
#define SOUND_DEBUG
#define SOUND_SHOW_FAILS
#define SONG_BUFFER_SIZE    (4096 * 32)


// Locals
namespace
{
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
    device              = NULL;
    context             = NULL;
    ov_open_callbacks   = NULL;
    ov_info             = NULL;
    ov_clear            = NULL;
    ov_read             = NULL;
    dll                 = NULL;

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
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSoundManager_PC::~prSoundManager_PC()
{
#ifdef SOUND_ALLOW
    // Stop the music
    //SongStop();
    TODO("Put back")
        
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
    //ASSERT(pImpl);    

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
        /*if (imp.playingSong)
        {
            if (!SongUpdate())
            {
                int index = imp.songIndex;
                SongStop();
                SongPlay(index);
                SongSetVolume(imp.songVolume);
                return;
            }

            if (imp.songFade > 0)
            {
                float v = (LERP(imp.songFade, 0.0f, imp.songTime) / imp.songFade); 
                v = (v / imp.songFade);
                imp.songTime -= (dt / 1000.0f);
    
                if (v > 0.0f)
                {
                    SongSetVolume(v);
                }
                else
                {
                    SongStop();
                }
            }
        }//*/
    }

#else
    UNUSED(dt);

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
            prTrace("Load %s\n",sfx[i].filename);

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

    UNUSED(count);
    UNUSED(sfx);

#endif
}


/// ---------------------------------------------------------------------------
/// Starts playing a song.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SongPlayByName(const char *filename)
{
}


/// ---------------------------------------------------------------------------
/// Sets the volume of the current song.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SongSetVolume(f32 volume)
{
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
                    handle = i;
                    break;
                }
            }
        }
    }

#else

    UNUSED(loop);
    UNUSED(volume);
    UNUSED(index);

#endif

    return handle;
}


/// ---------------------------------------------------------------------------
/// Stops the specified effect.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SFXStop(s32 index)
{
}


/// ---------------------------------------------------------------------------
/// Stops the specified effect.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SFXStop(const char *name)
{
}


/// ---------------------------------------------------------------------------
/// Stops all the sound effects.
/// ---------------------------------------------------------------------------
void prSoundManager_PC::SFXStopAll()
{
}


#endif//PLATFORM_PC
