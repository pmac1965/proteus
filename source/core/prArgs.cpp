/**
 * prArgs.cpp
 */


#include "../prConfig.h"


#include "prArgs.h"
#include "prTypes.h"
#include "prDefines.h"
#include "prCore.h"
#include "prRegistry.h"
#include "../debug/prTrace.h"


#if defined(PLATFORM_LINUX)
#include "../linux/prLinux.h"
#endif


namespace
{
#if defined(PLATFORM_PC)
    LPWSTR         *pArgv           = NULL;
#else
    char          **pArgv           = NULL;
#endif

    UnknownParamCB  pUnknownParamCB = NULL;
    s32             paramIndex      = -1;
    s32             paramTotal      = -1;
    bool            parseFailed     = false;
}


#if defined(PLATFORM_PC)
/// ---------------------------------------------------------------------------
/// Unknown command handler.
/// ---------------------------------------------------------------------------
namespace
{
    bool UnknownCommand(const _TCHAR* argv);
    bool UnknownCommand(const _TCHAR* argv)
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
                                UnknownCommand(pArgv[paramIndex]);
                                break;
                            }
                            break;

                        default:
                            UnknownCommand(pArgv[paramIndex]);
                            break;
                        }
                    }
                    else
                    {
                        UnknownCommand(pArgv[paramIndex]);
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


/// ---------------------------------------------------------------------------
/// Retrieves an argument if there is one
/// ---------------------------------------------------------------------------
const _TCHAR *prArgsPopNextArg()
{
    if (pArgv && paramIndex > -1 &&
                 paramTotal > -1)
    {
        if (paramIndex < (paramTotal - 1))
        {
            return pArgv[++paramIndex];
        }
    }

    return NULL;
}

#else //PLATFORM_PC

/// ---------------------------------------------------------------------------
/// Processes command line args
/// ---------------------------------------------------------------------------
void prArgsParseCommandLine(int argc, const char *args[])
{
#if defined(PLATFORM_LINUX)
	prStoreArgs(argc, args);
#endif

}


/// ---------------------------------------------------------------------------
/// Retrieves an argument if there is one
/// ---------------------------------------------------------------------------
const char *prArgsPopNextArg()
{
    if (pArgv && paramIndex > -1 &&
                 paramTotal > -1)
    {
        if (paramIndex < (paramTotal - 1))
        {
            return pArgv[++paramIndex];
        }
    }

    return NULL;
}

#endif//PLATFORM_PC


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
