// File: prMouse.h
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


#ifndef __PRMOUSE_H
#define __PRMOUSE_H


#include "../prConfig.h"


#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX)


#if defined(PLATFORM_PC)
    // Exclude MFC
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef WIN32_EXTRA_LEAN
    #define WIN32_EXTRA_LEAN
    #endif

    #include <windows.h>

    // Defines
    #define KEY_SHIFT       MK_SHIFT
    #define KEY_CONTROL     MK_CONTROL

#elif defined(PLATFORM_LINUX)
    // Defines
    #define KEY_SHIFT       0
    #define KEY_CONTROL     0

#else
    // Error
    #error Undefined platform

#endif


#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"
#include "../debug/prDebug.h"
#include "../math/prPoint.h"


// Enum: prMouseButton
//      The mouse buttons.
//
//      MOUSE_BUTTON_LEFT   - Left button
//      MOUSE_BUTTON_RIGHT  - Right button
//      MOUSE_BUTTON_MIDDLE - Middle button
//      MOUSE_BUTTON_X1     - Extra 1 button (Side buttons)
//      MOUSE_BUTTON_X2     - Extra 2 button (Side buttons)
typedef enum prMouseButton
{
#if defined(PLATFORM_PC)
    // Buttons
    MOUSE_BUTTON_LEFT   = MK_LBUTTON,
    MOUSE_BUTTON_RIGHT  = MK_RBUTTON,
    MOUSE_BUTTON_MIDDLE = MK_MBUTTON,

    #if (_WIN32_WINNT >= 0x0500)
        MOUSE_BUTTON_X1 = MK_XBUTTON1,
        MOUSE_BUTTON_X2 = MK_XBUTTON2,
    #else
        MOUSE_BUTTON_X1 = 0x0020,
        MOUSE_BUTTON_X2 = 0x0040,
    #endif

#elif defined(PLATFORM_LINUX)
    // Buttons
    MOUSE_BUTTON_LEFT   = 0x00001,
    MOUSE_BUTTON_RIGHT  = 0x00002,
    MOUSE_BUTTON_MIDDLE = 0x00004,
    MOUSE_BUTTON_X1     = 0x0020,
    MOUSE_BUTTON_X2     = 0x0040,

#else
    // Error
    #error Undefined platform

#endif
} prMouseButton;


// Class: prMouse
//      Basic class which represents the mouse.
//
// Notes:
//      This class is best suited for tools, and other
//      standard desktop applications.
//      For games use the prTouch class.
//
// See Also
//      <prTouch>
class prMouse :  public prCoreSystem
{
public:
    // Constructor: prMouse
    //      Ctor
    prMouse();

    // Destructor: ~prMouse
    //      Dtor
    ~prMouse();

    // Method: Reset
    //      Resets all the data which is useful when the mouse is outside the client rect.
    void Reset();

    // Method: Update
    //      Updates the mouse.
    //
    // Notes:
    //      This method is called by the engine. *You should not call it*
    void Update();

    // Method: ButtonPressed
    //      Determines if one or more mouse buttons have been pressed.
    //
    // Notes:
    //      The buttons must all have been pressed.
    //
    // Parameters:
    //      buttons - The buttons to test for
    //
    // See Also:
    //      <prMouseButton>
    bool ButtonPressed(prMouseButton buttons) const { return ((m_buttonsPressed & buttons) == (u32)buttons); }

    // Method: ButtonReleased
    //      Determines if one or more mouse buttons have been released.
    //
    // Notes:
    //      The buttons must all have been released.
    //
    // Parameters:
    //      buttons - The buttons to test for
    //
    // See Also:
    //      <prMouseButton>
    bool ButtonReleased(prMouseButton buttons) const { return ((m_buttonsReleased & buttons) == (u32)buttons); }

    // Method: ButtonDown
    //      Determines if one or more mouse buttons are down.
    //
    // Notes:
    //      All buttons must be down.
    //
    // Parameters:
    //      buttons - The buttons to test for
    //
    // See Also:
    //      <prMouseButton>
    bool ButtonDown(prMouseButton buttons) const { return ((m_buttonsDown & buttons) == (u32)buttons); }

    // Method: GetPosition
    //      Returns the mouse position.
    prPoint3 GetPosition() const { return prPoint3(x, y, 0); }

    // Method: GetPosition
    //      Receives mouse data messages from the application message handler.
    //
    // Notes:
    //      This method is called by the engine. *You should not call it*
    void SetMouseData(s32 x, s32 y, u32 flags);

    // Method: SetInRect
    //      Sets whether the mouse cursor is in the client rect.
    //
    // Notes:
    //      This method is called by the engine. *You should not call it*
    void SetInRect(bool state) { m_inrect = state; }

    // Method: ShowSystemCursor
    //      Shows the system cursor.
    void ShowSystemCursor(bool show);

    // Method: MouseWheelUpdate
    //      Updates the wheel mouse
    //
    // Notes:
    //      This method is called by the engine. *You should not call it*
    void MouseWheelUpdate(s32 dir, s32 clicks);

    // Method: IsVisible
    //      Determines if the system cursor is hidden.
    bool IsVisible() const { return m_visible; }

    // Method: IsInRect
    //      Determines if the mouse cursor in the client rect.
    bool IsInRect() const { return m_inrect; }

    // Method: IsPressingShift
    // @brief      Determines if the 'shift' key is being pressed
    //
    // Notes:
    //      Will only be set if the key is being held before a mouse key is pressed.
    bool IsPressingShift() const { return m_shift; }

    // Method: IsPressingControl
    //      Determines if the 'control' key is being pressed
    //
    // Notes:
    //      Will only be set if the key is being held before a mouse key is pressed.
    bool IsPressingControl() const { return m_control; }

    // Method: HasMouseWheelMoved
    //      Determines if the mouse wheel has moved.
    bool HasMouseWheelMoved() const { return m_mouseWheelMoved; }

    // Method: GetMouseWheelDir
    //      Gets the direction the wheel mouse moved in.
    s32 GetMouseWheelDir() const { return m_wheelDir; }

    // Method: GetMouseWheelClicks
    //      Gets the number of clicks moved in the specified direction.
    s32 GetMouseWheelClicks() const { return m_wheelClicks; }


public:

    // Variable: x
    //      Public mouse X position.
    s32     x;
                                    
    // Variable: y
    //      Public mouse Y position.
    s32     y;

private:

    s32     m_xPos;                 // Receiver data.
    s32     m_yPos;                 // Receiver data.
    u32     m_flags;                // Receiver data.

    s32     m_wheelDir;             // Wheel motion directions.
    s32     m_wheelClicks;          // Wheel motion clicks in the specified direction.

    u32     m_buttonsPressed;       // Buttons pressed.
    u32     m_buttonsReleased;      // Buttons released.
    u32     m_buttonsPrev;          // Previous button flags
    u32     m_buttonsDown;          // Current button flags

    bool    m_shift;                // Is the shift key being pressed? 
                                    // NOTE; Will only be set if the shift key is being held before a mouse key is pressed.

    bool    m_control;              // Is the control key being pressed?
                                    // NOTE; Will only be set if the control key is being held before a mouse key is pressed.

    bool    m_visible;              // Determines if the system mouse cursor is visible.
    bool    m_reset;                // Reset flag data.

    bool    m_inrect;               // Is the mouse cursor in the client rect.
    bool    m_mouseWheelMoved;      // Has the mouse wheel moved
    bool    m_mouseWheelReset;      // Time to reset mouse wheel data?
    bool    m_exp0;                 // Expansion
};


#endif//PLATFORM_PC || PLATFORM_LINUX


#endif//__PRMOUSE_H
