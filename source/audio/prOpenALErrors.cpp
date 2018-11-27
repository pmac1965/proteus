/**
 * prOpenALErrors.cpp
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
#include "prOpenALErrors.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"


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
    // Uses http://repo.or.cz/w/openal-soft/android.git
    #if defined(USE_OPENAL)
      #include "../android/AL/al.h"
      #include "../android/AL/alc.h"

    #else
      #ifdef SOUND_ALLOW
      #undef SOUND_ALLOW
      #endif
    #endif

#elif defined(PLATFORM_LINUX)
    #include <AL/al.h>
    #include <AL/alc.h>

#else
    #error Unsupported platform!

#endif


#if defined(SOUND_ALLOW)
namespace
{
    /// -----------------------------------------------------------------------
    /// Returns error text.
    /// -----------------------------------------------------------------------
    const char *AL_ErrText(int error)
    {
        const char * text = "An unknown OpenAL error occurred.\n";

        switch(error)
        {
        case AL_INVALID_NAME:
            text = "A bad name (ID) was passed to an OpenAL function.\n";
            break;

        case AL_INVALID_ENUM:
            text = "An invalid enum value was passed to an OpenAL function.\n";
            break;

        case AL_INVALID_VALUE:
            text = "An invalid value was passed to an OpenAL function.\n";
            break;

        case AL_INVALID_OPERATION:
            text = "The requested OpenAL operation is not valid.\n";
            break;

        case AL_OUT_OF_MEMORY:
            text = "The requested operation resulted in OpenAL running out of memory.\n";
            break;
        }

        return text;
    }


    /// -----------------------------------------------------------------------
    /// Returns error text.
    /// -----------------------------------------------------------------------
    const char *ALC_ErrText(int error)
    {
        const char * text = "An unknown OpenAL error occurred.\n";

        switch(error)
        {
        case ALC_INVALID_CONTEXT:
            text = "ALC: Invalid context.\n";
            break;

        case ALC_INVALID_ENUM:
            text = "ALC: An invalid enum value was passed to an ALC function.\n";
            break;

        case ALC_INVALID_VALUE:
            text = "ALC: An invalid value was passed to an ALC function.\n";
            break;

        case ALC_INVALID_DEVICE:
            text = "ALC: Invalid device.\n";
            break;

        case ALC_OUT_OF_MEMORY:
            text = "ALC: Out of memory.\n";
            break;
        }

        return text;
    }
}
#endif//SOUND_ALLOW


/// ---------------------------------------------------------------------------
/// Checks for errors
/// ---------------------------------------------------------------------------
int ALC_ErrorCheck(void *device)
{
#if defined(SOUND_ALLOW)

    int error = ALC_NO_ERROR;

    if (device)
    {
        error = alcGetError((ALCdevice *)device);

        if (error != ALC_NO_ERROR)
        {
            if (ALC_ErrText(error))
            {
                prTrace(prLogLevel::LogError, ALC_ErrText(error));
            }
        }
    }
    else
    {
        prTrace(prLogLevel::LogError, "No OpenAL device exists.\n");
    }

    return error;

#else

    PRUNUSED(device);
    return 0;

#endif
}


/// ---------------------------------------------------------------------------
/// Checks for errors
/// ---------------------------------------------------------------------------
int AL_ErrorCheck()
{
#if defined(SOUND_ALLOW)

    int error = alGetError();
    if (error != AL_NO_ERROR)
    {
        if (AL_ErrText(error))
        {
            prTrace(prLogLevel::LogError, AL_ErrText(error));
        }
    }

    return error;

#else

    return 0;

#endif
}
