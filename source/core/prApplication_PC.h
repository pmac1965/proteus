// File: prApplication_PC.h
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


#ifndef __PRAPPLICATIONPC_H
#define __PRAPPLICATIONPC_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include "prTypes.h"
#include "prApplication.h"
#include <Windows.h>


// Forward declarations
class prConsoleWindow;
class prGameTime;


// Namespaces
namespace Proteus {
namespace Core {


// Class: prApplication_PC
//      Application class for the PC
//
// Notes:
//      Overrides DisplayCreate and Run
//
// See Also:
//      <DisplayCreate>
//      <Run>
class prApplication_PC : public prApplication
{
public:
    // Method: prApplication_PC
    //      Ctor    
    prApplication_PC();

    // Method: ~prApplication_PC
    //      Dtor    
    virtual ~prApplication_PC();

    // Method: DisplayCreate
    //      Creates the application display.
    //
    // Parameters:
    //      width       - Display width.
    //      height      - Display height.
    //      pWindowName - The name of the window, which will only appear in windowed apps
    //      fullscreen  - Create a full screen application if possible
    //
    // Notes:
    //      This call will destroy the previous display if it exists.
    //
    // Returns:
    //      PRTRUE if the display was created successfully.
    //      PRFALSE if the display was NOT created successfully.
    PRBOOL DisplayCreate(u32 width, u32 height, const char *pWindowName, bool fullscreen);

    // Method: DisplayCreateTool
    //      Creates the application display for tools
    //
    // Parameters:
    //      width       - Display width.
    //      height      - Display height.
    //      menuID      - The ID of the menu to use. 0 to ignore
    //      iconID      - The ID of the icon to use. 0 to ignore
    //      accelID     - The ID of the accelerator to use. 0 to ignore
    //      pWindowName - The name of the window, which will only appear in windowed apps
    //
    // Notes:
    //      This call will destroy the previous display if it exists.
    //
    // Returns:
    //      PRTRUE if the display was created successfully.
    //      PRFALSE if the display was NOT created successfully.
    PRBOOL DisplayCreateTool(u32 width, u32 height, u32 menuID, u32 iconID, u32 accelID, const TCHAR *pWindowName);

    // Method: Run
    //      Runs the game.
    //
    // Notes:
    //      This function will the run the game continuously, and
    //      will not exit until the game is complete.
    //
    // Returns:
    //      Will return PRFALSE when the game is complete.
    PRBOOL Run();

    // Method: WindowsCommand
    //      Passes along windows commands
    //
    // Parameters:
    //      command  - Windows command.
    //      wParam   - Windows command parameter.
    //      lParam   - Windows command parameter.
    //
    // Notes:
    //      Standard commands like WM_CLOSE are passed, as well user defined commands in WM_COMMAND
    //      Sends WM_CLOSE
    //      Sends WM_COMMAND
    //      Sends WM_NOTIFY
    //
    // Returns:
    //      You should return true if the command is handled.
    //      Otherwise return false and the command is passed to the standard handler.
    virtual bool WindowsCommand(int command, WPARAM wParam, LPARAM lParam) = 0;


private:

    const char *BuildType();
    BOOL CheckPlatform();
    bool RunningAlready(const char *pWindowName);
    bool RunningAlreadyW(const TCHAR *pWindowName);
    bool IsRemoteSession();


private:
    prGameTime         *pGameTime;
    prConsoleWindow    *m_pCW;
    HACCEL              m_hAccel;
};


}} // Namespaces


#endif//PLATFORM_PC


#endif//__PRAPPLICATIONPC_H
