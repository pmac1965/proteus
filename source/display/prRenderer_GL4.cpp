/**
 * prRenderer_GL4.cpp
 *
 *  Copyright 2019 Paul Michael McNab
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


#include <glew.h>
#include "../prConfig.h"


#if defined(PLATFORM_PC)
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
  //#include "../core/prWindow_PC.h"

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>

#else
  #error No platform defined.

#endif


#include <math.h>
#include "prRenderer_GL4.h"
#include "prColour.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prMacros.h"
//#include "../debug/prDebug.h"
//#include "../math/prMathsUtil.h"
#include "../math/prVector3.h"
//#include "../core/prWindow.h"
#include "../core/prVertex.h"
//#include "../core/prCore.h"
//#include "../core/prRegistry.h"
//#include "../core/prStringUtil.h"
//#include "../core/prResourceManager.h"
#include "../display/prOglUtils.h"
//#include "../display/prTexture.h"
#include "prShadersEmbedded.h"
#include "../core/prATB.h"


// Splash image
#if defined(PLATFORM_PC)
#include "prSplash.h"
#endif


using namespace Proteus::Math;
using namespace Proteus::Core;


//static
float points[] = {
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f
};

//static 
GLuint vao = 0;


/// ---------------------------------------------------------------------------
/// Using a pimpl interface to minimize external includes in the header files,
/// which can be a right pain in the neck
/// ---------------------------------------------------------------------------
class RendererImplementation
{
public:

    RendererImplementation()
    {
    }

    ~RendererImplementation()
    {
    }

    void Initialise()
    {
		// Show extensions.
		#if defined(OPENGL_SHOW_EXTENSIONS)
		prOpenGLShowExtensions();
		#endif

		glEnable(GL_DEPTH_TEST);                            // Enables depth testing
		ERR_CHECK();
		glDepthFunc(GL_LESS);// GL_LEQUAL);                             // The type of depth test to do
		ERR_CHECK();

		prOpenGLInit();

		EmbeddedShadersInit();
    }

    // Show the display
    void Swap()
    {
    }
};



/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prRenderer_GL4::prRenderer_GL4() : prRenderer ()
                                 , pImpl      (new RendererImplementation())
                                 , imp        (*pImpl)
{
    PRASSERT(pImpl);
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prRenderer_GL4::~prRenderer_GL4()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Inits the renderer with basic settings.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::Init()
{
    imp.Initialise();
	PRLOGD("prRenderer_GL4::Init(MAX)\n");

	/*float points[] = {
	   0.0f,  0.5f,  0.0f,
	   0.5f, -0.5f,  0.0f,
	  -0.5f, -0.5f,  0.0f
	};*/

	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

//	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(vao);
	PRLOGD("VAO: %i\n", vao);
}


/// ---------------------------------------------------------------------------
/// Destroys the renderer.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::Destroy()
{
    // ?
}


/// ---------------------------------------------------------------------------
/// Begins the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::Begin()
{
	glClearColor(0.2f, 0.2f, 0.5f, 0.0f);
	ERR_CHECK();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	ERR_CHECK();

	EmbeddedShadersUsePrg();
	ERR_CHECK();

	//PRLOGD("prRenderer_GL4::Begin()");
	glBindVertexArray(vao);
	ERR_CHECK();

	glDrawArrays(GL_TRIANGLES, 0, 3);
	ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Ends the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::End()
{
    // For OpenGL we do nothing.
}


/// ---------------------------------------------------------------------------
/// Shows the previously rendered image.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::Present()
{
	PRASSERT(m_pWindow);
	//if (m_pWindow)
	{
		// Draw watermark
//#if defined(PLATFORM_PC) && defined(PROTEUS_ALLOW_WATERMARK) && !defined(PROTEUS_TOOL)
//		prDrawWaterMark(m_pWatermark);
//#endif

		// Draw tweak bars
#if defined(PROTEUS_USE_ANT_TWEAK_BAR) && defined(PLATFORM_PC)
		//prATBDraw();
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
void prRenderer_GL4::SetOrthographicView()
{
}


/// ---------------------------------------------------------------------------
/// Restore perspective projection.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::RestorePerspectiveView()
{
}


/// ---------------------------------------------------------------------------
/// Draws a single point.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawPoint(f32 x, f32 y)
{
	PRUNUSED(x);
	PRUNUSED(y);
}


/// ---------------------------------------------------------------------------
/// Draws a line
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawLine(f32 x1, f32 y1, f32 x2, f32 y2)
{
    PRUNUSED(x1);
    PRUNUSED(y1);
    PRUNUSED(x2);
    PRUNUSED(y2);
}


/// ---------------------------------------------------------------------------
/// Draws a line 3D.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawLine(Proteus::Math::prVector3 &from, Proteus::Math::prVector3 &to)
{
    PRUNUSED(from);
    PRUNUSED(to);
}


/// ---------------------------------------------------------------------------
/// Draws a line 3D.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawLine(const Proteus::Math::prVector3 &from, const Proteus::Math::prVector3 &to)
{
    PRUNUSED(from);
    PRUNUSED(to);
}


/// ---------------------------------------------------------------------------
/// Draws a hollow rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
    PRUNUSED(x1);
    PRUNUSED(y1);
    PRUNUSED(x2);
    PRUNUSED(y2);
}


/// ---------------------------------------------------------------------------
/// Draws a filled rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
    PRUNUSED(x1);
    PRUNUSED(y1);
    PRUNUSED(x2);
    PRUNUSED(y2);
}


/// ---------------------------------------------------------------------------
/// Draws a hollow circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawCircle(f32 x, f32 y, f32 radius)
{
    PRUNUSED(x);
    PRUNUSED(y);
    PRUNUSED(radius);
}


/// ---------------------------------------------------------------------------
/// Draws a filled circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawFilledCircle(f32 x, f32 y, f32 radius)
{
    PRUNUSED(x);
    PRUNUSED(y);
    PRUNUSED(radius);
}


/// ---------------------------------------------------------------------------
/// Draws a hollow polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawPolygon(prVertex2D *vertices, s32 count)
{
    PRUNUSED(vertices);
    PRUNUSED(count);
}


/// ---------------------------------------------------------------------------
/// Draws a filled polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawFilledPolygon(prVertex2D *vertices, s32 count)
{
    PRUNUSED(vertices);
    PRUNUSED(count);
}


/// ---------------------------------------------------------------------------
/// Sets the draw colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::SetColour(const prColour &colour)
{
    PRUNUSED(colour);
}


/// ---------------------------------------------------------------------------
/// Sets the clear colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::SetClearColour(const prColour &colour)
{
    PRUNUSED(colour);
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawQuad()
{
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawQuad(float u0, float v0, float u1, float v1)
{
    PRUNUSED(u0);
    PRUNUSED(v0);
    PRUNUSED(u1);
    PRUNUSED(v1);
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawQuad(float u0, float v0, float u1, float v1, prColour c)
{
    PRUNUSED(u0);
    PRUNUSED(v0);
    PRUNUSED(u1);
    PRUNUSED(v1);
    PRUNUSED(c);
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c)
{
    PRUNUSED(u0);
    PRUNUSED(v0);
    PRUNUSED(u1);
    PRUNUSED(v1);
    PRUNUSED(c);
}


/// ---------------------------------------------------------------------------
/// Enables/disables textures.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::TexturesEnabled(bool state)
{
    PRUNUSED(state);
}


/// ---------------------------------------------------------------------------
/// Enables/disables blending.
/// ---------------------------------------------------------------------------
void prRenderer_GL4::BlendEnabled(bool state)
{
    PRUNUSED(state);
}


/// ---------------------------------------------------------------------------
/// Draws a positioning grid
/// ---------------------------------------------------------------------------
void prRenderer_GL4::DrawGrid(s32 size)
{
}
