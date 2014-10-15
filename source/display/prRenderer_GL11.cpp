/**
 * prRenderer_GL11.cpp
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
 *
 * http://www.opengl.org/wiki/Platform_specifics:_Windows
 * http://www.realtimerendering.com/intersections.html
 * http://maniacdev.com/2009/09/the-categorized-opengl-es-tutorial-collection/
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>
  #include "../core/prWindow_PC.h"

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include "../core/prWindow_PC.h"
  #include "../linux/prLinux.h"

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  //#include "../core/prWindow_PC.h"
  //#include "../linux/prLinux.h"

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include "../core/prWindow_PC.h"

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>

#else
  #error No platform defined.

#endif


#include <math.h>
#include "prRenderer_GL11.h"
#include "prColour.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../math/prMathsUtil.h"
#include "../math/prVector3.h"
#include "../core/prWindow.h"
#include "../core/prVertex.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../core/prStringUtil.h"
#include "../core/prResourceManager.h"
#include "../display/prOglUtils.h"
#include "../display/prTexture.h"


// Splash image
#if defined(PLATFORM_PC)
#include "prSplash.h"
#endif


// Use ant tweak bar.
#if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
#include "../core/prATB.h"
#endif


// ----------------------------------------------------------------------------
// Constants for circle drawing.
// ----------------------------------------------------------------------------
static const float SEGMENTS  = 32.0f;
static const int   VERTICES  = 32;                                              // This must be the same as SEGMENTS
static const float INCREMENT = 2.0f * Proteus::Maths::Pi / SEGMENTS;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prRenderer_GL11::prRenderer_GL11() : prRenderer()
{
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prRenderer_GL11::~prRenderer_GL11()
{
}


/// ---------------------------------------------------------------------------
/// Inits the renderer with basic settings.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::Init()
{
    // Init OpenGL
    glShadeModel(GL_SMOOTH);                            // Enables smooth shading
    ERR_CHECK();

    glEnable(GL_TEXTURE_2D);                            // Enable Texture Mapping
    ERR_CHECK();
    
    // Colour
    glClearColor(0.0f, 0.0f, 0.2f, 0.0f);               // Clear colour
    ERR_CHECK();
    
    // Depth buffer setup
#if defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)
    glClearDepthf(1.0f);
    ERR_CHECK();
#else
    glClearDepth(1.0f);
    ERR_CHECK();
#endif

    glEnable(GL_DEPTH_TEST);                            // Enables depth testing
    ERR_CHECK();    
    glDepthFunc(GL_LEQUAL);                             // The type of depth test to do
    ERR_CHECK();

    glEnable(GL_CULL_FACE);                             // Enable culling.
    ERR_CHECK();
    glCullFace(GL_BACK);
    ERR_CHECK();

    // Really nice perspective calculations
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    ERR_CHECK();

    // Load watermark
    #if defined(PLATFORM_PC) && defined(PROTEUS_ALLOW_WATERMARK)
    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
    PRASSERT(pRM)
    //m_pWatermark = pRM->LoadFromMemory<prTexture>("proteusImg", proteusImg, sizeof(proteusImg));
    m_pWatermark = pRM->LoadFromMemory<prTexture>("proteusImg", proteusImg, prSplashSize());
    PRASSERT(m_pWatermark);

    #else
    m_pWatermark = NULL;

    #endif


    // Show extensions.
    #if defined(OPENGL_SHOW_EXTENSIONS)
    prOpenGLShowExtensions();
    #endif

    prOpenGLInit();
}


/// ---------------------------------------------------------------------------
/// Destroys the renderer.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::Destroy()
{
    // For OpenGL we do nothing.
}


/// ---------------------------------------------------------------------------
/// Begins the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::Begin()
{
    // Clear screen and depth buffer.
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    ERR_CHECK();

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    ERR_CHECK();

    // Enabled culling
    glEnable(GL_CULL_FACE);
    ERR_CHECK();
    glCullFace(GL_BACK);
    ERR_CHECK();

    // Reset the current modelview matrix.
    glMatrixMode(GL_MODELVIEW);
    ERR_CHECK();

    glLoadIdentity();
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Ends the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::End()
{
    // For OpenGL we do nothing.
}


/// ---------------------------------------------------------------------------
/// Shows the previously rendered image.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::Present()
{
    if (m_pWindow)
    {
        // Draw watermark
        #if defined(PLATFORM_PC) && defined(PROTEUS_ALLOW_WATERMARK)
        prDrawWaterMark(m_pWatermark);
        #endif

        // Draw tweak bars
        #if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
        prATBDraw();
        #endif

        #if defined(PLATFORM_PC)
        SwapBuffers(static_cast<prWindow_PC*>(m_pWindow)->GetDeviceContext());
        #endif

        #if defined(PLATFORM_LINUX)
        prLinuxSwapBuffers();
        #endif
    }
}


/// ---------------------------------------------------------------------------
/// Set orthographic view in preparation for 2D rendering.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::SetOrthographicView()
{
    if (m_pWindow)
    {
        glMatrixMode(GL_PROJECTION);
        ERR_CHECK();
        
        glPushMatrix();
        ERR_CHECK();

        glLoadIdentity();
        ERR_CHECK();
        
#if defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID) || defined(PLATFORM_IOS)
        glOrthof(0, m_pWindow->GetWidth(), m_pWindow->GetHeight(), 0, 0, 1);
        ERR_CHECK();
#else
        glOrtho(0, m_pWindow->GetWidth(), m_pWindow->GetHeight(), 0, 0, 1);
        ERR_CHECK();
#endif

        glMatrixMode(GL_MODELVIEW);
        ERR_CHECK();

        glLoadIdentity();
        ERR_CHECK();

        glDisable(GL_DEPTH_TEST);
        ERR_CHECK();

        glDisable(GL_CULL_FACE);
        ERR_CHECK();

//#if defined(PLATFORM_ANDROID)
//        // Displacement trick for exact pixelization
//        glTranslatef(0.375f, 0.375f, 0.0f);
//        ERR_CHECK();
//#endif
    }
}


/// ---------------------------------------------------------------------------
/// Restore perspective projection.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::RestorePerspectiveView()
{
    if (m_pWindow)
    {
        glMatrixMode(GL_PROJECTION);
        ERR_CHECK();

        glPopMatrix();
        ERR_CHECK();

        glMatrixMode(GL_MODELVIEW);
        ERR_CHECK();

        glEnable(GL_DEPTH_TEST);
        ERR_CHECK();

        // Enabled culling
        glEnable(GL_CULL_FACE);
        ERR_CHECK();
        glCullFace(GL_BACK);
        ERR_CHECK();
    }
}


/// ---------------------------------------------------------------------------
/// Draws a single point.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawPoint(f32 x, f32 y)
{
    prVertex2D vertices[] = 
    {
        {x, y},
    };

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_POINTS, 0, 1);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a line
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawLine(f32 x1, f32 y1, f32 x2, f32 y2)
{
    prVertex2D vertices[] = 
    {
        {x1, y1},
        {x2, y2},
    };

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_LINES, 0, 2);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a line 3D.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawLine(prVector3 &from, prVector3 &to)
{
    prVertex3D vertices[] = 
    {
        {from.x, from.y, from.z},
        {to.x,   to.y,   to.z},
    };

    glVertexPointer(3, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_LINES, 0, 2);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a hollow rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
    prVertex2D vertices[] = 
    {
        {x1, y1},
        {x2, y1},
        {x2, y2},
        {x1, y2},
    };
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_LINE_LOOP, 0, 4);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a filled rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
    prVertex2D vertices[] = 
    {
        {x1, y1},
        {x2, y1},
        {x2, y2},
        {x2, y2},
        {x1, y2},
        {x1, y1},
    };
    
    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a hollow circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawCircle(f32 x, f32 y, f32 radius)
{
    prVertex2D vertices[VERTICES];
    
    float theta = 0.0f;

    for (int i = 0; i < VERTICES; ++i, theta += INCREMENT)
    {
        vertices[i].x = x + cosf(theta) * radius;
        vertices[i].y = y + sinf(theta) * radius;
    }

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_LINE_LOOP, 0, VERTICES);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a filled circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawFilledCircle(f32 x, f32 y, f32 radius)
{
    prVertex2D vertices[VERTICES];
    
    float theta = 0.0f;

    for (int i = 0; i < VERTICES; ++i, theta += INCREMENT)
    {
        vertices[i].x = x + cosf(theta) * radius;
        vertices[i].y = y + sinf(theta) * radius;
    }

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_TRIANGLE_FAN, 0, VERTICES);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a hollow polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawPolygon(prVertex2D *vertices, s32 count)
{
    PRASSERT(vertices);
    PRASSERT(count > 0);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_LINE_STRIP, 0, count);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a filled polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawFilledPolygon(prVertex2D *vertices, s32 count)
{
    PRASSERT(vertices);
    PRASSERT(count > 0);

    glVertexPointer(2, GL_FLOAT, 0, vertices);
    ERR_CHECK();
    
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();

    glDrawArrays(GL_TRIANGLE_FAN, 0, count);
    ERR_CHECK();

    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Sets the draw colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::SetColour(const prColour &colour)
{
    glColor4f(colour.red, colour.green, colour.blue, colour.alpha);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Sets the clear colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::SetClearColour(const prColour &colour)
{
    glClearColor(colour.red, colour.green, colour.blue, colour.alpha);
    ERR_CHECK();
}


//(0,1)-------------(1,1)
//  |                 |
//  |                 |
//  |      Image      |
//  |                 |
//  |                 |
//(0,0)-------------(1,0)


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawQuad()
{
    DrawQuad(0.0f, 0.0f, 1.0f, 1.0f);
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawQuad(float u0, float v0, float u1, float v1)
{
    TODO("Remove colour set from draw quad")

    static float Colors[] =
    {
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };

    QuadData quadData[] =
    {
        {-0.5f,  0.5f, u0, v0, },
        {-0.5f, -0.5f, u0, v1, },
        { 0.5f,  0.5f, u1, v0, },
        { 0.5f, -0.5f, u1, v1, },
    };


    // Stops static analyzer moan.
    PRUNUSED(quadData->v);

    // Set states
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
    glEnableClientState(GL_COLOR_ARRAY);
    ERR_CHECK();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ERR_CHECK();

        glVertexPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->x);
        ERR_CHECK();
        
        glColorPointer(4, GL_FLOAT, 0,  Colors);
        ERR_CHECK();

        glTexCoordPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->u);
        ERR_CHECK();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        ERR_CHECK();

    // Reset states
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    ERR_CHECK();
    glDisableClientState(GL_COLOR_ARRAY);
    ERR_CHECK();
    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::DrawQuad(float u0, float v0, float u1, float v1, prColour c)
{
    float Colors[] =
    {
        c.red, c.green, c.blue, c.alpha,
        c.red, c.green, c.blue, c.alpha,
        c.red, c.green, c.blue, c.alpha,
        c.red, c.green, c.blue, c.alpha
    };

    QuadData quadData[] =
    {
        {-0.5f,  0.5f, u0, v0, },
        {-0.5f, -0.5f, u0, v1, },
        { 0.5f,  0.5f, u1, v0, },
        { 0.5f, -0.5f, u1, v1, },
    };


    // Set states
    glEnableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
    glEnableClientState(GL_COLOR_ARRAY);
    ERR_CHECK();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ERR_CHECK();

        glVertexPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->x);
        ERR_CHECK();
        
        glColorPointer(4, GL_FLOAT, 0,  Colors);
        ERR_CHECK();

        glTexCoordPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->u);
        ERR_CHECK();

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        ERR_CHECK();

    // Reset states
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    ERR_CHECK();
    glDisableClientState(GL_COLOR_ARRAY);
    ERR_CHECK();
    glDisableClientState(GL_VERTEX_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c)
{
    float Colors[] =
    {
        c.red, c.green, c.blue, c.alpha,
        c.red, c.green, c.blue, c.alpha,
        c.red, c.green, c.blue, c.alpha,
        c.red, c.green, c.blue, c.alpha
    };

    QuadData quadData[] =
    {
        {-0.5f,  0.5f, u0, v0, },
        {-0.5f, -0.5f, u0, v1, },
        { 0.5f,  0.5f, u1, v0, },
        { 0.5f, -0.5f, u1, v1, },
    };


 //   // Set states
 //   glEnableClientState(GL_VERTEX_ARRAY);
 //   ERR_CHECK();
 //   glEnableClientState(GL_COLOR_ARRAY);
 //   ERR_CHECK();
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 //   ERR_CHECK();

        glVertexPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->x);
        ERR_CHECK();        
        glColorPointer(4, GL_FLOAT, 0,  Colors);
        ERR_CHECK();
        glTexCoordPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->u);
        ERR_CHECK();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        ERR_CHECK();

 //   // Reset states
    //glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 //   ERR_CHECK();
 //   glDisableClientState(GL_COLOR_ARRAY);
 //   ERR_CHECK();
 //   glDisableClientState(GL_VERTEX_ARRAY);
 //   ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Enables/disables textures.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::TexturesEnabled(bool state)
{
    if (state)
    {
        glEnable(GL_TEXTURE_2D);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }

    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Enables/disables blending.
/// ---------------------------------------------------------------------------
void prRenderer_GL11::BlendEnabled(bool state)
{
    if (state)
    {
        glEnable(GL_BLEND);
        ERR_CHECK();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ERR_CHECK();
    }
    else
    {
        glDisable(GL_BLEND);
        ERR_CHECK();
    }
}
