/**
 * prSoundManager.cpp
 */


#include "prSoundManager.h"
#include "prSoundManagerShared.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../debug/prDebug.h"


/// ---------------------------------------------------------------------------
/// Construct.
/// ---------------------------------------------------------------------------
prSoundManager::prSoundManager() : prCoreSystem(PRSYSTEM_AUDIO, "prSoundManager")
{
    masterMusVolume         = AUDIO_MUS_MAX_VOLUME;
    masterSfxVolume         = AUDIO_SFX_MAX_VOLUME;
    songVolume              = AUDIO_MUS_INITIAL_VOL;
    songIndex               = -1;
    playingSong             = false;
    initialised             = false;
    exp1                    = false;
    exp2                    = false;
    pLoadedWaves            = NULL;
    pMusicTracks            = NULL;
    numTracks               = 0;
    numEffects              = 0;
    active                  = 0;
    effectId                = 1;

    // Set default states
    for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
    {
        soundEffects[i].state    = SFX_STATE_UNAVAILABLE;
        soundEffects[i].uiSource = 0xFFFFFFFF;
    }

    TODO("Add audio loop control")
    TODO("Add silent mode")
}


/// ---------------------------------------------------------------------------
/// Destruct.
/// ---------------------------------------------------------------------------
prSoundManager::~prSoundManager()
{
    PRSAFE_DELETE_ARRAY(pLoadedWaves);
}


/// ---------------------------------------------------------------------------
/// Registers a song list, so we can use play by index.
/// ---------------------------------------------------------------------------
void prSoundManager::LoadSongs(const char **filenames, int count)
{
    PRASSERT(filenames);
    PRASSERT(count > 0);
    pMusicTracks = filenames;
    numTracks    = count;
}


// ----------------------------------------------------------------------------
// Starts playing a song.
// ----------------------------------------------------------------------------
void prSoundManager::SongPlay(s32 index)
{
#ifdef SOUND_ALLOW    
    PRASSERT(index >= 0);
    PRASSERT(index  < numTracks);
    SongPlayByName(pMusicTracks[index]);
#else
    PRUNUSED(index);
#endif
}


/// ---------------------------------------------------------------------------
/// Gets the current song.
/// ---------------------------------------------------------------------------
s32 prSoundManager::SongGetCurrent() const
{
    return songIndex;
}


// ----------------------------------------------------------------------------
// Gets the current song.
// ----------------------------------------------------------------------------
bool prSoundManager::SongIsPlaying() const
{
    return playingSong;
}


/// ---------------------------------------------------------------------------
/// Sets the master volume.
/// ---------------------------------------------------------------------------
void prSoundManager::SongSetMasterVolume(f32 volume)
{
#ifdef SOUND_ALLOW    
    masterMusVolume = PRCLAMP(volume, AUDIO_MUS_MIN_VOLUME, AUDIO_MUS_MAX_VOLUME);
    SongSetVolume(songVolume);
#else
    UNUSED(volume);
#endif
}


/// ---------------------------------------------------------------------------
/// Gets the master volume.
/// ---------------------------------------------------------------------------
float prSoundManager::SongGetMasterVolume() const
{
    return masterMusVolume;
}


/// ---------------------------------------------------------------------------
/// Plays a sound effect.
/// ---------------------------------------------------------------------------
s32 prSoundManager::SFXPlay(const char *name, f32 volume, bool loop)
{
    s32 handle = -1;

#ifdef SOUND_ALLOW

    PRASSERT(name && *name);
    
    if (initialised && name && *name)
    {
        if (numEffects > 0)
        {
            u32 hash  = prStringHash(name);
        
            for (s32 index = 0 ; index < numEffects; index++)
            {
                if (hash == pLoadedWaves[index].hash)
                {
                    handle = SFXPlay(index, volume, loop);
                    break;
                }
            }
        }
    }

#else

    PRUNUSED(loop);
    PRUNUSED(volume);
    PRUNUSED(name);

#endif

    return handle;
}


/// ---------------------------------------------------------------------------
/// Sets the master volume for all effects.
/// ---------------------------------------------------------------------------
void prSoundManager::SFXSetMasterVolume(f32 volume)
{
    masterSfxVolume = PRCLAMP(volume, AUDIO_SFX_MIN_VOLUME, AUDIO_SFX_MAX_VOLUME);
}


/// ---------------------------------------------------------------------------
/// Gets the master volume for the all effects.
/// ---------------------------------------------------------------------------
f32 prSoundManager::SFXGetMasterVolume() const
{
    return masterSfxVolume;
}
