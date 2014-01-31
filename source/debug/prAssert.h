/**
 * @file       prAssert.h
 * @brief      Contains assertion and error message macros.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRASSERT_H
#define __PRASSERT_H


#include "../prConfig.h"


#if defined(_DEBUG) || defined(DEBUG)


    // Do not call these functions directly.
    bool prAssertPrint(const char *cond, const char *file, const char *function, int line, const char *fmt = 0, ...);
    void prPanicPrint(const char *file, const char *function, int line, const char *fmt = 0, ...);
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

    #else
        #error No platform defined.

    #endif


    #if defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_ANDROID)

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
