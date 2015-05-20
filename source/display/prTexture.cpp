/**
 * prTexture.cpp
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


// Platform specifics
#if defined(PLATFORM_PC)
  #include <windows.h>
  #include <gl/gl.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>  
  #include <stdio.h>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <stdio.h>

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include <GLES/glext.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <string.h>

#else
  #error No platform defined.

#endif


#include "prTexture.h"
#include "prRenderer.h"
#include "prPvr.h"
#include "prOglUtils.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../file/prFile.h"
#include "../file/prFileShared.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"


// prTexture formats.
#if defined(PLATFORM_PC) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC)
enum
{
    TEX_FMT_OGL888          = 0x00000015,
    TEX_FMT_OGL888_BMP_YN   = 0x00010015,
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,
};

#elif defined(PLATFORM_IOS)
enum
{
    TEX_FMT_OGL888          = 0x00000015,
    TEX_FMT_OGL888_BMP_YN   = 0x00010015,
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,
    TEX_FMT_OGL4444_TGA_YN  = 0x00018010,
    TEX_FMT_OGL4444_BMP_YN  = 0x00010010,
    TEX_FMT_OGL565          = 0x00010013,
    TEX_FMT_OGL5551         = 0x00018011,
};

#elif defined(PLATFORM_BADA)
enum
{
    // Standard texture formats
    TEX_FMT_OGL888          = 0x00000015,
    TEX_FMT_OGL888_BMP_YN   = 0x00010015,
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,
    TEX_FMT_OGL4444_TGA_YN  = 0x00018010,
    TEX_FMT_OGL4444_BMP_YN  = 0x00010010,
    TEX_FMT_OGL565          = 0x00010013,
    TEX_FMT_OGL5551         = 0x00018011,
};

#elif defined(PLATFORM_ANDROID)
enum
{
    // Standard texture formats
    TEX_FMT_OGL888          = 0x00000015,
    TEX_FMT_OGL888_BMP_YN   = 0x00010015,
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,
    TEX_FMT_OGL4444_TGA_YN  = 0x00018010,
    TEX_FMT_OGL4444_BMP_YN  = 0x00010010,
    TEX_FMT_OGL565          = 0x00010013,
    TEX_FMT_OGL5551         = 0x00018011,
};

#else
    #error No platform defined.

#endif


// Defines
#define HEADER_MAGIC    0x21525650


using namespace Proteus::Core;


// Local data
namespace
{
    u32 lastTextureID = 0xFFFFFFFF;
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prTexture::prTexture(const char *filename) : prResource(filename)
{
    m_width  = 0;
    m_height = 0;
    m_texID  = 0x00FFFFFF;
    m_alpha  = false;
    m_exp0   = false;
    m_exp1   = false;
    m_exp2   = false;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prTexture::~prTexture()
{
    lastTextureID = 0xFFFFFFFF;
    Unload();
}


/// ---------------------------------------------------------------------------
/// Bind the texture.
/// ---------------------------------------------------------------------------
bool prTexture::Bind()
{
    bool bound = true;

    // Check if bound already?
    if (lastTextureID == m_texID)
    {
        return bound;
    }

#if defined(PROTEUS_OPTIMISE_REMOVE_ISTEXTURE)
    glBindTexture(GL_TEXTURE_2D, m_texID);
    lastTextureID = m_texID;
    ERR_CHECK();

#else
    // Else bind
    GLboolean result = glIsTexture(m_texID);
    ERR_CHECK();
    if (result)
    {
        glBindTexture(GL_TEXTURE_2D, m_texID);
        lastTextureID = m_texID;
        ERR_CHECK();
    }
    else
    {
        prTrace("prTexture::Bind - Failed to bind texture: %s\n", Filename());
        lastTextureID = 0xFFFFFFFF;
        bound = false;
    }

#endif

    return bound;
}


/// ---------------------------------------------------------------------------
/// Unbind the texture.
/// ---------------------------------------------------------------------------
void prTexture::Unbind()
{
}


/// ---------------------------------------------------------------------------
/// Load the texture
/// ---------------------------------------------------------------------------
void prTexture::Load(s32 extra)
{
    prFile *file = new prFile(Filename());
    if (file->Open())
    {
        // Sanity checks
        u32 size = file->Size();
        if (size == 0)
        {
            PRWARN("prTexture::Load: File is empty.");
            file->Close();
            PRSAFE_DELETE(file);
            return;
        }

        SetSize(size);

        if (size < sizeof(prPVRTextureHeader))
        {
            PRWARN("prTexture::Load: File is too small to be a texture.");
            file->Close();
            PRSAFE_DELETE(file);
            return;
        }

        // Create load buffer
        u8 *pTextureData = new u8[size];

        // Load the data
        file->Read(pTextureData, size);
        file->Close();

        // Check header
        prPVRTextureHeader *header = (prPVRTextureHeader *)pTextureData;
        if (ValidateHeader(header))
        {
            // Set details
            m_width  = header->dwWidth;
            m_height = header->dwHeight;

#if defined(_DEBUG) || defined(DEBUG)
            // Size validation.
            if (!ValidateSize(m_width))
            {
                prTrace("prTexture: %s\n", Filename());
                prTrace("Invalid texture width\n");
            }

            // Size validation.
            if (!ValidateSize(m_height))
            {
                prTrace("Texture: %s\n", Filename());
                prTrace("Invalid texture height\n");
            }
#endif

            // allocate a texture name
            glGenTextures(1, &m_texID);
            if (glGetError() != GL_NO_ERROR)
            {
                prTrace("Failed to generate texture: %s\n", Filename());
                prOpenGLErrorCheck(__FILE__, __FUNCTION__, __LINE__);
                PRSAFE_DELETE_ARRAY(pTextureData);
                PRSAFE_DELETE(file);
                m_width  = 0;
                m_height = 0;
                m_texID  = 0xFFFFFFFF;
                return;
            }            

            // select our current texture
            glBindTexture(GL_TEXTURE_2D, m_texID);
            lastTextureID = m_texID;
            ERR_CHECK();

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            ERR_CHECK();

            // Extra Settings
            if (extra == TEXTRA_ANTIALIAS)
            {
                SetAntiAliasParameters();
            }
            else
            {
                SetAliasParameters();
            }

            // Create texture
            int internalFormat, format, type;
            bool compressed;
            if (GetTextureFormat(header->dwpfFlags, internalFormat, format, type, compressed))
            {
                if (compressed)
                {
                #if defined(PLATFORM_PC)
                    PRPANIC("Compressed textures not supported by this platform");
                #else
                    glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, header->dwWidth, header->dwHeight, 0, size - sizeof(prPVRTextureHeader), ((u8*)header + sizeof(prPVRTextureHeader)));
                    ERR_CHECK();
                #endif
                }
                else
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, header->dwWidth, header->dwHeight, 0, format, type, ((u8*)header + sizeof(prPVRTextureHeader)));
                    ERR_CHECK();
                }
            }
            else
            {
                PRWARN("Invalid texture format: %s", Filename());
                Unload();
            }
        }
        else
        {
            PRWARN("Invalid texture header: %s", Filename());
        }

        PRSAFE_DELETE_ARRAY(pTextureData);
    }

    PRSAFE_DELETE(file);
}


/// ---------------------------------------------------------------------------
/// Unloads the texture.
/// ---------------------------------------------------------------------------
void prTexture::Unload()
{
    if (m_texID != 0xFFFFFFFF)
    {
        GLboolean result = glIsTexture(m_texID);
        ERR_CHECK();
        if (result)
        {
            glDeleteTextures(1, &m_texID);
            ERR_CHECK();
        }
    }

    m_width       = 0;
    m_height      = 0;
    m_texID       = 0xFFFFFFFF;
    lastTextureID = 0xFFFFFFFF;
}


/// ---------------------------------------------------------------------------
/// Allows embedded data to be used.
/// ---------------------------------------------------------------------------
void prTexture::LoadFromMemory(void *pData, u32 size)
{
    PRASSERT(pData);
    if (pData)
    {
        // Sanity checks
        if (size == 0)
        {
            PRWARN("prTexture::LoadFromMemory: Size is zero.");
            return;
        }

        if (size < sizeof(prPVRTextureHeader))
        {
            PRWARN("prTexture::LoadFromMemory: File is too small to be a texture.");
            return;
        }

        // Check header
        prPVRTextureHeader *header = (prPVRTextureHeader *)pData;
        if (ValidateHeader(header))
        {
            // Set details
            m_width  = header->dwWidth;
            m_height = header->dwHeight;

#if defined(_DEBUG) || defined(DEBUG)
            // Size validation.
            if (!ValidateSize(m_width))
            {
                prTrace("prTexture: %s\n", Filename());
                prTrace("Invalid texture width\n");
            }

            // Size validation.
            if (!ValidateSize(m_height))
            {
                prTrace("prTexture: %s\n", Filename());
                prTrace("Invalid texture height\n");
            }
#endif

            // allocate a texture name
            glGenTextures(1, &m_texID);
            if (glGetError() != GL_NO_ERROR)
            {
                prTrace("Failed to generate texture: %s\n", Filename());
                m_width  = 0;
                m_height = 0;
                m_texID  = 0xFFFFFFFF;
                return;
            }            

            // select our current texture
            glBindTexture(GL_TEXTURE_2D, m_texID);
            lastTextureID = m_texID;
            ERR_CHECK();

            // Settings
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            ERR_CHECK();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            ERR_CHECK();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            ERR_CHECK();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            ERR_CHECK();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            ERR_CHECK();
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            ERR_CHECK();

            // Create texture
            int internalFormat, format, type;
            bool compressed;
            if (GetTextureFormat(header->dwpfFlags, internalFormat, format, type, compressed))
            {
                if (compressed)
                {
                #if defined(PLATFORM_PC)
                    PRPANIC("Compressed textures not supported by this platform");
                #else
                    glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, header->dwWidth, header->dwHeight, 0, size - sizeof(prPVRTextureHeader), ((u8*)header + sizeof(prPVRTextureHeader)));
                    ERR_CHECK();
                #endif
                }
                else
                {
                    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, header->dwWidth, header->dwHeight, 0, format, type, ((u8*)header + sizeof(prPVRTextureHeader)));
                    ERR_CHECK();
                }
            }
            else
            {
                PRWARN("Invalid texture format: %s", Filename());
                Unload();
            }
        }
        else
        {
            PRWARN("Invalid texture header: %s", Filename());
        }
    }
}


/// ---------------------------------------------------------------------------
/// Allows raw data to be used
/// ---------------------------------------------------------------------------
void prTexture::LoadFromRaw(void *pData, u32 size, u32 width, u32 height)
{
    PRASSERT(pData);
    if (pData)
    {
        // Sanity checks
        if (size == 0)
        {
            PRWARN("prTexture::LoadFromMemory: Size is zero.");
            return;
        }

        // Set details
        m_width  = width;
        m_height = height;

#if defined(_DEBUG) || defined(DEBUG)
        // Size validation.
        if (!ValidateSize(width))
        {
            prTrace("prTexture: %s\n", Filename());
            prTrace("Invalid texture width\n");
        }

        // Size validation.
        if (!ValidateSize(height))
        {
            prTrace("prTexture: %s\n", Filename());
            prTrace("Invalid texture height\n");
        }
#endif

        // allocate a texture name
        glGenTextures(1, &m_texID);
        if (glGetError() != GL_NO_ERROR)
        {
            prTrace("Failed to generate texture: %s\n", Filename());
            m_width  = 0;
            m_height = 0;
            m_texID  = 0xFFFFFFFF;
            return;
        }            

        // select our current texture
        glBindTexture(GL_TEXTURE_2D, m_texID);
        lastTextureID = m_texID;
        ERR_CHECK();

        // Settings
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        ERR_CHECK();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        ERR_CHECK();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        ERR_CHECK();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ERR_CHECK();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        ERR_CHECK();
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        ERR_CHECK();

        // Create texture
        int internalFormat, format, type;
        bool compressed;
        if (GetTextureFormat(TEX_FMT_OGL8888_BMP_YN, internalFormat, format, type, compressed))
        {
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pData);
            ERR_CHECK();
        }
        else
        {
            PRWARN("Invalid texture format: %s", Filename());
            Unload();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Checks for power of two compliance.
/// ---------------------------------------------------------------------------
bool prTexture::ValidateSize(s32 size)
{
    bool result = false;

    // Do 2 to 1024.
    for (s32 i = 1; i <= 10; i++)
    {
        if (size == (1 << i))
        {
            result = true;
            break;
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Validates that we have a PVR file.
/// ---------------------------------------------------------------------------
bool prTexture::ValidateHeader(prPVRTextureHeader *header)
{
    bool result = false;

    if (header->dwHeaderSize == sizeof(prPVRTextureHeader))
    {
        if (header->dwPVR == HEADER_MAGIC)
        {
            result = true;
        }
    }

    return result; //dwPVR
}


/// ---------------------------------------------------------------------------
/// Sets the texture to have alias (Normal setup)
/// ---------------------------------------------------------------------------
void prTexture::SetAliasParameters()
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    ERR_CHECK();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);    
    ERR_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    ERR_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Sets the texture to have anti-alias
/// ---------------------------------------------------------------------------
void prTexture::SetAntiAliasParameters()
{
    // Set clamp to edge if required!
    #if defined(PLATFORM_PC)
      #ifndef GL_CLAMP_TO_EDGE
        int GL_CLAMP_TO_EDGE = GL_REPEAT;
        if (PRGL_VERSION >= 1.2f)
        {    
            GL_CLAMP_TO_EDGE = 0x812F;
        }
      #endif
    #endif

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    ERR_CHECK();
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
    ERR_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    ERR_CHECK();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Acquires the texture format
/// ---------------------------------------------------------------------------
bool prTexture::GetTextureFormat(u32 texFormat, int &internalFormat, int &format, int &type, bool &compressed)
{
    bool result = true;

    switch(texFormat)
    {
    case TEX_FMT_OGL888_BMP_YN:
    case TEX_FMT_OGL888:
        internalFormat = GL_RGB;
        format         = GL_RGB;
        type           = GL_UNSIGNED_BYTE;
        m_alpha        = false;
        compressed     = false;
        break;

    case TEX_FMT_OGL8888_BMP_YI:
    case TEX_FMT_OGL8888_BMP_YN:
    case TEX_FMT_OGL8888_TGA_YI:
    case TEX_FMT_OGL8888_TGA_YN:
        internalFormat = GL_RGBA;
        format         = GL_RGBA;
        type           = GL_UNSIGNED_BYTE;
        m_alpha        = true;
        compressed     = false;
        break;
                
    #if defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID)
    case TEX_FMT_OGL4444_TGA_YN:
    case TEX_FMT_OGL4444_BMP_YN:
        internalFormat = GL_RGBA;
        format         = GL_RGBA;
        type           = GL_UNSIGNED_SHORT_4_4_4_4;
        m_alpha        = true;
        compressed     = false;
        break;

    case TEX_FMT_OGL5551:
        internalFormat = GL_RGBA;
        format         = GL_RGBA;
        type           = GL_UNSIGNED_SHORT_5_5_5_1;
        m_alpha        = true;
        compressed     = false;
        break;

    case TEX_FMT_OGL565:
        internalFormat = GL_RGB;
        format         = GL_RGB;
        type           = GL_UNSIGNED_SHORT_5_6_5;
        m_alpha        = false;
        compressed     = false;
        break;
    #endif
                
    default:
        prTrace("Unknown texture format: %08x\n", texFormat);
        result = false;
        break;
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// If you bind textures outside the texture class, you'll need to
/// call this or you'll get invalid textures
/// ---------------------------------------------------------------------------
void prTextureClearLastID()
{
    lastTextureID = 0xFFFFFFFF;
}
