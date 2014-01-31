/**
 * prDebug.cpp
 * Contains debug assistance code.
 * Copyright Paul Michael McNab. All rights reserved.
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
        printf("%s", text);
        #endif
        
        OutputDebugStringA(text);

#elif defined(PLATFORM_IOS)
        printf("%s", text);

#elif defined(PLATFORM_BADA)
        AppLog("%s", text);

#elif defined(PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_DEBUG, "Proteus", text);

#else
    #error Unsupported platform.

#endif
    }
}
