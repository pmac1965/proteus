/**
 * prSoundManagerShared.cpp
 */


#include "../prConfig.h"
#include "prSoundManagerShared.h"


#if defined(PLATFORM_PC)
  #include <al.h>
  #include <alc.h>

#elif defined(PLATFORM_IOS)
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
    // Uses the openal-soft-android-251011 lib for android as OpenAL is unsupported as yet!
    //#if defined(USE_OPENAL)
    #include "../android/AL/al.h"
    #include "../android/AL/alc.h"
    //#endif

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
    if (uiBuffer != 0xFFFFFFFF)
    {
    	alDeleteBuffers(1, &uiBuffer);
        AL_ERROR_CHECK();
    }

    uiBuffer = 0xFFFFFFFF;
}
