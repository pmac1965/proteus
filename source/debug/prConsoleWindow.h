// File: prConsoleWindow.h
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


#pragma once


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
#include <stdio.h>


// Class: prConsoleWindow
//      Class to represent a console window.
class prConsoleWindow
{
public:
    // Typedef: prConsoleColour
    //
    //  ConsoleColourBlack          - Black
    //  ConsoleColourDarkRed        - Dark RED
    //  ConsoleColourDarkGreen      - Dark GREEN
    //  ConsoleColourDarkBlue       - Dark BLUE
    //  ConsoleColourDarkWhite      - Dark WHITE
    //  ConsoleColourDarkYellow     - Dark YELLOW
    //  ConsoleColourDarkCyan       - Dark CYAN
    //  ConsoleColourDarkMagenta    - Dark MAGENTA
    //  ConsoleColourLightRed       - Light RED
    //  ConsoleColourLightGreen     - Light GREEN
    //  ConsoleColourLightBlue      - Light BLUE
    //  ConsoleColourLightWhite     - Light WHITE
    //  ConsoleColourLightYellow    - Light YELLOW
    //  ConsoleColourLightCyan      - Light CYAN
    //  ConsoleColourLightMagenta   - Light MAGENTA
    typedef enum prConsoleColour
    {
        ConsoleColourBlack,
        ConsoleColourDarkRed,
        ConsoleColourDarkGreen,
        ConsoleColourDarkBlue,
        ConsoleColourDarkWhite,
        ConsoleColourDarkYellow,
        ConsoleColourDarkCyan,
        ConsoleColourDarkMagenta,
        ConsoleColourLightRed,
        ConsoleColourLightGreen,
        ConsoleColourLightBlue,
        ConsoleColourLightWhite,
        ConsoleColourLightYellow,
        ConsoleColourLightCyan,
        ConsoleColourLightMagenta
    };


public:
    // Method: prConsoleWindow
    //      Creates the console window.
    //
    // Parameters:
    //      name    - The window name 
    //      width   - The window width
    //      height  - The window height
    prConsoleWindow(const char *name, int width, int height);

    // Method: ~prConsoleWindow
    //      Destructor.
    ~prConsoleWindow();

    // Method: DisplayCloseMessage
    //      Displays an exit message and waits for the user to press a key.
    //
    // Notes:
    //      Usage is optional
    void DisplayCloseMessage();

    // Method: SetConsoleColours
    //      Sets the console text and background colours
    //
    // Parameters:
    //      foreground - The foreground colour to use
    //      background - The background colour to use
    //
    // See Also:
    //      <prConsoleColour>
    void SetConsoleColours(prConsoleColour foreground, prConsoleColour background);


private:
    // Sets the text colour
    WORD SetTextColour(prConsoleColour colour);

    // Sets the background colour
    WORD SetBackgroundColour(prConsoleColour colour);

    // Closes all the consoles input/output handles.
    void CloseHandles();


private:
    HANDLE  scrbuf;
    FILE   *fstdout;
    FILE   *fstdin;
    FILE   *fstderr;
    int     mConsoleWidth; 
    int     mConsoleHeight; 
};


#endif//PLATFORM_PC
