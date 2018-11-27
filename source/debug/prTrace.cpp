/**
 * prTrace.cpp
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


#include <cstdarg>
#include <cstdio>
#include <cstring>
#include "prTrace.h"
#include "prDebug.h"
#include "../core/prCore.h"
#include "../core/prCoreSystem.h"
#include "../core/prRegistry.h"
#include "../core/prStringUtil.h"
#include "../core/prDefines.h"
#include "../file/prFileSystem.h"


/// ---------------------------------------------------------------------------
/// Defines
/// ---------------------------------------------------------------------------
#define MSG_BUFFER_SIZE     1024
#define RPT_BUFFER_SIZE     128
#define TRACE_LOG_NAME      "trace.txt"


//#define TRACE_TEST


/// ---------------------------------------------------------------------------
/// Locals
/// ---------------------------------------------------------------------------
namespace
{
#if (defined(_DEBUG) || defined(DEBUG))
    int         enabled         = TRUE;                     // Is logging enabled?
    int         duplicates      = TRUE;                     // Remove duplicate error messages?
    prLogLevel  logLevel        = prLogLevel::LogVerbose;   // Default log level

#else
    int         enabled         = FALSE;                    // Is logging enabled?
    int         duplicates      = TRUE;                     // Remove duplicate error messages?
    prLogLevel  logLevel        = prLogLevel::LogError;     // Default log level

#endif

    int  outputted    = 0;                                  // Specifies the number of unique messages output
    int  outputLength = 0;                                  // Length of the previously output message
    char output[MSG_BUFFER_SIZE] = { '\0' };                // Buffer to store the previously output message
}


/// ---------------------------------------------------------------------------
/// Write a trace message to file if enabled.
/// ---------------------------------------------------------------------------
void prTraceWriteToFile(int repeat, const char *bufferRpt, const char *bufferMsg)
{
#if defined(PLATFORM_PC)

    if (bufferMsg && *bufferMsg)
    {
        prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        if (reg)
        {
            // We must be allowed to log to file, and
            // the game cannot be exiting
            if (prStringCompare(reg->GetValue("LogToFile"), "true") == CMP_EQUALTO &&
                prStringCompare(reg->GetValue("Exit"), "true")      != CMP_EQUALTO)
            {
                FILE *fp = fopen(TRACE_LOG_NAME, "a");
                if (fp)
                {
                    if (repeat && bufferRpt && *bufferRpt)
                    {
                        fprintf(fp, "%s", bufferRpt);
                        fprintf(fp, "%s", bufferMsg);
                    }
                    else
                    {
                        fprintf(fp, "%s", bufferMsg);
                    }

                    fflush(fp);
                    fclose(fp);
                }
            }
        }
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Outputs a debug string.
/// ---------------------------------------------------------------------------
void prTrace(prLogLevel level, const char *fmt, ...)
{
    if (enabled)
    {
        if (fmt && *fmt)
        {
            if (level >= logLevel)
            {
                char bufferMsg[MSG_BUFFER_SIZE];
                char bufferRpt[RPT_BUFFER_SIZE];


                // Format the output.
                va_list args;
                va_start(args, fmt);
                vsnprintf(bufferMsg, MSG_BUFFER_SIZE - 1, fmt, args);
                va_end(args);


                // Duplicate removal?
                int repeat = FALSE;
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


                // Write to console.
                if (repeat)
                {
                    prOutputString(level, bufferRpt);
                    prOutputString(level, bufferMsg);
                }
                else
                {
                    prOutputString(level, bufferMsg);
                }


                // Write to a file?
                prTraceWriteToFile(repeat, bufferRpt, bufferMsg);
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Allows tracing to be enabled/disabled.
/// ---------------------------------------------------------------------------
void prTraceEnable(int state)
{
    enabled = state;
}


/// ---------------------------------------------------------------------------
/// Enables or disables duplicate string output.
/// ---------------------------------------------------------------------------
void prTraceRemoveDuplicates(int state)
{
    duplicates = state;
}


/// ---------------------------------------------------------------------------
/// Clears the old log file.
/// ---------------------------------------------------------------------------
void prTraceLogClear()
{
#if defined(PLATFORM_PC)
    
    if (prFileExist(TRACE_LOG_NAME))
    {
        prFileRemoveReadOnly(TRACE_LOG_NAME);
        prFileDelete(TRACE_LOG_NAME);
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Sets the trace log level
/// ---------------------------------------------------------------------------
void prTraceSetLogLevel(prLogLevel level)
{
    if (PRBETWEEN(level, prLogLevel::LogVerbose, prLogLevel::LogError))
    {
        logLevel = level;

#ifdef TRACE_TEST
        prOutputString("prTraceSetLogLevel: %s\n", prTraceGetLogLevel());
#endif
    }
}


/// ---------------------------------------------------------------------------
/// Gets the currently set trace log level as a string
/// ---------------------------------------------------------------------------
const char *prTraceGetLogLevel()
{
    const char *pLevel = nullptr;

    switch(logLevel)
    {
    case prLogLevel::LogVerbose:
        pLevel = "Verbose";
        break;

    case prLogLevel::LogDebug:
        pLevel = "Debug";
        break;

    case prLogLevel::LogWarning:
        pLevel = "Warning";
        break;

    case prLogLevel::LogInformation:
        pLevel = "Information";
        break;

    case prLogLevel::LogError:
        pLevel = "Error";
        break;

    default:
        pLevel = "Unknown";
        break;
    }

    return pLevel;
}


/// ---------------------------------------------------------------------------
/// Returns whether tracing to be enabled/disabled
/// ---------------------------------------------------------------------------
int prTraceIsEnabled()
{
    return enabled;
}
