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

#elif defined(PLATFORM_BADA)
  #if defined(BADA_SDK_2)
    #include <AL/al.h>
    #include <AL/alc.h>
    #include <AL/alut.h>
    #include <AL/alext.h>
  
  #else
    #ifdef SOUND_ALLOW
    #undef SOUND_ALLOW
    #endif
  
  #endif

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


#if defined(PLATFORM_PC) || defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX)


#if defined(SOUND_ALLOW)
/// ---------------------------------------------------------------------------
/// Returns error text.
/// ---------------------------------------------------------------------------
static const char *AL_ErrText(int error);
static const char *AL_ErrText(int error)
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


/// ---------------------------------------------------------------------------
/// Returns error text.
/// ---------------------------------------------------------------------------
static const char *ALC_ErrText(int error);
static const char *ALC_ErrText(int error)
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


#if defined(PLATFORM_BADA)
static const char *ALUT_ErrText(int error);
static const char *ALUT_ErrText(int error)
{
    const char * text = "An unknown ALUT error occurred.";

    switch(error)
    {
    case ALUT_ERROR_NO_ERROR:
        text = "No ALUT error found.";
        break;

    case ALUT_ERROR_OUT_OF_MEMORY:
        text = "ALUT ran out of memory.";
        break;

    case ALUT_ERROR_INVALID_ENUM:
        text = "ALUT was given an invalid enumeration token.";
        break;

    case ALUT_ERROR_INVALID_VALUE:
        text = "ALUT was given an invalid value.";
        break;

    case ALUT_ERROR_INVALID_OPERATION:
        text = "The operation is invalid in the current ALUT state.";
        break;

    case ALUT_ERROR_NO_CURRENT_CONTEXT:
        text = "There is no current AL context.";
        break;

    case ALUT_ERROR_AL_ERROR_ON_ENTRY:
        text = "There was already an AL error on entry to an ALUT function.";
        break;

    case ALUT_ERROR_ALC_ERROR_ON_ENTRY:
        text = "There was already an ALC error on entry to an ALUT function.";
        break;

    case ALUT_ERROR_OPEN_DEVICE:
        text = "There was an error opening the ALC device.";
        break;

    case ALUT_ERROR_CLOSE_DEVICE:
        text = "There was an error closing the ALC device.";
        break;

    case ALUT_ERROR_CREATE_CONTEXT:
        text = "There was an error creating an ALC context.";
        break;

    case ALUT_ERROR_MAKE_CONTEXT_CURRENT:
        text = "Could not change the current ALC context.";
        break;

    case ALUT_ERROR_DESTROY_CONTEXT:
        text = "There was an error destroying the ALC context.";
        break;

    case ALUT_ERROR_GEN_BUFFERS:
        text = "There was an error generating an AL buffer.";
        break;

    case ALUT_ERROR_BUFFER_DATA:
        text = "There was an error passing buffer data to AL.";
        break;

    case ALUT_ERROR_IO_ERROR:
        text = "I/O error, consult errno for more details.";
        break;

    case ALUT_ERROR_UNSUPPORTED_FILE_TYPE:
        text = "Unsupported file type.";
        break;

    case ALUT_ERROR_UNSUPPORTED_FILE_SUBTYPE:
        text = "Unsupported mode within an otherwise usable file type.";
        break;

    case ALUT_ERROR_CORRUPT_OR_TRUNCATED_DATA:
        text = "The sound data was corrupt or truncated.";
        break;
    }

    return text;
}
#endif


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
                prTrace(ALC_ErrText(error));
            }
        }
    }
    else
    {
        prTrace("No OpenAL device exists.\n");
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

    int error = AL_NO_ERROR;

    error = alGetError();

    if (error != AL_NO_ERROR)
    {
        if (AL_ErrText(error))
        {
            prTrace(AL_ErrText(error));
        }
    }

    return error;

#else

    return 0;

#endif
}


/// ---------------------------------------------------------------------------
/// Checks for errors
/// ---------------------------------------------------------------------------
int ALUT_ErrorCheck()
{
#if defined(SOUND_ALLOW) && defined(PLATFORM_BADA) && defined(BADA_SDK_2)

    int error = ALC_NO_ERROR;

    error = alutGetError();

    if (error != ALUT_ERROR_NO_ERROR)
    {
        prTrace("%s\n", alutGetErrorString(error));

        if (ALUT_ErrText(error))
        {
            prTrace("%s\n", ALUT_ErrText(error));
        }
    }

    return error;

#else

    return 0;

#endif
}


#endif
