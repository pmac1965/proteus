/**
 * prDebug.cpp
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
 *
 */


#include "../prConfig.h"
#include "prDebug.h"


#if defined(PLATFORM_PC)
    // Exclude MFC
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
    #endif

    #include <windows.h>
    #include <stdio.h>

#elif defined(PLATFORM_IOS)
    #include <stdio.h>

#elif defined(PLATFORM_ANDROID)
    #include <android/log.h>

#elif defined(PLATFORM_BADA)
    #include <FBase.h>

#elif defined(PLATFORM_LINUX)
    #include <stdio.h>

#else
    #error Undefined platform

#endif


#include "prTrace.h"
#include "../core/prMacros.h"


/// ---------------------------------------------------------------------------
/// Displays the last windows error message.
/// ---------------------------------------------------------------------------
void prDebugShowLastError(const char *msg)
{
#if defined(PLATFORM_PC)

    DWORD dw = GetLastError();
    if (dw != 0)
    {
        LPVOID lpMsgBuf = NULL;

        FormatMessageW
            (
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR)&lpMsgBuf,
            0,
            NULL
            );

        prTrace("============================================================\n");
        prTrace("Message   : %s\n", (msg && *msg) ? msg : "None");
        prTrace("Last error: %ls", lpMsgBuf);
        prTrace("============================================================\n");

        if (lpMsgBuf)
        {
            LocalFree(lpMsgBuf);
        }
    }

    PRUNUSED(msg);

#else

    PRUNUSED(msg);

#endif
}


// ----------------------------------------------------------------------------
// Outputs a string of text to the platforms debug output window.
// ----------------------------------------------------------------------------
void prOutputString(const char *text)
{
    if (text && *text)
    {
#if defined(PLATFORM_PC)
        #if defined(PROTEUS_ALLOW_CONSOLE)
        fprintf(stderr, "%s", text);
        #endif
        
        OutputDebugStringA(text);

#elif defined(PLATFORM_IOS)
        printf("%s", text);

#elif defined(PLATFORM_LINUX)
        printf("%s", text);

#elif defined(PLATFORM_BADA)
        AppLog("%s", text);

#elif defined(PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_DEBUG, "Proteus", "%s", text);

#else
    #error Unsupported platform.

#endif
    }
}
