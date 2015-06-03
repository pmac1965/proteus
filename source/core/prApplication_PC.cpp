/**
 * prApplication_PC.cpp
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

 
#include "prApplication_PC.h"
#include "prWindow_PC.h"
#include "prWindowProcedure.h"
#include "prCore.h"
#include "prRegistry.h"
#include "prVersion.h"
#include "prGameTime.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../debug/prConsoleWindow.h"
#include "../debug/prOnScreenLogger.h"
#include "../debug/prFps.h"
#include "../input/prMouse.h"
#include "../input/prKeyboard.h"
#include "../display/prRenderer.h"
#include "../core/prStringUtil.h"
#include "../audio/prSoundManager.h"
#include "../input/prTouch.h"
#include "../prVerNum.h"


// ----------------------------------------------------------------------------
// Local data
// ----------------------------------------------------------------------------
namespace
{
    const char *embedded[] = 
    {
        "The Proteus Game Engine. ",
        "Author Paul Michael McNab. ",
        "Copyright Paul Michael McNab. All rights reserved.",
    };
}


// Namespaces
namespace Proteus {
namespace Core {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prApplication_PC::prApplication_PC() : prApplication()
{
    // Debug console
    #if (defined(_DEBUG) || defined(DEBUG)) && defined (PROTEUS_ALLOW_CONSOLE)
      m_pCW = new prConsoleWindow(TEXT("Debug console"), 40, 80);
    #else
      m_pCW = NULL;
    #endif


    // Game timer
    pGameTime  = new prGameTime();


    // Init data
    m_hAccel   = NULL;


#if defined(_DEBUG) || defined(DEBUG)
    // Write startup info.
    prRegistry *reg = (prRegistry *)prCoreGetComponent(PRSYSTEM_REGISTRY);
    if (reg)
    {
        if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
        {
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
            prTrace(LogError, "Engine version : %s\n", prGetVersionAsString());
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
            prTrace(LogError, "Components:\n");
            prTrace(LogError, "Lua              : 5.2.3 - Copyright (C) 1994-2013 Lua.org, PUC-Rio.\n");
            prTrace(LogError, "Box2D            : 2.3.0 - Copyright (C) 2006-2013 Erin Catto\n");
            prTrace(LogError, "TinyXML          : 2.6.2 - Copyright (C) 2000-2006 Lee Thomason.\n");
            prTrace(LogError, "Zlib             : 1.2.8 - Copyright (C) 1995-2013 Jean-loup Gailly and\n");
            prTrace(LogError, "                           Mark Adler.\n");
            prTrace(LogError, "Ogg              : 1.2.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
            prTrace(LogError, "Vorbis           : 1.3.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
            prTrace(LogError, "AntTweakBar      : 1.16  - Copyright (C) 2005-2013 Philippe Decaudin.\n");        
            prTrace(LogError, "libutf8proc      : 1.1.5 - Copyright (C) 2009 Public Software Group e. V.,\n");
            prTrace(LogError, "                                                         Berlin, Germany\n");
            prTrace(LogError, "Mersenne Twister : 1.1   - Copyright (C) 1997 - 2002, Makoto Matsumoto and\n");        
            prTrace(LogError, "                           Takuji Nishimura,\n");        
            prTrace(LogError, "                           Copyright (C) 2000 - 2009, Richard J. Wagner\n");        
            prTrace(LogError, "Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus\n");        
            prTrace(LogError, "Richard J. Wagner  v1.1  28 September 2009  wagnerr@umich.edu\n");        
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
            prTrace(LogError, "Config         : %s - PC - Debug\n", BuildType());
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
            prTrace(LogError, "Options        : Verbose      %s\n", reg->GetValue("Verbose"));
            prTrace(LogError, "               : Use archives %s\n", reg->GetValue("UseArchives"));
            prTrace(LogError, "               : Log to file  %s\n", reg->GetValue("LogToFile"));
            prTrace(LogError, "               : Help         %s\n", reg->GetValue("Help"));
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
            prTrace(LogError, "Build number   : %i\n", BUILD_NUMBER);
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
        }
        else
        {
            // Write startup info.
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
            prTrace(LogError, "Engine version : %s\n", prGetVersionAsString());
            prTrace(LogError, "Build number   : %i\n", BUILD_NUMBER);
            prTrace(LogError, "-------------------------------------------------------------------------------\n");
        }
    }
#endif

    // Keeps CppCheck happy
    BuildType();

    // Access the embedded data, so it'll stay linked into the game.
    for (u32 j=0; j<PRARRAY_SIZE(embedded); j++)
    {
        const char *text = embedded[j];

        // Just compare the first 10 characters.
        for (int k=0; k<10; k++)
        {
            if (text[k] == 0)
            {
            }
        }
    }


    // Give the window procedure a pointer to the application.
    prSetApplicationForWindowProcedure(this);
    prTraceLogClear();
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prApplication_PC::~prApplication_PC()
{
    PRSAFE_DELETE(m_pCW);
    PRSAFE_DELETE(m_pWindow);
    PRSAFE_DELETE(pGameTime);
}


/// ---------------------------------------------------------------------------
/// Creates the application display.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_PC::DisplayCreate(u32 width, u32 height, const char *pWindowName, bool fullscreen)
{
    PRBOOL result = PRFALSE;


    // Check window name is not null
    if (pWindowName == NULL)
    {
        MessageBoxW(HWND_DESKTOP, L"The window name cannot NULL.", L"Error", MB_ICONERROR | MB_OK);
        return result;
    }

    // Check window name is not empty
    if (pWindowName && strlen(pWindowName) == 0)
    {
        MessageBoxW(HWND_DESKTOP, L"The window name is empty.", L"Error", MB_ICONERROR | MB_OK);
        return result;
    }

    // Check platform
    if (!CheckPlatform())
    {
        return result;
    }

    // Running, then switch to running app.
    if (RunningAlready(pWindowName))
    {
        return result;
    }

    // Don't allow remote sessions.
    if (IsRemoteSession())
    {
        return result;
    }


    // Kill old.
    PRSAFE_DELETE(m_pWindow);
    m_pWindow = new prWindow_PC();
    PRASSERT(m_pWindow);


    // Create the window
    bool success = m_pWindow->Create(width, height, 32, fullscreen);
    if (success)
    {
        m_running = PRTRUE;
        result    = PRTRUE;

        // Set window.
        prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
        if (pRenderer)
        {
            pRenderer->SetWindow(m_pWindow);
            pRenderer->Init();
        }

        // Set registry
        prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        if (reg)
        {
            // Set title here
            static_cast<prWindow_PC *>(m_pWindow)->SetTitle(pWindowName);

            reg->SetValue("WindowName", pWindowName);
            reg->SetValue("ScreenWidth", width);
            reg->SetValue("ScreenHeight", height);
            
            // Show startup entries
            prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (reg)
            {
                if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
                {
                   reg->ShowKeyValuePairs();
                }
            }
        }
    }
    else
    {
        // Exit app.
        m_running = PRFALSE;
    }


    return result;
}


/// ---------------------------------------------------------------------------
/// Creates the application display.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_PC::DisplayCreateTool(u32 width, u32 height, u32 menuID, u32 iconID, u32 accelID, const TCHAR *pWindowName)
{
    PRBOOL result = PRFALSE;


    // Check window name is not null
    if (pWindowName == NULL)
    {
        MessageBoxW(HWND_DESKTOP, L"The window name cannot NULL.", L"Error", MB_ICONERROR | MB_OK);
        return result;
    }

    // Check window name is not empty
    if (pWindowName && prStringLengthW(pWindowName) == 0)
    {
        MessageBoxW(HWND_DESKTOP, L"The window name is empty.", L"Error", MB_ICONERROR | MB_OK);
        return result;
    }

    // Check platform
    if (!CheckPlatform())
    {
        return result;
    }

    // Running, then switch to running app.
    if (RunningAlreadyW(pWindowName))
    {
        return result;
    }

    // Don't allow remote sessions.
    if (IsRemoteSession())
    {
        return result;
    }


    // Create.
    PRSAFE_DELETE(m_pWindow);
    m_pWindow = new prWindow_PC();


    // Create the window
    bool success = static_cast<prWindow_PC *>(m_pWindow)->CreateTool(width, height, menuID, iconID, pWindowName);
    if (success)
    {
        // Set window.
        m_running = PRTRUE;
        result    = PRTRUE;

        // Set window.
        prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
        if (pRenderer)
        {
            pRenderer->SetWindow(m_pWindow);
            pRenderer->Init();
        }

        // Let system know the screen size.
        prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
        if (reg)
        {
            //reg->SetValue("WindowName", pWindowName);
            reg->SetValue("ScreenWidth", width);
            reg->SetValue("ScreenHeight", height);

            // Show startup entries
            prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (reg)
            {
                if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
                {
                   reg->ShowKeyValuePairs();
                }
            }
        }

        // Load accelarator?
        if (accelID > 0)
        {
            m_hAccel = LoadAccelerators(GetModuleHandle(NULL), MAKEINTRESOURCE(accelID));
            if (m_hAccel == NULL)
            {
                prTrace(LogError, "Failed to load accelerator\n");
            }
        }
    }
    else
    {
        // Exit app.
        m_running = false;
    }


    return result;

}


/// ---------------------------------------------------------------------------
/// Run the game.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_PC::Run()
{
    // Game loop
    MSG msg;
    while(m_running)
    {
        // Game loop.
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
        {
            GetMessage(&msg, NULL, 0, 0);
            
            // Quit?
            if (msg.message == WM_QUIT)
            {
                // Notify engine.
                prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
                if (pReg)
                {
                    pReg->SetValue("Exit", "true");
                }

                m_running = PRFALSE;
                OnExit();
            }

            if (m_hAccel)
            {
                HWND hwnd = static_cast<prWindow_PC*>(m_pWindow)->GetWindowHandle();

                if (!TranslateAccelerator(hwnd, m_hAccel, &msg) &&
                    !IsDialogMessage(hwnd, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            // Get systems
            prMouse         *pMouse = static_cast<prMouse *>       (prCoreGetComponent(PRSYSTEM_MOUSE));
            prSoundManager  *pSound = static_cast<prSoundManager *>(prCoreGetComponent(PRSYSTEM_AUDIO));
            prTouch         *pTouch = static_cast<prTouch *>       (prCoreGetComponent(PRSYSTEM_TOUCH));
            prKeyboard      *pKeyb  = static_cast<prKeyboard *>    (prCoreGetComponent(PRSYSTEM_KEYBOARD));
            prFps           *pFps   = static_cast<prFps *>         (prCoreGetComponent(PRSYSTEM_FPS));

            if (pGameTime)
            {
                pGameTime->Update();

                // Update game
                if (m_pWindow && m_pWindow->GetActive())
                {
                    float dt = pGameTime->ElapsedTime();

                    // System updates
                    if (pMouse) { pMouse->Update(); }
                    if (pSound) { pSound->Update(dt); }
                    if (pTouch) { pTouch->Update(); }
                    if (pFps)   { pFps->Begin(); }

                    // Update and draw the game
                    Update(dt);
                    Draw();

                    // Clears keyboard, so do after update.
                    if (pKeyb) { pKeyb->Update(); }
                }
            }

            // Needs modified to track game speed!
            Sleep(1);

            if (pFps)   { pFps->End(); }
        }
    }


    // Clear here as this system allocates memory
    prOnScreenLogger *pOSL = static_cast<prOnScreenLogger *>(prCoreGetComponent(PRSYSTEM_ONSCREENLOGGER));
    if (pOSL) { pOSL->Clear(); }


    // Stop sound
    prSoundManager  *pSound = static_cast<prSoundManager *>(prCoreGetComponent(PRSYSTEM_AUDIO));
    if (pSound) { pSound->SongStop(); }


    // Clear app pointer.
    prSetApplicationForWindowProcedure(NULL);
    prCoreDestroy();


    return PRFALSE;
}


/// ---------------------------------------------------------------------------
/// Returns the build
/// ---------------------------------------------------------------------------
const char *prApplication_PC::BuildType()
{
    #if defined(PROTEUS_TOOL)
        return "Tool";
    #else
        return "Game";
    #endif
}


/// ---------------------------------------------------------------------------
/// Acquire some info about the PC we're running on.
/// ---------------------------------------------------------------------------
BOOL prApplication_PC::CheckPlatform()
{
    bool isOSInvalid = false;
    bool initFailed = false;


    // Lets get the OS we're running on.
    OSVERSIONINFOEX osvi;
    memset(&osvi, 0, sizeof(osvi));
    osvi.dwOSVersionInfoSize = sizeof(osvi);


    // Removes this warning - 'GetVersionExW': was declared deprecated
    #ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning( disable : 4996 )
    #endif


    if (GetVersionEx((LPOSVERSIONINFO)&osvi) == 0)
    {
        TODO("Change the above call as its deprecated!")
        prDebugShowLastError();
        initFailed = true;
    }
    else
    {
        // Is the operating system is Windows 7, Windows Server 2008, Windows Vista, Windows Server 2003, Windows XP, or Windows 2000?
        if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
        {
            // Write startup info.
            prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (reg)
            {
                if (prStringCompare(reg->GetValue("Verbose"), "true") == CMP_EQUALTO)
                {
                    prTrace(LogError, "Windows version %i.%i\n", osvi.dwMajorVersion, osvi.dwMinorVersion);
                    prTrace(LogError, "%ls\n", osvi.szCSDVersion);
                }
            }
        }
        else
        {
            isOSInvalid = true;
        }
    }


    // Restores warning
    #ifdef _MSC_VER
    #pragma warning( pop )
    #endif


    // Couldn't get info about windows?
    if (initFailed)
    {
        MessageBoxW(HWND_DESKTOP, L"Unable to acquire information about your version of windows.", L"Error", MB_ICONERROR | MB_OK);
        return FALSE;
    }


    // Wrong version of windows?
    if (isOSInvalid)
    {
        MessageBoxW(HWND_DESKTOP, L"You cannot run this application as you do not have the correct version of Windows.", L"Error", MB_ICONERROR | MB_OK);
        return FALSE;
    }

    return TRUE;
}


/// ---------------------------------------------------------------------------
/// Current only context... 
/// This needs to allow switching of users, so no global stuff,
/// therefore we search for the window.
/// ---------------------------------------------------------------------------
bool prApplication_PC::RunningAlready(const char *pWindowName)
{
    HWND hwnd = FindWindowA("Proteus Class", pWindowName);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_SHOW);
        SetFocus(hwnd);
        SetForegroundWindow(hwnd);
        return true;
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Current only context... 
/// This needs to allow switching of users, so no global stuff,
/// therefore we search for the window.
/// ---------------------------------------------------------------------------
bool prApplication_PC::RunningAlreadyW(const TCHAR *pWindowName)
{
    HWND hwnd = FindWindow(L"Proteus Class", pWindowName);
    if (hwnd)
    {
        ShowWindow(hwnd, SW_SHOW);
        SetFocus(hwnd);
        SetForegroundWindow(hwnd);
        return true;
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Check if user is trying to run the game remotely
/// ---------------------------------------------------------------------------
bool prApplication_PC::IsRemoteSession()
{
    bool remote = (GetSystemMetrics(SM_REMOTESESSION) != 0);
    if (remote)
    {
        prMouse *pMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
        if (pMouse)
        {
            pMouse->ShowSystemCursor(true);
        }

        MessageBoxW(HWND_DESKTOP, L"This game cannot be run remotely.", L"Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
        return true;
    }

    return false;
}


}}// Namespaces


#endif//PLATFORM_PC
