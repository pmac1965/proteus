/**
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


#ifndef __PRCONFIG_H
#define __PRCONFIG_H


// Audio platform
#define AUDIO_PC        1
#define AUDIO_IOS       2
#define AUDIO_BADA      3
#define AUDIO_ANDROID   4
#define AUDIO_MAC       5
#define AUDIO_LINUX     6
#define AUDIO_WINPHONE  7


// Determine platform. First check for PC
#if defined(_MSC_VER)
    #define PLATFORM_PC
    #define AUDIO_TYPE  AUDIO_PC

// Else, Is it the iphone? Mac?
#elif defined(__APPLE_CC__)
    #include "TargetConditionals.h"

    // iOS Simulator
    #if TARGET_IPHONE_SIMULATOR
    #define PLATFORM_IOS
    #define IOS_SIMULATOR
    #define AUDIO_TYPE  AUDIO_IOS

    // iOS device
    #elif TARGET_OS_IPHONE
        #define PLATFORM_IOS
        #define AUDIO_TYPE     AUDIO_IOS

    // Mac OS
    #elif TARGET_OS_MAC
        #define PLATFORM_MAC
        #define AUDIO_TYPE     AUDIO_MAC

    #else
        // Unsupported platform
        #error No platform has been defined!

    #endif

// Or, Is it the bada? Android?
#elif defined(__GNUC__)
    // Bada?
    #if defined(SHP)
        //#error "Its Bada"
        #define PLATFORM_BADA
        #define AUDIO_TYPE  AUDIO_BADA
    
    // Linux
    #elif (defined(linux) && !defined(__ANDROID__))
        //#error "Its linux"
        #define PLATFORM_LINUX
        #define AUDIO_TYPE  AUDIO_LINUX

    // Android
    #elif (defined(linux) && defined(__ANDROID__))
        //#error "Its Android"
        #define PLATFORM_ANDROID
        #define AUDIO_TYPE  AUDIO_ANDROID
    #endif

#else
    #error No platform has been defined!

#endif


// ----------------------------------------------------------------------------
// Use these defines to configure the engine setup
// ----------------------------------------------------------------------------
#ifndef SOUND_ALLOW                                     // Use the sound system. Can be removed for debugging.
#define SOUND_ALLOW
#endif

#ifndef ALLOW_DIRECTX                                   // For builds that don't need DirectX
//#define ALLOW_DIRECTX
#endif

#ifndef ALLOW_FREETYPE                                  // For builds that need freetype
#define ALLOW_FREETYPE
#endif

#ifndef REMOVE_EXCEPTIONS                               // Allows try/catch code to be removed. For example bada doesn't allow try/catch code.
#define REMOVE_EXCEPTIONS
#endif

#ifndef ALLOW_PROTEUS_NAMESPACE                         // Allows namespace to be removed.
//#define ALLOW_PROTEUS_NAMESPACE
#endif

#ifndef SHOW_MESSAGES                                   // Allows the TODO messages to be displayed by the compiler.
//#define SHOW_MESSAGES
#endif

#ifndef PROTEUS_ALLOW_CONSOLE                           // Allows the debug console to be optionally removed. (PC only)
#define PROTEUS_ALLOW_CONSOLE
#endif

#ifndef PROTEUS_ALLOW_WINDOW_MINIMIZE                   // Allows the PC window to have a minimize button
//#define PROTEUS_ALLOW_WINDOW_MINIMIZE
#endif

#ifndef PROTEUS_ALLOW_WINDOW_MAXIMIZE                   // Allows the PC window to have a maximize button
//#define PROTEUS_ALLOW_WINDOW_MAXIMIZE
#endif

#ifndef PROTEUS_ALLOW_RESIZEABLE_WINDOW                 // Allows the PC window to be resizeable.
//#define PROTEUS_ALLOW_RESIZEABLE_WINDOW
#endif


// ----------------------------------------------------------------------------
// Android config
// ----------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
#ifndef USE_OPENAL                                      // Allows the android to use OpenAL
#define USE_OPENAL
#endif
#endif


// ----------------------------------------------------------------------------
// Bada config
// ----------------------------------------------------------------------------
#if defined(PLATFORM_BADA)                              // Bada can't allow try/catch as they're not supported.
#ifndef REMOVE_EXCEPTIONS                               // This also affects 'new' as 'new' no longer throws exceptions
#define REMOVE_EXCEPTIONS
#endif

#ifndef ALLOW_PROTEUS_NAMESPACE                         // Prevents naming collisions.
//#define ALLOW_PROTEUS_NAMESPACE
#endif

#ifndef BADA_SDK_2                                      // Bada SDK version 2 and above have different C library implementations
#define BADA_SDK_2                                      // Need to be careful when using standard C code.
#endif

#endif//PLATFORM_BADA


// ----------------------------------------------------------------------------
// Namespace
// ----------------------------------------------------------------------------
#if defined(ALLOW_PROTEUS_NAMESPACE)                    // This code allows optional namespace usage.
  #define PRNS_BEGIN        namespace Proteus {
  #define PRNS              Proteus::
  #define PRNS_END          }

#else
  #define PRNS_BEGIN
  #define PRNS
  #define PRNS_END

#endif//ALLOW_PROTEUS_NAMESPACE


// Tweakbar
#ifndef PROTEUS_USE_ANT_TWEAK_BAR                       // Use AntTweakBar?
#define PROTEUS_USE_ANT_TWEAK_BAR
#endif


// Watermark?
#ifndef PROTEUS_ALLOW_WATERMARK                         // Allow watermark
#define PROTEUS_ALLOW_WATERMARK
#endif


// OpenGL debug settings
//#define OPENGL_SHOW_EXTENSIONS
#define OPENGL_CHECK_CALLS


// Optimizations
#define PROTEUS_OPTIMISE_REMOVE_ISTEXTURE


#endif//__PRCONFIG_H
