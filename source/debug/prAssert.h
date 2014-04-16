// File: prAssert.h
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


#ifndef __PRASSERT_H
#define __PRASSERT_H


#include "../prConfig.h"


#if defined(PLATFORM_MAC)
#include <csignal>
#endif


#if defined(_DEBUG) || defined(DEBUG)

    // Function: prAssertPrint
    //      Prints the assert message
    //
    // Parameters:
    //      cond     - Condition description
    //      file     - File nane
    //      function - Function
    //      line     - Line number
    //      fmt      - Format text
    //      ...      - Optional parameters
    //
    // Notes:
    //      *Do not call this function directly*
    bool prAssertPrint(const char *cond, const char *file, const char *function, int line, const char *fmt = 0, ...);

    // Function: prPanicPrint
    //      Prints the panic message
    //
    // Parameters:
    //      file     - File nane
    //      function - Function
    //      line     - Line number
    //      fmt      - Format text
    //      ...      - Optional parameters
    //
    // Notes:
    //      *Do not call this function directly*
    void prPanicPrint(const char *file, const char *function, int line, const char *fmt = 0, ...);

    // Function: prWarnPrint
    //      Prints the warning message
    //
    // Parameters:
    //      file     - File nane
    //      function - Function
    //      line     - Line number
    //      fmt      - Format text
    //      ...      - Optional parameters
    //
    // Notes:
    //      *Do not call this function directly*
    void prWarnPrint(const char *file, const char *function, int line, const char *fmt = 0, ...);


    /// @def PRBREAKPOINT
    /// This macro will trigger a breakpoint.
    #if defined(PLATFORM_PC)
        #define PRBREAKPOINT()                                                      \
        {                                                                           \
            _asm int 3                                                              \
        }

    #elif defined(PLATFORM_IOS)
        #define PRBREAKPOINT()                                                      \
        {                                                                           \
            while (true) {}                                                         \
        }

    #elif defined(PLATFORM_BADA)
        #define PRBREAKPOINT()                                                      \
        {                                                                           \
            while (true) {}                                                         \
        }

    #elif defined(PLATFORM_ANDROID)
        #define PRBREAKPOINT()                                                      \
        {                                                                           \
            *((int*)0) = 0;                                                         \
        }

    #elif defined(PLATFORM_LINUX)
        #define PRBREAKPOINT()                                                      \
        {                                                                           \
            *((int*)0) = 0;                                                         \
        }

    #elif defined(PLATFORM_MAC)
        #define PRBREAKPOINT()                                                      \
        {                                                                           \
            raise(SIGINT);                                                          \
        }

    #else
        #error No platform defined.

    #endif


    #if defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)

        // Assertion macro which allows additional information to be displayed.
        #define PRASSERT(cond, args...)                                             \
        if (!(cond))                                                                \
        {                                                                           \
            if (prAssertPrint(#cond, __FILE__, __FUNCTION__, __LINE__, ## args))    \
            {                                                                       \
                PRBREAKPOINT();                                                     \
            }                                                                       \
        }


        // Use when you need to halt the system in order to report a critical error.
        #define PRPANIC(fmt, args...)                                               \
        {                                                                           \
            prPanicPrint(__FILE__, __FUNCTION__, __LINE__, fmt, ## args);           \
            PRBREAKPOINT();                                                         \
        }


        // Use when you need to issue a serious warning.
        #define PRWARN(fmt, args...)                                                \
        {                                                                           \
            prWarnPrint(__FILE__, __FUNCTION__, __LINE__, fmt, ## args);            \
        }

    #else

        /// @def PRASSERT
        /// Assertion macro which allows additional information to be displayed if required.
        #define PRASSERT(cond, ...)                                                 \
        if (!(cond))                                                                \
        {                                                                           \
            if (prAssertPrint(#cond, __FILE__, __FUNCTION__, __LINE__, __VA_ARGS__))\
            {                                                                       \
                PRBREAKPOINT();                                                     \
            }                                                                       \
        }


        /// @def PRPANIC
        /// Use when you need to halt the system in order to report a critical error.
        #define PRPANIC(fmt, ...)                                                   \
        {                                                                           \
            prPanicPrint(__FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__);       \
            PRBREAKPOINT();                                                         \
        }


        /// @def PRWARN
        /// Use when you need to issue a serious warning.
        #define PRWARN(fmt, ...)                                                    \
        {                                                                           \
            prWarnPrint(__FILE__, __FUNCTION__, __LINE__, fmt, __VA_ARGS__);        \
        }

    #endif

#else

    #define PRBREAKPOINT()
    #define PRASSERT(cond, ...)        
    #define PRPANIC(...)
    #define PRWARN(...)    

#endif//_DEBUG


/// @def PRCOMPILER_ASSERT
/// Used to check values at compile time.
#ifndef PRCOMPILER_ASSERT
#define PRCOMPILER_ASSERT(check) typedef int COMPILER_ASSERT__[(check) ? 1 : -1]
#endif


#endif//__PRASSERT_H
