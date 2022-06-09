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
    #include "prConsoleWindow.h"

#elif defined(PLATFORM_IOS)
    #include <stdio.h>

#elif defined(PLATFORM_MAC)
    #include <stdio.h>

#elif defined(PLATFORM_ANDROID)
    #include <android/log.h>

#elif defined(PLATFORM_LINUX)
    #include <stdio.h>

#else
    #error Undefined platform

#endif


#include "prTrace.h"
#include "../core/prMacros.h"


// Platform specific consoles
#if defined(PLATFORM_PC)
namespace
{
    prConsoleWindow *pConsole = nullptr;
}
#endif


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

        prTrace(prLogLevel::LogError, "============================================================\n");
        prTrace(prLogLevel::LogError, "Message   : %s\n", (msg && *msg) ? msg : "None");
        prTrace(prLogLevel::LogError, "Last error: %ls\n", lpMsgBuf);
        prTrace(prLogLevel::LogError, "============================================================\n");

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


/// ---------------------------------------------------------------------------
/// Allows the console windows to be registered with the debug system.
/// ---------------------------------------------------------------------------
void prDebugRegisterConsoleWindow(void *pConsoleWindow)
{
#if defined(PLATFORM_PC)
    if (pConsoleWindow)
    {
        pConsole = reinterpret_cast<prConsoleWindow *>(pConsoleWindow);
    }
#endif
}


/// ---------------------------------------------------------------------------
/// Outputs a string of text to the platforms debug output window.
/// ---------------------------------------------------------------------------
void prOutputString(prLogLevel level, const char *text)
{
    if (text && *text)
    {
#if defined(PLATFORM_PC)
        
        switch(level)
        {
        case prLogLevel::LogVerbose:
            //if (pConsole) { pConsole->SetConsoleColours(prConsoleWindow::ConsoleColourLightCyan, prConsoleWindow::ConsoleColourBlack); }
            fprintf(stderr, "%s", text);
            break;

        case prLogLevel::LogDebug:
            //if (pConsole) { pConsole->SetConsoleColours(prConsoleWindow::ConsoleColourLightWhite, prConsoleWindow::ConsoleColourBlack); }
            fprintf(stderr, "%s", text);
            break;

        case prLogLevel::LogInformation:
            //if (pConsole) { pConsole->SetConsoleColours(prConsoleWindow::ConsoleColourLightGreen, prConsoleWindow::ConsoleColourBlack); }
            fprintf(stderr, "%s", text);
            break;

        case prLogLevel::LogWarning:
            //if (pConsole) { pConsole->SetConsoleColours(prConsoleWindow::ConsoleColourLightYellow, prConsoleWindow::ConsoleColourBlack); }
            fprintf(stderr, "%s", text);
            break;

        case prLogLevel::LogError:
            //if (pConsole) { pConsole->SetConsoleColours(prConsoleWindow::ConsoleColourLightRed, prConsoleWindow::ConsoleColourBlack); }
            fprintf(stderr, "%s", text);
            break;
        }
        
        OutputDebugStringA(text);

#elif defined(PLATFORM_IOS)
        PRUNUSED(level);
        printf("%s", text);
        
#elif defined(PLATFORM_MAC)
        PRUNUSED(level);
        printf("%s", text);

#elif defined(PLATFORM_LINUX)
        PRUNUSED(level);
        printf("%s", text);

#elif defined(PLATFORM_ANDROID)
        switch(level)
        {
        case LogVerbose:
            __android_log_print(ANDROID_LOG_VERBOSE, "Proteus", "%s", text);
            break;

        case LogDebug:
            __android_log_print(ANDROID_LOG_DEBUG, "Proteus", "%s", text);
            break;

        case LogInformation:
            __android_log_print(ANDROID_LOG_INFO, "Proteus", "%s", text);
            break;

        case LogWarning:
            __android_log_print(ANDROID_LOG_WARN, "Proteus", "%s", text);
            break;

        case LogError:
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "%s", text);
            break;
        }

#else
    #error Unsupported platform.

#endif
    }
}
