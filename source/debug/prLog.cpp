/**
 * prLog.cpp
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
#include "../debug/prDebug.h"
#include "prLog.h"
#include <stdarg.h>
#include <stdio.h>


namespace
{
    bool    enabled = false;
}


// We use the trace function to write to file.
extern void prTraceWriteToFile(bool repeat, const char *bufferRpt, const char *bufferMsg);


/// ---------------------------------------------------------------------------
/// Outputs a debug string to an error log window.
/// The actual location is platform dependant, but will generally be the debuggers output window.
/// ---------------------------------------------------------------------------
void prLog(const char *fmt, ...)
{
    if (enabled)
    {
        if (fmt && *fmt)
        {
            char output[512];

            // Format the output.
            va_list args;
            va_start(args, fmt);
            vsnprintf(output, sizeof(output) - 1, fmt, args);
            va_end(args);

            prOutputString(output);

            prTraceWriteToFile(false, NULL, output);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Enables logging.
/// ---------------------------------------------------------------------------
void prLogEnable(bool state)
{
    enabled = state;
}

