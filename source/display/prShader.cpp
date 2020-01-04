/**
 * prShader.cpp
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


//#define TEMP_DEFINE	1


#if defined(PLATFORM_PC)
  #include <Windows.h>
#if defined(ALLOW_GLEW)
  #include <glew.h>
  #include <wglew.h>
#endif
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include "../core/prWindow_PC.h"
  #include "../linux/prLinux.h"

  #ifdef TEMP_DEFINE
  #undef TEMP_DEFINE
  #endif

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES2/gl2.h>
  #include <GLES2/gl2ext.h>
  #include <EGL/egl.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES2/gl.h>
  #include <cstdlib>

#else
  #error No platform defined.

#endif


#include "prShader.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../display/prOglUtils.h"
#include <glm/glm.hpp>


namespace Proteus { 
namespace Display {

    // ---------------------------------------------------------------------------
    // Ctor
    // ---------------------------------------------------------------------------
    prShader::prShader()
        : id(0)
    {
    }


    ///// ---------------------------------------------------------------------------
    ///// Dtor
    ///// ---------------------------------------------------------------------------
    //prShader::~prShader()
    //{
    //}


    // ---------------------------------------------------------------------------
    // Load an embedded shader.
    // ---------------------------------------------------------------------------
    bool prShader::LoadFromMemory(const char* vertexSrc, const char* fragmentSrc)
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
            prTrace(prLogLevel::LogError, "Failed to create shaders\n");
            glDeleteShader(vertexShader);
            ERR_CHECK();
            glDeleteShader(fragmentShader);
            ERR_CHECK();
            return false;
        }


        // Set the shaders source
        SetSource(vertexShader, vertexSrc);
        SetSource(fragmentShader, fragmentSrc);


        // Compile the shaders
        glCompileShader(vertexShader);
        ERR_CHECK();
        CheckCompileErrors(vertexShader, "VERTEX");

        glCompileShader(fragmentShader);
        ERR_CHECK();
        CheckCompileErrors(fragmentShader, "FRAGMENT");


        // Test for errors
        GLint val;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &val);
        ERR_CHECK();
        if (val == GL_FALSE)
        {
            prTrace(prLogLevel::LogError, "Error found in vertex shader\n");
            //ShaderInfoLog(vertexShader);
            glDeleteShader(vertexShader);
            ERR_CHECK();
            glDeleteShader(fragmentShader);
            ERR_CHECK();
            return false;
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &val);
        ERR_CHECK();
        if (val == GL_FALSE)
        {
            prTrace(prLogLevel::LogError, "Error found in fragment shader\n");
            //ShaderInfoLog(fragmentShader);
            glDeleteShader(vertexShader);
            ERR_CHECK();
            glDeleteShader(fragmentShader);
            ERR_CHECK();
            return false;
        }


        // Create, attach and link program. This assumes it all worked!
        id = glCreateProgram();
        ERR_CHECK();
        glAttachShader(id, vertexShader);
        ERR_CHECK();
        glAttachShader(id, fragmentShader);
        ERR_CHECK();
        glLinkProgram(id);
        ERR_CHECK();
        CheckCompileErrors(id, "PROGRAM");


        // These are no longer needed
        glDeleteShader(vertexShader);
        ERR_CHECK();
        glDeleteShader(fragmentShader);
        ERR_CHECK();


        // Check program linked
        glGetProgramiv(id, GL_LINK_STATUS, &val);
        ERR_CHECK();
        if (val == GL_FALSE)
        {
            //ProgramInfoLog(id);
            glDeleteProgram(id);
            ERR_CHECK();
            return false;
        }
        //ProgramInfoLog(id);

    //#endif

        return true;
    }


    /// ---------------------------------------------------------------------------
    /// // Sets the source for a memory based shader
    /// ---------------------------------------------------------------------------
    void prShader::SetSource(u32 shader, const char* src)
    {
        PRASSERT(src && *src);
        //#ifdef TEMP_DEFINE

        GLchar* stringPtr[] = { (GLchar*)src };

        glShaderSource(shader, 1, (const GLchar * *)stringPtr, NULL);
        ERR_CHECK();
        //#endif
    }


    /// ---------------------------------------------------------------------------
    /// Shows the shader info log
    /// ---------------------------------------------------------------------------
    /*void prShader::ShaderInfoLog(u32 shader)
    {
    #ifdef TEMP_DEFINE

        GLint logLength;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        ERR_CHECK();

        if (logLength > 0)
        {
            char *log = (char *)malloc(logLength);
            int written = 0;

            glGetShaderInfoLog(shader, logLength, &written, log);
            ERR_CHECK();

            prTrace(prLogLevel::LogError, "%s\n", log);
            free(log);
        }

    #endif
    }//*/


    void prShader::CheckCompileErrors(u32 shader, std::string type)
    {
        GLint success;
        GLchar infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                //std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                prTrace(prLogLevel::LogDebug, "ERROR::SHADER_COMPILATION_ERROR of type: %s\n %s\n", type.c_str(), infoLog);
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                //
                prTrace(prLogLevel::LogDebug, "ERROR::PROGRAM_LINKING_ERROR of type: %s\n %s\n", type.c_str(), infoLog);
            }
        }
    }


    /// ---------------------------------------------------------------------------
    /// Shows the program info log
    /// ---------------------------------------------------------------------------
    /*void prShader::ProgramInfoLog(u32 shader)
    {
    #ifdef TEMP_DEFINE

        GLint logLength;

        glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        ERR_CHECK();

        if (logLength > 0)
        {
            char *log = (char *)malloc(logLength);
            int written = 0;

            glGetProgramInfoLog(shader, logLength, &written, log);
            ERR_CHECK();

            prTrace(prLogLevel::LogError, "Error: %s\n", log);
            free(log);
        }
    #endif
    }//*/


    void prShader::Use()
    {
        glUseProgram(id);
    }


    void prShader::SetBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }


    void prShader::SetInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }


    void prShader::SetFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }


    // ------------------------------------------------------------------------
    void prShader::SetVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }

    void prShader::SetVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(id, name.c_str()), x, y);
    }


    // ------------------------------------------------------------------------
    void prShader::SetVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }

    void prShader::SetVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
    }


    // ------------------------------------------------------------------------
    void prShader::SetVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
    }

    void prShader::SetVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(id, name.c_str()), x, y, z, w);
    }


    // ------------------------------------------------------------------------
    void prShader::SetMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


    // ------------------------------------------------------------------------
    void prShader::SetMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }


    // ------------------------------------------------------------------------
    void prShader::SetMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
}}