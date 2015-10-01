/**
 * prUtility_PC.cpp
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


#include "prUtility_PC.h"
#include "../debug/prDebug.h"


/// ---------------------------------------------------------------------------
/// Disable window button
/// ---------------------------------------------------------------------------
void prDisableMinimizeButton(HWND hwnd)
{
    PRASSERT(hwnd);
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MINIMIZEBOX);
}
    

/// ---------------------------------------------------------------------------
/// Enable window button
/// ---------------------------------------------------------------------------
void prEnableMinimizeButton(HWND hwnd)
{
    PRASSERT(hwnd);
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) | WS_MINIMIZEBOX);
}
    

/// ---------------------------------------------------------------------------
/// Disable window button
/// ---------------------------------------------------------------------------
void prDisableMaximizeButton(HWND hwnd)
{
    PRASSERT(hwnd);
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & ~WS_MAXIMIZEBOX);
}


/// ---------------------------------------------------------------------------
/// Enable window button
/// ---------------------------------------------------------------------------
void prEnableMaximizeButton(HWND hwnd)
{
    PRASSERT(hwnd);
    SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) | WS_MAXIMIZEBOX);
}


/// ---------------------------------------------------------------------------
/// Disable window button
/// ---------------------------------------------------------------------------
void prDisableCloseButton(HWND hwnd)
{
    PRASSERT(hwnd);
    EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
}


/// ---------------------------------------------------------------------------
/// Enable window button
/// ---------------------------------------------------------------------------
void prEnableCloseButton(HWND hwnd)
{
    PRASSERT(hwnd);
    EnableMenuItem(GetSystemMenu(hwnd, FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);
}


#endif
