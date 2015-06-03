/**
 * prFixedWidthFont.cpp
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


// ----------------------------------------------------------------------------
// Platform specifics
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>
  #include <string.h>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <string.h>

#elif defined(PLATFORM_BADA)
  #include <string.h>
  #include <stdio.h>
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include <string.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <string.h>
  #include <stdio.h>

#else
  #error No platform defined.

#endif


#include <stdarg.h>
#include "prFixedWidthFont.h"
#include "prOglUtils.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prResourceManager.h"
#include "../display/prTexture.h"
#include "../display/prRenderer.h"
#include "../display/prColour.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prFixedWidthFont::prFixedWidthFont(const char *filename, u32 width, u32 height, u32 offset)
{
    // Invariants
    PRASSERT(filename && *filename);
    PRASSERT(width);
    PRASSERT(height);
    PRASSERT(offset);

    // Create the texture.
    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
    PRASSERT(pRM)
    m_pTexture = pRM->Load<prTexture>(filename);
    PRASSERT(m_pTexture);

    // Create the width data
    m_frameWidth   = width;
    m_frameHeight  = height;
    m_framesAcross = m_pTexture->GetWidth()  / width;
    m_framesDown   = m_pTexture->GetHeight() / height;
    m_framesTotal  = m_framesAcross * m_framesDown;
    m_offset       = offset;

    float pw = 1.0f / m_pTexture->GetWidth();
    float ph = 1.0f / m_pTexture->GetHeight();
    m_fw = pw * width;
    m_fh = ph * height;

    // Init
    m_u0        = 0.0f;
    m_u1        = 0.0f;
    m_v0        = 0.0f;
    m_v1        = 0.0f;
    m_scale     = 1.0f;
    m_alignment = FW_ALIGN_LEFT;
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prFixedWidthFont::prFixedWidthFont(prTexture *pTexture, u32 width, u32 height, u32 offset)
{
    // Invariants
    PRASSERT(pTexture);
    PRASSERT(width);
    PRASSERT(height);
    PRASSERT(offset);

    // Create the sprites texture and the sprite.
    m_pTexture = pTexture;
    m_offset   = offset;

    // Create the width data
    m_frameWidth   = width;
    m_frameHeight  = height;
    m_framesAcross = m_pTexture->GetWidth()  / width;
    m_framesDown   = m_pTexture->GetHeight() / height;
    m_framesTotal  = m_framesAcross * m_framesDown;

    float pw = 1.0f / m_pTexture->GetWidth();
    float ph = 1.0f / m_pTexture->GetHeight();
    m_fw = pw * width;
    m_fh = ph * height;

    // Init
    m_u0        = 0.0f;
    m_u1        = 0.0f;
    m_v0        = 0.0f;
    m_v1        = 0.0f;
    m_scale     = 1.0f;
    m_alignment = FW_ALIGN_LEFT;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFixedWidthFont::~prFixedWidthFont()
{
    if (m_pTexture)
    {
        prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
        if (pRM)
        {
            pRM->Unload(m_pTexture);
        }
        m_pTexture = NULL;
    }
}


/// ---------------------------------------------------------------------------
/// Draw text
/// ---------------------------------------------------------------------------
void prFixedWidthFont::Draw(f32 x, f32 y, const char *fmt, ...)
{
    if (fmt && *fmt && m_pTexture)
    {
        char message[512];


        // Format the output.
        va_list args;
        va_start(args, fmt);        


        #if defined(PLATFORM_BADA)
        vsnprintf(message, sizeof(message), fmt, args);
        #else
        vsprintf(message, fmt, args);
        #endif
        va_end(args);


        //glEnable(GL_TEXTURE_2D);


        // Enable blending
        glEnable(GL_BLEND);
        ERR_CHECK();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ERR_CHECK();

        // Set states
        //glEnableClientState(GL_VERTEX_ARRAY);
        //ERR_CHECK();
        glEnableClientState(GL_COLOR_ARRAY);
        ERR_CHECK();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        ERR_CHECK();

        // Check alignment
        if (m_alignment == FW_ALIGN_RIGHT)
        {
            x -= PixelWidth(message);
        }
        else if (m_alignment == FW_ALIGN_CENTRE)
        {
            x -= (PixelWidth(message) / 2);
        }


        m_pTexture->Bind();


        char     c;
        int      index = 0;
        float    dist  = 0;
        do
        {
            c = message[index++];

            if (c == ' ')
            {
                dist += (float)m_offset * m_scale;
            }
            else if (c != '\0')
            {
                // Set the frame source rect position indices
                s32 xpos = c % m_framesAcross;
                s32 ypos = c / m_framesAcross;

                m_u0 = xpos * m_fw;
                m_u1 = m_u0 + m_fw;
                m_v0 = 1.0f - ((ypos * m_fh) + m_fh);
                m_v1 = m_v0 + m_fh;

                glPushMatrix();
                ERR_CHECK();
            
                // translate to quad center, the translate for position
                glTranslatef((float)(m_frameWidth >> 1), (float)(m_frameHeight >> 1), 0);
                ERR_CHECK();
                glTranslatef(x + dist, y, 0);
                ERR_CHECK();

                dist += (float)m_offset * m_scale;

                glScalef((float)m_frameWidth * m_scale, (float)m_frameHeight * m_scale, 0);
                ERR_CHECK();

                prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
                if (pRenderer)
                {
                    pRenderer->BatchDrawQuad(m_u0, m_v0, m_u1, m_v1, prColour::White);
                }
            
                glPopMatrix();
                ERR_CHECK();
            }
        }
        while(c != '\0');


        // Reset states
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        ERR_CHECK();
        glDisableClientState(GL_COLOR_ARRAY);
        ERR_CHECK();
        //glDisableClientState(GL_VERTEX_ARRAY);
        //ERR_CHECK();

        // Disable blending
        glDisable(GL_BLEND);
        ERR_CHECK();

        //glDisable(GL_TEXTURE_2D);
    }
}


/// ---------------------------------------------------------------------------
/// Get the width of the passed string.
/// ---------------------------------------------------------------------------
f32 prFixedWidthFont::PixelWidth(const char *pText) const
{
    f32 width = 0.0f;

    if (pText && *pText)
    {
        width = (strlen(pText) * m_offset) * m_scale;
    }

    return width;
}


/// ---------------------------------------------------------------------------
/// Set the text alignment.
/// ---------------------------------------------------------------------------
void prFixedWidthFont::SetAlignment(u32 alignment)
{
    switch(alignment)
    {
    case FW_ALIGN_LEFT:
    case FW_ALIGN_RIGHT:
    case FW_ALIGN_CENTER:
        m_alignment = alignment;
        break;

    default:
        prTrace(LogError, "prFixedWidthFont error: Invalid alignment\n");
        break;
    }
}

