/**
 * prFixedWidthFont.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prconfig.h"


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

#elif defined(PLATFORM_BADA)
  #include <string.h>
  #include <stdio.h>
  #include <FGraphicsOpengl.h>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include <string.h>

#else
  #error No platform defined.

#endif


#include <stdarg.h>
#include "prFixedWidthFont.h"
#include "prOglUtils.h"
#include "../core/prMacros.h"
#include "../core/prResourceManager.h"
//#include "../core/prSystemResourceManager.h"
#include "../display/prTexture.h"
#include "../display/prRenderer.h"
#include "../display/prColour.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"


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

    // Create the sprites texture and the sprite.
    TODO("Fix")
    m_pTexture = 0;// prSystemResourceManager::Get()->Load<Texture>(filename);
    PRASSERT(m_pTexture);
    m_width  = width;
    m_height = height;
    m_offset = offset;

    // Create the width data
    m_frameWidth   = width;
    m_frameHeight  = height;
    m_framesAcross = m_pTexture->GetWidth()  / width;
    m_framesDown   = m_pTexture->GetHeight() / height;
    m_framesTotal  = m_framesAcross * m_framesDown;
    m_frame        = 0;
    m_alignment    = 0;

    float pw = 1.0f / m_pTexture->GetWidth();
    float ph = 1.0f / m_pTexture->GetHeight();
    m_fw = pw * width;
    m_fh = ph * height;

    // Init
    m_u0    = 0.0f;
    m_u1    = 0.0f;
    m_v0    = 0.0f;
    m_v1    = 0.0f;
    m_scale = 1.0f;
    m_alignment = ALIGN_LEFT;
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
    m_width    = width;
    m_height   = height;
    m_offset   = offset;

    // Create the width data
    m_frameWidth   = width;
    m_frameHeight  = height;
    m_framesAcross = m_pTexture->GetWidth()  / width;
    m_framesDown   = m_pTexture->GetHeight() / height;
    m_framesTotal  = m_framesAcross * m_framesDown;
    m_frame        = 0;
    m_alignment    = 0;

    float pw = 1.0f / m_pTexture->GetWidth();
    float ph = 1.0f / m_pTexture->GetHeight();
    m_fw = pw * width;
    m_fh = ph * height;

    // Init
    m_u0    = 0.0f;
    m_u1    = 0.0f;
    m_v0    = 0.0f;
    m_v1    = 0.0f;
    m_scale = 1.0f;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFixedWidthFont::~prFixedWidthFont()
{
    if (m_pTexture)
    {
        TODO("Fix")
        //prSystemResourceManager::Get()->Unload(m_pTexture);
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


        // Enable blending
        glEnable(GL_BLEND);
        ERR_CHECK();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ERR_CHECK();

        // Set states
        glEnableClientState(GL_VERTEX_ARRAY);
        ERR_CHECK();
        glEnableClientState(GL_COLOR_ARRAY);
        ERR_CHECK();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        ERR_CHECK();

        // Check alignment
        if (m_alignment == ALIGN_RIGHT)
        {
            //Trace("R: %f, '%s'\n", PixelWidth(message), message, strlen(message), m_width);
            x -= PixelWidth(message);
        }

        char c;
        int      index = 0;
        float    dist  = 0;

        m_pTexture->Bind();

        do
        {
            c = message[index++];

            if (c == ' ')
            {
//                dist += m_offset;
                dist += (float)m_offset * m_scale;
                //y += m_frameHeight;
            }
            else if (c != '\0')
            {
                c -= 32;

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

                TODO("Fix")
                //Renderer::GetInstance()->BatchDrawQuad(m_u0, m_v0, m_u1, m_v1, Colour::White);
            
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
        glDisableClientState(GL_VERTEX_ARRAY);
        ERR_CHECK();

        // Disable blending
        glDisable(GL_BLEND);
        ERR_CHECK();
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
    case ALIGN_LEFT:
    case ALIGN_RIGHT:
        m_alignment = alignment;
        break;
    }
}

