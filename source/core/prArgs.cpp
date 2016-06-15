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
#include "prStringUtil.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../debug/prAlert.h"


using namespace Proteus::Core;


//#define SHOW_ARGS


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
            prTrace(LogError, "Unknown command line parameter was null or empty\n");
        }

        return parseFailed;
    }
}


#if defined(PLATFORM_PC)
/// ---------------------------------------------------------------------------
/// Processes command line args
/// ---------------------------------------------------------------------------
bool prArgsParseCommandLine(LPTSTR lpCmdLine)
{
    // Init control data
    paramIndex  = 0;
    paramTotal  = 0;
    parseFailed = false;


    if (lpCmdLine && *lpCmdLine)
    {
        // Show args
#ifdef SHOW_ARGS
        char buffer[1024];
        size_t cnt = wcstombs(buffer, lpCmdLine, sizeof(buffer));
        MessageBoxA(HWND_DESKTOP, buffer, "Args", MB_ICONINFORMATION | MB_OK);
#endif

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
                            // Turn on console?
                            else if (_tcsicmp(L"-prconsole", pArgv[paramIndex]) == 0)
                            {
                                reg->SetValue("Console", "true");
                                prTraceEnable(true);
                                prTraceRemoveDuplicates(true);
                            }
                            // Turn on file logging?
                            else if (_tcsicmp(L"-prlogfile", pArgv[paramIndex]) == 0)
                            {
                                reg->SetValue("LogToFile", "true");
                                prTraceEnable(true);
                                prTraceRemoveDuplicates(true);
                            }
                            // Log level
                            else if (_tcsicmp(L"-prloglev", pArgv[paramIndex]) == 0)
                            {
                                // Needs one arg
                                if (prArgsGetRemainingArgCount() >= 1)
                                {
                                    char buffer[256];
                                    prArgsPopNextArg(buffer, sizeof(buffer));

                                    if (prStringCompare(buffer, "verb") == 0)
                                    {
                                        prTraceSetLogLevel(prLogLevel::LogVerbose);
                                    }
                                    else if (prStringCompare(buffer, "debug") == 0)
                                    {
                                        prTraceSetLogLevel(prLogLevel::LogDebug);
                                    }
                                    else if (prStringCompare(buffer, "info") == 0)
                                    {
                                        prTraceSetLogLevel(prLogLevel::LogInformation);
                                    }
                                    else if (prStringCompare(buffer, "warn") == 0)
                                    {
                                        prTraceSetLogLevel(prLogLevel::LogWarning);
                                    }
                                    else if (prStringCompare(buffer, "error") == 0)
                                    {
                                        prTraceSetLogLevel(prLogLevel::LogError);
                                    }
                                }
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
    else
    {
        //MessageBoxW(HWND_DESKTOP, L"No arguments.", L"Error", MB_ICONERROR | MB_OK);
        return true;
    }


    // Reset handler
    pUnknownParamCB = nullptr;

    // Init control data
    pArgv       = nullptr;
    paramIndex  = -1;
    paramTotal  = -1;

    // Returns the parse failed state
    return parseFailed;
}


#else


/// ---------------------------------------------------------------------------
/// Processes command line args
/// ---------------------------------------------------------------------------
bool prArgsParseCommandLine(int argc, const char *args[])
{
    if (argc == 1)
        return true;

    // Init control data
    paramIndex  = 0;
    paramTotal  = 0;
    parseFailed = false;

    #if defined(PLATFORM_LINUX)
    prLinuxStoreArgs(argc, args);
    #endif

    // Returns the parse failed state
    return parseFailed;
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
#if defined(PLATFORM_PC)
    const char *pHelpMessage =  "-prlogfile\t\t- Turns on logging to a disk file\r\n"
                                "\t\t    Enables logging\r\n"
                                "-prconsole\t- Turns on the console window\r\n"
                                "\t\t    Enables logging\r\n"
                                "-prloglev param\t- Sets the error log level\r\n"
                                "\t\t    (Parameters)\r\n"
                                "\t\t    verb = Verbose or above\r\n"
                                "\t\t    debug = Debug or above\r\n"
                                "\t\t    info = Information or above\r\n"
                                "\t\t    warn = Warnings or above\r\n"
                                "\t\t    error = Errors\r\n"
                                "-prhelp\t\t- Displays the help text\r\n"
                                "-prnoarc\t\t- Disables archives\r\n";

    prAlertShowInformation("Help", pHelpMessage);

#else
    const char *pHelpMessage =  "-prlogfile      - Turns on logging to a disk file\n"
                                "                  Enables logging\n"
                                "-prconsole      - Turns on the console window\n"
                                "                  Enables logging\n"
                                "-prloglev param - Sets the error log level\n"
                                "                  (Parameters)\n"
                                "                  verb  = Verbose or above\n"
                                "                  debug = Debug or above\n"
                                "                  info  = Information or above\n"
                                "                  warn  = Warnings or above\n"
                                "                  error = Errors\n"
                                "-prhelp         - Displays the help text\n"
                                "-prnoarc        - Disables archives\n";

    prTrace(LogError, "%s", pHelpMessage);

#endif
}
