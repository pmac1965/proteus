/**
 * prTouch.cpp
 */


#include "../prConfig.h"


#include <stdio.h>
#include <list>
#include "prTouch.h"
#include "prMouse.h"
#include "prTouchListener.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"


// The touch states.
namespace TouchState
{
    enum
    {
        Invalid,
        None,
        Released,
        Pressed,
        Down,
    };
}


// Internal use
typedef struct TouchLocation
{
    s32 state;
    u32 id;
    s32 x;
    s32 y;

} TouchLocation;


// A touch event.
typedef struct TouchData
{
    TouchData()
    {
        state   = TouchState::None;
        px      = -1;
        py      = -1;
        x       = -1;
        y       = -1;
        id      = 0xFFFFFFFF;
        idx     = 0xFFFFFFFF;
    }

    s32 state;
    s32 px;
    s32 py;
    s32 x;
    s32 y;
    u32 id;
    u32 idx;

} TouchData;


// Implementation data
typedef struct TouchImplementation
{
    // ------------------------------------------------------------------------
    // Ctor
    // ------------------------------------------------------------------------
    TouchImplementation()
    {
        touches             = 0;
        id                  = 1;
        down                = false;
        exp0                = false;
        exp1                = false;
        exp2                = false;
        ClearListeners();
	}


    // ------------------------------------------------------------------------
    // Updates the active touches.
    // ------------------------------------------------------------------------
    void UpdateTouchData(TouchLocation &touchLocation)
    {
        // Look for an active touch
        for (int i = 0; i < MAX_TOUCHPOINTS; i++)
        {
            if (touchLocation.id == touchData[i].id)
            {
                // Invalid?
                if (touchLocation.state == TouchState::Invalid)
                {
                    touchData[i].id    = 0xFFFFFFFF;
                    touchData[i].x     = -1;
                    touchData[i].y     = -1;
                    touchData[i].state = TouchState::None;
                    return;
                }          
                // Released?
                else if (touchLocation.state == TouchState::Released)
                {
                    // Callback?
                    {
                        prTouchEvent e;
                        e.type  = PRNS prTouch::Released;
                        e.x     = touchData[i].px;
                        e.y     = touchData[i].py;
                        e.rx    = 0;
                        e.ry    = 0;
                        e.index = i;
#if defined(PLATFORM_PC)
                        e.index = 0;
#endif
                        e.id    = touchLocation.id;
                        //prLog("Released: X:%i, Y:%i, Index: %i, ID: %i\n", e.x, e.x, e.index, e.index, e.id);
                        Released(e);
                    }
                    
					touchData[i].id    = 0xFFFFFFFF;
                    touchData[i].x     = -1;
                    touchData[i].y     = -1;
                    touchData[i].state = TouchState::Released;
                    return;
                }
				else if (touchLocation.state == TouchState::Down)
				{
					// Its still down.
					touchData[i].state = TouchState::Down;
					touchData[i].px    = touchData[i].x;
					touchData[i].py    = touchData[i].y;
					touchData[i].x     = touchLocation.x;
					touchData[i].y     = touchLocation.y;

					// Callback?
					{
						prTouchEvent e;
						e.type  = PRNS prTouch::Axis;
						e.x     = touchData[i].x;
						e.y     = touchData[i].y;
						e.rx    = touchData[i].x - touchData[i].px;
						e.ry    = touchData[i].y - touchData[i].py;
						e.index = i;
#if defined(PLATFORM_PC)
                        e.index = 0;
#endif
						e.id    = touchLocation.id;
                        //prLog("Axis: X:%i, Y:%i, Index: %i, ID: %i\n", e.x, e.x, e.index, e.index, e.id);
						Axis(e);
					}
					return;
				}
                else
                {
                    return;
                }
            }
        }


        // Add new touch.
        for (int i = 0; i < MAX_TOUCHPOINTS; i++)
        {
            // Free slot?
            if (touchData[i].id == 0xFFFFFFFF)
            {
                touchData[i].id    = touchLocation.id;
                touchData[i].x     = touchLocation.x;
                touchData[i].y     = touchLocation.y;
                touchData[i].px    = touchLocation.x;
                touchData[i].py    = touchLocation.y;
                touchData[i].state = TouchState::Pressed;
                touchData[i].idx   = i;

                // Callback?
                {
                    prTouchEvent e;
                    e.type  = PRNS prTouch::Pressed;
                    e.x     = touchLocation.x;
                    e.y     = touchLocation.y;
                    e.rx    = 0;
                    e.ry    = 0;
                    e.index = i;
#if defined(PLATFORM_PC)
                        e.index = 0;
#endif
                    e.id    = touchLocation.id;
                    //prLog("Pressed: X:%i, Y:%i, Index: %i, ID: %i\n", e.x, e.x, e.index, e.index, e.id);
                    Pressed(e);
                }

                // Callback?
                {
                    prTouchEvent e;
                    e.type  = PRNS prTouch::Axis;
                    e.x     = touchData[i].x;
                    e.y     = touchData[i].y;
                    e.rx    = touchData[i].px - touchData[i].x;
                    e.ry    = touchData[i].py - touchData[i].y;
                    e.index = i;
#if defined(PLATFORM_PC)
                        e.index = 0;
#endif
                    e.id    = touchLocation.id;
                    //prLog("Axis: X:%i, Y:%i, Index: %i, ID: %i\n", e.x, e.x, e.index, e.index, e.id);
                    Axis(e);
                }                
                return;
            }
        }
    }


    // ------------------------------------------------------------------------
    // Set up optional callbacks.
    // ------------------------------------------------------------------------
    void RegisterListener(ITouchListener *listener)
    {
        PRASSERT(listener);

        // Check for duplicate entry.
        std::list<ITouchListener *>::iterator itr = listeners.begin();
        std::list<ITouchListener *>::iterator end = listeners.end();
        for (; itr != end; ++itr)
        {
            if ((*itr) == listener)
            {
                return;
            }
        }

        listeners.push_back(listener);
    }


    // ----------------------------------------------------------------------------
    // Remove callbacks.
    // ----------------------------------------------------------------------------
    void UnregisterListener(ITouchListener *listener)
    {
        PRASSERT(listener);
        listeners.remove(listener);
    }

    
    // ------------------------------------------------------------------------
    // Removes all callbacks in the touch system.
    // ------------------------------------------------------------------------
    void ClearListeners()
    {
        listeners.clear();
    }


    // ------------------------------------------------------------------------
    // Gets the listener count.
    // ------------------------------------------------------------------------
    s32 GetListenerCount()
    {
        return listeners.size();
    }


    // ------------------------------------------------------------------------
    // Call the callbacks.
    // ------------------------------------------------------------------------
    void Axis(const prTouchEvent &e)
    {
        std::list<ITouchListener *>::iterator itr = listeners.begin();
        std::list<ITouchListener *>::iterator end = listeners.end();
        for (; itr != end; ++itr)
        {
            (*itr)->InputAxis(e);
        }
    }


    // ------------------------------------------------------------------------
    // Call the callbacks.
    // ------------------------------------------------------------------------
    void Pressed(const prTouchEvent &e)
    {
        std::list<ITouchListener *>::iterator itr = listeners.begin();
        std::list<ITouchListener *>::iterator end = listeners.end();
        for (; itr != end; ++itr)
        {
            (*itr)->InputPressed(e);
        }
    }


    // ------------------------------------------------------------------------
    // Call the callbacks.
    // ------------------------------------------------------------------------
    void Released(const prTouchEvent &e)
    {
        std::list<ITouchListener *>::iterator itr = listeners.begin();
        std::list<ITouchListener *>::iterator end = listeners.end();
        for (; itr != end; ++itr)
        {
            (*itr)->InputReleased(e);
        }
    }


    // ------------------------------------------------------------------------
    // Test for down already.
    // ------------------------------------------------------------------------
    bool IsDownAlready(u32 id)
    {
        bool down = false;
        // Look for an active touch
        for (int i = 0; i < MAX_TOUCHPOINTS; i++)
        {
            if (id == touchData[i].id)
            {
                down = true;
                break;
            }
        }

        return down;
    }

    // ------------------------------------------------------------------------
    // Data
    // ------------------------------------------------------------------------
	s32             touches;
    u32             id;
    bool            down;
    bool            exp0;
    bool            exp1;
    bool            exp2;
    TouchData       touchData[MAX_TOUCHPOINTS];

    std::list<ITouchListener *> listeners;
 
} TouchImplementation;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prTouch::prTouch()  : prCoreSystem(PRSYSTEM_TOUCH, "prTouch")
                    , pImpl (new TouchImplementation())
                    , imp(*pImpl)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prTouch::~prTouch()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prTouch::Update()
{
    PRASSERT(pImpl);

#if defined(PLATFORM_PC)
    prMouse *pMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));

    if (!imp.down)
    {
        // First press?
        if (pMouse->ButtonDown(MOUSE_BUTTON_LEFT))
        {
            imp.down    = true;
            imp.touches = 1;

            TouchLocation touch;
            touch.state = TouchState::Pressed;
            touch.x     = pMouse->x;
            touch.y     = pMouse->y;
            touch.id    = imp.id++;
            imp.UpdateTouchData(touch);
        }
    }
    else
    {
        // Released?
        if (!pMouse->ButtonDown(MOUSE_BUTTON_LEFT))
        {
            imp.down    = false;
            imp.touches = 0;

            TouchLocation touch;
            touch.state = TouchState::Released;
            touch.x     = -1;
            touch.y     = -1;
            touch.id    = imp.id - 1;
            imp.UpdateTouchData(touch);
        }
        // Axis movement
        else
        {
            imp.down    = true;
            imp.touches = 1;

            TouchLocation touch;
            touch.state = TouchState::Down;
            touch.x     = pMouse->x;
            touch.y     = pMouse->y;
            touch.id    = imp.id;
            imp.UpdateTouchData(touch);
        }
    }

#endif    
}


/// ---------------------------------------------------------------------------
/// Returns the number of simultaneous touch events occurring.
/// ---------------------------------------------------------------------------
s32 prTouch::TouchCount() const
{
    PRASSERT(pImpl);
    return imp.touches;
}


/// ---------------------------------------------------------------------------
/// Set up optional callbacks.
/// ---------------------------------------------------------------------------
void prTouch::RegisterListener(ITouchListener *listener)
{
    PRASSERT(pImpl);
    imp.RegisterListener(listener);
}


/// ---------------------------------------------------------------------------
/// Remove callbacks.
/// ---------------------------------------------------------------------------
void prTouch::UnregisterListener(ITouchListener *listener)
{
    PRASSERT(pImpl);
    imp.UnregisterListener(listener);
}


/// ---------------------------------------------------------------------------
/// Removes all callbacks in the touch system.
/// ---------------------------------------------------------------------------
void prTouch::ClearListeners()
{
    PRASSERT(pImpl);
    imp.ClearListeners();
}


/// ---------------------------------------------------------------------------
/// Update touches.
/// ---------------------------------------------------------------------------
void prTouch::TouchBegin(u32 id, float x, float y)
{
    PRASSERT(pImpl);

#if defined(PLATFORM_IPHONE) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID)
    
    // Test down event duplication
/*    if (imp.IsDownAlready(id))
    {
        return;
    }//*/

    imp.down = true;
    
    imp.touches++;

    TouchLocation touch;
    touch.state = TouchState::Pressed;
    touch.x     = x;
    touch.y     = y;
    touch.id    = id;
    imp.UpdateTouchData(touch);

#elif defined(WIN32)

    PRUNUSED(id);
    PRUNUSED(x);
    PRUNUSED(y);

#else

    #error No platform defined.

#endif
}


/// ---------------------------------------------------------------------------
/// Update touches.
/// ---------------------------------------------------------------------------
void prTouch::TouchMove(u32 id, float x, float y)
{
    PRASSERT(pImpl);

#if defined(PLATFORM_IPHONE) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID)

    TouchLocation touch;
    touch.state = TouchState::Down;
    touch.x     = x;
    touch.y     = y;
    touch.id    = id;
    imp.UpdateTouchData(touch);

#elif defined(WIN32)

    PRUNUSED(id);
    PRUNUSED(x);
    PRUNUSED(y);

#else

    #error No platform defined.

#endif
}


/// ---------------------------------------------------------------------------
/// Update touches.
/// ---------------------------------------------------------------------------
void prTouch::TouchEnd(u32 id, float x, float y)
{
    PRASSERT(pImpl);

#if defined(PLATFORM_IPHONE) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID)

    imp.touches--;
    imp.down = (imp.touches > 0);

    TouchLocation touch;
    touch.state = TouchState::Released;
    touch.x     = x;
    touch.y     = y;
    touch.id    = id;
    imp.UpdateTouchData(touch);

#elif defined(WIN32)

    PRUNUSED(id);
    PRUNUSED(x);
    PRUNUSED(y);

#else

    #error No platform defined.

#endif
}


/// ---------------------------------------------------------------------------
/// Gets the listener count.
/// ---------------------------------------------------------------------------
int prTouch::GetListenerCount()
{
    return imp.GetListenerCount();
}
