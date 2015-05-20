// File: prWindow_PC.h
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


#ifndef __PRWINDOWPC_H
#define __PRWINDOWPC_H


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
#include <gl/gl.h>
#include <gl/glu.h>
#include "prTypes.h"
#include "prWindow.h"


// Class: prWindow_PC
//      Class for the PC.
class prWindow_PC : public prWindow
{
public:
    // Method: prWindow_PC
    //      Ctor    
    prWindow_PC();

    // Method: ~prWindow_PC
    //      Dtor    
    ~prWindow_PC();

    // Method: Create
    //      Creates the PC application window.
    //
    // Parameters:
    //      width      - Desired window width.
    //      height     - Desired window height.
    //      bits       - Window bit depth.
    //      fullScreen - Fullscreen or windowed.
    //
    // Returns:
    //      Window created true or false.
    bool Create(Proteus::Core::u32 width, Proteus::Core::u32 height, Proteus::Core::u32 bits, bool fullScreen);

    // Method: CreateTool
    //      Creates the PC application window for engine tools.
    //
    // Parameters:
    //      width      - Desired window width.
    //      height     - Desired window height.
    //      menuID     - The ID if a menu is used. Sets the apps menu
    //      iconID     - The ID if an icon is used. Sets the app icon
    //      title      - The windows title.
    //
    // Returns:
    //      Window created true or false.
    bool CreateTool(Proteus::Core::u32 width, Proteus::Core::u32 height, Proteus::Core::u32 menuID, Proteus::Core::u32 iconID, const TCHAR *title);

    // Method: Destroy
    //      Destroys the PC application window.
    void Destroy();

    // Method: Resize
    //      Resizes the application window.
    //
    // Parameters:
    //      width  - New window width.
    //      height - New window height.
    //
    // Notes:
    //      Called by engine. *DO NOT USE*
    void Resize(Proteus::Core::u32 width, Proteus::Core::u32 height);

    // Method: SetTitle
    //      Sets the PC application window title.
    //
    // Parameters:
    //      title      - The windows title.
    void SetTitle(const TCHAR *title);

    // Method: SetTitle
    //      Sets the PC application window title.
    //
    // Parameters:
    //      title      - The windows title.
    void SetTitle(const char *title);

    // Method: GetWindowHandle
    //      Gets the PC applications window handle.
    HWND GetWindowHandle() { return m_hwnd; }

    // Method: GetDeviceContext
    //      Gets the PC applications device context.
    HDC  GetDeviceContext() { return m_hdc; }


private:

    void CenterWindow(HWND hwnd);
    bool ChangeToFullScreen();
    bool CreateOpenGLWindow(Proteus::Core::u32 menuID, Proteus::Core::u32 iconID);
    bool RegisterWindowClass(Proteus::Core::u32 menuID, Proteus::Core::u32 iconID);
    bool SetOpenGLPixelFormat();


private:

    HGLRC           m_glrc;         // Handle to the OpenGL rendering context.
    HWND            m_hwnd;         // Handle to the window.
    HDC             m_hdc;          // Handle to a device context.
    const TCHAR    *m_title;        // Text displayed in the title bar.
};


#endif//PLATFORM_PC


#endif//__PRWINDOWPC_H
