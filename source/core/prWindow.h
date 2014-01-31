/**
 * @file       prWindow.h
 * @brief      Contains the base window class
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


#ifndef __PRWINDOW_H
#define __PRWINDOW_H


#include "prTypes.h"


/// @brief  Base class for all window classes.
/// @note   The class is designed to allow for the window to be recreated by
/// @note   using Destroy followed by Create
class prWindow
{
public:

    /// @brief      Ctor
    prWindow();

    /// @brief      Dtor
    virtual ~prWindow();

    /// @brief      Creates the application window.
    /// @param      width      - Desired window width.
    /// @param      height     - Desired window height.
    /// @param      bits       - Window bit depth.
    /// @param      fullScreen - Fullscreen or windowed.
    /// @return     bool       - Created window true or false.
    virtual bool Create(u32 width, u32 height, u32 bits, bool fullScreen) = 0;

    /// @brief      Destroys the application window.
    virtual void Destroy() = 0;

    /// @brief      Resizes the application window.
    /// @param      width  - New window width.
    /// @param      height - New window height.
    virtual void Resize(u32 width, u32 height) = 0;

    /// @brief      Sets whether the window is active or inactive.
    /// @param      state - true or false.
    void SetActive(bool state) { m_active = state; }

    /// @brief      Gets whether the window is full screen or not
    /// @return     bool - true or false.
    bool GetFullScreen() const { return m_fullScreen; }

    /// @brief  Gets whether the window is active or inactive.
    /// @return     bool - true or false.
    bool GetActive() const { return m_active; }

    /// @brief      Gets the windows width
    /// @return     u32 - The windows width.
    u32 GetWidth() const { return m_width; }

    /// @brief      Gets the windows height
    /// @return     u32 - The windows height.
    u32 GetHeight() const { return m_height; }

    /// @brief      Gets the windows bit depth
    /// @return     u32 - The windows bit depth.
    u32 GetBits() const { return m_bits; }


protected:

    u32             m_width;        // Width of the window.
    u32             m_height;       // Height of the window.
    u32             m_bits;         // Bits per pixel.

    bool            m_fullScreen;   // Are we fullscreen or windowed?
    bool            m_active;       // Is the window active. This is set by the system.
    bool            m_exp0;         // Expansion.
    bool            m_exp1;         // Expansion.
};


#endif//__PRWINDOW_H
