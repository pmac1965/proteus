// File: prWindow_Android.h
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


#ifndef __PRWINDOW_ANDROID_H
#define __PRWINDOW_ANDROID_H


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)


#include <GLES/gl.h>
//#include <GLES/glu.h>
#include "prTypes.h"
#include "prWindow.h"


// Class: prWindow_Android
//      Window class for android.
class prWindow_Android : public prWindow
{
public:
    // Method: prWindow_Android
    //      Ctor    
    prWindow_Android();

    // Method: ~prWindow_Android
    //      Dtor    
    ~prWindow_Android();

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
    bool Create(Proteus::Core::u32 width, Proteus::Core::u32 height, Proteus::Core::u32 bits, bool fullScreen);

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
    void Resize(Proteus::Core::u32 width, Proteus::Core::u32 height);

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


#endif//PLATFORM_ANDROID


#endif//__PRWINDOW_ANDROID_H

