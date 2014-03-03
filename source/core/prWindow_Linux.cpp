/**
 * prWindow_Linux.cpp
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


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWindow_Linux::prWindow_Linux() : prWindow()
{
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
