/**
 * prRenderer_GL20.cpp
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


#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

//

  #include "../core/prWindow_PC.h"

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include "../core/prWindow_PC.h"
  #include "../linux/prLinux.h"

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>

#elif defined(PLATFORM_ANDROID)
  //#include <GLES/gl.h>
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <EGL/egl.h>
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
#include "prRenderer_GL20.h"
#include "prColour.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prMacros.h"
//#include "../debug/prDebug.h"
//#include "../math/prMathsUtil.h"
//#include "../math/prVector3.h"
//#include "../core/prWindow.h"
//#include "../core/prVertex.h"
//#include "../core/prCore.h"
//#include "../core/prRegistry.h"
//#include "../core/prStringUtil.h"
//#include "../core/prResourceManager.h"
//#include "../display/prOglUtils.h"
//#include "../display/prTexture.h"


// Splash image
#if defined(PLATFORM_PC)
#include "prSplash.h"
#endif


//// Use ant tweak bar.
//#if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
//#include "../core/prATB.h"
//#endif


//// ----------------------------------------------------------------------------
//// Constants for circle drawing.
//// ----------------------------------------------------------------------------
//static const float SEGMENTS  = 32.0f;
//static const int   VERTICES  = 32;                                              // This must be the same as SEGMENTS
//static const float INCREMENT = 2.0f * Proteus::Maths::Pi / SEGMENTS;


/// ---------------------------------------------------------------------------
/// Using a pimpl interface to minimize external includes in the header files,
/// which can be a right pain in the neck
/// ---------------------------------------------------------------------------
class RendererImplementation
{
public:

    RendererImplementation()
    {
#if defined(PLATFORM_ANDROID)
        mDisplay = EGL_NO_DISPLAY;
        mContext = EGL_NO_CONTEXT;
        mSurface = EGL_NO_SURFACE;
#endif
    }

    ~RendererImplementation()
    {
#if defined(PLATFORM_ANDROID)
        mDisplay = EGL_NO_DISPLAY;
        mContext = EGL_NO_CONTEXT;
        mSurface = EGL_NO_SURFACE;
#endif
    }

    void Initialise()
    {
#if defined(PLATFORM_PC)

#elif defined(PLATFORM_ANDROID)

		const EGLint attribs[] =
		{
				EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
				EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
				EGL_BLUE_SIZE, 8,
				EGL_GREEN_SIZE, 8,
				EGL_RED_SIZE, 8,
				EGL_NONE
		};

		EGLint      format;
		EGLint      numConfigs;
		EGLConfig   config;
        EGLint      majorVersion;
        EGLint      minorVersion;

        // Get a display
		mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mDisplay ==  EGL_NO_DISPLAY)
        {
            prTrace("EGL: No display\n");
            return;
        }

        // Initialise
		eglInitialize(mDisplay, &majorVersion, &minorVersion);
        prTrace("EGL VER %i, %i\n", majorVersion, minorVersion);


		/* Here, the application chooses the configuration it desires. In this
		 * sample, we have a very simplified selection process, where we pick
		 * the first EGLConfig that matches our criteria */
		eglChooseConfig(mDisplay, attribs, &config, 1, &numConfigs);

		/* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
		 * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
		 * As soon as we picked a EGLConfig, we can safely reconfigure the
		 * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
//		eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format);

//		ANativeWindow_setBuffersGeometry(m_pState->window, 0, 0, format);

		//mSurface = eglCreateWindowSurface(mDisplay, config, window, NULL);

	/*	EGLint contextAttribs[] =
		{
				EGL_CONTEXT_CLIENT_VERSION, 2,
				EGL_NONE
		};

		mContext = eglCreateContext(mDisplay, config, NULL, contextAttribs);//*/

		//eglMakeCurrent(mDisplay, mSurface, m_surface, m_context);

		//eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &m_width);
		//eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &m_height);//*/
#endif
    }

    // Show the display
    void Swap()
    {
    }

#if defined(PLATFORM_ANDROID)
	EGLDisplay		mDisplay;
	EGLContext		mContext;
	EGLSurface		mSurface;
    //ANativeWindow   mWindow;
#endif
};



/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prRenderer_GL20::prRenderer_GL20() : prRenderer ()
                                   , pImpl      (new RendererImplementation())
                                   , imp        (*pImpl)
{
    PRASSERT(pImpl);
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prRenderer_GL20::~prRenderer_GL20()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Inits the renderer with basic settings.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::Init()
{
    imp.Initialise();
}


/// ---------------------------------------------------------------------------
/// Destroys the renderer.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::Destroy()
{
    // For OpenGL we do nothing.
}


/// ---------------------------------------------------------------------------
/// Begins the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::Begin()
{
}


/// ---------------------------------------------------------------------------
/// Ends the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::End()
{
    // For OpenGL we do nothing.
}


/// ---------------------------------------------------------------------------
/// Shows the previously rendered image.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::Present()
{
#if defined(PLATFORM_ANDROID)
#endif
}


/// ---------------------------------------------------------------------------
/// Set orthographic view in preparation for 2D rendering.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::SetOrthographicView()
{
}


/// ---------------------------------------------------------------------------
/// Restore perspective projection.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::RestorePerspectiveView()
{
}


/// ---------------------------------------------------------------------------
/// Draws a single point.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawPoint(f32 x, f32 y)
{
}


/// ---------------------------------------------------------------------------
/// Draws a line
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawLine(f32 x1, f32 y1, f32 x2, f32 y2)
{
}


/// ---------------------------------------------------------------------------
/// Draws a line 3D.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawLine(prVector3 &from, prVector3 &to)
{
}


/// ---------------------------------------------------------------------------
/// Draws a hollow rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
}


/// ---------------------------------------------------------------------------
/// Draws a filled rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
}


/// ---------------------------------------------------------------------------
/// Draws a hollow circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawCircle(f32 x, f32 y, f32 radius)
{
}


/// ---------------------------------------------------------------------------
/// Draws a filled circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawFilledCircle(f32 x, f32 y, f32 radius)
{
}


/// ---------------------------------------------------------------------------
/// Draws a hollow polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawPolygon(prVertex2D *vertices, s32 count)
{
}


/// ---------------------------------------------------------------------------
/// Draws a filled polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawFilledPolygon(prVertex2D *vertices, s32 count)
{
}


/// ---------------------------------------------------------------------------
/// Sets the draw colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::SetColour(const prColour &colour)
{
}


/// ---------------------------------------------------------------------------
/// Sets the clear colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::SetClearColour(const prColour &colour)
{
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawQuad()
{
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawQuad(float u0, float v0, float u1, float v1)
{
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::DrawQuad(float u0, float v0, float u1, float v1, prColour c)
{
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c)
{
}


/// ---------------------------------------------------------------------------
/// Enables/disables textures.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::TexturesEnabled(bool state)
{
}


/// ---------------------------------------------------------------------------
/// Enables/disables blending.
/// ---------------------------------------------------------------------------
void prRenderer_GL20::BlendEnabled(bool state)
{
}
