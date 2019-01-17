/**
 * prWindow_Linux.cpp
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


#if defined(PLATFORM_LINUX)


#include "prWindow_Linux.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../display/prOglUtils.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../core/prStringUtil.h"
#include "../linux/prLinux.h"


//#if defined(ALLOW_GLEW)
//#include <glew.h>
//#include <wglew.h>
//#endif


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWindow_Linux::prWindow_Linux() : prWindow()
{
    m_title = NULL;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prWindow_Linux::~prWindow_Linux()
{
    Destroy();
}


/// ---------------------------------------------------------------------------
/// Creates the application window.
/// ---------------------------------------------------------------------------
bool prWindow_Linux::Create(u32 width, u32 height, u32 bits, bool fullScreen)
{
    // Ensure the previous window is destroyed.
    Destroy();


    // Init data
    m_fullScreen = fullScreen;
    m_title      = "Proteus";
    m_width      = width;
    m_height     = height;
    m_bits       = bits;

    bool result = (prLinuxCreateDisplay(width, height) == PRTRUE);
    if (result)
    {
        Resize(width, height);
        SetTitle(m_title);
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Destroys the PC application window.
/// ------------------------------ ---------------------------------------------
void prWindow_Linux::Destroy()
{
}


/// ---------------------------------------------------------------------------
/// Resizes the window
/// ---------------------------------------------------------------------------
void prWindow_Linux::Resize(u32 width, u32 height)
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
void prWindow_Linux::SetTitle(const char *title)
{
    if (title && *title)
    {
        m_title = title;
    }
}


#endif
