/**
 * prOglUtils.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"


// ----------------------------------------------------------------------------
// Platform specifics.
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
  #include <windows.h>
  #include <gl/gl.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>  
  #include <stdio.h>

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>

#else
  #error No platform defined.

#endif


#include "../core/prTypes.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "prOglUtils.h"
//#include "Renderer.h"
//#include "Texture.h"


/// ---------------------------------------------------------------------------
/// Shows the opengl extensions available for the current platform.
/// ---------------------------------------------------------------------------
void prOpenGLShowExtensions()
{
    // Show GL details
    prTrace("OpenGL\n");
    prTrace("Vendor  : %s\n", (const char *)glGetString(GL_VENDOR));
    prTrace("Renderer: %s\n", (const char *)glGetString(GL_RENDERER));
    prTrace("Version : %s\n", (const char *)glGetString(GL_VERSION));
    prTrace("-\n");

    // Show extensions.
    const char *extensions = (const char *)glGetString(GL_EXTENSIONS);
    char c = 0;
    u32  i = 0;
    char buffer[256];
    do
    {
        c = *extensions++;

        if (c == '\0' || c == ' ')
        {
            buffer[i] = 0;
            
            if (i > 0)
            {
                i = 0;
                prTrace("Extension: %s\n", buffer);
                //OnScreenLogger::GetInstance()->Add(buffer);
            }
        }
        else
        {
            if (i < sizeof(buffer) + 2)
            {
                buffer[i++] = c;
            }
        }
    }
    while(c != '\0');
}


/// ---------------------------------------------------------------------------
/// Draws the engine water mark.
/// ---------------------------------------------------------------------------
void prDrawWaterMark(Texture *pTexture)
{
/*    if (pTexture)
    {
        Renderer::GetInstance()->SetOrthographicView(true);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glPushMatrix();            
            //glEnable(GL_TEXTURE_2D);

            glColor4f(1, 1, 1, 0.1f);
            glTranslatef(8, 8, 0);
            glScalef(64, 24, 0);

            pTexture->Bind();
            Renderer::GetInstance()->DrawQuad();

            glColor4f(1, 1, 1, 1);            
            glPopMatrix();

        //glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        Renderer::GetInstance()->RestorePerspectiveView();
    }*/
}


/// ---------------------------------------------------------------------------
/// Checks if the previous action caused an error, If so its displayed
/// ---------------------------------------------------------------------------
void prOpenGLErrorCheck(const char *file, const char *func, int line)
{
#if defined(_DEBUG) || defined(DEBUG)

    PRASSERT(file && *file);
    PRASSERT(func && *func);

    // Don't print error messages if game is exiting
//    if (System::GameExitInProgress())
 //       return;
    TODO("fix")

    // Write message
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        const char *err = NULL;

        switch(error)
        {
        case GL_INVALID_ENUM:
            err = "GL_INVALID_ENUM";
            break;

        case GL_INVALID_VALUE:
            err = "GL_INVALID_VALUE";
            break;

        case GL_INVALID_OPERATION:
            err = "GL_INVALID_OPERATION";
            break;

        case GL_STACK_OVERFLOW:
            err = "GL_STACK_OVERFLOW";
            break;

        case GL_STACK_UNDERFLOW:
            err = "GL_STACK_UNDERFLOW";
            break;

        case GL_OUT_OF_MEMORY:
            err = "GL_OUT_OF_MEMORY";
            break;

        default:
            err = "UNKNOWN";
            break;
        }

        prTrace("prRenderer error: %s\n", err); 
        prTrace("File: %s\nFunc: %s\nLine: %i\n", file, func, line);
    }

#else

    PRUNUSED(file);
    PRUNUSED(func);
    PRUNUSED(line);

#endif
}
