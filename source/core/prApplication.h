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


#ifndef __PRAPPLICATION_H
#define __PRAPPLICATION_H


#include "prTypes.h"


// Forward declarations
class prWindow;


/** 
 * Interface class for all the application type classes.
 *
 * The class for a specification application type should implement,
 *
 * DisplayCreate and Run. The game class should implement the rest
 *
 * @see    DisplayCreate
 * @see    Run
 */
class prApplication
{
public:
    
    /**
     * Ctor
     */
    prApplication();

    /**
     * Dtor
     */
    virtual ~prApplication();

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
    virtual PRBOOL DisplayCreate(u32 width, u32 height, const char *pWindowName) = 0;

    /** 
     * Updates the application.
     *
     * @param      dt - Delta time
     */
    virtual void Update(f32 dt) = 0;

    /**
     * Draws the application.
     */
    virtual void Draw() = 0;

    /**
     * Runs the game.
     *
     * @note       Depending on the platform this function may the run the game continuously, and
     * @note       may not exit until the game is complete, or
     * @note       on other platforms it will be called once per frame.
     *
     * @return     PRTRUE if the game is running.
     * @return     PRFALSE if the game is NOT running.
     */
    virtual PRBOOL Run() = 0;

    /**
     * Notification callback that is received when the application is
     *
     * being activated.
     */
    virtual void OnActivate() = 0;

    /**
     * Notification callback that is received when the application is
     *
     * being deactivated.
     */
    virtual void OnDeactivate() = 0;

    /**
     * Notification callback that is received when the application is
     *
     * exitting.
     */
    virtual void OnExit() = 0;

    /**
     * Notification callback that is received when the application is
     *
     * low on memory. Allows the game to free memory if possible
     */
    virtual void OnLowMemory() = 0;

    /**
     * Gets a pointer to the applications window.
     *
     * @return     Pointer to the applications window.
     * @return     May return NULL.
     */
    prWindow *GetAppWindow() const { return m_pWindow; }

    /**
     * Gets the applications running status.
     *
     * @return     PRTRUE if the game is running.
     * @return     PRFALSE if the game is NOT running.
     */
    PRBOOL GetIsRunning() const { return m_running; }


protected:
    
    prWindow   *m_pWindow;              ///< The applications window
    PRBOOL      m_running;              ///< Whether than app is running or not
};


#endif//__PRAPPLICATION_H
