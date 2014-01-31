/**
 * @file       prMouse.h
 * @brief      Contains basic mouse access class.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 * @note       This mouse class is best suited to standard windows application.
 * @n          For games use, you should use the prTouch class instead.
 * @n          This class is used to simulate TouchInput for 'phone' type devices.
 * @see        prTouch
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRMOUSE_H
#define __PRMOUSE_H


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
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"
#include "../debug/prDebug.h"
#include "../math/prPoint.h"


/// @enum   prMouseButton
/// @brief  The mouse buttons.
typedef enum prMouseButton
{
    MOUSE_BUTTON_LEFT   = MK_LBUTTON,           ///< Left button
    MOUSE_BUTTON_RIGHT  = MK_RBUTTON,           ///< Right button
    MOUSE_BUTTON_MIDDLE = MK_MBUTTON,           ///< Middle button

    #if (_WIN32_WINNT >= 0x0500)
        MOUSE_BUTTON_X1 = MK_XBUTTON1,          ///< Extra 1 button (Side buttons)
        MOUSE_BUTTON_X2 = MK_XBUTTON2           ///< Extra 2 button (Side buttons)
    #else
        MOUSE_BUTTON_X1 = 0x0020,               ///< Extra 1 button (Side buttons)
        MOUSE_BUTTON_X2 = 0x0040,               ///< Extra 2 button (Side buttons)
    #endif

} prMouseButton;


/// @brief      Basic class which represents the mouse.
/// @note       This class is best suited for tools, and other
/// @n          standard desktop applications.
/// @note       For games use the prTouch class.
/// @see        prTouch
class prMouse :  public prCoreSystem
{
public:
    /// @brief      Ctor.
    prMouse();

    /// @brief      Dtor.
    ~prMouse();

    /// @brief      Resets all the data which is useful when the mouse is outside the client rect.
    void Reset();

    /// Brief       Updates the mouse.
    /// @note       This method is called by the application. you should not call it.
    void Update();

    /// @brief      Determines if one or more mouse buttons have been pressed.
    /// @note       The buttons must all have been pressed.
    /// @param      buttons - The buttons to test for
    /// @see        prMouseButton
    /// @return     true or false
    bool ButtonPressed(prMouseButton buttons) const { return ((m_buttonsPressed & buttons) == (u32)buttons); }

    /// @brief      Determines if one or more mouse buttons have been released.
    /// @note       The buttons must all have been released.
    /// @param      buttons - The buttons to test for
    /// @see        prMouseButton
    /// @return     true or false
    bool ButtonReleased(prMouseButton buttons) const { return ((m_buttonsReleased & buttons) == (u32)buttons); }

    /// @brief      Determines if one or more mouse buttons are down.
    /// @note       All buttons must be down
    /// @param      buttons - The buttons to test for
    /// @see        prMouseButton
    /// @return     true or false
    bool ButtonDown(prMouseButton buttons) const { return ((m_buttonsDown & buttons) == (u32)buttons); }

    /// @brief      Returns the mouse position.
    prPoint3 GetPosition() const { return prPoint3(x, y, 0); }

    /// @brief      Receives mouse data messages from the application message handler.
    void SetMouseData(s32 x, s32 y, u32 flags);

    /// @brief      Sets whether the mouse cursor is in the client rect.
    void SetInRect(bool state) { m_inrect = state; }

    /// @brief      Shows the system cursor.
    void ShowSystemCursor(bool show);

    /// @brief      Updates the wheel mouse
    void MouseWheelUpdate(s32 dir, s32 clicks);

    /// @brief      Determines if the system cursor is hidden.
    /// @return     true or false
    bool IsVisible() const { return m_visible; }

    /// @brief      Determines if the mouse cursor in the client rect.
    /// @return     true or false
    bool IsInRect() const { return m_inrect; }

    /// @brief      Determines if a 'shift' key is being pressed
    /// @note       Will only be set if the key is being held before a mouse key is pressed.
    /// @return     true or false
    bool IsPressingShift() const { return m_shift; }

    /// @brief      Determines if a 'control' key is being pressed
    /// @note       Will only be set if the key is being held before a mouse key is pressed.
    /// @return     true or false
    bool IsPressingControl() const { return m_control; }

    /// @brief      Determines if the mouse wheel has moved.
    /// @return     true or false
    bool HasMouseWheelMoved() const { return m_mouseWheelMoved; }

    /// @brief      Gets the direction the wheel mouse moved in.
    s32 GetMouseWheelDir() const { return m_wheelDir; }

    /// @brief      Gets the number of clicks moved in the specified direction.
    s32 GetMouseWheelClicks() const { return m_wheelClicks; }


public:

    s32     x;                      ///< Mouse X position.
    s32     y;                      ///< Mouse Y position.


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


#endif//PLATFORM_PC


#endif//__PRMOUSE_H
