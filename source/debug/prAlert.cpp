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
#include "prTrace.h"


#if defined(PLATFORM_PC)
    // Exclude MFC
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
    #endif

    #include <windows.h>
    #include "../core/prWindow_PC.h"

#else


#endif


/// ---------------------------------------------------------------------------
/// Shows a cross platfrom error message dialog
/// ---------------------------------------------------------------------------
void prAlertShowError(const char *title, const char *message, prWindow *pWindow)
{
    PRASSERT(title && *title);
    PRASSERT(message && *message);

    char buffer[512];

#if defined(PLATFORM_PC)

    prTrace(LogError, "=======================================================\n");
    prTrace(LogError, "ALERT\n");
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prTrace(LogError, "=======================================================\n");

    // Get windows handle
    HWND hwnd = (pWindow == 0) ? HWND_DESKTOP : static_cast<prWindow_PC*>(pWindow)->GetWindowHandle();

    // Create error dialog
    MessageBoxA(hwnd, message, title, MB_OK | MB_ICONERROR | MB_TASKMODAL);

#else

    prTrace(LogError, "=======================================================\n");
    prTrace(LogError, "ALERT\n");
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prTrace(LogError, "=======================================================\n");

#endif
}


/// ---------------------------------------------------------------------------
/// Shows a cross platfrom information message dialog
/// ---------------------------------------------------------------------------
void prAlertShowInformation(const char *title, const char *message, prWindow *pWindow)
{
    PRASSERT(title && *title);
    PRASSERT(message && *message);

    char buffer[512];

#if defined(PLATFORM_PC)

    prTrace(LogError, "=======================================================\n");
    prTrace(LogError, "ALERT\n");
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prTrace(LogError, "=======================================================\n");

    // Get windows handle
    HWND hwnd = (pWindow == 0) ? HWND_DESKTOP : static_cast<prWindow_PC*>(pWindow)->GetWindowHandle();

    // Create error dialog
    MessageBoxA(hwnd, message, title, MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);

#else

    prTrace(LogError, "=======================================================\n");
    prTrace(LogError, "ALERT\n");
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prTrace(LogError, "=======================================================\n");

#endif
}


/// ---------------------------------------------------------------------------
/// Shows a cross platfrom warning message dialog
/// ---------------------------------------------------------------------------
void prAlertShowWarning(const char *title, const char *message, prWindow *pWindow)
{
    PRASSERT(title && *title);
    PRASSERT(message && *message);

    char buffer[512];

#if defined(PLATFORM_PC)

    prTrace(LogError, "=======================================================\n");
    prTrace(LogError, "ALERT\n");
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prTrace(LogError, "=======================================================\n");

    // Get windows handle
    HWND hwnd = (pWindow == 0) ? HWND_DESKTOP : static_cast<prWindow_PC*>(pWindow)->GetWindowHandle();

    // Create error dialog
    MessageBoxA(hwnd, message, title, MB_OK | MB_ICONWARNING | MB_TASKMODAL);

#else

    prTrace(LogError, "=======================================================\n");
    prTrace(LogError, "ALERT\n");
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Title  : %s\n", title));
    prTrace(LogError, prStringSnprintf(buffer, sizeof(buffer), "Message: %s\n", message));
    prTrace(LogError, "=======================================================\n");

#endif
}
