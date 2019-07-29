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
#include "../lua/lua.h"
#include "../Box2D/Common/b2Settings.h"
#include <versionhelpers.h>


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
    prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));

    // Debug - created by default by debug builds
    #if (defined(_DEBUG) || defined(DEBUG)) && defined (PROTEUS_ALLOW_CONSOLE)
        m_pCW = new prConsoleWindow("Debug console", 40, 80);
        prDebugRegisterConsoleWindow(m_pCW);

    #else
        m_pCW = nullptr;

        if (reg)
        {
            if (prStringCompare(reg->GetValue("Console"), "true") == CMP_EQUALTO)
            {
                m_pCW = new prConsoleWindow("Debug console", 40, 80);
                prDebugRegisterConsoleWindow(m_pCW);
            }
        }

    #endif

    // Game timer
    pGameTime  = new prGameTime();

    // Init data
    m_hAccel   = nullptr;

    // Remove old log file
    prTraceLogClear();

    // Write startup info.
    if (reg)
    {
        char buffer[32];
        prGetVersionAsString(buffer, sizeof(buffer));

        prTrace(prLogLevel::LogInformation, "-------------------------------------------------------------------------------\n");
        prTrace(prLogLevel::LogInformation, "Engine version : %s\n", buffer);
        prTrace(prLogLevel::LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(prLogLevel::LogVerbose, "Components:\n");
        prTrace(prLogLevel::LogVerbose, "Lua              : %s.\n", LUA_COPYRIGHT);
        prTrace(prLogLevel::LogVerbose, "Box2D            : %i.%i.%i - Copyright (C) 2006-2009 Erin Catto\n", b2_version.major, b2_version.minor, b2_version.revision);
        prTrace(prLogLevel::LogVerbose, "TinyXML          : 2.6.2 - Copyright (C) 2000-2006 Lee Thomason.\n");
        prTrace(prLogLevel::LogVerbose, "Zlib             : 1.2.8 - Copyright (C) 1995-2013 Jean-loup Gailly and\n");
        prTrace(prLogLevel::LogVerbose, "                           Mark Adler.\n");
        prTrace(prLogLevel::LogVerbose, "Ogg              : 1.2.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
        prTrace(prLogLevel::LogVerbose, "Vorbis           : 1.3.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
        prTrace(prLogLevel::LogVerbose, "AntTweakBar      : 1.16  - Copyright (C) 2005-2013 Philippe Decaudin.\n");        
        prTrace(prLogLevel::LogVerbose, "libutf8proc      : 1.1.5 - Copyright (C) 2009 Public Software Group e. V.,\n");
        prTrace(prLogLevel::LogVerbose, "                                                         Berlin, Germany\n");
        prTrace(prLogLevel::LogVerbose, "Mersenne Twister : 1.1   - Copyright (C) 1997 - 2002, Makoto Matsumoto and\n");        
        prTrace(prLogLevel::LogVerbose, "                           Takuji Nishimura,\n");        
        prTrace(prLogLevel::LogVerbose, "                           Copyright (C) 2000 - 2009, Richard J. Wagner\n");        
        prTrace(prLogLevel::LogVerbose, "Based on code by Makoto Matsumoto, Takuji Nishimura, and Shawn Cokus\n");        
        prTrace(prLogLevel::LogVerbose, "Richard J. Wagner  v1.1  28 September 2009  wagnerr@umich.edu\n");        
        prTrace(prLogLevel::LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(prLogLevel::LogVerbose, "Config         : %s - PC - %s\n", BuildType(), BuildIsDebug());
        prTrace(prLogLevel::LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(prLogLevel::LogVerbose, "Logging        : Enabled   %s\n", prTraceIsEnabled() ? "True" : "False");
        prTrace(prLogLevel::LogVerbose, "               : Log level %s\n", prTraceGetLogLevel());
        prTrace(prLogLevel::LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(prLogLevel::LogVerbose, "Options        : Use archives %s\n", reg->GetValue("UseArchives"));
        prTrace(prLogLevel::LogVerbose, "               : Log to file  %s\n", reg->GetValue("LogToFile"));
        prTrace(prLogLevel::LogVerbose, "               : Console      %s\n", reg->GetValue("Console"));
        prTrace(prLogLevel::LogVerbose, "               : Help         %s\n", reg->GetValue("Help"));
        prTrace(prLogLevel::LogVerbose, "-------------------------------------------------------------------------------\n");
        prTrace(prLogLevel::LogInformation, "Build number   : %i\n", BUILD_NUMBER);
        prTrace(prLogLevel::LogInformation, "-------------------------------------------------------------------------------\n");
    }

    // Keeps CppCheck happy
    //BuildType();

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
    if (pWindowName == nullptr)
    {
        MessageBoxW(HWND_DESKTOP, L"The window name cannot be NULL.", L"Error", MB_ICONERROR | MB_OK);
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
            reg->ShowKeyValuePairs();
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
    if (pWindowName == nullptr)
    {
        MessageBoxW(HWND_DESKTOP, L"The window name cannot be null.", L"Error", MB_ICONERROR | MB_OK);
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
            reg->ShowKeyValuePairs();
        }

        // Load accelarator?
        if (accelID > 0)
        {
            m_hAccel = LoadAccelerators(GetModuleHandle(nullptr), MAKEINTRESOURCE(accelID));
            if (m_hAccel == nullptr)
            {
                prTrace(prLogLevel::LogError, "Failed to load accelerator\n");
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
        if (PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE))
        {
            GetMessage(&msg, nullptr, 0, 0);
            
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

                    // Clears keyboard, so do after game update.
                    if (pKeyb)
                    {
                        // First check for keyboard back. Most PC games will ignore this as its really
                        // just here so we can test the functionality on the PC debug versions
                        // of any game
                        if (pKeyb->IsKeyPressed(VK_BACK))
                        {
                            BackKeyPressed();
                        }

                        pKeyb->Update();
                    }
                }
            }

            // Needs modified to track game speed!
            Sleep(1);

            if (pFps) { pFps->End(); }
        }
    }


    // Clear here as this system allocates memory
    prOnScreenLogger *pOSL = static_cast<prOnScreenLogger *>(prCoreGetComponent(PRSYSTEM_ONSCREENLOGGER));
    if (pOSL) { pOSL->Clear(); }


    // Stop sound
    prSoundManager  *pSound = static_cast<prSoundManager *>(prCoreGetComponent(PRSYSTEM_AUDIO));
    if (pSound) { pSound->SongStop(); }


    // Clear app pointer.
    prSetApplicationForWindowProcedure(nullptr);
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
/// Returns the build
/// ---------------------------------------------------------------------------
const char *prApplication_PC::BuildIsDebug()
{
    #if (defined(_DEBUG) || defined(DEBUG))
        return "Debug";
    #else
        return "Release";
    #endif
}


/// ---------------------------------------------------------------------------
/// Acquire some info about the PC we're running on.
/// ---------------------------------------------------------------------------
BOOL prApplication_PC::CheckPlatform()
{
    bool isOSInvalid = true;            // Set both to fail
    bool initFailed  = true;
    s32  verMajor     = 0;              // Version major/minor. Won't match windows, but that's not important
    s32  verMinor     = 0;              // as data usage is internal


    // Lets get the OS we're running on. Vista is used as a base version
    if (!IsWindowsVistaOrGreater())
    {
        PRLOGE("Failed to acquire windows version\n");
        prDebugShowLastError();
    }
    else
    {
        // Okay, we're on vista at a minimum
        initFailed = false;

        // Find which version
        if (IsWindowsVistaOrGreater())
        {
            verMajor    = 6;
            verMinor    = 0;
            isOSInvalid = false;
        }

        if (IsWindowsVistaSP1OrGreater())
        {
            verMajor    = 6;
            verMinor    = 1;
            isOSInvalid = false;
        }

        if (IsWindowsVistaSP2OrGreater())
        {
            verMajor    = 6;
            verMinor    = 2;
            isOSInvalid = false;
        }

        if (IsWindows7OrGreater())
        {
            verMajor    = 7;
            verMinor    = 0;
            isOSInvalid = false;
        }

        if (IsWindows7SP1OrGreater())
        {
            verMajor    = 7;
            verMinor    = 1;
            isOSInvalid = false;
        }

        if (IsWindows8OrGreater())
        {
            verMajor    = 8;
            verMinor    = 0;
            isOSInvalid = false;
        }

        if (IsWindows8Point1OrGreater())
        {
            verMajor    = 8;
            verMinor    = 1;
            isOSInvalid = false;
        }

        if (IsWindows10OrGreater())
        {
            verMajor    = 10;
            verMinor    = 0;
            isOSInvalid = false;
        }
    }

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

    // Write startup info.
    PRLOGI("Windows version %i.%i\n", verMajor, verMinor);

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
