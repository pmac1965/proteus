/**
 * @file       prArgs.h
 * @brief      Contains the code used to parse args from the OS
 */
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


#ifndef __PRARGS_H
#define __PRARGS_H


#include "../prConfig.h"
#include "prTypes.h"


#if defined(PLATFORM_PC)
    #include <Windows.h>
    #include <tchar.h>

    /**
     * Unknown parameter callback
     */
    typedef void (*UnknownParamCB)(const _TCHAR* arg, s32 index);

    /**
     * @brief   Processes command line args
     *
     * @note    The unknown parameter handler is cleared after this function
     * @n       is called.
     *
     * @param   lpCmdLine - The args
     */
    void prArgsParseCommandLine(LPTSTR lpCmdLine);

    const _TCHAR *prArgsPopArg(s32 index);

#else

    /**
     * Unknown parameter callback
     */
    typedef void (*UnknownParamCB)(const char* arg, s32 index);

    const char *prArgsPopArg(s32 index);

#endif


/**
 * @brief   Sets the unknown parameter callback handler
 *
 * @param   cb - Pointer to the callback handler
 */
void prArgsRegisterUnknownParamHandler(UnknownParamCB cb);

/// ---------------------------------------------------------------------------
/// Return the total number of arguments
/// ---------------------------------------------------------------------------
s32 prArgsGetTotalArgCount();

/// ---------------------------------------------------------------------------
/// Return the total number of arguments remaing to be processed
/// ---------------------------------------------------------------------------
s32 prArgsGetRemainingArgCount();



#endif//__PRARGS_H
