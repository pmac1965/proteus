/**
 * prWindowProcedure.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include "prWindowProcedure.h"
#include "prTypes.h"
#include "prMacros.h"
#include "prApplication.h"
#include "prApplication_PC.h"
#include "prWindow.h"
#include "prCore.h"
#include "prATB.h"
#include "prRegistry.h"
#include "prStringUtil.h"
#include "prCoreSystem.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../input/prMouse.h"
#include "../input/prKeyboard_PC.h"


using namespace Proteus::Core;


// Use ant tweak bar.
#if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
#include "../tool/AntTweakBar.h"
#endif


// Local data
namespace
{
    bool                            mouseCaptured       = false;
    Proteus::Core::prApplication   *pApp                = NULL;
    WNDPROC                         originalWinProc     = NULL;
    const TCHAR                    *characters          = NULL;
    BOOL                            enableTextFilter    = FALSE;
}


// Local functions
namespace 
{
    void MouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam);
    void MouseUpdate(s32 x, s32 y, u32 flags);
    void MouseWheelUpdate(float delta);
    void WindowMessage_Activate(prWindow* window, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void WindowMessage_Destroy(prWindow* window, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void WindowMessage_Resize(prWindow* window, u32 width, u32 height);
    void WindowMessage_InjectPressed(u32 charcode);
    void WindowMessage_InjectControl(u32 charcode);
    void WindowMessage_InjectDown(u32 charcode);


    // ------------------------------------------------------------------------
    // Updates the system mouse upon a mouse move event.
    // ------------------------------------------------------------------------
    void MouseMove(HWND hwnd, WPARAM wParam, LPARAM lParam)
    {
        POINT p;
        p.x = LOWORD(lParam);
        p.y = HIWORD(lParam);

        if (!mouseCaptured)
        {
            SetCapture(hwnd);
            mouseCaptured = true;
            MouseUpdate(p.x, p.y, (s32)wParam);
        }
        else
        {
            RECT   rect;
            GetClientRect(hwnd, &rect);

            // Outside client rect?
            int width  = rect.right  - rect.left;
            int height = rect.bottom - rect.top;
            if (p.x < 0 || p.y < 0 || p.x >= width || p.y >= height)
            {
                prMouse *pMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
                if (pMouse)
                {
                    pMouse->SetInRect(false);
                }

                ReleaseCapture();
                mouseCaptured = false;
            }
            else
            {
                MouseUpdate(p.x, p.y, (s32)wParam);
            }
        }
    }


    // ------------------------------------------------------------------------
    // Updates the mouse position.
    // ------------------------------------------------------------------------
    void MouseUpdate(s32 x, s32 y, u32 flags)
    {
        prMouse *pMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
        if (pMouse)
        {
            pMouse->SetMouseData(x, y, flags, false);
            pMouse->SetInRect(true);
        }
    }


    // ------------------------------------------------------------------------
    // Updates the mouse wheel.
    // ------------------------------------------------------------------------
    void MouseWheelUpdate(float delta)
    {
        prMouse *pMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
        if (pMouse)
        {
            s32 dir    = (delta < 0.0f) ? -1 : 1;
            s32 clicks = (s32)PRFABS(delta);
            pMouse->MouseWheelUpdate(dir, clicks);
        }
    }


    // ------------------------------------------------------------------------
    // Handles: WM_ACTIVATE
    // ------------------------------------------------------------------------
    void WindowMessage_Activate(prWindow* window, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        PRUNUSED(hwnd);
        PRUNUSED(msg);
        PRUNUSED(lParam);

        int  active    = LOWORD(wParam);        // activation flag 
        BOOL minimized = (BOOL)HIWORD(wParam);  // minimized flag
        PRUNUSED(minimized);

        if (window)
        {
            if (active == WA_INACTIVE)
            {
                window->SetActive(false);
            }
            else
            {
                window->SetActive(true);
            }
        }

        if (pApp)
        {
            if (active == WA_INACTIVE)
            {
                pApp->OnDeactivate();
            }
            else
            {
                pApp->OnActivate();
            }
        }
    }


    // ------------------------------------------------------------------------
    // Handles: WM_DESTROY
    // ------------------------------------------------------------------------
    void WindowMessage_Destroy(prWindow* window, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        PRUNUSED(hwnd);
        PRUNUSED(msg);
        PRUNUSED(wParam);
        PRUNUSED(lParam);

        // Let the game know we're exiting.
        prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        if (pReg)
        {
            pReg->SetValue("Exit", "true");
        }
    
        // Terminate AntTweakBar
        #if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)    
        TwTerminate();
        #endif

        // Destroy the window.
        if (window)
        {
            window->SetActive(false);
            window->Destroy();
        }
    
        // And exit
        PostQuitMessage(0);
    }


    // ------------------------------------------------------------------------
    // Handles: Window sizing
    // ------------------------------------------------------------------------
    void WindowMessage_Resize(prWindow* window, u32 width, u32 height)
    {
        if (window)
        {
            window->Resize(width, height);
        }
    }


    // ------------------------------------------------------------------------
    // Handles: Handles keyboard injection
    // ------------------------------------------------------------------------
    void WindowMessage_InjectPressed(u32 charcode)
    {
        prKeyboard *pKB = static_cast<prKeyboard *>(prCoreGetComponent(PRSYSTEM_KEYBOARD));
        if (pKB)
        {
            pKB->KeyboardInjectPressed(charcode);
        }
    }


    // ------------------------------------------------------------------------
    // Handles: Handles keyboard injection
    // ------------------------------------------------------------------------
    void WindowMessage_InjectControl(u32 charcode)
    {
        prKeyboard *pKB = static_cast<prKeyboard *>(prCoreGetComponent(PRSYSTEM_KEYBOARD));
        if (pKB)
        {
            pKB->KeyboardInjectControl(charcode);
        }
    }


    // ------------------------------------------------------------------------
    // Handles: Handles keyboard injection
    // ------------------------------------------------------------------------
    void WindowMessage_InjectDown(u32 charcode)
    {
        prKeyboard *pKB = static_cast<prKeyboard *>(prCoreGetComponent(PRSYSTEM_KEYBOARD));
        if (pKB)
        {
            pKB->KeyboardInjectDown(charcode);
        }
    }


    // ------------------------------------------------------------------------
    // Handles: Minimum window size
    // ------------------------------------------------------------------------
    bool WindowMessage_GetMinMaxInfo(prWindow* window, WPARAM wParam, LPARAM lParam)
    {
        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        bool handled = false;
        
        if (window)
        {
            if (window->GetMinWidth() != 0xFFFFFFFF)
            {
                mmi->ptMinTrackSize.x = window->GetMinWidth();
                handled = true;
            }

            if (window->GetMinHeight() != 0xFFFFFFFF)
            {
                mmi->ptMinTrackSize.y = window->GetMinHeight();
                handled = true;
            }
        }

        return handled;
    }


    // ------------------------------------------------------------------------
    // Determines if the character should be allowed in input
    // ------------------------------------------------------------------------
    bool IsAcceptableCharacter(WPARAM wparam)
    {
        bool result = false;

        if (characters && *characters)
        {
            switch(wparam)
            {
            // Allow the edit keys
            case VK_DELETE:
            case VK_BACK:
            case VK_HOME:
            case VK_END:
            case VK_LEFT:
            case VK_RIGHT:
                result = true;
                break;

            default:
                {
                    int   index = 0;
                    TCHAR curr  = 0;
                    do
                    {
                        curr = characters[index++];
                        if (curr == wparam)
                        {
                            result = true;
                            break;
                        }
                    }
                    while(curr != 0);
                }
                break;
            }
        }
        else
        {
            // No filter characters! Always allow
            result = true;
        }

        return result;
    }


    // ------------------------------------------------------------------------
    // Subclass procedure
    // ------------------------------------------------------------------------
    LRESULT CALLBACK prSubclassWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch(uMsg)
        {
        case WM_CHAR:
            if (enableTextFilter && !IsAcceptableCharacter(wParam))
            {
                return 0;
            }
            break;
        }

        return CallWindowProc(originalWinProc, hwnd, uMsg, wParam, lParam);
    }
}


/// ---------------------------------------------------------------------------
/// Pointer to allow game to receive specific messages like Activate
/// and Deactivate.
/// ---------------------------------------------------------------------------
void prSetApplicationForWindowProcedure(Proteus::Core::prApplication *app)
{
    pApp = app;

    // Initialize AntTweakBar?
    #if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
    prATBInit();
    #endif
}


/// ---------------------------------------------------------------------------
/// The default window message handler.
/// ---------------------------------------------------------------------------
LRESULT CALLBACK prWindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Get window.
    #ifdef _WIN64
      prWindow* window = (prWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    #else
      prWindow* window = (prWindow*)(s64)GetWindowLong(hwnd, GWL_USERDATA);
    #endif


    // Send event message to AntTweakBar
    #if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
    if (prATBUpdate(hwnd, msg, wParam, lParam))
    {
        return 0;
    }
    #endif


    switch(msg)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
            if (pApp)
            {
                pApp->Draw();
            }
            EndPaint(hwnd, &ps);
        }
        break;

    case WM_MOUSEMOVE:
        MouseMove(hwnd, wParam, lParam);
        break;

    case WM_XBUTTONUP:
    case WM_XBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MBUTTONDOWN:
        MouseUpdate(LOWORD(lParam), HIWORD(lParam), (s32)wParam);
        break;

    case WM_MOUSEWHEEL:
        {
            float delta = (float)((float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
            MouseWheelUpdate(delta);
        }
        break;

    case WM_ERASEBKGND:
        return TRUE;

    // Pause the game if moving, or sizing the window.
    case WM_MOVE:
    case WM_MOVING:         
        if (pApp)
        {
            pApp->OnDeactivate();
        }
        break;

    // Pause the game if moving, or sizing the window.
    case WM_SIZE:
    case WM_SIZING:
        {
            // Size window
            RECT r;
            GetClientRect(hwnd, &r);
            u32 width  = r.right - r.left;
            u32 height = r.bottom - r.top;

            WindowMessage_Resize(window, width, height);
            
            // Also do tweak bar
            #if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
            TwWindowSize(r.right - r.left, r.bottom - r.top);
            #endif

            if (pApp)
            {
                pApp->OnDeactivate();

                // Pass on the size messages
                static_cast<Proteus::Core::prApplication_PC *>(pApp)->WindowsCommand(msg, wParam, lParam);
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);

    // Exit size
    case WM_EXITSIZEMOVE:
        if (pApp)
        {
            // Active on move/size exit
            pApp->OnActivate();

            // Pass on the exit size/move message
            static_cast<Proteus::Core::prApplication_PC *>(pApp)->WindowsCommand(msg, wParam, lParam);
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);

    // Set min/max size
#if defined(PROTEUS_TOOL) || defined(PLATFORM_PC)
    case WM_GETMINMAXINFO:
        WindowMessage_GetMinMaxInfo(window, wParam, lParam);
        return DefWindowProc(hwnd, msg, wParam, lParam);
        break;
#endif

    case WM_ACTIVATE:
        WindowMessage_Activate(window, hwnd, msg, wParam, lParam);
        break;

    case WM_DESTROY:
        WindowMessage_Destroy(window, hwnd, msg, wParam, lParam);
        break;

    case WM_CHAR:
        WindowMessage_InjectPressed((u32)wParam);
        break;

    // Keys down
    case WM_KEYDOWN:
        switch(wParam)
        {
        case VK_SHIFT:
            if (GetKeyState(VK_LSHIFT) & 0x8000)
            {
                WindowMessage_InjectControl(PRCTRL_KEY_SHIFT_LEFT);
            }            
            if (GetKeyState(VK_RSHIFT) & 0x8000)
            {
                WindowMessage_InjectControl(PRCTRL_KEY_SHIFT_RIGHT);
            }
            break;

        case VK_CONTROL:
            if (GetKeyState(VK_LCONTROL) & 0x8000)
            {
                WindowMessage_InjectControl(PRCTRL_KEY_CTRL_LEFT);
            }            
            if (GetKeyState(VK_RCONTROL) & 0x8000)
            {
                WindowMessage_InjectControl(PRCTRL_KEY_CTRL_RIGHT);
            }
            break;

        default:
            //prTrace(LogError, "down: %c\n", wParam);
            WindowMessage_InjectDown((u32)wParam);
            break;
        }
        break;

    // Tool only code
    #if defined(PROTEUS_TOOL) || defined(PLATFORM_PC)
    case WM_COMMAND:
        if (pApp)
        {
            // Deactivate the game.
            pApp->OnDeactivate();

            // Send the command.
            int command = LOWORD(wParam);
            static_cast<Proteus::Core::prApplication_PC *>(pApp)->WindowsCommand(command, wParam, lParam);
        }
        break;

    // Cases which the application may wish to handle.
    case WM_CLOSE:
        if (pApp)
        {
            // Handled?
            if (static_cast<Proteus::Core::prApplication_PC *>(pApp)->WindowsCommand(msg, wParam, lParam) == true)
            {
                break;
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);

    // Cases which the application may wish to handle.
    case WM_NOTIFY:
        if (pApp)
        {
            // Handled?
            if (static_cast<Proteus::Core::prApplication_PC *>(pApp)->WindowsCommand(msg, wParam, lParam) == true)
            {
                break;
            }
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    #endif

    // This lot is bad for tool apps
    #if !defined(PROTEUS_TOOL)    
    case WM_SYSCOMMAND:     
        if ((wParam & 0xFFF0) == SC_SCREENSAVE   ||   // Stops screen saver from starting up.
            (wParam & 0xFFF0) == SC_MONITORPOWER ||   // Stops monitor power save mode from interfering.
            (wParam & 0xFFF0) == SC_KEYMENU)          // Stops ALT from interfering with rendering.
        {
            return 0;
        }
        else
        {
            // Handles other system messages like minimize, maximize and close.
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
        break;
    #endif

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}


/// ---------------------------------------------------------------------------
/// Subclasses the main window
/// ---------------------------------------------------------------------------
void prSubclassWindow(HWND hwnd)
{
    // Subclass the window so we can filter keystrokes, etc
    WNDPROC oldProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(
                      hwnd,
                      GWLP_WNDPROC,
                      reinterpret_cast<LONG_PTR>(prSubclassWindowProc)));

    if (originalWinProc == NULL)
    {
        originalWinProc = oldProc;
    }
}


/// ---------------------------------------------------------------------------
/// Sets text filter
/// ---------------------------------------------------------------------------
void prSubclassSetTextFilter(const TCHAR *pChars)
{
    if (pChars && *pChars)
    {
        characters       = pChars;
        enableTextFilter = TRUE;
    }
    else
    {
        characters        = NULL;
        enableTextFilter  = FALSE;
    }
}


#endif//PLATFORM_PC
