/**
 * prAssert_Android.cpp
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


#if defined(PLATFORM_ANDROID)
#include <stdio.h>
#include <stdarg.h>
#include <android/log.h>
#include "prAssert.h"


#define TEXT_BUFFER_SIZE    1024


/// ---------------------------------------------------------------------------
/// Prints an assertion failure message with additional explanaton text.
/// ---------------------------------------------------------------------------
bool prAssertPrint(const char *cond, const char *file, const char *function, int line, const char *fmt, ...)
{
    char buffer [TEXT_BUFFER_SIZE];
    char message[TEXT_BUFFER_SIZE];

    const char *format = "Condition  : %s\n"
                         "Message    : %s\n"
                         "File       : %s\n"
                         "Line       : %i\n"
                         "Function   : %s\n";

    // Create message message
    if (fmt && *fmt)
    {
        va_list args;        
        va_start(args, fmt);        
        vsprintf(message, fmt, args);    
        va_end(args);
    }
    else
    {
        sprintf(message, "Assert: No message\n");
    }

    sprintf(buffer, format, cond, message, file, line, function);

    // Display
    __android_log_write(ANDROID_LOG_VERBOSE,  "Proteus", buffer);

    return true;
}


/// ---------------------------------------------------------------------------
/// Prints a message.
/// ---------------------------------------------------------------------------
void prPanicPrint(const char *file, const char *function, int line, const char *fmt, ...)
{
    char buffer [TEXT_BUFFER_SIZE];
    char message[TEXT_BUFFER_SIZE];

    const char *format = "PANIC:\n"
                         "Message    : %s\n"
                         "File       : %s\n"
                         "Line       : %i\n"
                         "Function   : %s\n";

    // Create message message
    if (fmt && *fmt)
    {
        va_list args;        
        va_start(args, fmt);        
        vsprintf(message,  fmt, args);    
        va_end(args);
    }
    else
    {
        sprintf(message, "PanicPrint: No message\n");
    }

    sprintf(buffer, format, message, file, line, function);

    // Display
    __android_log_write(ANDROID_LOG_VERBOSE,  "Proteus", buffer);
}


/// ---------------------------------------------------------------------------
/// Prints a message.
/// ---------------------------------------------------------------------------
void prWarnPrint(const char *file, const char *function, int line, const char *fmt, ...)
{
    char buffer [TEXT_BUFFER_SIZE];
    char message[TEXT_BUFFER_SIZE];

    const char *format = "WARNING:\n"
                         "Message    : %s\n"
                         "File       : %s\n"
                         "Line       : %i\n"
                         "Function   : %s\n";

    // Create message message
    if (fmt && *fmt)
    {
        va_list args;        
        va_start(args, fmt);        
        vsprintf(message,  fmt, args);    
        va_end(args);
    }
    else
    {
        sprintf(message, "WarnPrint: No message\n");
    }

    sprintf(buffer, format, message, file, line, function);

    // Display
    __android_log_write(ANDROID_LOG_VERBOSE,  "Proteus", buffer);
}


#endif
