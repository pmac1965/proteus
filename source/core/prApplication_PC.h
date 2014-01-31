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
#include "prTypes.h"
#include "prApplication.h"


#if defined(PLATFORM_PC)
#include <Windows.h>
#endif


// Forward declarations
class prConsoleWindow;


/** Application class for the PC
 *
 * @note   Overrides DisplayCreate and Run
 *
 * @see    DisplayCreate
 * @see    Run
 */
class prApplication_PC : public prApplication
{
public:
    /**
     * Ctor    
     */
    prApplication_PC();

    /**
     * Dtor    
     */
    virtual ~prApplication_PC();

    /**
     * Creates the application display.
     *
     * @param      width       - Display width.
     * @param      height      - Display height.
     * @param      pWindowName - The name of the window, which will only appear in windowed apps
     *
     * @note       This call will destroy the previous display if it exists.
     *
     * @return     PRTRUE if the display was created successfully.
     * @return     PRFALSE if the display was NOT created successfully.
     */
    PRBOOL DisplayCreate(u32 width, u32 height, const char *pWindowName);

    /**
     * Runs the game.
     *
     * @note       This function will the run the game continuously, and
     * @note       will not exit until the game is complete.
     *
     * @return     Will return PRFALSE when the game is complete.
     */
    PRBOOL Run();

    /** Passes along windows commands
     *
     * @param      command  - Windows command.
     *
     * @note       Standard commands like WM_CLOSE are passed, as well user defined commands.
     *
     * @return     You should return true if the command is handled.
     * @return     Otherwise return false and the command is passed to the standard handler.
     */
    virtual bool WindowsCommand(int command) = 0;


private:

    prConsoleWindow    *m_pCW;
};


#endif//__PRAPPLICATIONPC_H
