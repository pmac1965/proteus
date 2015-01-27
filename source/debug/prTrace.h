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


#ifndef __PRTRACE_H
#define __PRTRACE_H


#if defined(_DEBUG) || defined(DEBUG)

// Function: prTrace 
//      Outputs a debug string to an error log window.
//      The actual location is platform dependant, but will generally be the debuggers output window.
//
// Notes:
//      The release version of this function compiles to nothing.
//
// Parameters:
//      fmt - The format string.
//      ... - Format string parameters.
void prTrace(const char* fmt, ...);

// Function: prTraceEnable 
//      Allows tracing to be enabled/disabled. By default tracing is enabled
//
// Parameters:
//      state - true or false.
//
// Notes:
//      The release version of this function compiles to nothing.
void prTraceEnable(bool state);

// Function: prTraceRemoveDuplicates 
//      Enables or disables duplicate string output. This prevents logging from spamming the debug output window.
//
// Parameters:
//      state - true or false.
//
// Notes:
//      The release version of this function compiles to nothing.
void prTraceRemoveDuplicates(bool state);

// Function: prTraceLogClear 
//      Deletes the old file log.
//      *This function is called by the engine on startup. You do not need to call*
//
// Notes:
//      The release version of this function compiles to nothing.
void prTraceLogClear();

#else

// Release versions compile to nothing
#define prTrace(fmt, ...)
#define prTraceEnable(state)
#define prTraceRemoveDuplicates(state)
#define prTraceLogClear()

#endif


#endif//__PRTRACE_H
