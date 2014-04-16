/**
 * prPerspective.cpp
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
/*
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 */


#include "../prConfig.h"
#include "prPerspective.h"


#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <math.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>
  #include <OpenGLES/ES1/glext.h>
  #include <math.h>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <math.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include <math.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <math.h>

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  #include <math.h>
  using namespace Osp::Graphics::Opengl;

#else
  #error Platform not specified

#endif


// Some platforms can't find this define. Even though its in math.h
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif


/// ---------------------------------------------------------------------------
/// Perspective function.
/// ---------------------------------------------------------------------------
void prPerspective(double fovy, double aspect, double zNear, double zFar)
{
    // Start in projection mode.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double xmin, xmax, ymin, ymax;
    ymax = zNear * tan(fovy * M_PI / 360.0);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)
    glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);

#elif defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID)
    glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);

#else
    #error Unsupported platform

#endif
}
