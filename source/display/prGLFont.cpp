/**
 * prGLFont.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "prGLFont.h"
#include "../debug/prDebug.h"


#define FONT_CHARACTER_COUNT    256


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prGLFont::prGLFont(HDC dc, s32 fontSize, const char *fontName)
{
    // Create display lists.
    m_base = glGenLists(FONT_CHARACTER_COUNT);

    HFONT font;

    if (_stricmp(fontName, "symbol") == 0)
    {    
        font = CreateFontA
        (
            fontSize,
            0,
            0,
            0,
            FW_BOLD,
            FALSE,
            FALSE,
            FALSE,                            
            SYMBOL_CHARSET,
            OUT_TT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY,
            FF_DONTCARE | DEFAULT_PITCH,
            fontName
        );
    }
    else
    {
        font = CreateFontA
        (
            fontSize,
            0,
            0,
            0,
            FW_BOLD,
            FALSE,
            FALSE,
            FALSE,                            
            ANSI_CHARSET,
            OUT_TT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            ANTIALIASED_QUALITY,
            FF_DONTCARE | DEFAULT_PITCH,
            fontName
        );
    }


    // Does the font exist?
    if (!font)
    {
        if (glIsList(m_base))
        {
            glDeleteLists(m_base, 256);
        }
       
        m_base = 0;
        return;
    }


    // Set up the font
    HFONT oldFont = (HFONT)SelectObject(dc, font);

    wglUseFontBitmaps(dc, 0, 256, m_base);

    SelectObject(dc, oldFont);

    DeleteObject(font);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prGLFont::~prGLFont()
{
    if (glIsList(m_base))
    {
        glDeleteLists(m_base, 256);
        m_base = 0;
    }
}


/// ---------------------------------------------------------------------------
/// Draws the text.
/// ---------------------------------------------------------------------------
void prGLFont::Draw(s32 x, s32 y, const char *text)
{
    Draw((float)x, (float)y, text);
}


/// ---------------------------------------------------------------------------
/// Draws the text.
/// ---------------------------------------------------------------------------
void prGLFont::Draw(float x, float y, const char *text)
{
    if (glIsList(m_base) && text && *text)
    {
        // Disable textures, so we use the colour.
        glDisable(GL_TEXTURE_2D);

        // Set the color of the text.
        glColor3f(1.0f, 1.0f, 1.0f);

        TODO("Fix")
        SetOrthographicProjection
        (
            800, 600
            //(float)System::GetInstance().Get(SVT_DISPLAY_WIDTH),
            //(float)System::GetInstance().Get(SVT_DISPLAY_HEIGHT)
        );
        
        // Draw the text.
        {
            glPushMatrix();
            glLoadIdentity();

            // Set position.
            glRasterPos2f(x, y);

            // Render the text.
            glPushAttrib(GL_LIST_BIT);
            glListBase(m_base);
            glCallLists((GLsizei)strlen(text), GL_UNSIGNED_BYTE, text); 
            glPopAttrib();

            glPopMatrix();
        }

        ResetPerspectiveProjection();

        glEnable(GL_TEXTURE_2D);
    }
}


/// ---------------------------------------------------------------------------
/// Sets othographic perspective.
/// ---------------------------------------------------------------------------
void prGLFont::SetOrthographicProjection(float w, float h)
{
    // Switch to projection mode
    glMatrixMode(GL_PROJECTION);

    // save previous matrix which contains the settings for the perspective projection
    glPushMatrix();

    // Reset matrix
    glLoadIdentity();
    
    // Set a 2D orthographic projection
    gluOrtho2D(0, w, 0, h);
    
    // Invert the y axis, down is positive.
    glScalef(1, -1, 1);
    
    // Move the origin from the bottom left corner to the upper left corner
    glTranslatef(0, -h, 0);
    glMatrixMode(GL_MODELVIEW);
}


/// ---------------------------------------------------------------------------
/// Resets perpective.
/// ---------------------------------------------------------------------------
void prGLFont::ResetPerspectiveProjection() 
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}


#endif//PLATFORM_PC
