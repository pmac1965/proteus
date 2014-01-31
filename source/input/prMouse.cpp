/**
 * prMouse.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include "prMouse.h"
#include "../core/prCore.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"


/// ---------------------------------------------------------------------------
/// Construct.
/// ---------------------------------------------------------------------------
prMouse::prMouse() : prCoreSystem(PRSYSTEM_MOUSE, "prMouse")
{
    Reset();
    ShowSystemCursor(true);
    m_visible = true;
}


/// ---------------------------------------------------------------------------
/// Destruct.
/// ---------------------------------------------------------------------------
prMouse::~prMouse()
{
    ShowSystemCursor(true);
}


/// ---------------------------------------------------------------------------
/// Resets all the data which is useful when the mouse is outside the client rect.
/// ---------------------------------------------------------------------------
void prMouse::Reset()
{
    // Position.
    x                 = 0;
    y                 = 0;

    // Wheel.
    m_wheelDir        = 0;   
    m_wheelClicks     = 0; 
    m_mouseWheelMoved = false;
    m_mouseWheelReset = false;

    // Keys.
    m_shift           = false;
    m_control         = false;

    // Buttons.
    m_buttonsPrev     = 0;
    m_buttonsPressed  = 0;
    m_buttonsReleased = 0;
    m_buttonsDown     = 0;

    // Data receiving.
    m_xPos            = 0;
    m_yPos            = 0;
    m_flags           = 0;
    m_reset           = false;
    m_inrect          = false;
    m_exp0            = false;
}


/// ---------------------------------------------------------------------------
/// Updates the mouse.
/// ---------------------------------------------------------------------------
void prMouse::Update()
{
    // Set position.
    x = m_xPos;
    y = m_yPos;


    // Shift/control pressed?
    m_shift   = ((m_flags & MK_SHIFT)   == MK_SHIFT);
    m_control = ((m_flags & MK_CONTROL) == MK_CONTROL);
    

    // Set the key states.
    m_buttonsPrev     = m_buttonsDown;
    m_buttonsPressed  = m_flags & (m_flags ^ m_buttonsDown);
    m_buttonsReleased = m_buttonsPrev & (m_buttonsPrev ^ m_flags);
    m_buttonsDown     = m_flags;


    // As we don't receive data continuously,
    // we only reset the flag bits when updated.
    m_reset = true;


    // Update the mouse wheel
    if (m_mouseWheelMoved)
    {
        if (m_mouseWheelReset)
        {
            m_mouseWheelMoved = false;
            m_mouseWheelReset = false;
            m_wheelDir        = 0;   
            m_wheelClicks     = 0; 
        }
        else
        {
            // Clear mouse wheel data next update
            m_mouseWheelReset = true;
        }
    }

    //prTrace("prMouse: %i %i %i %i %i\n", x, y, m_buttonsPressed, m_buttonsReleased, m_buttonsDown);
}


/// ---------------------------------------------------------------------------
/// Receives mouse data messages from the window message handler.
/// ---------------------------------------------------------------------------
void prMouse::SetMouseData(s32 x, s32 y, u32 flags)
{
    if (m_reset)
    {
        m_reset = false;
        m_flags = 0;
    }

    m_xPos   = x;
    m_yPos   = y;
    m_flags |= flags;
}


/// ---------------------------------------------------------------------------
/// Shows the system cursor.
/// ---------------------------------------------------------------------------
void prMouse::ShowSystemCursor(bool show)
{
    if (show)
    {
        // Test if mouse is attached!
        if (ShowCursor(TRUE) != -1)
        {
            while(ShowCursor(TRUE) < 0)
            {}
        }
    }
    else
    {
        // Test if mouse is attached!
        if (ShowCursor(FALSE) != -1)
        {
            while(ShowCursor(FALSE) >= 0)
            {}
        }
    }

    m_visible = show;
}


/// ---------------------------------------------------------------------------
/// Updates the wheel mouse
/// ---------------------------------------------------------------------------
void prMouse::MouseWheelUpdate(s32 dir, s32 clicks)
{
    if (!m_mouseWheelMoved)
    {
        m_mouseWheelMoved = true;
        m_mouseWheelReset = false;
        m_wheelDir        = dir;   
        m_wheelClicks     = clicks; 
    }
}


#endif
