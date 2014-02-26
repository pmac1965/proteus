/**
 * prATB.cpp
 */


#include "../prConfig.h"
#include "prATB.h"


#if defined(PLATFORM_PC) && defined(PROTEUS_USE_ANT_TWEAK_BAR)


#include "../tool/AntTweakBar.h"
#include "../debug/prTrace.h"


// Local data
namespace
{
    bool            antTweakBarInit = false;
    bool            antTweakBarShow = true;
}


/// ---------------------------------------------------------------------------
/// Initialises ant tweak bar
/// ---------------------------------------------------------------------------
void prATBInit()
{
    if (!antTweakBarInit)
    {
        int res = TwInit(TW_OPENGL, NULL);
        if (res == 0)
        {
            prTrace("prATBInit error: %i\n", TwGetLastError());
            return;
        }

        antTweakBarInit = true;
    }
}


/// ---------------------------------------------------------------------------
/// Tests if the ant tweak bar has been initialised
/// ---------------------------------------------------------------------------
bool prATBIsInit()
{
    return antTweakBarInit;
}


/// ---------------------------------------------------------------------------
/// Updates ant tweak bar
/// ---------------------------------------------------------------------------
bool prATBUpdate(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (antTweakBarInit &&
        antTweakBarShow)
    {
        if (TwEventWin(hwnd, msg, wParam, lParam))
        {
            return true;
        }
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Draws ant tweak bar
/// ---------------------------------------------------------------------------
void prATBDraw()
{
    if (antTweakBarInit &&
        antTweakBarShow)
    {
        TwDraw();
    }
}


/// ---------------------------------------------------------------------------
/// Shows/hides ant tweak bar
/// ---------------------------------------------------------------------------
void prATBSetShow(bool state)
{
    antTweakBarShow = state;
}


/// ---------------------------------------------------------------------------
/// Get the ant tweak bar show/hide flag
/// ---------------------------------------------------------------------------
bool prATBGetShow()
{
    return antTweakBarShow;
}


#endif
