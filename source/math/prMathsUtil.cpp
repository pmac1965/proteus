/**
 * prMathsUtil.cpp
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
 */


#include "../prConfig.h"
#include "prMathsUtil.h"
#include "../core/prMacros.h"


#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>

#else
  #error Platform not defined.

#endif


/// ---------------------------------------------------------------------------
/// Returns the specified percentage of a value.
/// ---------------------------------------------------------------------------
s32 prPercentageOfValue(s32 percent, s32 value)
{
    float first  = (float)(value * 100);
    float second = (float)(value * percent);
    float third  = (first - second) / 100.0f;

    return value - ((s32)third);
}


/// ---------------------------------------------------------------------------
/// Calculates 'amount's percentage in relation to 'total'. E.g. (50, 200) == 25% of total.
/// ---------------------------------------------------------------------------
s32 prPercentageOfTotal(s32 amount, s32 total)
{
    float f_amount      = (float)amount;
    float f_total       = (float)total;
    float f_percentage  = (f_amount / f_total) * 100;

    return (s32)f_percentage;
}


/// ---------------------------------------------------------------------------
/// Converts a world position to a screen position.
/// ---------------------------------------------------------------------------
void prWorldToScreen(f32 wx, f32 wy, f32 wz, f64 &ox, f64 &oy, f64 &oz)
{
    GLint viewport[4];                              // Where the viewport values will be stored.
    glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves the viewport values (x, y, width, height) 

#if defined(PLATFORM_PC)
    GLdouble modelview[16];                         // Where the 16 doubles of the modelview matrix are to be stored.
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // Retrieve the modelview matrix.

    GLdouble projection[16];                        // Where the 16 doubles of the projection matrix are to be stored.
    glGetDoublev(GL_PROJECTION_MATRIX, projection); // Retrieve the projection matrix.

    gluProject(wx, wy, wz, modelview, projection, viewport, &ox, &oy, &oz);
#else
    
    //

#endif
    
    oy = viewport[3] - oy;                          // Subtract the current mouse y coordinate from the screen height.
}


/// ---------------------------------------------------------------------------
/// Converts a screen position to a world position. 
/// From the Nehe article - http://nehe.gamedev.net/article/using_gluunproject/16013/
/// ---------------------------------------------------------------------------
void prScreenToWorld(s32 sx, s32 sy, f64 &ox, f64 &oy, f64 &oz)
{
    GLint viewport[4];                              // Where the viewport values will be stored.
    glGetIntegerv(GL_VIEWPORT, viewport);           // Retrieves the viewport values (x, y, width, height) 

#if defined(PLATFORM_PC)
    GLdouble modelview[16];                         // Where the 16 doubles of the modelview matrix are to be stored.
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);   // Retrieve the modelview matrix.

    GLdouble projection[16];                        // Where the 16 doubles of the projection matrix are to be stored.
    glGetDoublev(GL_PROJECTION_MATRIX, projection); // Retrieve the projection matrix.


    // Now Windows coordinates start with (0, 0) being at the top left whereas OpenGL coords start at the lower left.
    // To convert to opengl coordinates we do the following.
    sy = viewport[3] - sy;                          // Subtract the current mouse y coordinate from the screen height.

    f32 sz = 0;

    glReadPixels(sx, sy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &sz); 
 
    gluUnProject(sx, sy, sz, modelview, projection, viewport, &ox, &oy, &oz); 
    
#else

    //

#endif
}
