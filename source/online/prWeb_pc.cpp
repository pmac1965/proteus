/**
 * prWeb_pc.cpp
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


#if defined(PLATFORM_PC)


#include "prWeb.h"
#include "../debug/prTrace.h"
#include <Windows.h>


#define SHELL_EXECUTE_SUCCESS   32


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWeb::prWeb()
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prWeb::~prWeb()
{
}


/// ---------------------------------------------------------------------------
/// Opens a web page in a browser
/// ---------------------------------------------------------------------------
void prWeb::OpenURL(const char *address)
{
    if (address && *address)
    {
        int result = (int)ShellExecuteA(NULL, "open", address, NULL, NULL, SW_SHOWNORMAL);

        if (result != SHELL_EXECUTE_SUCCESS)
        {
            switch(result)
            {
            case 0:
                PRLOGE("The operating system is out of memory or resources.\n");
                break;

            case ERROR_FILE_NOT_FOUND:
                PRLOGE("The specified file was not found '%s'\n", address);
                break;

            case ERROR_PATH_NOT_FOUND:
                PRLOGE("The specified path was not found '%s'\n", address);
                break;

            case ERROR_BAD_FORMAT:
                PRLOGE("The .exe file is invalid (non-Win32 .exe or error in .exe image).\n");
                break;

            case SE_ERR_ACCESSDENIED:
                PRLOGE("The operating system denied access to the specified file.\n");
                break;

            case SE_ERR_ASSOCINCOMPLETE:
                PRLOGE("The file name association is incomplete or invalid.\n");
                break;

            case SE_ERR_DDEBUSY:
                PRLOGE("The DDE transaction could not be completed because other DDE transactions were being processed.\n");
                break;

            case SE_ERR_DDEFAIL:
                PRLOGE("The DDE transaction failed.\n");
                break;

            case SE_ERR_DDETIMEOUT:
                PRLOGE("The DDE transaction could not be completed because the request timed out.\n");
                break;

            case SE_ERR_DLLNOTFOUND:
                PRLOGE("The DDE transaction could not be completed because the request timed out '%s'\n", address);
                break;

            case SE_ERR_NOASSOC:
                PRLOGE("There is no application associated with the given file name extension. This error will also be returned if you attempt to print a file that is not printable.\n");
                break;

            case SE_ERR_OOM:
                PRLOGE("There was not enough memory to complete the operation.\n");
                break;

            case SE_ERR_SHARE:
                PRLOGE("A sharing violation occurred.\n");
                break;

            default:
                PRLOGE("prWeb::OpenURL - Unknown error.\n");
                break;
            }
        }
#if defined(_DEBUG) || defined(DEBUG) 
        else
        {
            PRLOGE("Opened URL '%s'\n", address);
        }
#endif
    }
}


#endif
