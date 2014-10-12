// File: prWindow_Ios.h
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


#ifndef __PRWINDOW_IOS_H
#define __PRWINDOW_IOS_H


#include "../prConfig.h"


#if defined(PLATFORM_IOS)


#include <OpenGLES/ES1/gl.h>
#include "prTypes.h"
#include "prWindow.h"


// Class: prWindow_Ios
//      Window class for ios.
class prWindow_Ios : public prWindow
{
public:
    // Method: prWindow_Ios
    //      Ctor    
    prWindow_Ios();

    // Method: ~prWindow_Ios
    //      Dtor    
    ~prWindow_Ios();

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
    bool Create(u32 width, u32 height, u32 bits, bool fullScreen);

    // Method: Destroy
    //      Destroys the application window.
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
    void Resize(u32 width, u32 height);

    // Method: SetTitle
    //      Sets the applications window title.
    //
    // Parameters:
    //      title      - The windows title.
    void SetTitle(const char *title);


private:

    //
    const char     *m_title;        // Text displayed in the title bar.
};


#endif//PLATFORM_IOS


#endif//__PRWINDOW_IOS_H

