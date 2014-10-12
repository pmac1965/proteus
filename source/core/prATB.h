// File: prATB.h
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


#ifndef __PRATB_H
#define __PRATB_H


#include "../prConfig.h"


#if defined(PLATFORM_PC) && defined(PROTEUS_USE_ANT_TWEAK_BAR)


// Exclude MFC
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef WIN32_EXTRA_LEAN
#define WIN32_EXTRA_LEAN
#endif


#include <windows.h>


// Function: prATBInit
//      Initialises the ant tweak bar
//
// Notes:
//      *This is PC only code*
//
// Notes:
//      Handled by the engine do not call
void prATBInit();

// Function: prATBIsInit
//      Tests if the ant tweak bar has been initialised
//
// Notes:
//      *This is PC only code*
bool prATBIsInit();

// Function: prATBUpdate
//      Updates the ant tweak bar
//
// Parameters:
//      hwnd    - Handle to the application window
//      msg     - A windows message
//      wParam  - Message data
//      lParam  - Message data
//
// Returns:
//      true if ant tweak bar handles the message.
//
// Notes:
//      *This is PC only code*
//
// Notes:
//      Handled by the engine do not call
bool prATBUpdate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Function: prATBDraw
//      Draws the ant tweak bar
//
// Notes:
//      *This is PC only code*
//
// Notes:
//      Handled by the engine do not call
void prATBDraw();

// Function: prATBSetShow
//      Shows/hides ant tweak bar
//
// Notes:
//      *This is PC only code*
void prATBSetShow(bool state);

// Function: prATBGetShow
//      Get the ant tweak bar show/hide flag
//
// Notes:
//      *This is PC only code*
bool prATBGetShow();

#else

// These external calls are macros for easy removal
// All other callls should trigger a compiler error
#define prATBIsInit()       false
#define prATBGetShow()      false
#define prATBSetShow(state)

#endif


#endif//__PRATB_H
