/**
 * prTrace.cpp
 * Contains tracing functions.
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"
#include "prDebug.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
//#include "../file/prFileSystem.h"
//#include "../core/system.h"


// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define MSG_BUFFER_SIZE     1024
#define RPT_BUFFER_SIZE     256
#define TRACE_LOG_NAME      "trace.txt"


// ----------------------------------------------------------------------------
// Locals
// ----------------------------------------------------------------------------
namespace
{
    static bool enabled      = true;                    // Is logging enabled?
    static bool duplicates   = false;                   // Remove duplicate error messages?
    static int  outputted    = 0;                       // Specifies the number of unique messages output
    static int  outputLength = 0;                       // Length of the previously output message
    static char output[MSG_BUFFER_SIZE] = { '\0' };     // Buffer to store the previously output message
}


// ----------------------------------------------------------------------------
// Outputs a debug string.
// ----------------------------------------------------------------------------
void prTrace(const char *fmt, ...)
{
    if (enabled)
    {
        if (fmt && *fmt)
        {
            char bufferMsg[MSG_BUFFER_SIZE];
            char bufferRpt[RPT_BUFFER_SIZE];


            // Format the output.
            va_list args;
            va_start(args, fmt);
            vsnprintf(bufferMsg, MSG_BUFFER_SIZE - 1, fmt, args);
            va_end(args);


            // Duplicate removal?
            bool repeat = false;
            if (duplicates)
            {
                if (strcmp(bufferMsg, output) == 0)
                {
                    outputted++;
                    return;
                }
                else
                {
                    repeat = (outputted > 0);

                    if (repeat)
                    {
                        // I know this is pedantic, but this ensures that the repeat message always starts on a new line by checking
                        // if the last character of the previous string was a carriage return.
                        const char* msg = (outputLength == 0) ? "(Repeat x %i)\n" : output[outputLength - 1] == '\n' ? "(Repeat x %i)\n" :
                                                                                                                       "\n(Repeat x %i)\n";

                        sprintf(bufferRpt, msg, outputted + 1);
                    }

                    // Reset
                    outputted = 0;

                    // Store string.
                    strcpy(output, bufferMsg);

                    // Store length
                    outputLength = (int)strlen(output);
                }
            }


            // Debugger
            if (repeat)
            {
                prOutputString(bufferRpt);
                prOutputString(bufferMsg);
            }
            else
            {
                prOutputString(bufferMsg);
            }


            // Write to a file?
/*            if (System::LogToFile())
            {
                FILE *fp = NULL;
                errno_t err = fopen_s(&fp, TRACE_LOG_NAME, "a");
                if (err == 0)
                {
                    if (fp)
                    {
                        if (repeat)
                        {
                            fprintf(fp, bufferRpt);
                            fprintf(fp, bufferMsg);
                        }
                        else
                        {
                            fprintf(fp, bufferMsg);
                        }

                        fflush(fp);
                        fclose(fp);
                    }
                }
            }*/
        }
    }
}


// ----------------------------------------------------------------------------
// Allows tracing to be enabled/disabled.
// ----------------------------------------------------------------------------
void prTraceEnable(bool state)
{
    enabled = state;
}


// ----------------------------------------------------------------------------
// Enables or disables duplicate string output.
// ----------------------------------------------------------------------------
void prTraceRemoveDuplicates(bool state)
{
    duplicates = state;
}


// ----------------------------------------------------------------------------
// Clears the old log file.
// ----------------------------------------------------------------------------
void prTraceLogClear()
{
#if defined(PLATFORM_PC)
    TODO("Re-add")
    //if (FileExist(TRACE_LOG_NAME))
    //{
    //    FileRemoveReadOnly(TRACE_LOG_NAME);
    //    FileDelete(TRACE_LOG_NAME);
    //}
#endif
}


// ----------------------------------------------------------------------------
// Outputs a debug string to an error log window.
// The actual location is platform dependant, but will generally be the debuggers output window.
// The release version of the function will still output a message.
// ----------------------------------------------------------------------------
void prLog(const char* fmt, ...)
{
}
