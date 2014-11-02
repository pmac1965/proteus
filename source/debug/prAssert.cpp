/**
 * prAssert.cpp
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
#include "prAssert.h"


#define TEXT_BUFFER_SIZE    1024


/// ---------------------------------------------------------------------------
/// Prints an assertion failure message with additional explanaton text.
/// ---------------------------------------------------------------------------
bool prAssertPrint(const char *cond, const char *file, const char *function, int line, const char *fmt, ...)
{
    char buffer[TEXT_BUFFER_SIZE];
    char message[TEXT_BUFFER_SIZE];

    char *format = "Condition\t\t: %s\n"
        "\n"
        "Message \t\t: %s\n"
        "File    \t\t: %s\n"
        "Line    \t\t: %i\n"
        "Function\t\t: %s\n"
        "\n"
        "Do you wish to continue?\n"
        "Clicking no will trigger a breakpoint";

    // Create message message
    if (fmt && *fmt)
    {
        va_list args;
        va_start(args, fmt);
        vsprintf_s(message, sizeof(message), fmt, args);
        va_end(args);
    }
    else
    {
        sprintf_s(message, sizeof(message), "No message");
    }

    // Create dialog message
    sprintf_s(buffer, sizeof(buffer), format, cond, message, file, line, function);

    // Create dialog
    int result = MessageBoxA(HWND_DESKTOP, buffer, "Assertion failure", MB_YESNO | MB_ICONERROR | MB_TASKMODAL);

    // Stop?
    return (result == IDNO);
}


/// ---------------------------------------------------------------------------
/// Prints a message.
/// ---------------------------------------------------------------------------
void prPanicPrint(const char *file, const char *function, int line, const char *fmt, ...)
{
    char buffer[TEXT_BUFFER_SIZE];
    char message[TEXT_BUFFER_SIZE];

    char *format = "Message \t: %s\n"
        "File    \t: %s\n"
        "Line    \t: %i\n"
        "Function\t: %s\n";

    // Create message message
    if (fmt && *fmt)
    {
        va_list args;
        va_start(args, fmt);
        vsprintf_s(message, sizeof(message), fmt, args);
        va_end(args);
    }
    else
    {
        sprintf_s(message, sizeof(message), "No message");
    }

    // Create dialog message
    sprintf_s(buffer, sizeof(buffer), format, message, file, line, function);

    // Create dialog
    MessageBoxA(HWND_DESKTOP, buffer, "Panic", MB_OK | MB_ICONERROR | MB_TASKMODAL);
}


/// ---------------------------------------------------------------------------
/// Prints a message.
/// ---------------------------------------------------------------------------
void prWarnPrint(const char *file, const char *function, int line, const char *fmt, ...)
{
    char buffer[TEXT_BUFFER_SIZE];
    char message[TEXT_BUFFER_SIZE];

    char *format = "Message \t: %s\n"
        "File    \t: %s\n"
        "Line    \t: %i\n"
        "Function\t: %s\n";

    // Create message message
    if (fmt && *fmt)
    {
        va_list args;
        va_start(args, fmt);
        vsprintf_s(message, sizeof(message), fmt, args);
        va_end(args);
    }
    else
    {
        sprintf_s(message, sizeof(message), "No message");
    }

    // Create dialog message
    sprintf_s(buffer, sizeof(buffer), format, message, file, line, function);

    // Create dialog
    MessageBoxA(HWND_DESKTOP, buffer, "Warning", MB_OK | MB_ICONWARNING | MB_TASKMODAL);
}


#endif
