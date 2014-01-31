/**
 * prTexture.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"


// ----------------------------------------------------------------------------
// Platform specifics
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
  #include <windows.h>
  #include <gl/gl.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>  
  #include <stdio.h>

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include <GLES/glext.h>

#else
  #error No platform defined.

#endif


#include "prTexture.h"
#include "prRenderer.h"
#include "prPvr.h"
#include "prOglUtils.h"
//#include "prTextureManager.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../file/prFile.h"
#include "../file/prFileShared.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
//#include "../debug/prOnScreenLogger.h"


//#if defined(PLATFORM_ANDROID)
////#define USE_SHARED_TEXTURES
//#endif


//#if defined(PLATFORM_BADA)
//#include "prATC.h"
//
/////* GL_AMD_compressed_ATC_texture */
////#ifndef GL_AMD_compressed_ATC_texture
////#define GL_ATC_RGB_AMD                                          0x8C92
////#define GL_ATC_RGBA_EXPLICIT_ALPHA_AMD                          0x8C93
////#define GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD                      0x87EE
////#endif
//
//#endif


// ----------------------------------------------------------------------------
// prTexture formats.
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
enum
{
    TEX_FMT_OGL888          = 0x00000015,
    TEX_FMT_OGL888_BMP_YN   = 0x00010015,
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,           // Y inverted
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,           // Y normal
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,           // Y inverted
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,           // Y normal
};

#elif defined(PLATFORM_IOS)
enum
{
    TEX_FMT_OGL888          = 0x00000015,
    TEX_FMT_OGL888_BMP_YN   = 0x00010015,
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,           // Y inverted
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,           // Y normal
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,           // Y inverted
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,           // Y normal
    TEX_FMT_OGL4444_TGA_YN  = 0x00018010,           // Y normal
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
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,           // Y inverted
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,           // Y normal
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,           // Y inverted
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,           // Y normal
    TEX_FMT_OGL4444_TGA_YN  = 0x00018010,           // Y normal
    TEX_FMT_OGL4444_BMP_YN  = 0x00010010,
    TEX_FMT_OGL565          = 0x00010013,
    TEX_FMT_OGL5551         = 0x00018011,

    //// PVR prTexture compression
    //#if defined(GL_IMG_texture_compression_pvrtc)
    //TEX_FMT_PVRTC_4BPPV1    = 0x00010219,           // No alpha
    //TEX_FMT_PVRTC_2BPPV1    = 0x00000000,           // No alpha   - TBC
    //TEX_FMT_PVRTC_4BPPV1A   = 0x00018219,           // With alpha
    //TEX_FMT_PVRTC_2BPPV1A   = 0x00000000,           // With alpha - TBC
    //#endif

    //// Ericsson texture compression
    //#if defined(GL_OES_compressed_ETC1_RGB8_texture)
    //TEX_FMT_ERIC_ETC1       = 0x00010036,           // No alpha
    //#endif

    //#if defined(GL_AMD_compressed_ATC_texture)
    //TEX_FMT_ATC_RGBA        = 0x00000016,
    //#endif
};

#elif defined(PLATFORM_ANDROID)
enum
{
    // Standard texture formats
    TEX_FMT_OGL888          = 0x00000015,
    TEX_FMT_OGL888_BMP_YN   = 0x00010015,
    TEX_FMT_OGL8888_BMP_YI  = 0x00000012,           // Y inverted
    TEX_FMT_OGL8888_BMP_YN  = 0x00010012,           // Y normal
    TEX_FMT_OGL8888_TGA_YI  = 0x00008012,           // Y inverted
    TEX_FMT_OGL8888_TGA_YN  = 0x00018012,           // Y normal
    TEX_FMT_OGL4444_TGA_YN  = 0x00018010,           // Y normal
    TEX_FMT_OGL4444_BMP_YN  = 0x00010010,
    TEX_FMT_OGL565          = 0x00010013,
    TEX_FMT_OGL5551         = 0x00018011,

    //// PVR prTexture compression
    //#if defined(GL_IMG_texture_compression_pvrtc)
    //TEX_FMT_PVRTC_4BPPV1    = 0x0000020D,           // No alpha
    //TEX_FMT_PVRTC_2BPPV1    = 0x00000000,           // No alpha   - TBC
    //TEX_FMT_PVRTC_4BPPV1A   = 0x0000820D,           // With alpha
    //TEX_FMT_PVRTC_4BPPV1A_F1= 0x0001820D,           // With alpha and flipped on the Y
    //TEX_FMT_PVRTC_4BPPV1A_F2= 0x0001020D,           // With alpha and flipped on the Y
    //TEX_FMT_PVRTC_2BPPV1A   = 0x00000000,           // With alpha - TBC
    //#endif

    //// Ericsson texture compression
    //#if defined(GL_OES_compressed_ETC1_RGB8_texture)
    //TEX_FMT_ERIC_ETC1       = 0x00010036,           // No alpha
    //#endif
};
#else
#error No platform defined.
#endif


// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define HEADER_MAGIC    0x21525650


// ----------------------------------------------------------------------------
// Static data
// ----------------------------------------------------------------------------
static u32 lastTextureID = 0xFFFFFFFF;


// ----------------------------------------------------------------------------
// Internal data.
// ----------------------------------------------------------------------------
//typedef struct TextureImplementation
//{
//    TextureImplementation()
//    {
//        width   = 0;
//        height  = 0;
//        texID   = 0xFFFFFFFF;
//        alpha   = false;
//        duplicate   = false;
//        exp1    = false;
//        exp2    = false;
//    }
//
//
//    // Validates that we have a PVR file.
//    bool ValidateHeader(prPVRTextureHeader *header)
//    {
//        bool result = false;
//
//        if (header->dwHeaderSize == sizeof(prPVRTextureHeader))
//        {
//            if (header->dwPVR == HEADER_MAGIC)
//            {
//                //prTrace("HS: %08x\n", header->dwPVR);
//                result = true;
//            }
//        }
//
//        return result; //dwPVR
//    }
//
//    // Acquires the texture format
//    bool GetTextureFormat(u32 texFormat, int &internalFormat, int &format, int &type, bool &compressed)
//    {
//        bool result = true;
//
//        switch(texFormat)
//        {
//        case TEX_FMT_OGL888_BMP_YN:
//        case TEX_FMT_OGL888:
//            internalFormat = GL_RGB;
//            format         = GL_RGB;
//            type           = GL_UNSIGNED_BYTE;
//            alpha          = false;
//            compressed     = false;
//            break;
//
//        case TEX_FMT_OGL8888_BMP_YI:
//        case TEX_FMT_OGL8888_BMP_YN:
//        case TEX_FMT_OGL8888_TGA_YI:
//        case TEX_FMT_OGL8888_TGA_YN:
//            internalFormat = GL_RGBA;
//            format         = GL_RGBA;
//            type           = GL_UNSIGNED_BYTE;
//            alpha          = true;
//            compressed     = false;
//            break;
//                
//        #if defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID)
//        case TEX_FMT_OGL4444_TGA_YN:
//        case TEX_FMT_OGL4444_BMP_YN:
//            internalFormat = GL_RGBA;
//            format         = GL_RGBA;
//            type           = GL_UNSIGNED_SHORT_4_4_4_4;
//            alpha          = true;
//            compressed     = false;
//            break;
//
//        case TEX_FMT_OGL5551:
//            internalFormat = GL_RGBA;
//            format         = GL_RGBA;
//            type           = GL_UNSIGNED_SHORT_5_5_5_1;
//            alpha          = true;
//            compressed     = false;
//            break;
//
//        case TEX_FMT_OGL565:
//            internalFormat = GL_RGB;
//            format         = GL_RGB;
//            type           = GL_UNSIGNED_SHORT_5_6_5;
//            alpha          = false;
//            compressed     = false;
//            break;
//
//        // PVR texture formats
//        #if defined(GL_IMG_texture_compression_pvrtc)
//        #if defined(PLATFORM_ANDROID)
//        case TEX_FMT_PVRTC_4BPPV1A_F1:
//        case TEX_FMT_PVRTC_4BPPV1A_F2:
//        #endif
//        case TEX_FMT_PVRTC_4BPPV1A:
//            internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
//            alpha          = true;
//            compressed     = true;
//            break;
//
//        case TEX_FMT_PVRTC_4BPPV1:
//            internalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
//            alpha          = false;
//            compressed     = true;
//            break;
//        #endif
//
//        // Ericsson texture formats
//        #if defined(GL_OES_compressed_ETC1_RGB8_texture)
//        case TEX_FMT_ERIC_ETC1:
//            internalFormat = GL_ETC1_RGB8_OES;
//            alpha          = false;
//            compressed     = true;
//            break;
//        #endif
//
//        #if defined(GL_AMD_compressed_ATC_texture) && defined(PLATFORM_BADA)
//        case TEX_FMT_ATC_RGBA:
//            internalFormat = GL_ATC_RGBA_EXPLICIT_ALPHA_AMD; // http://www.brokenteapotstudios.com/android-game-development-blog/
//            alpha          = true;
//            compressed     = true;
//            break;
//        #endif
//
//        #endif
//                
//        default:
//            prTrace("Unknown texture format: %08x\n", texFormat);
//
//            // So we see on iphone release.
//            #if defined(PLATFORM_BADA)
//            OnScreenLogger::GetInstance()->Add(StringPrintf("Unknown texture format: %08x\n", texFormat));
//            #endif
//
//            // So we see on iphone release.
//            #if defined(PLATFORM_IOS)
//            printf("Unknown texture format: %08x\n", texFormat);
//            #endif
//
//            result = false;
//            break;
//        }
//
//        return result;
//    }
//
//    // Data
//    s32     width;
//    s32     height;
//    u32     texID;
//    bool    alpha;
//    bool    duplicate;
//    bool    exp1;
//    bool    exp2;
//
//} TextureImplementation;


// ----------------------------------------------------------------------------
// Ctor
// ----------------------------------------------------------------------------
prTexture::prTexture(const char *filename) : prResource(filename)
{
//#if defined(USE_SHARED_TEXTURES)
//    prTextureManager::GetInstance()->Add(this);
//#endif
}


// ----------------------------------------------------------------------------
// Dtor
// ----------------------------------------------------------------------------
prTexture::~prTexture()
{
//#if defined(USE_SHARED_TEXTURES)
//    prTextureManager::GetInstance()->Remove(this);
//#endif

    lastTextureID = 0xFFFFFFFF;
    Unload();
}


// ----------------------------------------------------------------------------
// Bind the texture.
// ----------------------------------------------------------------------------
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


// ----------------------------------------------------------------------------
// Unbind the texture.
// ----------------------------------------------------------------------------
void prTexture::Unbind()
{
}


// ----------------------------------------------------------------------------
// Load the texture
// ----------------------------------------------------------------------------
void prTexture::Load()
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
                    glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, header->dwWidth, header->dwHeight, 0, size - sizeof(PVRTextureHeader), ((u8*)header + sizeof(PVRTextureHeader)));
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


// ----------------------------------------------------------------------------
// Unloads the texture.
// ----------------------------------------------------------------------------
void prTexture::Unload()
{
    if (m_texID != 0xFFFFFFFF)
    {
//        if (!m_duplicate)
        {
            GLboolean result = glIsTexture(m_texID);
            ERR_CHECK();
            if (result)
            {
                glDeleteTextures(1, &m_texID);
                ERR_CHECK();
            }
        //else
        //{
        //    PRWARN("prTexture::Unload - Invalid ID: %s", Filename());
        //}
        }
    }

    m_width       = 0;
    m_height      = 0;
    m_texID       = 0xFFFFFFFF;
    m_duplicate   = false;
    lastTextureID = 0xFFFFFFFF;
}


// ----------------------------------------------------------------------------
// Allows embedded data to be used.
// ----------------------------------------------------------------------------
void prTexture::LoadFromMemory(void *pData, u32 size)
{
    PRASSERT(pData);
    
    //Unload();

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
                    glCompressedTexImage2D(GL_TEXTURE_2D, 0, internalFormat, header->dwWidth, header->dwHeight, 0, size - sizeof(prPVRTextureHeader), ((u8*)header + sizeof(PVRTextureHeader)));
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


//// ----------------------------------------------------------------------------
//// Allows raw data to be used
//// ----------------------------------------------------------------------------
//void prTexture::LoadFromRaw(void *pData, u32 size, u32 width, u32 height)
//{
//    PRASSERT(pData);
//
//    if (pData)
//    {
//        // Sanity checks
//        if (size == 0)
//        {
//            PRWARN("prTexture::LoadFromMemory: Size is zero.");
//            return;
//        }
//
//        // Set details
//        m_width  = width;
//        m_height = height;
//
//#if defined(_DEBUG) || defined(DEBUG)
//        // Size validation.
//        if (!ValidateSize(width))
//        {
//            prTrace("prTexture: %s\n", Filename());
//            prTrace("Invalid texture width\n");
//        }
//
//        // Size validation.
//        if (!ValidateSize(height))
//        {
//            prTrace("prTexture: %s\n", Filename());
//            prTrace("Invalid texture height\n");
//        }
//#endif
//
//        // allocate a texture name
//        glGenTextures(1, &m_texID);
//        if (glGetError() != GL_NO_ERROR)
//        {
//            prTrace("Failed to generate texture: %s\n", Filename());
//            m_width  = 0;
//            m_height = 0;
//            m_texID  = 0xFFFFFFFF;
//            return;
//        }            
//
//        // select our current texture
//        glBindTexture(GL_TEXTURE_2D, m_texID);
//        lastTextureID = m_texID;
//        ERR_CHECK();
//
//        // Settings
//        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
//        ERR_CHECK();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        ERR_CHECK();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        ERR_CHECK();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        ERR_CHECK();
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//        ERR_CHECK();
//        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//        ERR_CHECK();
//
//        // Create texture
//        int internalFormat, format, type;
//        bool compressed;
//        if (GetTextureFormat(TEX_FMT_OGL8888_BMP_YN, internalFormat, format, type, compressed))
//        {
//            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, pData);
//            ERR_CHECK();
//        }
//        else
//        {
//            PRWARN("Invalid texture format: %s", Filename());
//            Unload();
//        }
//    }
//}


// ----------------------------------------------------------------------------
// Checks for power of two compliance.
// ----------------------------------------------------------------------------
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


// Validates that we have a PVR file.
bool prTexture::ValidateHeader(prPVRTextureHeader *header)
{
    bool result = false;

    if (header->dwHeaderSize == sizeof(prPVRTextureHeader))
    {
        if (header->dwPVR == HEADER_MAGIC)
        {
            //prTrace("HS: %08x\n", header->dwPVR);
            result = true;
        }
    }

    return result; //dwPVR
}

// Acquires the texture format
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
        alpha          = true;
        compressed     = false;
        break;

    case TEX_FMT_OGL5551:
        internalFormat = GL_RGBA;
        format         = GL_RGBA;
        type           = GL_UNSIGNED_SHORT_5_5_5_1;
        alpha          = true;
        compressed     = false;
        break;

    case TEX_FMT_OGL565:
        internalFormat = GL_RGB;
        format         = GL_RGB;
        type           = GL_UNSIGNED_SHORT_5_6_5;
        alpha          = false;
        compressed     = false;
        break;

    //// PVR texture formats
    //#if defined(GL_IMG_texture_compression_pvrtc)
    //    #if defined(PLATFORM_ANDROID)
    //    case TEX_FMT_PVRTC_4BPPV1A_F1:
    //    case TEX_FMT_PVRTC_4BPPV1A_F2:
    //    #endif
    //case TEX_FMT_PVRTC_4BPPV1A:
    //    internalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
    //    alpha          = true;
    //    compressed     = true;
    //    break;

    //case TEX_FMT_PVRTC_4BPPV1:
    //    internalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
    //    alpha          = false;
    //    compressed     = true;
    //    break;
    //#endif

    //// Ericsson texture formats
    //#if defined(GL_OES_compressed_ETC1_RGB8_texture)
    //case TEX_FMT_ERIC_ETC1:
    //    internalFormat = GL_ETC1_RGB8_OES;
    //    alpha          = false;
    //    compressed     = true;
    //    break;
    //#endif

    //#if defined(GL_AMD_compressed_ATC_texture) && defined(PLATFORM_BADA)
    //case TEX_FMT_ATC_RGBA:
    //    internalFormat = GL_ATC_RGBA_EXPLICIT_ALPHA_AMD; // http://www.brokenteapotstudios.com/android-game-development-blog/
    //    alpha          = true;
    //    compressed     = true;
    //    break;
    //#endif

    #endif
                
    default:
        prTrace("Unknown texture format: %08x\n", texFormat);

        // So we see on other releases
        #if defined(PLATFORM_BADA)
        OnScreenLogger::GetInstance()->Add(StringPrintf("Unknown texture format: %08x\n", texFormat));
        #endif

        //// So we see on iphone release.
        //#if defined(PLATFORM_IOS)
        //printf("Unknown texture format: %08x\n", texFormat);
        //#endif

        result = false;
        break;
    }

    return result;
}
