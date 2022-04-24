// File: prApplication.h
//      The base interface class for all the application classes.
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


#include "prTypes.h"


// Forward declarations
class prWindow;


namespace Proteus {
namespace Core {

    // Class: prApplication
    //      The base interface class for all the application classes.
    //
    // Notes:
    //      The platform specific class will implement <DisplayCreate> and <Run>.
    //
    //      Your application/game class needs to implement the rest
    //
    // See Also:
    //      <DisplayCreate> and <Run>
    //
    // See Also:
    //      <prApplication_PC>, <prApplication_Android>, <prApplication_Ios> and <prApplication_Linux>
    class prApplication
    {
    public:    
        // Method: prApplication
        //      Ctor    
        prApplication();

        // Method: ~prApplication
        //      Dtor    
        virtual ~prApplication() {}

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
        // Notes:
        //      Some applications do not need to create a display. For example command line tools
        //
        // Returns:
        //      PRTRUE if the display was created successfully.
        //      PRFALSE if the display was NOT created successfully.
        virtual PRBOOL DisplayCreate(u32 width, u32 height, const char *pWindowName, bool fullscreen) = 0;

        // Method: Update
        //      Updates the application.
        //
        // Parameters:
        //      dt - Delta time
        virtual void Update(f32 dt) = 0;

        // Method: Draw
        //      Draws the application.
        virtual void Draw() = 0;

        // Method: Run
        //      Runs the game.
        //
        // Notes:
        //      Depending on the platform this function may the run the game continuously, and
        //      may not exit until the game is complete, or
        //      on other platforms it will be called once per frame.
        //
        // Returns:
        //      PRTRUE if the game is running.
        //      PRFALSE if the game is NOT running.
        virtual PRBOOL Run() = 0;

        // Method: OnActivate
        //      Notification callback that is received when the application is     
        //      being activated.
        virtual void OnActivate() = 0;

        // Method: OnDeactivate
        //      Notification callback that is received when the application is     
        //      being deactivated.
        virtual void OnDeactivate() = 0;

        // Method: OnExit
        //      Notification callback that is received when the application is     
        //      exitting.
        virtual void OnExit() = 0;

        // Method: OnLowMemory
        //      Notification callback that is received when the application is     
        //      low on memory. Allows the game to free memory if possible
        virtual void OnLowMemory() = 0;

        // Method: GetAppWindow
        //      Gets a pointer to the applications window.
        //
        // Returns:
        //      Pointer to the applications window.
        //      May return NULL.
        prWindow *GetAppWindow() const { return m_pWindow; }

        // Method: GetIsRunning
        //      Gets the applications running status.
        //
        // Returns:
        //      PRTRUE if the game is running.
        // Returns:
        //      PRFALSE if the game is NOT running.
        PRBOOL GetIsRunning() const { return m_running; }

        // Method: BackKeyPressed
        //      Lets the application know the back key was pressed.
        //
        // Notes:
        //      Intended for use in menu navigation and game exit.
        //
        // Notes:
        //      Only usable in devices with back keys.
        //
        // Returns:
        //      Returns false by default as its up to the user what to
        //      do with the back key.
        virtual bool BackKeyPressed() { return false; }

    protected:    
        prWindow   *m_pWindow;              // The applications window
        PRBOOL      m_running;              // Whether the app is running or not
    };

}}
