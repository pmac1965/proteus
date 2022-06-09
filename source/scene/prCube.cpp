/**
 * prCube.cpp
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


#include "prCube.h"
#include "../display/prOglUtils.h"
#include "../display/prOglConfig.h"
#include "../core/prMacros.h"
#include "../debug/prDebug.h"


#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>
  //#include <OpenGLES/ES1/glu.h>

#else
  #error No platform defined.

#endif


using namespace Proteus::Core;


namespace
{
    // A cube constructed by triangles as its designed for OpenGLES too
    f32 vertices[] = {
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f,      // Top
         0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f,  0.5f, 
         0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,      // Bottom
         0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,      // Left
         0.5f, -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f,     
        -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f,      // Right
        -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,      // Front
         0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f,      // Back
         0.5f, -0.5f,  0.5f,  0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, 
    };

    //f32 colours[] =
    //{
    //    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
    //    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f,
    //    0.9f, 0.9f, 0.9f,    0.9f, 0.9f, 0.9f,    0.9f, 0.9f, 0.9f,
    //    0.9f, 0.9f, 0.9f,    0.9f, 0.9f, 0.9f,    0.9f, 0.9f, 0.9f,
    //    0.8f, 0.8f, 0.8f,    0.8f, 0.8f, 0.8f,    0.8f, 0.8f, 0.8f,
    //    0.8f, 0.8f, 0.8f,    0.8f, 0.8f, 0.8f,    0.8f, 0.8f, 0.8f,
    //    0.7f, 0.7f, 0.7f,    0.7f, 0.7f, 0.7f,    0.7f, 0.7f, 0.7f,
    //    0.7f, 0.7f, 0.7f,    0.7f, 0.7f, 0.7f,    0.7f, 0.7f, 0.7f,
    //    0.6f, 0.6f, 0.6f,    0.6f, 0.6f, 0.6f,    0.6f, 0.6f, 0.6f,
    //    0.6f, 0.6f, 0.6f,    0.6f, 0.6f, 0.6f,    0.6f, 0.6f, 0.6f,
    //    0.5f, 0.5f, 0.5f,    0.5f, 0.5f, 0.5f,    0.5f, 0.5f, 0.5f,
    //    0.5f, 0.5f, 0.5f,    0.5f, 0.5f, 0.5f,    0.5f, 0.5f, 0.5f,
    //};
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prCube::prCube() //: prSceneObject("Cube")
{
}


/// ---------------------------------------------------------------------------
/// Draws this primitive
/// ---------------------------------------------------------------------------
void prCube::Draw()
{
// Base PC version?
#if defined(PROGL_VERSION_11)
    ERR_CHECK(glPushMatrix());
    //ERR_CHECK();

    // Translate
//    glTranslatef(position.x, position.y, position.z);
    //ERR_CHECK();

    // Roate
//    glRotatef(rotation.x, 1, 0, 0);
//    glRotatef(rotation.y, 0, 1, 0);
//    glRotatef(rotation.z, 0, 0, 1);

    // Scale
//    glScalef(scale.x, scale.y, scale.z);
    //ERR_CHECK();
#endif

        // Base class draw
//        prSceneObject::Draw();

        // We're using colour for the cube as its the most used primitive
//        glEnableClientState(GL_COLOR_ARRAY);
//        ERR_CHECK();

        // Set the colour
//        glColor4f(colour.red, colour.green, colour.blue, colour.alpha);
//        ERR_CHECK();

        ERR_CHECK(glVertexPointer(3, GL_FLOAT, 0, vertices));
        //ERR_CHECK();

//        glColorPointer(3, GL_FLOAT, 0,  colours);
//        ERR_CHECK();

        // Use alpha to get transparent primitives
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
        // Draw
        ERR_CHECK(glDrawArrays(GL_TRIANGLES, 0, PRARRAY_SIZE(vertices) / 3));
        //ERR_CHECK();

//            glDisableClientState(GL_COLOR_ARRAY);
//            ERR_CHECK();

            // Draw wireframe outline?
            glEnable(GL_LINE_SMOOTH);

            // Turn on offset mode
            glPolygonOffset(-1, -1);
    TODO("Sort this");
#ifndef PLATFORM_IOS
            glEnable(GL_POLYGON_OFFSET_LINE);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glLineWidth(1.5f);
            glColor3f(0.5f, 0.5f, 1.0f);
            glDrawArrays(GL_TRIANGLES, 0, PRARRAY_SIZE(vertices) / 3);
            glLineWidth(1.0f);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            glDisable(GL_POLYGON_OFFSET_LINE);
#endif
            glDisable(GL_LINE_SMOOTH);//*/

        glDisable(GL_BLEND);

#if defined(PROGL_VERSION_11)
    ERR_CHECK(glPopMatrix());
    //ERR_CHECK();
#endif
}
