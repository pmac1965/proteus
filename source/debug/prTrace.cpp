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


#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "prDebug.h"
#include "../core/prCore.h"
#include "../core/prCoreSystem.h"
#include "../core/prRegistry.h"
#include "../core/prStringUtil.h"
#include "../file/prFileSystem.h"


/// ---------------------------------------------------------------------------
/// Defines
/// ---------------------------------------------------------------------------
#define MSG_BUFFER_SIZE     1024
#define RPT_BUFFER_SIZE     256
#define TRACE_LOG_NAME      "trace.txt"


/// ---------------------------------------------------------------------------
/// Locals
/// ---------------------------------------------------------------------------
namespace
{
    static bool enabled      = true;                    // Is logging enabled?
    static bool duplicates   = false;                   // Remove duplicate error messages?
    static int  outputted    = 0;                       // Specifies the number of unique messages output
    static int  outputLength = 0;                       // Length of the previously output message
    static char output[MSG_BUFFER_SIZE] = { '\0' };     // Buffer to store the previously output message
}


/// ---------------------------------------------------------------------------
/// Write a trace message to file if enabled.
/// ---------------------------------------------------------------------------
void prTraceWriteToFile(bool repeat, const char *bufferRpt, const char *bufferMsg)
{
#if defined(PLATFORM_PC)

    prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
    if (reg)
    {
        if (prStringCompare(reg->GetValue("LogToFile"), "true") == CMP_EQUALTO &&
            prStringCompare(reg->GetValue("Exit"), "true")      != CMP_EQUALTO)
        {
            FILE *fp = fopen(TRACE_LOG_NAME, "a");
            if (fp)
            {
                if (repeat)
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

#endif
}


/// ---------------------------------------------------------------------------
/// Outputs a debug string.
/// ---------------------------------------------------------------------------
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


            // Write to console.=
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
            prTraceWriteToFile(repeat, bufferRpt, bufferMsg);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Allows tracing to be enabled/disabled.
/// ---------------------------------------------------------------------------
void prTraceEnable(bool state)
{
    enabled = state;
}


/// ---------------------------------------------------------------------------
/// Enables or disables duplicate string output.
/// ---------------------------------------------------------------------------
void prTraceRemoveDuplicates(bool state)
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
