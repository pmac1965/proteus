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
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <conio.h>
#include <iostream>
#include "prConsoleWindow.h"
#include "prTrace.h"
#include "prDebug.h"
#include "../debug/prAssert.h"


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
prConsoleWindow::prConsoleWindow(const char *name, int width, int height)
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
    SetConsoleTitleA(name);

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

    mConsoleWidth   = width;
    mConsoleHeight  = height;

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
/// Sets the console text and background colours
/// ---------------------------------------------------------------------------
void prConsoleWindow::SetConsoleColours(prConsoleColour foreground, prConsoleColour background)
{
    if (scrbuf != INVALID_HANDLE_VALUE)
    {
        WORD fore = SetTextColour(foreground);
        WORD back = SetBackgroundColour(background);
        SetConsoleTextAttribute(scrbuf, fore | back);

        //DWORD written = 0;
        //COORD writeCoord = {0};
        //WORD attribute;
        //for (int y = 0; y < mConsoleHeight; y++)
        //{
        //    for (int x = 0; x < mConsoleWidth; x++)
        //    {
        //        writeCoord.X = x; 
        //        writeCoord.Y = y;
        //        ReadConsoleOutputAttribute(scrbuf, &attribute, 1, writeCoord, &written);
        //        attribute &= 0xFF0F;    // Clear background color
        //        attribute |= back;      // Change to new background color
        //        FillConsoleOutputAttribute(scrbuf, attribute, 1, writeCoord, &written);
        //    }
        //}
    }
    else
    {
        prTrace(prLogLevel::LogError, "Failed to set console colours. Invalid screen buffer\n");
    }
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
WORD prConsoleWindow::SetTextColour(prConsoleColour colour)
{
    WORD attribs = 0;

    switch(colour)
    {
    case ConsoleColourDarkRed:
        attribs = FOREGROUND_RED;
        break;

    case ConsoleColourDarkGreen:
        attribs = FOREGROUND_GREEN;
        break;

    case ConsoleColourDarkBlue:
        attribs = FOREGROUND_BLUE;
        break;

    case ConsoleColourDarkWhite:
        attribs = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;

    case ConsoleColourDarkYellow:
        attribs = FOREGROUND_RED | FOREGROUND_GREEN;
        break;

    case ConsoleColourDarkCyan:
        attribs = FOREGROUND_GREEN | FOREGROUND_BLUE;
        break;

    case ConsoleColourDarkMagenta:
        attribs = FOREGROUND_RED | FOREGROUND_BLUE;
        break;

    case ConsoleColourLightRed:
        attribs = FOREGROUND_RED | FOREGROUND_INTENSITY;
        break;

    case ConsoleColourLightGreen:
        attribs = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;

    case ConsoleColourLightBlue:
        attribs = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;

    case ConsoleColourLightWhite:
        attribs = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;

    case ConsoleColourLightYellow:
        attribs = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
        break;

    case ConsoleColourLightCyan:
        attribs = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;

    case ConsoleColourLightMagenta:
        attribs = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
        break;

    default:
        break;
    }        

    return attribs;
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
WORD prConsoleWindow::SetBackgroundColour(prConsoleColour colour)
{
    WORD attribs = 0;

    switch(colour)
    {
    case ConsoleColourDarkRed:
        attribs = BACKGROUND_RED;
        break;

    case ConsoleColourDarkGreen:
        attribs = BACKGROUND_GREEN;
        break;

    case ConsoleColourDarkBlue:
        attribs = BACKGROUND_BLUE;
        break;

    case ConsoleColourDarkWhite:
        attribs = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        break;

    case ConsoleColourDarkYellow:
        attribs = BACKGROUND_RED | BACKGROUND_GREEN;
        break;

    case ConsoleColourDarkCyan:
        attribs = BACKGROUND_GREEN | BACKGROUND_BLUE;
        break;

    case ConsoleColourDarkMagenta:
        attribs = BACKGROUND_RED | BACKGROUND_BLUE;
        break;

    case ConsoleColourLightRed:
        attribs = BACKGROUND_RED | BACKGROUND_INTENSITY;
        break;

    case ConsoleColourLightGreen:
        attribs = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;

    case ConsoleColourLightBlue:
        attribs = BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;

    case ConsoleColourLightWhite:
        attribs = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;

    case ConsoleColourLightYellow:
        attribs = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY;
        break;

    case ConsoleColourLightCyan:
        attribs = BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;

    case ConsoleColourLightMagenta:
        attribs = BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY;
        break;

    default:
        break;
    }        

    return attribs;
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
