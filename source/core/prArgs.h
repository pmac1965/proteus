// File: prArgs.h
/*
    Copyright 2014 Paul Michael McNab

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef __PRARGS_H
#define __PRARGS_H


#include "../prConfig.h"
#include "prTypes.h"


#if defined(PLATFORM_PC)
    #include <Windows.h>
    #include <tchar.h>

    
    // Unknown parameter callback
    typedef bool (*UnknownParamCB)(const _TCHAR* arg, s32 index);


    // Function: prArgsParseCommandLine
    //      Processes command line args
    //
    // Notes:
    //      The unknown parameter handler is cleared after this function
    //      is called. *This is the windows version*
    //
    // Parameters:
    //      lpCmdLine - The args
    void prArgsParseCommandLine(LPTSTR lpCmdLine);

    // Function: prArgsPopNextArg
    //      Retrieves an argument if there is one
    //
    // Notes:
    //      This is the windows version
    const _TCHAR *prArgsPopNextArg();

#else

    // Unknown parameter callback
    typedef bool (*UnknownParamCB)(const char* arg, s32 index);


    // Function: prArgsParseCommandLine
    //      Processes command line args
    //
    // Notes:
    //      The unknown parameter handler is cleared after this function
    //      is called.
    //
    // Parameters:
    //      argc - The number of args
    //      args - The arguments array
    void prArgsParseCommandLine(int argc, char *[]args);

    // Function: prArgsPopNextArg
    //      Retrieves an argument if there is one
    const char *prArgsPopNextArg();

#endif


// Function: prArgsRegisterUnknownParamHandler
//      Sets the unknown parameter callback handler
//
// Parameters:
//      cb - Pointer to the callback handler
void prArgsRegisterUnknownParamHandler(UnknownParamCB cb);

// Function: prArgsGetTotalArgCount
//      Returns the total number of arguments
//
// Notes:
//      This function is only of use in the unknown parameter callback handler as
//      the returned data is cleared after the args are processed.
//
// See Also:
//      <prArgsRegisterUnknownParamHandler>
//
// Returns:
//      Returns -1 when not processing args, else
//      will return a number greater or equal to zero
s32 prArgsGetTotalArgCount();

// Function: prArgsGetRemainingArgCount
//      Returns the total number of arguments remaining to be processed
//
// Notes:
//      This function is only of use in the unknown parameter callback handler as
//      the returned data is cleared after the args are processed.
//
// See Also:
//      <prArgsRegisterUnknownParamHandler>
//
// Returns:
//      Returns -1 when not processing args, else
//      will return a number greater or equal to zero
s32 prArgsGetRemainingArgCount();

// Function: prArgsParseFailed
//      Determines if command line parsing failed
//
// Notes:
//      Requires that the unknown parameter callback is setup and
//      used correctly. *An example can be seen in the sammple code*
//
// Returns:
//      true if parsing failed
//      false if parsing reported no errors
bool prArgsParseFailed();

// Function: prArgsShowHelp
//      Shows the default help file
//
// Notes:
//      Since most of the command line options control debug related
//      functionality, the help information is not displayed on release
//      builds.
void prArgsShowHelp();


#endif//__PRARGS_H