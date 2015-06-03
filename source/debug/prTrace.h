// File: prTrace.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#pragma once


// Enum: prLogLevel
//      The log levels are designed to limit the amount of information logged
//
//  - LogVerbose      - Verbose logs all messages. This is the lowest log level
//  - LogDebug        - Logs messages of debug level or above
//  - LogInformation  - Logs messages of information level or above
//  - LogWarning      - Logs messages of warning level or above
//  - LogError        - Logs messages of error level or above
typedef enum prLogLevel
{
    LogVerbose,
    LogDebug,
    LogInformation,
    LogWarning,
    LogError

} prLogLevel;


// Function: prTrace 
//      Outputs a debug string to an error log window.
//      The actual location is platform dependant, but will generally be the debuggers output window.
//
// Notes:
//      Use the macros versions for removeable logging.
//
// Parameters:
//      level   - The log level.
//      fmt     - The format string.
//      ...     - Format string parameters.
//
// See Also:
//      <prLogLevel>
void prTrace(prLogLevel level, const char* fmt, ...);

// Function: prTraceEnable 
//      Allows tracing to be enabled/disabled. By default tracing is enabled
//
// Parameters:
//      state - true or false.
void prTraceEnable(bool state);

// Function: prTraceRemoveDuplicates 
//      Enables or disables duplicate string output. This prevents logging from spamming the debug output window.
//
// Parameters:
//      state - true or false.
void prTraceRemoveDuplicates(bool state);

// Function: prTraceLogClear 
//      Deletes the old file log.
//
// Notes:
//      This function is called by the engine on startup. You do not need to call
void prTraceLogClear();

// Function: prTraceSetLogLevel 
//      Sets the trace log level
void prTraceSetLogLevel(prLogLevel level);


// Macro versions
#if (defined(_DEBUG) || defined(DEBUG))
    #if defined(PLATFORM_PC)
        #define PRLOGV(msg, ...)        prTrace(LogVerbose,     msg, __VA_ARGS__)
        #define PRLOGD(msg, ...)        prTrace(LogDebug,       msg, __VA_ARGS__)
        #define PRLOGI(msg, ...)        prTrace(LogInformation, msg, __VA_ARGS__)
        #define PRLOGW(msg, ...)        prTrace(LogWarning,     msg, __VA_ARGS__)
        #define PRLOGE(msg, ...)        prTrace(LogError,       msg, __VA_ARGS__)

    #elif defined(PLATFORM_ANDROID)
        #define PRLOGV(msg, args...)    prTrace(LogVerbose,     msg, ## args)
        #define PRLOGD(msg, args...)    prTrace(LogDebug,       msg, ## args)
        #define PRLOGI(msg, args...)    prTrace(LogInformation, msg, ## args)
        #define PRLOGW(msg, args...)    prTrace(LogWarning,     msg, ## args)
        #define PRLOGE(msg, args...)    prTrace(LogError,       msg, ## args)

    #endif

#else
    // Macro: PRLOGV(msg, ...)
    //      Logs a verbose log level message
    //
    // Parameters:
    //      msg - The format string.
    //      ... - Format string parameters.
    //
    // See Also:
    //      <prLogLevel>
    #define PRLOGV(msg, ...)

    // Macro: PRLOGD(msg, ...)
    //      Logs a debug log level message
    //
    // Parameters:
    //      msg - The format string.
    //      ... - Format string parameters.
    //
    // See Also:
    //      <prLogLevel>
    #define PRLOGD(msg, ...)

    // Macro: PRLOGI(msg, ...)
    //      Logs an information log level message
    //
    // Parameters:
    //      msg - The format string.
    //      ... - Format string parameters.
    //
    // See Also:
    //      <prLogLevel>
    #define PRLOGI(msg, ...)

    // Macro: PRLOGW(msg, ...)
    //      Logs a warning log level message
    //
    // Parameters:
    //      msg - The format string.
    //      ... - Format string parameters.
    //
    // See Also:
    //      <prLogLevel>
    #define PRLOGW(msg, ...)

    // Macro: PRLOGE(msg, ...)
    //      Logs an error log level message
    //
    // Parameters:
    //      msg - The format string.
    //      ... - Format string parameters.
    //
    // See Also:
    //      <prLogLevel>
    #define PRLOGE(msg, ...)

#endif
