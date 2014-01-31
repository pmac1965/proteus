/**
 * prApplication_PC.cpp
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)

 
#include "prApplication_PC.h"
#include "prWindow_PC.h"
#include "prWindowProcedure.h"
#include "prCore.h"
#include "prRegistry.h"
#include "prVersion.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../debug/prConsoleWindow.h"
#include "../input/prMouse.h"
#include "../display/prRenderer.h"


// ----------------------------------------------------------------------------
// Static data
// ----------------------------------------------------------------------------
static const char *embedded[] = 
{
    "The Proteus Game Engine. ",
    "Author Paul Michael McNab. ",
    "Copyright Paul Michael McNab. All rights reserved.",
};


// ----------------------------------------------------------------------------
// Local functions
// ----------------------------------------------------------------------------
namespace
{
    const char *BuildType();
    BOOL CheckPlatform();
    bool RunningAlready(const char *pWindowName);
    bool IsRemoteSession();


    #if defined(_DEBUG) || defined(DEBUG)
    // ----------------------------------------------------------------------------
    // Returns the build
    // ----------------------------------------------------------------------------
    const char *BuildType()
    {
        #if defined(PROTEUS_TOOL)
            return "Tool";
        #else
            return "Game";
        #endif
    }
    #endif


    // ------------------------------------------------------------------------
    // Acquire some info about the PC we're running on.
    // ------------------------------------------------------------------------
    BOOL CheckPlatform()
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
            prDebugShowLastError();
            initFailed = true;
        }
        else
        {
            // Is the operating system is Windows 7, Windows Server 2008, Windows Vista, Windows Server 2003, Windows XP, or Windows 2000?
            if (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
            {
                prTrace("Windows version %i.%i\n", osvi.dwMajorVersion, osvi.dwMinorVersion);
                prTrace("%ls\n", osvi.szCSDVersion);
            }
            else
            {
                isOSInvalid = true;
            }
        }

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


    // ------------------------------------------------------------------------
    // Current only context... 
    // This needs to allow switching of users, so no global stuff,
    // therefore we search for the window.
    // ------------------------------------------------------------------------
    bool RunningAlready(const char *pWindowName)
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


    // ------------------------------------------------------------------------
    // Check if user is trying to run the game remotely
    // ------------------------------------------------------------------------
    bool IsRemoteSession()
    {
        bool remote = (GetSystemMetrics(SM_REMOTESESSION) != 0);
        if (remote)
        {
            prMouse *pMouse = (prMouse *)prCoreGetComponent(PRSYSTEM_MOUSE);
            if (pMouse)
            {
                pMouse->ShowSystemCursor(true);
            }

            MessageBoxW(HWND_DESKTOP, L"This game cannot be run remotely.", L"Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
            return true;
        }

        return false;
    }
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prApplication_PC::prApplication_PC() : prApplication()
{
    #if (defined(_DEBUG) || defined(DEBUG)) && defined (PROTEUS_ALLOW_CONSOLE)
        m_pCW = new prConsoleWindow(TEXT("Debug console"), 40, 80);
    #else
        m_pCW = NULL;
    #endif

    //System::Init();

#if defined(_DEBUG) || defined(DEBUG)
    // Write startup info.
    //if (System::Verbose())
    {    
        prTrace("-------------------------------------------------------------------------------\n");
        prTrace("Engine version : %s\n", prGetVersionAsString());
        prTrace("-------------------------------------------------------------------------------\n");
        prTrace("Components:\n");
        prTrace("Lua            : 5.2.3 - Copyright (C) 1994-2013 Lua.org, PUC-Rio.\n");
        prTrace("Box2D          : 2.3.0 - Copyright (c) 2006-2013 Erin Catto\n");
        prTrace("TinyXML        : 2.6.2 - Copyright (c) 2000-2006 Lee Thomason.\n");
        prTrace("Zlib           : 1.2.8 - Copyright (C) 1995-2013 Jean-loup Gailly and Mark Adler.\n");
//        prTrace("Ogg            : 1.2.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
//        prTrace("Vorbis         : 1.3.1 - Copyright (C) 1995-2011 The Xiph.Org Foundation.\n");
        prTrace("AntTweakBar    : 1.16  - Copyright (C) 2005-2013 Philippe Decaudin.\n");        
        prTrace("-------------------------------------------------------------------------------\n");
        prTrace("Config         : %s - PC - Debug\n", BuildType());
        prTrace("-------------------------------------------------------------------------------\n");
//        prTrace("Options   : Verbose      %s\n", System::Verbose()     ? "true" : "false");
//        prTrace("          : Use archives %s\n", System::UseArchives() ? "true" : "false");
//        prTrace("          : Log to file  %s\n", System::LogToFile()   ? "true" : "false");
//        prTrace("-------------------------------------------------------------------------------\n");
    }
    
/*    if (System::ShowHelp())
    {
        Trace("-------------------------------------------------------------------------------\n");
        Trace("Command line options\n");
        Trace("-verb      = Verbose startup. Startup logs everything\n");
        Trace("-noarc     = Don't use archives. All data is read from local storage.\n");
        Trace("-logfile   = Logs all debug output to a text file.\n");
        Trace("-help      = Logs the command line options.\n");
        Trace("-------------------------------------------------------------------------------\n");
    }*/
#endif


    //Trace("-------------------------------------------------------------------------------\n");
    //Trace("Build number %0.4f\n", BUILD_NUMBER);
    //Trace("-------------------------------------------------------------------------------\n");


    // Create the systems.    
/*    Renderer            ::GetInstance()->SingletonCreate();
    Fps                 ::GetInstance()->SingletonCreate();
    Input               ::GetInstance()->SingletonCreate();
    GameTime            ::GetInstance()->SingletonCreate();
    prMouse             ::GetInstance()->SingletonCreate();
    prTouch             ::GetInstance()->SingletonCreate();
    FileManager         ::GetInstance()->SingletonCreate();
    SoundManager        ::GetInstance()->SingletonCreate();
    prTextureManager    ::GetInstance()->SingletonCreate();*/

    // Access the embedded data, so it'll stay linked into the game.
    for (int j=0; j<PRARRAY_SIZE(embedded); j++)
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
}


/// ---------------------------------------------------------------------------
/// Creates the application display.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_PC::DisplayCreate(u32 width, u32 height, const char *pWindowName)
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


    // Create.
    PRSAFE_DELETE(m_pWindow);
    m_pWindow = new prWindow_PC();


    // Create the window
    bool success = m_pWindow->Create(width, height, 32, false);
    if (success)
    {
        m_running = PRTRUE;
        result    = PRTRUE;

        // Set window.
        prRenderer *pRenderer = (prRenderer *)prCoreGetComponent(PRSYSTEM_RENDERER);
        if (pRenderer)
        {
            pRenderer->SetWindow(m_pWindow);
            pRenderer->Init();
        }

        // Set/Reset input
/*        ASSERT(Input::SingletonExists());        
        Input::GetInstance()->Deinit();
        Input::GetInstance()->Init(imp.pWindow);*/

        prRegistry *reg = (prRegistry *)prCoreGetComponent(PRSYSTEM_REGISTRY);
        if (reg)
        {
            reg->SetValue("WindowName", pWindowName);
            reg->SetValue("ScreenWidth", width);
            reg->SetValue("ScreenHeight", height);
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
/// Run the game.
/// ---------------------------------------------------------------------------
PRBOOL prApplication_PC::Run()
{
    // Get systems
    prMouse *pMouse = (prMouse *)prCoreGetComponent(PRSYSTEM_MOUSE);


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
                m_running = PRFALSE;
                OnExit();
            }


/*            if (imp.hasAccel && imp.hAccel)
            {
                if (!TranslateAccelerator(imp.pWindow->GetWindowHandle(), imp.hAccel, &msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            else//*/
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
//            GameTime::GetInstance()->Update();
//            Fps::GetInstance()->Begin();

            // Update game
            if (m_pWindow && m_pWindow->GetActive())
            {
//                float dt = GameTime::GetInstance()->ElapsedTime();

                // System updates
                if (pMouse) { pMouse->Update(); }
                //Input       ::GetInstance()->Update();
                //prMouse     ::GetInstance()->Update();
                //prTouch     ::GetInstance()->Update();
                //SoundManager::GetInstance()->Update(dt);

                // Update and draw the game
                Update(16.0f);// dt);
                Draw();
            }

            // Needs modified to track game speed!
            Sleep(15);
            TODO("Needs modified to track game speed!")
            //Sleep(1);

//            Fps::GetInstance()->End();
        }
    }


    // Clear app pointer.
    prSetApplicationForWindowProcedure(NULL);


    return PRFALSE;
}


#endif//PLATFORM_PC
