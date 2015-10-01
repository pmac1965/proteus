/**
 * prSoundManagerShared.cpp
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
#include "prSoundManagerShared.h"
#include "prOpenALErrors.h"
#include "../debug/prDebug.h"


#if defined(PLATFORM_PC)
  #include <al.h>
  #include <alc.h>

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_MAC))
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
  #include <AudioToolbox/AudioToolbox.h>
  #include <AudioToolbox/ExtendedAudioFile.h>
  #include <AudioToolbox/AudioServices.h>

#elif defined(PLATFORM_ANDROID)
	#include "../android/AL/al.h"
	#include "../android/AL/alc.h"

#elif defined(PLATFORM_LINUX)
    #include <AL/al.h>
    #include <AL/alc.h>

#else
    #error Unsupported platform!

#endif


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prLoadedWave::prLoadedWave()
{
    uiBuffer = 0xFFFFFFFF;
    hash     = 0;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prLoadedWave::~prLoadedWave()
{
#if defined(SOUND_ALLOW)
    // Delete the sound buffer
    if (uiBuffer != 0xFFFFFFFF)
    {
        #if defined(PLATFORM_ANDROID) && defined(USE_OPENAL)
            alDeleteBuffers(1, &uiBuffer);
            AL_ERROR_CHECK();
        #else
            alDeleteBuffers(1, &uiBuffer);
            AL_ERROR_CHECK();
        #endif
    }

#endif

    uiBuffer = 0xFFFFFFFF;
    hash     = 0;
}
