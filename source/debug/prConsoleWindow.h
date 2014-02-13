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


#ifndef __PRDEBUGCONSOLE_H
#define __PRDEBUGCONSOLE_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <stdio.h>


// Class: prConsoleWindow
//      Class to represent a console window.
class prConsoleWindow
{
public:

    // Method: prConsoleWindow
    //      Creates the console window.
    //
    // Parameters:
    //      name    - The window name 
    //      width   - The window width
    //      height  - The window height
    prConsoleWindow(const TCHAR *name, int width, int height);


    // Method: ~prConsoleWindow
    //      Destructor.
    ~prConsoleWindow();


    // Method: DisplayCloseMessage
    //      Displays an exit message and waits for the user to press a key.
    void DisplayCloseMessage();


private:

    // Method: CloseHandles
    //      Closes all the consoles input/output handles.
    void CloseHandles();


private:

    HANDLE   scrbuf;
    FILE    *fstdout;
    FILE    *fstdin;
    FILE    *fstderr;
};


#endif//PLATFORM_PC


#endif//__PRDEBUGCONSOLE_H
