/**
 * prAssert_Android.cpp
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
