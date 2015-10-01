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


#pragma once


#include "../prConfig.h"
#include "../core/prMacros.h"
#include "prTrace.h"


// Method: prDebugShowLastError
//      Displays the last *windows* error message.
//
// Parameters:
//      msg - An optional text message to explain issue
//
// Notes:
//      Compiles on other platforms, but does nothing.
void prDebugShowLastError(const char *msg = 0);

// Method: prDebugRegisterConsoleWindow
//      Allows the console windows to be registered with the debug system.
//
// Parameters:
//      pConsoleWindow - A pointer to a valid console window
//
// Notes:
//      A void pointer is used so we can use it with 'C' code
//
// Notes:
//      We register the console so we can changes colours, etc
void prDebugRegisterConsoleWindow(void *pConsoleWindow);

// Method: prOutputString
//      Outputs a string of text to the platforms debug output window.
//
// Parameters:
//      level   - The log level.
//      text    - The message to write
//
// See Also:
//      <prLogLevel>
void prOutputString(prLogLevel level, const char *text);


// Cross platform compiler messages.
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)
    #define DO_PRAGMA(msg)          _Pragma(#msg)
#endif


// In code reminder messages.
#if !defined(HIDE_MESSAGES)
    #if defined(PLATFORM_PC)
        // Reminder macro
        #define REMINDER                __FILE__ "(" PRCURRLINE ") : Reminder: " 
        #define TODO(msg)               __pragma(message(REMINDER msg))

    #elif defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)
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

#endif// HIDE_MESSAGES


// Compiler messages
#if defined(PLATFORM_ANDROID)
    #define CMESSAGE(msg)    DO_PRAGMA(message #msg)
#endif


// Allows NSLog to be removeable
#if defined(PLATFORM_IOS)
    // Debug assist
    #if defined(DEBUG)
        #define DBG(text, args...)      NSLog(text, ## args)
    #else
        #define DBG(text, args...)
    #endif
#endif


// Used to log line numbers. Just a lazy shortcut... I know :-)
#if (defined(DEBUG) || defined(_DEBUG)) && defined(PROTEUS_ALLOW_AT)
    #define AT()    prTrace(LogError, "In func '%s' at line %i\n", __FUNCTION__, __LINE__)
#else
    #define AT()
#endif
