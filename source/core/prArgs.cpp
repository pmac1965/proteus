/**
 * prArgs.cpp
 */


#include "prArgs.h"
#include "../debug/prTrace.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"


namespace
{
    UnknownParamCB  pUnknownParamCB = NULL;
    s32             paramIndex      = 0;
    s32             paramTotal      = 0;
}


#if defined(PLATFORM_PC)
/// ---------------------------------------------------------------------------
/// Unknown command handler.
/// ---------------------------------------------------------------------------
namespace
{
    static void UnknownCommand(const _TCHAR* argv);
    static void UnknownCommand(const _TCHAR* argv)
    {
        PRASSERT(argv);
        if (argv && *argv)
        {
            if (pUnknownParamCB)
            {
                pUnknownParamCB(argv, paramIndex);
            }

            prTrace("Unknown command line parameter: %ls\n", argv);
		    return;
        }

	    prTrace("Unknown command line parameter was null or empty\n");
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

    if (lpCmdLine && *lpCmdLine)
    {
        LPWSTR     *argv = CommandLineToArgvW(lpCmdLine, &paramTotal);
        prRegistry *reg  = (prRegistry *)prCoreGetComponent(PRSYSTEM_REGISTRY);

        if (argv && reg)
        {
            if (paramTotal > 0)
            {
                // Parse args
                for (; paramIndex < paramTotal; paramIndex++)
                {
                    // Is command?
                    if (argv[paramIndex][0] == L'-')
                    {
                        switch(argv[paramIndex][1])
                        {
                        // n
                        case L'n':
                            if (_tcsicmp(L"-noarc", argv[paramIndex]) == 0)
                            {
                                reg->SetValue("UseArchives", "false");
                            }
                            else
                            {
                                UnknownCommand(argv[paramIndex]);
                            }
                            break;

                        // v
                        case L'v':
                            if (_tcsicmp(L"-verb", argv[paramIndex]) == 0)
                            {
                                reg->SetValue("Verbose", "true");
                            }
                            else
                            {
                                UnknownCommand(argv[paramIndex]);
                            }
                            break;

                        // l
                        case L'l':
                            if (_tcsicmp(L"-logfile", argv[paramIndex]) == 0)
                            {
                                reg->SetValue("LogToFile", "true");
                            }
                            else
                            {
                                UnknownCommand(argv[paramIndex]);
                            }
                            break;

                        // h
                        case L'h':
                            if (_tcsicmp(L"-help", argv[paramIndex]) == 0)
                            {
                                reg->SetValue("Help", "true");
                            }
                            else
                            {
                                UnknownCommand(argv[paramIndex]);
                            }
                            break;

                        default:
                            UnknownCommand(argv[paramIndex]);
                            break;
                        }
                    }
                    else
                    {
                        UnknownCommand(argv[paramIndex]);
                    }
                }
            }

            LocalFree(argv);
        }
    }

    // Reset handler
    pUnknownParamCB = NULL;
    
    // Init control data
    paramIndex  = 0;
    paramTotal  = 0;
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
const _TCHAR *prArgsPopArg(s32 index)
{
    return NULL;
}

#else

/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
const char *prArgsPopArg(s32 index)
{
    return NULL;
}

#endif


/// ---------------------------------------------------------------------------
/// Set the handler.
/// ---------------------------------------------------------------------------
void prArgsRegisterUnknownParamHandler(UnknownParamCB cb)
{
    pUnknownParamCB = cb;
}


/// ---------------------------------------------------------------------------
/// Return the total number of arguments
/// ---------------------------------------------------------------------------
s32 prArgsGetTotalArgCount()
{
    return paramTotal;
}


/// ---------------------------------------------------------------------------
/// Return the total number of arguments remaing to be processed
/// ---------------------------------------------------------------------------
s32 prArgsGetRemainingArgCount()
{
    return (paramTotal - paramIndex) - 1;
}
