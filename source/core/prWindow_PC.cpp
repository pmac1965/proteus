/**
 * prWindow_PC.cpp
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include "prWindow_PC.h"
#include "prWindowProcedure.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../display/prOglUtils.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../core/prStringUtil.h"


// Local data
static const TCHAR *g_ClassName = TEXT("Proteus Class");


// Is the window resizeable?
#if defined(PROTEUS_ALLOW_RESIZEABLE_WINDOW)
#define PR_THICKFRAME   WS_THICKFRAME
#else
#define PR_THICKFRAME   0
#endif


// Can the window be maximized.
#if defined(PROTEUS_ALLOW_WINDOW_MAXIMIZE)
#define PR_MAXIMIZEBOX  WS_MAXIMIZEBOX
#else
#define PR_MAXIMIZEBOX  0
#endif


// Can the window be maximized.
#if defined(PROTEUS_ALLOW_WINDOW_MINIMIZE)
#define PR_MINIMIZEBOX  WS_MINIMIZEBOX
#else
#define PR_MINIMIZEBOX  0
#endif


// Window configuration
#define PR_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             PR_THICKFRAME     | \
                             PR_MINIMIZEBOX    | \
                             PR_MAXIMIZEBOX)


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWindow_PC::prWindow_PC() : prWindow()
{
    m_hwnd        = NULL;
    m_glrc        = NULL;
    m_hdc         = NULL;
    m_title       = NULL;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prWindow_PC::~prWindow_PC()
{
    Destroy();
}


/// ---------------------------------------------------------------------------
/// Creates the application window.
/// ---------------------------------------------------------------------------
bool prWindow_PC::Create(u32 width, u32 height, u32 bits, bool fullScreen)
{
    // Ensure the previous window is destroyed.
    Destroy();


    // Init data
    m_fullScreen = fullScreen;
    m_title      = L"Proteus";
    m_width      = width;
    m_height     = height;
    m_bits       = bits;


    // Fullscreen?
    if (fullScreen)
    {
        if (ChangeToFullScreen() == false)
        {
            return false;
        }
    }


    // Create the window.
    if (CreateOpenGLWindow(0, 0) == false)
    {
        return false;
    }


    // Get a device context to our window
    PRASSERT(m_hwnd);
    m_hdc = GetDC(m_hwnd);
    if (!m_hdc)
    {
        prTrace("Failed to create device context.\n");
        prDebugShowLastError();
        Destroy();
        return false;
    }


    // Set pixel format.
    if (SetOpenGLPixelFormat() == false)
    {
        return false;
    }


    // Create rendering context.
    m_glrc = wglCreateContext(m_hdc);
    if (!m_glrc)
    {
        prTrace("Couldn't create an OpenGL rendering context.\n");
        prDebugShowLastError();
        Destroy();
        return false;
    }


    // Activate rendering context.
    if (!wglMakeCurrent(m_hdc, m_glrc))
    {
        prTrace("Couldn't activate the OpenGL rendering context.\n");
        prDebugShowLastError();
        Destroy();
        return false;
    }


    // Show the window.
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);


    // Set up our perspective.
    Resize(width, height);


    // Store a pointer to the window class in the windows user data.
    SetWindowLong(m_hwnd, GWL_USERDATA, (long)(__int64)this);
    SetTitle(m_title);


    return true;
}


/// ---------------------------------------------------------------------------
/// Creates the application window.
/// ---------------------------------------------------------------------------
bool prWindow_PC::CreateTool(u32 width, u32 height, u32 menuID, u32 iconID, const TCHAR *title)
{
    PRUNUSED(menuID);
    PRUNUSED(iconID);

    // Ensure the previous window is destroyed.
    Destroy();

    
    // Init data
    m_fullScreen = false;
    m_title      = title;
    m_width      = width;
    m_height     = height;
    m_bits       = 32;

    
    // Create the window.
    if (CreateOpenGLWindow(menuID, iconID) == false)
    {
        return false;
    }


    // Get a device context to our window
    PRASSERT(m_hwnd);
    m_hdc = GetDC(m_hwnd);
    if (!m_hdc)
    {
        prTrace("Failed to create device context.\n");
        prDebugShowLastError();
        Destroy();
        return false;
    }


    // Set pixel format.
    if (SetOpenGLPixelFormat() == false)
    {
        return false;
    }


    // Create rendering context.
    m_glrc = wglCreateContext(m_hdc);
    if (!m_glrc)
    {
        prTrace("Couldn't create an OpenGL rendering context.\n");
        prDebugShowLastError();
        Destroy();
        return false;
    }


    // Activate rendering context.
    if (!wglMakeCurrent(m_hdc, m_glrc))
    {
        prTrace("Couldn't activate the OpenGL rendering context.\n");
        prDebugShowLastError();
        Destroy();
        return false;
    }


    // Show the window.
    ShowWindow(m_hwnd, SW_SHOW);
    SetForegroundWindow(m_hwnd);
    SetFocus(m_hwnd);


    // Set up our perspective.
    Resize(width, height);


    // Store a pointer to the window class in the windows user data.
    SetWindowLong(m_hwnd, GWL_USERDATA, (long)(__int64)this);
    SetTitle(m_title);


    return true;
}


/// ---------------------------------------------------------------------------
/// Destroys the PC application window.
/// ------------------------------ ---------------------------------------------
void prWindow_PC::Destroy()
{
    // Release rendering context.
    if (m_glrc && m_hdc)
    {
        // Make current render context null.
        if (!wglMakeCurrent(m_hdc, NULL))
        {
            prTrace("Failed to release rendering context.\n");
            prDebugShowLastError();
        }

        // Delete render context
        if (!wglDeleteContext(m_glrc))
        {
            prTrace("Failed to delete rendering context.\n");
            prDebugShowLastError();
        }
    }


    // Release device context.
    if (m_hdc && m_hwnd && !ReleaseDC(m_hwnd, m_hdc))
    {
        prTrace("Failed to release device context.\n");
        prDebugShowLastError();
    }


    // Got a window handle?
    if (m_hwnd)
    {
        // Remove pointer to this window.
        SetLastError(0);
        if (SetWindowLong(m_hwnd, GWL_USERDATA, 0) == 0)
        {
            if (GetLastError() != 0)
            {
                prTrace("Failed to remove user data.\n");
                prDebugShowLastError();
            }
        }

        // Destroy the window.
        if (!DestroyWindow(m_hwnd))
        {
            prTrace("Failed to destroy window.\n");
            prDebugShowLastError();
        }

        // Unregister the class.
        if (!UnregisterClass(g_ClassName, GetModuleHandle(NULL)))
        {
            prTrace("Failed to unregister the windows class.\n");
            prDebugShowLastError();
        }
    }


    // Are we in fullscreen mode?
    if (m_fullScreen)
    {
        // If yes, the switch back to the desktop.
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(TRUE);
    }


    // Reset data.
    m_glrc       = NULL;
    m_hdc        = NULL;
    m_hwnd       = NULL;
    m_fullScreen = false;
    m_width      = 0;
    m_height     = 0;
    m_bits       = 0;
    m_title      = 0;
}


/// ---------------------------------------------------------------------------
/// Resizes the window
/// ---------------------------------------------------------------------------
void prWindow_PC::Resize(u32 width, u32 height)
{
    // Prevent possible divide by zero.
    if (height == 0)
        height = 1;


    // Reset the current viewport
    glViewport(0, 0, width, height);
    ERR_CHECK();

    // Select the projection matrix and reset it.
    glMatrixMode(GL_PROJECTION);
    ERR_CHECK();
    glLoadIdentity();
    ERR_CHECK();


    // Calculate the aspect ratio of the window
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.5f, 2000.0f);
    ERR_CHECK();


    // Select the modelview matrix and reset it.
    glMatrixMode(GL_MODELVIEW);
    ERR_CHECK();
    glLoadIdentity();
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Sets the windows title.
/// ---------------------------------------------------------------------------
void prWindow_PC::SetTitle(const TCHAR *title)
{
    if (title && *title)
    {
        if (m_hwnd)
        {
            SetWindowText(m_hwnd, title);

            // Set registry
            prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (reg)
            {
                s32 size = prStringLengthW(title) + 1;
                PRASSERT(size > 0);

                char *buffer = new char [prStringLengthW(title) + 1];
                PRASSERT(buffer);

                prStringSprintf(buffer, size, "%s", title);
                reg->SetValue("WindowName", buffer);
                
                PRSAFE_DELETE_ARRAY(buffer);
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets the windows title.
/// ---------------------------------------------------------------------------
void prWindow_PC::SetTitle(const char *title)
{
    if (title && *title)
    {
        if (m_hwnd)
        {
            SetWindowTextA(m_hwnd, title);

            // Set registry
            prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            if (reg)
            {
                reg->SetValue("WindowName", title);
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Centers the window
/// ---------------------------------------------------------------------------
void prWindow_PC::CenterWindow(HWND hwnd)
{
    RECT rect;
    int width, height;      
    int screenwidth, screenheight;
    int x, y;

    // Get windows width and height
    GetWindowRect(hwnd, &rect);        
    width  = rect.right  - rect.left;
    height = rect.bottom - rect.top;


    // Get desktop width and height
    screenwidth  = GetSystemMetrics(SM_CXSCREEN);
    screenheight = GetSystemMetrics(SM_CYSCREEN);

    // Windows position
    x = (screenwidth  / 2) - (width / 2);
    y = (screenheight / 2) - (height / 2);
  
    MoveWindow(hwnd, x, y, width, height, FALSE);
}


/// ---------------------------------------------------------------------------
/// Switches to full screen display.
/// ---------------------------------------------------------------------------
bool prWindow_PC::ChangeToFullScreen()
{
    DEVMODE dmScreenSettings;
    

    ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));

    dmScreenSettings.dmSize         = sizeof(DEVMODE);                              // Size of the devmode structure.
    dmScreenSettings.dmPelsWidth    = m_width;                                      // Requested screen width.
    dmScreenSettings.dmPelsHeight   = m_height;                                     // Requested screen height.
    dmScreenSettings.dmBitsPerPel   = m_bits;                                       // Requested bits per pixel.
    dmScreenSettings.dmFields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT; // State which DEVMODE members to use.


    // Try to set requested mode.  NOTE: CDS_FULLSCREEN gets rid of the start bar.
    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
        prTrace("Display setting not supported.\n");
        return false;
    }

    return true;
}


/// ---------------------------------------------------------------------------
/// Creates the window.
/// ---------------------------------------------------------------------------
bool prWindow_PC::CreateOpenGLWindow(u32 menuID, u32 iconID)
{
    DWORD       dwExStyle;              // Window extended style
    DWORD       dwStyle;                // Window style
    RECT        rect;                   // Rect for the window size.


    // Set required screen size
    rect.left   = 0;
    rect.right  = m_width;
    rect.top    = 0;
    rect.bottom = m_height;


    // Set window style dependent on whether we are going into full screen mode or windowed mode.
    if (m_fullScreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_POPUP;
        
        ShowCursor(FALSE);
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle   = PR_OVERLAPPEDWINDOW;
    }


    // Adjust window to true requested size
    AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);


    // Register the window class
    if (!RegisterWindowClass(menuID, iconID))
    {
        return false;
    }


    // Create the window
    m_hwnd = CreateWindowEx
    (   
        dwExStyle,                                      // Extended style for the window.
        g_ClassName,                                    // Class name.
        m_title,                                        // Window title.
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,    // Window style.
        CW_USEDEFAULT, CW_USEDEFAULT,                   // Window Position.
        rect.right  - rect.left,                        // Window width.
        rect.bottom - rect.top,                         // Window height.
        NULL,                                           // Parent window.
        NULL,                                           // Menu.
        GetModuleHandle(NULL),                          // Application instance.
        NULL                                            // Pointer to window creation data.
    );


    if (m_hwnd == NULL)
    {
        Destroy();
        prTrace("Window creation failed.\n");
        return false;
    }
    else
    {
        CenterWindow(m_hwnd);
    }


    return true;        
}


/// ---------------------------------------------------------------------------
/// Registers the windows class.
/// ---------------------------------------------------------------------------
bool prWindow_PC::RegisterWindowClass(u32 menuID, u32 iconID)
{
    WNDCLASSEX  wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = prWindowProcedure;
    wc.style         = 0;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hbrBackground = 0;
    wc.hInstance     = GetModuleHandle(NULL);
    wc.hIcon         = (iconID == 0) ? LoadIcon(NULL, IDI_APPLICATION) : (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(iconID), IMAGE_ICON, 32, 32, 0);
    wc.hIconSm       = (iconID == 0) ? LoadIcon(NULL, IDI_APPLICATION) : (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(iconID), IMAGE_ICON, 16, 16, 0);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszMenuName  = (menuID == 0) ? 0 : MAKEINTRESOURCE(menuID);
    wc.lpszClassName = g_ClassName;

    if (!RegisterClassEx(&wc))
    {
        prTrace("Window Registration Failed.\n");
        return false;
    }

    return true;
}


/// ---------------------------------------------------------------------------
/// Sets the pixel format.
/// ---------------------------------------------------------------------------
bool prWindow_PC::SetOpenGLPixelFormat()
{
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),              // Size of this pixel format descriptor.
        1,                                          // Version number.
        PFD_DRAW_TO_WINDOW |                        // Format must support window.
        PFD_SUPPORT_OPENGL |                        // Format must support opengl.
        PFD_DOUBLEBUFFER,                           // Must support double buffering.
        PFD_TYPE_RGBA,                              // Request an rgba format.
        (BYTE)m_bits,                               // Select our color depth.
        0, 0, 0, 0, 0, 0,                           // Color bits. (ignored)
        0,                                          // No alpha buffer.
        0,                                          // Alpha bits. (ignored)
        0,                                          // No accumulation. buffer
        0, 0, 0, 0,                                 // Accumulation bits. (ignored)
        16,                                         // 16bit z-buffer. (depth buffer)  
        0,                                          // No stencil buffer.
        0,                                          // No auxiliary buffer.
        PFD_MAIN_PLANE,                             // Main drawing layer.
        0,                                          // Reserved.
        0, 0, 0                                     // No layer, visible, damage masks.
    };


    // Find a matching pixel format
    int pixelFormat = ChoosePixelFormat(m_hdc, &pfd);
    if (pixelFormat == 0)
    {
        Destroy();
        prTrace("Can't find a suitable pixel format.\n");
        return false;
    }


    // Set pixel format
    if (SetPixelFormat(m_hdc, pixelFormat, &pfd) == false)
    {
        Destroy();
        prTrace("Can't set the pixel format.\n");
        return false;
    }


    return true;
}


#endif
