/**
 * prRenderer_GL3.cpp
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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
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
#include "prRenderer_GL3.h"
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



#include "prShader.h"


// Splash image
#if defined(PLATFORM_PC)
#include "prSplash.h"
#endif


using namespace Proteus::Math;
//using namespace Proteus::Core;


////static
//float points[] = {
//   0.0f,  0.5f,  0.0f,
//   0.5f, -0.5f,  0.0f,
//  -0.5f, -0.5f,  0.0f
//};

//static 
//static GLuint vao = 0;

//static float verticestemp[] = {
//    // positions          // colors           // texture coords
//     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
//};


unsigned int texture;
unsigned int VBO, VAO, EBO;


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

        ERR_CHECK(glEnable(GL_DEPTH_TEST));                            // Enables depth testing
        //ERR_CHECK();
        ERR_CHECK(glDepthFunc(GL_LESS));// GL_LEQUAL);                             // The type of depth test to do
        //ERR_CHECK();

        prOpenGLInit();

        //EmbeddedShadersInit();
    }

    // Show the display
    void Swap()
    {
    }
};



/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prRenderer_GL3::prRenderer_GL3() : prRenderer()
, pImpl(new RendererImplementation())
, imp(*pImpl)
{
    PRASSERT(pImpl);
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prRenderer_GL3::~prRenderer_GL3()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Inits the renderer with basic settings.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::Init()
{
    imp.Initialise();
    //PRLOGD("prRenderer_GL3::Init(MAX)\n");

    /*float points[] = {
       0.0f,  0.5f,  0.0f,
       0.5f, -0.5f,  0.0f,
      -0.5f, -0.5f,  0.0f
    };*/

/*    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    //	GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexArray(vao);//*/
    //PRLOGD("VAO: %i\n", vao);
    // ------------------------------------------------------------------

}


/// ---------------------------------------------------------------------------
/// Destroys the renderer.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::Destroy()
{
    // ?
}


/// ---------------------------------------------------------------------------
/// Begins the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::Begin()
{
    ERR_CHECK(glClearColor(0.2f, 0.2f, 0.5f, 0.0f));
    //ERR_CHECK();

    ERR_CHECK(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
    //ERR_CHECK();

/*    EmbeddedShadersUsePrg();
    ERR_CHECK();

    //PRLOGD("prRenderer_GL3::Begin()");
    glBindVertexArray(vao);
    ERR_CHECK();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    ERR_CHECK();//*/
}


/// ---------------------------------------------------------------------------
/// Ends the image rendering cycle.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::End()
{
    // For OpenGL we do nothing.
}


/// ---------------------------------------------------------------------------
/// Shows the previously rendered image.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::Present()
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
void prRenderer_GL3::SetOrthographicView()
{
}


/// ---------------------------------------------------------------------------
/// Restore perspective projection.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::RestorePerspectiveView()
{
}


/// ---------------------------------------------------------------------------
/// Draws a single point.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawPoint(f32 x, f32 y)
{
    PRUNUSED(x);
    PRUNUSED(y);
}


/// ---------------------------------------------------------------------------
/// Draws a line
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawLine(f32 x1, f32 y1, f32 x2, f32 y2)
{
    PRUNUSED(x1);
    PRUNUSED(y1);
    PRUNUSED(x2);
    PRUNUSED(y2);
}


/// ---------------------------------------------------------------------------
/// Draws a line 3D.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawLine(Proteus::Math::prVector3& from, Proteus::Math::prVector3& to)
{
    PRUNUSED(from);
    PRUNUSED(to);
}


/// ---------------------------------------------------------------------------
/// Draws a line 3D.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawLine(const Proteus::Math::prVector3& from, const Proteus::Math::prVector3& to)
{
    PRUNUSED(from);
    PRUNUSED(to);
}


/// ---------------------------------------------------------------------------
/// Draws a hollow rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
    PRUNUSED(x1);
    PRUNUSED(y1);
    PRUNUSED(x2);
    PRUNUSED(y2);
}


/// ---------------------------------------------------------------------------
/// Draws a filled rectangle.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawFilledRect(f32 x1, f32 y1, f32 x2, f32 y2)
{
    PRUNUSED(x1);
    PRUNUSED(y1);
    PRUNUSED(x2);
    PRUNUSED(y2);
}


/// ---------------------------------------------------------------------------
/// Draws a hollow circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawCircle(f32 x, f32 y, f32 radius)
{
    PRUNUSED(x);
    PRUNUSED(y);
    PRUNUSED(radius);
}


/// ---------------------------------------------------------------------------
/// Draws a filled circle.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawFilledCircle(f32 x, f32 y, f32 radius)
{
    PRUNUSED(x);
    PRUNUSED(y);
    PRUNUSED(radius);
}


/// ---------------------------------------------------------------------------
/// Draws a hollow polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawPolygon(prVertex2D* vertices, s32 count)
{
    PRUNUSED(vertices);
    PRUNUSED(count);
}


/// ---------------------------------------------------------------------------
/// Draws a filled polygon.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawFilledPolygon(prVertex2D* vertices, s32 count)
{
    PRUNUSED(vertices);
    PRUNUSED(count);
}


/// ---------------------------------------------------------------------------
/// Sets the draw colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::SetColour(const prColour& colour)
{
    PRUNUSED(colour);
}


/// ---------------------------------------------------------------------------
/// Sets the clear colour.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::SetClearColour(const prColour& colour)
{
    PRUNUSED(colour);
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawQuad()
{
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawQuad(float u0, float v0, float u1, float v1)
{
    PRUNUSED(u0);
    PRUNUSED(v0);
    PRUNUSED(u1);
    PRUNUSED(v1);
}


/// ---------------------------------------------------------------------------
/// Draws a textured quad.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawQuad(float u0, float v0, float u1, float v1, prColour c)
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
void prRenderer_GL3::BatchDrawQuad(float u0, float v0, float u1, float v1, prColour c)
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
void prRenderer_GL3::TexturesEnabled(bool state)
{
    PRUNUSED(state);
}


/// ---------------------------------------------------------------------------
/// Enables/disables blending.
/// ---------------------------------------------------------------------------
void prRenderer_GL3::BlendEnabled(bool state)
{
    PRUNUSED(state);
}


/// ---------------------------------------------------------------------------
/// Draws a positioning grid
/// ---------------------------------------------------------------------------
void prRenderer_GL3::DrawGrid(s32 size)
{
}


void /*prRenderer_GL3::*/TestCode(const u8 *data)
{
    /*float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };//*/
    // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
/*    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);//*/
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create a texture 
    // -------------------------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width = 512, height = 512;// , nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    //unsigned char* data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        //prTrace(prLogLevel::LogDebug, "Set texture 2\n");
    }
    //else
    //{
    ////    std::cout << "Failed to load texture" << std::endl;
    //}
    /*stbi_image_free(data);//*/
}


void /*prRenderer_GL3::*/DrawTestCode(prShader& ourShader)
{
/*    static float time = 0.0f;
    
    time += 0.016f;

    // bind Texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // render container
    ourShader.Use();
    
    // create transformations
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, (float)time, glm::vec3(0.0f, 0.0f, 1.0f));

    unsigned int transformLoc = glGetUniformLocation(ourShader.GetID(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//*/
}
