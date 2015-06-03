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
  #include <glew.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include "../core/prWindow_PC.h"
  #include "../linux/prLinux.h"

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <EGL/egl.h>

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>

#else
  #error No platform defined.

#endif


#include "prShader.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../display/prOglUtils.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prShader::prShader()
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prShader::~prShader()
{
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
bool prShader::LoadFromMemory(const char *vertexSrc, const char *fragmentSrc)
{
    PRASSERT(vertexSrc && *vertexSrc);
    PRASSERT(fragmentSrc && *fragmentSrc);

    
    // Create the shaders
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    ERR_CHECK();

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    ERR_CHECK();

    if (vertexShader == 0 || fragmentShader == 0)
    {
        prTrace(LogError, "Failed to create shaders\n");
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }


    // Set the shaders source
    SetSource(vertexShader, vertexSrc);
    SetSource(fragmentShader, fragmentSrc);


    // Compile the shaders
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);


    // Test for errors
    GLint val;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &val);
    if (val == GL_FALSE) 
    {
        prTrace(LogError, "Error found in vertex shader\n");        
        ShaderInfoLog(vertexShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &val);
    if (val == GL_FALSE) 
    {
        prTrace(LogError, "Error found in fragment shader\n");        
        ShaderInfoLog(fragmentShader);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return false;
    }

    
    // Create, attach and link program. This assumes it all worked!
    u32 id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);


    // These are no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Check program linked
    glGetProgramiv(id, GL_LINK_STATUS, &val);
    if(val == GL_FALSE)
    {
        ProgramInfoLog(id);
        glDeleteProgram(id);
        return false;
    }


    return true;
}


/// ---------------------------------------------------------------------------
/// // Sets the source for a memory based shader
/// ---------------------------------------------------------------------------
void prShader::SetSource(u32 shader, const char *src)
{
    PRASSERT(src && *src);

    GLchar *stringPtr[] = { (GLchar *)src };

    glShaderSource(shader, 1, (const GLchar **)stringPtr, NULL);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Shows the shader info log
/// ---------------------------------------------------------------------------
void prShader::ShaderInfoLog(u32 shader)
{
    GLint logLength;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    ERR_CHECK();

    if (logLength > 0)
    {
        char *log = (char *)malloc(logLength);
        int written = 0;
        
        glGetShaderInfoLog(shader, logLength, &written, log);
        ERR_CHECK();
        
        prTrace(LogError, "%s\n", log);
        free(log);
    }
}


/// ---------------------------------------------------------------------------
/// Shows the program info log
/// ---------------------------------------------------------------------------
void prShader::ProgramInfoLog(u32 shader)
{
    GLint logLength;

    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    ERR_CHECK();

    if (logLength > 0)
    {
        char *log = (char *)malloc(logLength);
        int written = 0;
        
        glGetProgramInfoLog(shader, logLength, &written, log);
        ERR_CHECK();
        
        prTrace(LogError, "%s\n", log);
        free(log);
    }
}
