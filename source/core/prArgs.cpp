/**
 * prArgs.cpp
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


#include "prArgs.h"
#include "prTypes.h"
#include "prDefines.h"
#include "prCore.h"
#include "prRegistry.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"


#if defined(PLATFORM_LINUX)
#include "../linux/prLinux.h"
#endif


namespace
{
    // Internal data
#if defined(PLATFORM_PC)
    LPWSTR         *pArgv           = NULL;

#else
    char          **pArgv           = NULL;

#endif

    UnknownParamCB  pUnknownParamCB = NULL;
    s32             paramIndex      = -1;
    s32             paramTotal      = -1;
    bool            parseFailed     = false;


    /// -----------------------------------------------------------------------
    /// Unknown command handler.
    /// -----------------------------------------------------------------------
    bool UnknownCommand(const char* argv);
    bool UnknownCommand(const char* argv)
    {
        if (argv && *argv)
        {
            if (pUnknownParamCB)
            {
                parseFailed = pUnknownParamCB(argv, paramIndex);
            }
        }
        else
        {
            prTrace("Unknown command line parameter was null or empty\n");
        }

        return parseFailed;
    }
}


#if defined(PLATFORM_PC)
/// ---------------------------------------------------------------------------
/// Processes command line args
/// ---------------------------------------------------------------------------
void prArgsParseCommandLine(LPTSTR lpCmdLine)
{
    // Init control data
    paramIndex  = 0;
    paramTotal  = 0;
    parseFailed = false;


    if (lpCmdLine && *lpCmdLine)
    {
        pArgv = CommandLineToArgvW(lpCmdLine, &paramTotal);

        prRegistry *reg  = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        if (pArgv && reg)
        {
            if (paramTotal > 0)
            {
                // Parse args
                for (; paramIndex < paramTotal; paramIndex++)
                {
                    // Is command?
                    if (pArgv[paramIndex][0] == L'-')
                    {
                        switch(pArgv[paramIndex][1])
                        {
                        // p
                        case L'p':
                            // Disable archives?
                            if (_tcsicmp(L"-prnoarc", pArgv[paramIndex]) == 0)
                            {
                                reg->SetValue("UseArchives", "false");
                            }
                            // Turn on verbose logging?
                            else if (_tcsicmp(L"-prverb", pArgv[paramIndex]) == 0)
                            {
                                reg->SetValue("Verbose", "true");
                            }
                            // Turn on file logging?
                            else if (_tcsicmp(L"-prlogfile", pArgv[paramIndex]) == 0)
                            {
                                reg->SetValue("LogToFile", "true");
                            }
                            // Show help?
                            else if (_tcsicmp(L"-prhelp", pArgv[paramIndex]) == 0)
                            {
                                reg->SetValue("Help", "true");
                                prArgsShowHelp();
                            }
                            else
                            {
                                char buffer[256];
                                size_t cnt = wcstombs(buffer, pArgv[paramIndex], sizeof(buffer));
                                buffer[cnt] = 0;
                                UnknownCommand(buffer);
                                break;
                            }
                            break;

                        default:
                            char buffer[256];
                            size_t cnt = wcstombs(buffer, pArgv[paramIndex], sizeof(buffer));
                            buffer[cnt] = 0;
                            UnknownCommand(buffer);
                            break;
                        }
                    }
                    else
                    {
                        char buffer[256];
                        size_t cnt = wcstombs(buffer, pArgv[paramIndex], sizeof(buffer));
                        buffer[cnt] = 0;
                        UnknownCommand(buffer);
                        break;
                    }
                
                    if (parseFailed)
                        break;
                }
            }

            LocalFree(pArgv);
        }
    }

    // Reset handler
    pUnknownParamCB = NULL;

    // Init control data
    pArgv       = NULL;
    paramIndex  = -1;
    paramTotal  = -1;
}


#else


/// ---------------------------------------------------------------------------
/// Processes command line args
/// ---------------------------------------------------------------------------
void prArgsParseCommandLine(int argc, const char *args[])
{
    // Init control data
    paramIndex  = 0;
    paramTotal  = 0;
    parseFailed = false;

    #if defined(PLATFORM_LINUX)
    prLinuxStoreArgs(argc, args);
    #endif
}


#endif


/// ---------------------------------------------------------------------------
/// Retrieves an argument if there is one
/// ---------------------------------------------------------------------------
void prArgsPopNextArg(char *buffer, s32 bufferSize)
{
    PRASSERT(buffer);
    PRASSERT(bufferSize > 0);

    if (pArgv && paramIndex > -1 &&
                 paramTotal > -1)
    {
        if (paramIndex < (paramTotal - 1))
        {
#if defined(PLATFORM_PC)
            size_t cnt = wcstombs(buffer, pArgv[++paramIndex], bufferSize);            
            buffer[cnt] = 0;
#else
#endif
        }
    }
}


/// ---------------------------------------------------------------------------
/// Set the handler.
/// ---------------------------------------------------------------------------
void prArgsRegisterUnknownParamHandler(UnknownParamCB cb)
{
    pUnknownParamCB = cb;
}


/// ---------------------------------------------------------------------------
/// Returns the total number of arguments
/// ---------------------------------------------------------------------------
s32 prArgsGetTotalArgCount()
{
    if (pArgv)
    {
        return paramTotal;
    }
    else
    {
        return -1;
    }
}


/// ---------------------------------------------------------------------------
/// Returns the total number of arguments remaining to be processed
/// ---------------------------------------------------------------------------
s32 prArgsGetRemainingArgCount()
{
    if (pArgv)
    {
        return (paramTotal - paramIndex) - 1;
    }
    else
    {
        return -1;
    }
}


/// ---------------------------------------------------------------------------
/// Determines if command line parsing failed
/// ---------------------------------------------------------------------------
bool prArgsParseFailed()
{
    return parseFailed;
}


/// ---------------------------------------------------------------------------
/// Shows the default help file
/// ---------------------------------------------------------------------------
void prArgsShowHelp()
{
    prTrace("-prverb        = Turns on verbose engine logging\n");
    prTrace("-prlogfile     = Turns on logging to a disk file\n");
    prTrace("-prhelp        = Displays the help text\n");
    prTrace("-prnoarc       = Disables archives\n");
}
