/**
 * prSoundManager.cpp
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


#include "prSoundManager.h"
#include "prSoundManagerShared.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../debug/prDebug.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Construct.
/// ---------------------------------------------------------------------------
prSoundManager::prSoundManager() : prCoreSystem(PRSYSTEM_AUDIO, "prSoundManager")
{
    masterMusVolume         = AUDIO_MUS_MAX_VOLUME;
    masterSfxVolume         = AUDIO_SFX_MAX_VOLUME;
    masterMusVolumeBackup   = 0.0f;
    masterSfxVolumeBackup   = 0.0f;
    songVolume              = AUDIO_MUS_INITIAL_VOL;
    songIndex               = -1;
    songState               = prSongState::SONG_STATE_FREE;
    songFade                = 0.0f;
    songTime                = 0.0f;
    songPlaying             = false;
    initialised             = false;
    sfxPaused               = false;
    mIsMuted                = false;
    pLoadedWaves            = nullptr;
    pMusicTracks            = nullptr;
    numTracks               = 0;
    numEffects              = 0;
    active                  = 0;
    effectId                = 1;

    // Set default states
    for (int i=0; i<AUDIO_MAX_ACTIVE; i++)
    {
        soundEffects[i].state    = prSoundEffectEntryState::SFX_STATE_UNAVAILABLE;
        soundEffects[i].uiSource = 0xFFFFFFFF;
    }
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


/// ---------------------------------------------------------------------------
/// Starts playing a song.
/// ---------------------------------------------------------------------------
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
/// Sets the master volume.
/// ---------------------------------------------------------------------------
void prSoundManager::SongSetMasterVolume(f32 volume)
{
#ifdef SOUND_ALLOW    
    masterMusVolume = PRCLAMP(volume, AUDIO_MUS_MIN_VOLUME, AUDIO_MUS_MAX_VOLUME);
    SongSetVolume(songVolume);

#else
    PRUNUSED(volume);

#endif
}


/// ---------------------------------------------------------------------------
/// Plays a sound effect.
/// ---------------------------------------------------------------------------
s32 prSoundManager::SFXPlay(const char *name, f32 volume, bool loop)
{
    s32 handle = -1;

#ifdef SOUND_ALLOW
    PRASSERT(initialised && name && *name);
    PRASSERT(pLoadedWaves);

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

    if (handle == -1)
    {
        prTrace(LogWarning, "Failed to play effect '%s'\n", name);
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
/// Debug function which determines if the sound system is available.
/// ---------------------------------------------------------------------------
bool prSoundManager::IsSoundAvailable() const
{
#ifdef SOUND_ALLOW
    return true;

#else
    return false;

#endif
}


/// ---------------------------------------------------------------------------
/// Mute/unmute all sound
/// ---------------------------------------------------------------------------
void prSoundManager::Mute(bool state)
{
#ifdef SOUND_ALLOW    
    if (state)
    {
        if (!mIsMuted)
        {
            masterMusVolumeBackup = masterMusVolume;
            masterSfxVolumeBackup = masterSfxVolume;
            masterMusVolume       = 0.0f;
            masterSfxVolume       = 0.0f;
            mIsMuted              = true;
        }
    }
    else
    {
        if (mIsMuted)
        {
            masterMusVolume       = masterMusVolumeBackup;
            masterSfxVolume       = masterSfxVolumeBackup;
            masterMusVolumeBackup = 0.0f;
            masterSfxVolumeBackup = 0.0f;
            mIsMuted              = false;
        }
    }

#else
    PRUNUSED(state);

#endif
}


/// ---------------------------------------------------------------------------
/// Is sound muted?
/// ---------------------------------------------------------------------------
bool prSoundManager::IsMuted() const
{
#ifdef SOUND_ALLOW
    return mIsMuted;

#else
    return false;

#endif
}
