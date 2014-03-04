/**
 * prConsoleWindow.cpp
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
 *
 */


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
#include <conio.h>
#include <iostream>
#include "prConsoleWindow.h"
#include "prTrace.h"
#include "prDebug.h"


// ----------------------------------------------------------------------------
// Console handler - stops a whole bunch of leaks if the user uses the console 
//                   window to close the app.
// ----------------------------------------------------------------------------
BOOL HandlerRoutine(DWORD dwCtrlType)
{
    PRUNUSED(dwCtrlType);
    return TRUE;
}


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prConsoleWindow::prConsoleWindow(const TCHAR *name, int width, int height)
{
    PRASSERT(name && *name);


    scrbuf  = INVALID_HANDLE_VALUE;
    fstdin  = 0;
    fstderr = 0;
    fstdout = 0;


    // Allocate
    if (AllocConsole() == 0)
    {
        prDebugShowLastError(PRFILE_LINE);
        return;
    }

    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, true) == 0)
    {
        prDebugShowLastError(PRFILE_LINE);
        return;
    }


    // Create
    SetConsoleTitle(name);

    scrbuf = CreateConsoleScreenBuffer
    (
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        CONSOLE_TEXTMODE_BUFFER,
        NULL
    );

    if (scrbuf == INVALID_HANDLE_VALUE)
    {
        prDebugShowLastError(PRFILE_LINE);
        FreeConsole();
        return;
    }


    // Set active
    if (SetConsoleActiveScreenBuffer(scrbuf) == 0)
    {
        prDebugShowLastError(PRFILE_LINE);
        FreeConsole();
        return;
    }


    // Set console size.
    if (width < GetSystemMetrics(SM_CXMIN))
    {
        width = GetSystemMetrics(SM_CXMIN);
    }

    if (height < GetSystemMetrics(SM_CYMIN))
    {
        height = GetSystemMetrics(SM_CYMIN);
    }

    COORD coord;
    coord.X = (SHORT)width;
    coord.Y = (SHORT)height;

    if (SetConsoleScreenBufferSize(scrbuf, coord) == 0)
    {
        prDebugShowLastError(PRFILE_LINE);
        FreeConsole();
        return;
    }


    // Redirect input/output.
    fstdin  = freopen("CONIN$",  "rt", stdin);
    fstderr = freopen("CONOUT$", "wt", stderr);
    fstdout = freopen("CONOUT$", "wt", stdout);
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prConsoleWindow::~prConsoleWindow()
{
    if (SetConsoleCtrlHandler((PHANDLER_ROUTINE)HandlerRoutine, false) == 0)
    {
        prDebugShowLastError(PRFILE_LINE);
    }

    CloseHandles();
    FreeConsole();
}


/// ---------------------------------------------------------------------------
/// Closes all the consoles input/output handles.
/// ---------------------------------------------------------------------------
void prConsoleWindow::CloseHandles()
{
    if (fstdin)
    {
        fclose(fstdin);
        fstdin = 0;
    }

    if (fstderr)
    {
        fclose(fstderr);
        fstderr = 0;
    }

    if (fstdout)
    {
        fclose(fstdout);
        fstdout = 0;
    }
}


/// ---------------------------------------------------------------------------
/// Displays an exit message and waits for the user to press a key.
/// ---------------------------------------------------------------------------
void prConsoleWindow::DisplayCloseMessage()
{
    std::cout << std::endl << "Press any key to close this console window.";
    std::cout.flush();

    while (!_kbhit())
    {
        Sleep(250);
    }
}


#endif
