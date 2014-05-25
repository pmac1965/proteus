// File: prDebug.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __PRDEBUG_H
#define __PRDEBUG_H


#include "../prConfig.h"
#include "../core/prMacros.h"


// Method: prDebugShowLastError
//      Displays the last *windows* error message.
//
// Notes:
//      Compiles on other platforms, but does nothing.
void prDebugShowLastError(const char *msg = 0);

// Method: prOutputString
//      Outputs a string of text to the platforms debug output window.
//
// Parameters:
//      text - The message to write
void prOutputString(const char *text);


// Cross platform compiler messages.
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)
    #define DO_PRAGMA(msg)          _Pragma(#msg)
#endif


// In code reminder messages.
#if defined(SHOW_MESSAGES)

    #if defined(PLATFORM_PC)
        // Reminder macro
        #define REMINDER                __FILE__ "(" PRCURRLINE ") : Reminder: " 
        #define TODO(msg)               __pragma(message(REMINDER msg))

    #elif defined(PLATFORM_ANDROID) || defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)
        // Reminder macro
        #define TODO(msg)               DO_PRAGMA(message ("TODO: " #msg))

    #else
        // Default to release version.
        #define TODO(msg)
        #pragma message("No 'TODO' macro")

    #endif

#else
    // Release version
    #define TODO(msg)

#endif// SHOW_MESSAGES


// Compiler messages
#if defined(PLATFORM_ANDROID)
    #define CMESSAGE(msg)    DO_PRAGMA(message #msg)
#endif


#endif//__PRDEBUG_H
