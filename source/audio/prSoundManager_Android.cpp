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


// ----------------------------------------------------------------------------
// Construct.
// ----------------------------------------------------------------------------
prSoundManager_Android::prSoundManager_Android()
{
}


// ----------------------------------------------------------------------------
// Destruct.
// ----------------------------------------------------------------------------
prSoundManager_Android::~prSoundManager_Android()
{
}


// ----------------------------------------------------------------------------
// Initialises the sound system.
// ----------------------------------------------------------------------------
bool prSoundManager_Android::Initialise()
{
    return true;
}


// ----------------------------------------------------------------------------
// Releases all sound system assets.
// ----------------------------------------------------------------------------
void prSoundManager_Android::Release()
{
}


// ----------------------------------------------------------------------------
// Updates the sound system.
// ----------------------------------------------------------------------------
void prSoundManager_Android::Update(f32 dt)
{
    PRUNUSED(dt);
}


// ----------------------------------------------------------------------------
// Loads the effects into sound memory.
// ----------------------------------------------------------------------------
void prSoundManager_Android::LoadSFX(const prSFXInfo *sfx, s32 count)
{
}


// ----------------------------------------------------------------------------
// Starts playing a song.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SongPlayByName(const char *filename)
{
}


// ----------------------------------------------------------------------------
// Stops the current song.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SongStop(f32 time)
{
}


// ----------------------------------------------------------------------------
// Pauses the active song.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SongPause(bool pause)
{
    PRUNUSED(pause);
}


// ----------------------------------------------------------------------------
// Determines if the current song is paused.
// ----------------------------------------------------------------------------
bool prSoundManager_Android::SongGetPaused() const
{
    bool result = false;
    return result;
}


// ----------------------------------------------------------------------------
// Sets the volume of the current song.
// ----------------------------------------------------------------------------
void prSoundManager_Android::SongSetVolume(f32 volume)
{
    PRUNUSED(volume);
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
