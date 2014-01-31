/**
 * @file       prDebug.h
 * @brief      Contains debug assistance code.
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


#ifndef __PRDEBUG_H
#define __PRDEBUG_H


#include "../prConfig.h"
#include "../core/prMacros.h"


// Displays the last windows error message.
void prDebugShowLastError(const char *msg = 0);

// Outputs a string of text to the platforms debug output window.
void prOutputString(const char *text);


// Cross platform compiler messages.
#if defined(PLATFORM_ANDROID) || defined (PLATFORM_IOS) || defined(PLATFORM_BADA) || defined (PLATFORM_LINUX)
    #define DO_PRAGMA(msg)          _Pragma(#msg)
#endif


// In code reminder messages.
#if defined(SHOW_MESSAGES)

    #if defined(PLATFORM_PC)
        // Reminder macro
        #define REMINDER                __FILE__ "(" PRCURRLINE ") : Reminder: " 
        #define TODO(msg)               __pragma(message(REMINDER msg))

    #elif defined(PLATFORM_ANDROID) || defined(PLATFORM_BADA) || defined (PLATFORM_IOS) || defined (PLATFORM_LINUX)
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
