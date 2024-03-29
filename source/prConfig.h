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
 *
 * Notes:
 *      Feb 2020 - Cleaned up the code by removing unnecessary inclusion/duplication guards.
 */


#pragma once


// Audio platform
#define AUDIO_PC        1
#define AUDIO_IOS       2
#define AUDIO_ANDROID   3
#define AUDIO_MAC       4
#define AUDIO_LINUX     5


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

// Or Linux? Android?
#elif defined(__GNUC__)
    // Linux
    #if ((defined(linux) || (__linux__)) && !defined(ANDROID))
        #define PLATFORM_LINUX
        #define AUDIO_TYPE  AUDIO_LINUX

    // Android
    #elif (ANDROID)
        #define PLATFORM_ANDROID
        #define AUDIO_TYPE  AUDIO_ANDROID
    #endif

#else
    #error No platform has been defined!

#endif


// ----------------------------------------------------------------------------
// Use these defines to configure the engine setup
// ----------------------------------------------------------------------------
#define SOUND_ALLOW                                     // Use the sound system. Can be removed for debugging.
//#define HIDE_MESSAGES                                   // Allows the TODO messages to be displayed by the compiler.
#define PROTEUS_ALLOW_CONSOLE                           // Allows the debug console to be optionally removed. (PC only)
#define PROTEUS_ALLOW_AT                                // Allows the AT define to exist
//#define PROTEUS_IAP_TEST                                // Allows the in app purchase test code
//#define ALLOW_STEAM                                     // Allows the steam code
#define ALLOW_FREETYPE                                  // For builds that need freetype
#define ALLOW_IMGUI                                     // For imgui UI
#define REMOVE_EXCEPTIONS                               // Allows try/catch code to be removed.
#define ALLOW_GLEW                                      // Allows glew to be used
#define STATIC_GLEW                                     // for glew static library
#define PROTEUS_ALLOW_WATERMARK                         // Allow watermark?


// Tools will always have min/max buttons and be resizeble
#if defined(PROTEUS_TOOL)
    #define PROTEUS_ALLOW_WINDOW_MINIMIZE               // Allows the tool window to have a minimize button
    #define PROTEUS_ALLOW_WINDOW_MAXIMIZE               // Allows the tool window to have a maximize button
    #define PROTEUS_ALLOW_RESIZEABLE_WINDOW             // Allows the tool window to be resizeable.

// Games generally won't
#else
    //#define PROTEUS_ALLOW_WINDOW_MINIMIZE               // Allows the game window to have a minimize button (Optionally)
    //#define PROTEUS_ALLOW_WINDOW_MAXIMIZE               // Allows the game window to have a maximize button (Optionally)
    //#define PROTEUS_ALLOW_RESIZEABLE_WINDOW             // Allows the game window to be resizeable. (Optionally)

#endif


// Android config
#if defined(PLATFORM_ANDROID)
    #define USE_OPENAL                                  // Allows android to use OpenAL
#endif


// PC Config
#if defined(PLATFORM_PC)
  #define PROTEUS_USE_ANT_TWEAK_BAR                     // Use AntTweakBar?
#endif


// OpenGL debug settings
#define OPENGL_SHOW_EXTENSIONS
#define OPENGL_CHECK_CALLS


// Optimizations
#define PROTEUS_OPTIMISE_REMOVE_ISTEXTURE               // If defined this removes the IsTexture text in texture release
//#define PROTEUS_OPTIMISE_NO_VECTOR2_INIT                // If defined then the prVector2 class does not zero its members in the constructor
//#define PROTEUS_OPTIMISE_NO_VECTOR3_INIT                // If defined then the prVector3 class does not zero its members in the constructor
