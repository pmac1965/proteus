// File: prUtility_PC.h
/**
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


#ifndef __PRUTILITY_PC_H
#define __PRUTILITY_PC_H


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


// Function: prDisableMinimizeButton
//      Disable window button
void prDisableMinimizeButton(HWND hwnd);
    
// Function: prEnableMinimizeButton
//      Enable window button
void prEnableMinimizeButton(HWND hwnd);
    
// Function: prDisableMaximizeButton
//      Disable window button
void prDisableMaximizeButton(HWND hwnd);

// Function: prEnableMaximizeButton
//      Enable window button
void prEnableMaximizeButton(HWND hwnd);

// Function: prDisableCloseButton
//      Disable window button
void prDisableCloseButton(HWND hwnd);

// Function: prEnableCloseButton
//      Enable window button
void prEnableCloseButton(HWND hwnd);


#endif//PLATFORM_PC


#endif//__PRUTILITY_PC_H
