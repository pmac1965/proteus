// File: prWindowProcedure.h
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


#ifndef __PRWINDOWPROCEDURE_H
#define __PRWINDOWPROCEDURE_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


// Exclude MFC
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WIN32_EXTRA_LEAN
#define WIN32_EXTRA_LEAN
#endif


#include <windows.h>


//  Forward declarations
class prApplication;

// Function: prSetApplicationForWindowProcedure
//      Pointer to allow game to receive specific messages like Activate and Deactivate.
//
// Parameters:
//      app - A pointer to the current application
void prSetApplicationForWindowProcedure(prApplication *app);


// Function: prWindowProcedure
//      The default window message handler.
//
// Parameters:
//      hwnd    - Handle to the application window
//      msg     - A windows message
//      wParam  - Message data
//      lParam  - Message data
//
// Returns:
//      Returns zero if a message is handled, otherwise the function returns the value returned
//      by the default window procedure
LRESULT CALLBACK prWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif//PLATFORM_PC


#endif//__PRWINDOWPROCEDURE_H
