/**
 * @file       prWindow_PC.h
 * @brief      Contains the PC window class
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
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


#include "../prConfig.h"


#ifndef __PRWINDOWPC_H
#define __PRWINDOWPC_H


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


/// @brief  Class for the PC classes.
/// @note   The class is designed to allow for the window to be recreated by
/// @note   using Destroy followed by Create
class prWindow_PC : public prWindow
{
public:
    /// @brief      Ctor
    prWindow_PC();

    /// @brief      Dtor
    ~prWindow_PC();

    /// @brief      Creates the PC application window.
    /// @param      width      - Desired window width.
    /// @param      height     - Desired window height.
    /// @param      bits       - Window bit depth.
    /// @param      fullScreen - Fullscreen or windowed.
    /// @return     true on success, false on failure.
    bool Create(u32 width, u32 height, u32 bits, bool fullScreen);// , const TCHAR *title);

    /// @brief      Creates the PC application window.
    /// @param      width      - Desired window width.
    /// @param      height     - Desired window height.
    /// @param      menuID     - The ID if a menu is used. Sets the apps menu
    /// @param      iconID     - The ID if an icon is used. Sets the app icon
    /// @param      title      - The windows title.
    /// @return     true on success, false on failure.
    bool Create(u32 width, u32 height, u32 menuID, u32 iconID, const TCHAR *title);

    /// @brief      Destroys the PC application window.
    void Destroy();

    /// @brief      Resizes the PC application window.
    /// @param      width      - Desired window width.
    /// @param      height     - Desired window height.
    void Resize(u32 width, u32 height);

    /// @brief      Sets the PC application window title.
    /// @param      title      - The windows title.
    void SetTitle(const TCHAR *title);

    /// @brief      Gets the PC applications window handle.
    /// @return     The windows handle.
    HWND GetWindowHandle() { return m_hwnd; }

    /// @brief      Gets the PC applications device context.
    /// @return     The applications device context.
    HDC  GetDeviceContext() { return m_hdc; }


private:

    bool ChangeToFullScreen();
    bool CreateOpenGLWindow(u32 menuID, u32 iconID);
    bool RegisterWindowClass(u32 menuID, u32 iconID);
    bool SetOpenGLPixelFormat();

private:

    HGLRC           m_glrc;         // Handle to the OpenGL rendering context.
    HWND            m_hwnd;         // Handle to the window.
    HDC             m_hdc;          // Handle to a device context.
    const TCHAR    *m_title;        // Text displayed in the title bar.
};


#endif//PLATFORM_PC


#endif//__PRWINDOWPC_H
