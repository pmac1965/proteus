/**
 * prAlert.cpp
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
#include "../core/prStringUtil.h"
#include "prAlert.h"
#include "prAssert.h"
#include "prLog.h"


#if defined(PLATFORM_PC)
    // Exclude MFC
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
    #endif

    #include <windows.h>

#else


#endif


/// ---------------------------------------------------------------------------
/// Shows a cross platfrom error message dialog
/// ---------------------------------------------------------------------------
void prAlertShowError(const char *title, const char *message)
{
    PRASSERT(title && *title);
    PRASSERT(message && *message);

    char buffer[256];

#if defined(PLATFORM_PC)

    prLog("=======================================================\n");
    prLog("ALERT\n");
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prLog("=======================================================\n");

    // Create error dialog
    MessageBoxA(HWND_DESKTOP, message, title, MB_OK | MB_ICONERROR | MB_TASKMODAL);

#else

    prLog("=======================================================\n");
    prLog("ALERT\n");
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prLog("=======================================================\n");

#endif
}


/// ---------------------------------------------------------------------------
/// Shows a cross platfrom information message dialog
/// ---------------------------------------------------------------------------
void prAlertShowInformation(const char *title, const char *message)
{
    PRASSERT(title && *title);
    PRASSERT(message && *message);

    char buffer[256];

#if defined(PLATFORM_PC)

    prLog("=======================================================\n");
    prLog("ALERT\n");
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prLog("=======================================================\n");

    // Create error dialog
    MessageBoxA(HWND_DESKTOP, message, title, MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);

#else

    prLog("=======================================================\n");
    prLog("ALERT\n");
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prLog(prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prLog("=======================================================\n");

#endif
}
