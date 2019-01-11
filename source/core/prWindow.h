// File: prWindow.h
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


#pragma once


#include "prTypes.h"
#include "../debug/prAssert.h"


// Class: prWindow
//      Base class for all window classes.
//
// Notes:
//      The class is designed to allow for the window to be recreated.
//
// Notes:
//      The minimum width/height are '0xFFFFFFFF' and need to be set to
//      a sensible value to be used properly
class prWindow
{
public:
    // Method: prWindow
    //      Ctor    
    prWindow();

    // Method: ~prWindow
    //      Dtor    
    virtual ~prWindow();

    // Method: Create
    //      Creates the application window.
    //
    // Parameters:
    //      width      - Desired window width.
    //      height     - Desired window height.
    //      bits       - Window bit depth.
    //      fullScreen - Fullscreen or windowed.
    //
    // Returns:
    //      Window created true or false.
    virtual bool Create(u32 width, u32 height, u32 bits, bool fullScreen) = 0;

    // Method: Destroy
    //      Destroys the application window.
    virtual void Destroy() = 0;

    // Method: Resize
    //      Resizes the application window.
    //
    // Parameters:
    //      width  - New window width.
    //      height - New window height.
    //
    // Notes:
    //      Called by engine. *DO NOT USE*
    virtual void Resize(u32 width, u32 height) = 0;

    // Method: SetActive
    //      Sets whether the window is active or inactive.
    //
    // Parameters:
    //      state - true or false.
    //
    // Notes:
    //      Called by engine. *DO NOT USE*
    void SetActive(bool state) { m_active = state; }

    // Method: GetFullScreen
    //      Gets whether the window is full screen or not
    bool GetFullScreen() const { return m_fullScreen; }

    // Method: GetActive
    //      Gets whether the window is active or inactive.
    bool GetActive() const { return m_active; }

    // Method: GetWidth
    //  Gets the windows width
    u32 GetWidth() const { return m_width; }

    // Method: GetHeight
    //      Gets the windows height
    u32 GetHeight() const { return m_height; }

    // Method: GetBits
    //      Gets the windows bit depth
    u32 GetBits() const { return m_bits; }

    // Method: GetMinWidth
    //      Gets the windows minimum width
    //
    // Notes:
    //      This is used during window resizing by the engine
    u32 GetMinWidth() const { return m_minWidth; }

    // Method: GetMinHeight
    //      Gets the windows minimum height
    //
    // Notes:
    //      This is used during window resizing by the engine
    u32 GetMinHeight() const { return m_minHeight; }

    // Method: SetMinSize
    //      Gets the windows minimum size
    //
    // Parameters:
    //      width  - minimum width
    //      height - minimum height
    //
    // Notes:
    //      This is used during window resizing by the engine
    //
    // Notes:
    //      Neither values should be zero
    void SetMinSize(u32 width, u32 height) { PRASSERT(width > 0);
                                                                           PRASSERT(height > 0);
                                                                           m_minWidth  = width;
                                                                           m_minHeight = height; }

protected:

    u32     m_width;        // Width of the window.
    u32     m_height;       // Height of the window.
    u32     m_bits;         // Bits per pixel.
    u32     m_minWidth;     // Minimum width for windowed OS's like linux, mac and windows
    u32     m_minHeight;    // Minimum height for windowed OS's like linux, mac and windows

    bool    m_fullScreen;   // Are we fullscreen or windowed?
    bool    m_active;       // Is the window active. This is set by the system.
    bool    m_exp0;         // Expansion.
    bool    m_exp1;         // Expansion.
};
