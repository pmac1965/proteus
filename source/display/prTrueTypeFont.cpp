/**
 * prTrueTypeFont.cpp
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
#include "../font/prFontGlyph.h"
#include "../font/prTagParser.h"


// Defines
#define MSG_BUFFER_SIZE     1024
#define RESOLUTION          72


#if defined(ALLOW_FREETYPE)
#if defined(PLATFORM_PC)                // The PC points at the full freetype2 project.
    // FreeType Headers
    #include <ft2build.h>
    #include <freetype.h>
    #include <ftglyph.h>
    #include <ftoutln.h>
    #include <fttrigon.h>
#else                                   // OpenglES uses an abbreviated version
    // FreeType Headers
    #include <ft2build.h>
    #include <freetype/freetype.h>
    #include <freetype/ftglyph.h>
    #include <freetype/ftoutln.h>
    #include <freetype/fttrigon.h>
#endif
#endif


// Platform
#if defined(PLATFORM_PC)
  #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
  #endif
  
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
	#include <FGraphicsOpengl.h>
	using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>

#else
  #error No platform defined.

#endif


#include <stdarg.h>
#include "prTrueTypeFont.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"
#include "../display/prTexture.h"
#include "../display/prRenderer.h"
#include "../display/prOglUtils.h"
#include "../display/prTexture.h"
#include "../math/prMathsUtil.h"
#include "../file/prFile.h"


// Implementation data.
typedef struct TrueTypeFontImplementation
{
    // ------------------------------------------------------------------------
    // Ctor
    // ------------------------------------------------------------------------
    TrueTypeFontImplementation()
    {
        mpGlyphs    = NULL;
        mGlyphCount = 0;
        mPointSize  = 0;
    }

    
    // ------------------------------------------------------------------------
    // Dtor
    // ------------------------------------------------------------------------
    ~TrueTypeFontImplementation()
    {
        DeinitGlyphs();
    }


    /// -----------------------------------------------------------------------
    /// Creates a characters texture and positioning data
    /// -----------------------------------------------------------------------
    #if defined(ALLOW_FREETYPE)
    void GenerateCharacter(FT_Face face, char charcode)
    {
        // Get glyph index
        u32 glyphIndex = FT_Get_Char_Index(face, charcode);
        if (glyphIndex == 0)
        {
            return;
        }

	    // Load the glyph for our character
	    if (FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT))
        {
		    prTrace("FT_Load_Glyph failed for character %i, Glyph index %i\n", charcode, glyphIndex);
            return;
        }

	    // Move the faces glyph into a glyph object.
        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph))
        {
		    prTrace("FT_Get_Glyph failed\n");
            return;
        }

	    // Convert the glyph to a bitmap.
	    FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
        FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	    // This reference will make accessing the bitmap easier
	    FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	    // Create texture width/height
	    int width  = prNextPowerOf2(bitmap.width);
	    int height = prNextPowerOf2(bitmap.rows);
        //prTrace("Bitmap: %i, %i, (%i, %i) == %c\n", bitmap.width, bitmap.rows, width, height, charcode);

	    // Allocate memory for the texture data.
	    GLubyte* texData = new GLubyte[2 * width * height];

	    // Here we fill in the data for the expanded bitmap.
	    for(int j = 0; j < height; j++)
        {
		    for(int i=0; i < width; i++)
            {
			    texData[2 * (i + j * width)    ] = 255;
                texData[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
		    }
	    }

	    // Generate texture
        u32 texID = 0;
        glGenTextures(1, &texID);
        if (glGetError() != GL_NO_ERROR)
        {
            prTrace("Failed to generate texture for TTF font\n");
            prOpenGLErrorCheck(__FILE__, __FUNCTION__, __LINE__);
            PRSAFE_DELETE_ARRAY(texData);
            FT_Done_Glyph(glyph);
            return;
        }            

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

	    // Now we just setup some texture paramaters.
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        ERR_CHECK();    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);    
        ERR_CHECK();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ERR_CHECK();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        ERR_CHECK();

	    // Here we actually create the texture itself, notice that we are using GL_LUMINANCE_ALPHA to indicate that we are using 2 channel data.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, texData);

	    // Clear used data
        PRSAFE_DELETE_ARRAY(texData);

	    // Now we need to account for the fact that many of
	    // our textures are filled with empty padding space.
	    // We figure what portion of the texture is used by 
	    // the actual character and store that information in 
	    // the x and y variables, then when we draw the
	    // quad, we will only reference the parts of the texture
	    // that we contain the character itself.
	    float x = (float)bitmap.width / (float)width,
			  y = (float)bitmap.rows  / (float)height;

        // Create glyph
        prFontGlyph *pGlyph = new prFontGlyph((f32)(face->glyph->advance.x >> 6), (f32)(face->glyph->advance.y >> 6),   // Advance X, Y
                                              (f32)(bitmap_glyph->left), (f32)mPointSize - bitmap_glyph->top,           // Positioning offset X, Y
                                              charcode,                                                                 // The char code
                                              texID);                                                                   // The texture ID

        pGlyph->SetTextureCoords(0, 0, y);
        pGlyph->SetTextureCoords(1, x, y);
        pGlyph->SetTextureCoords(2, x, 0);
        pGlyph->SetTextureCoords(3, 0, y);
        pGlyph->SetTextureCoords(4, x, 0);
        pGlyph->SetTextureCoords(5, 0, 0);

        pGlyph->SetVertexCoords(0, 0,                 (f32)bitmap.rows);
        pGlyph->SetVertexCoords(1, (f32)bitmap.width, (f32)bitmap.rows);
        pGlyph->SetVertexCoords(2, (f32)bitmap.width, 0);
        pGlyph->SetVertexCoords(3, 0,                 (f32)bitmap.rows);
        pGlyph->SetVertexCoords(4, (f32)bitmap.width, 0);
        pGlyph->SetVertexCoords(5, 0,                 0);

        mpGlyphs[charcode] = pGlyph;

        // Clean up
        FT_Done_Glyph(glyph);
    }
    #endif


    /// -----------------------------------------------------------------------
    /// Creates the glyph data
    /// -----------------------------------------------------------------------
    void InitGlyphs(u32 glyphCount)
    {
        if (glyphCount == 0)
        {
            prTrace("Glyph count cannot be zero\n");
            return;
        }

        if (mpGlyphs != NULL)
        {
            prTrace("Glyph array already created\n");
            return;
        }

        // Create array
        mpGlyphs    = new prFontGlyph*[glyphCount];
        mGlyphCount = glyphCount;

        // Clear array
        memset(mpGlyphs, 0, sizeof(prFontGlyph*) * glyphCount);
    }


    /// -----------------------------------------------------------------------
    /// Destroys the glyph data
    /// -----------------------------------------------------------------------
    void DeinitGlyphs()
    {
        // Kill the glyphs
        if (mpGlyphs && mGlyphCount > 0)
        {
            for (u32 i=0; i<mGlyphCount; i++)
            {
                PRSAFE_DELETE(mpGlyphs[i]);
            }
        }

        // Kill the array
        PRSAFE_DELETE_ARRAY(mpGlyphs);
    }


    // Data
    prFontGlyph   **mpGlyphs;       // Points to the glyph data
    u32             mGlyphCount;    // Holds the numbers of glyphs      
    s32             mPointSize;     // Holds the height of the font.

} TrueTypeFontImplementation;




/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prTrueTypeFont::prTrueTypeFont() : pImpl (new TrueTypeFontImplementation())
                                 , imp   (*pImpl)
{
    PRASSERT(pImpl);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prTrueTypeFont::~prTrueTypeFont()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Loads the font data.
/// ---------------------------------------------------------------------------
void prTrueTypeFont::Load(const char *filename, s32 height)
{
#if defined(ALLOW_FREETYPE)

    PRASSERT(filename && *filename);
    PRASSERT(pImpl);
    PRASSERT(height > 0);

    // Create and initilize the freetype font library.
    FT_Library library;    
    if (FT_Init_FreeType(&library))
    {
        prTrace("FT_Init_FreeType failed\n");
        return;
    }

    // Load the font
    prFile *pFile = new prFile(filename);
    if (pFile)
    {
        pFile->Open();

        // Get and check size.
        u32 size = pFile->Size();
        if (size == 0)
        {
            prTrace("prTrueTypeFont::Load failed. File was zero length\n");
            return;
        }

        // Create buffer
        char *buffer = new char [size];

        // Read file
        pFile->Read(buffer, size);
        pFile->Close();
        PRSAFE_DELETE(pFile);

        // Load the font
        FT_Face face;
        //if (FT_New_Face(library, filename, 0, &face))
        //{
        //    prTrace("FT_New_Face failed. There is probably a problem with your font file\n");
        //    return;
        //}
        if (FT_New_Memory_Face(library,
                               (const FT_Byte *)buffer,
                               size,
                               0,
                               &face))
        {
            prTrace("FT_New_Face failed. There is probably a problem with your font file\n");
            PRSAFE_DELETE_ARRAY(buffer);
            return;
        }

        // Init glyphs
        imp.InitGlyphs(256);

        // For some reason, FreeType measures font size
        // in terms of 1/64ths of pixels. To make a font
        // 'height' pixels high, we need to request a size of height * 64.
        FT_Set_Char_Size(face, 0L, height << 6, RESOLUTION, RESOLUTION);
 
        // Init data
        imp.mPointSize = height;

        // This is where we actually create the fonts texture
        for(unsigned char i=0; i<128; i++)
        {
            imp.GenerateCharacter(face, i);
        }

        // Clean up
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        PRSAFE_DELETE_ARRAY(buffer);
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Loads the font data.
/// ---------------------------------------------------------------------------
void prTrueTypeFont::Load(const char *filename, s32 height, const char *characters)
{
#if defined(ALLOW_FREETYPE)

    PRASSERT(filename && *filename);
    PRASSERT(characters && *characters);
    PRASSERT(pImpl);
    PRASSERT(height > 0);

    // Create and initilize the freetype font library.
    FT_Library library;    
    if (FT_Init_FreeType(&library))
    {
        prTrace("FT_Init_FreeType failed\n");
        return;
    }

    // Load the font
    prFile *pFile = new prFile(filename);
    if (pFile)
    {
        pFile->Open();

        // Get and check size.
        u32 size = pFile->Size();
        if (size == 0)
        {
            prTrace("prTrueTypeFont::Load failed. File was zero length\n");
            return;
        }

        // Create buffer
        char *buffer = new char [size];

        // Read file
        pFile->Read(buffer, size);
        pFile->Close();
        PRSAFE_DELETE(pFile);

        // Load the font
        FT_Face face;
        if (FT_New_Memory_Face(library,
                               (const FT_Byte *)buffer,
                               size,
                               0,
                               &face))
        {
            prTrace("FT_New_Face failed. There is probably a problem with your font file\n");
            PRSAFE_DELETE_ARRAY(buffer);
            return;
        }

        // Init glyphs
        imp.InitGlyphs(256);

        // For some reason, FreeType measures font size
        // in terms of 1/64ths of pixels. To make a font
        // 'height' pixels high, we need to request a size of height * 64.
        FT_Set_Char_Size(face, 0L, height << 6, RESOLUTION, RESOLUTION);
 
        // Init data
        imp.mPointSize = height;

        // This is where we actually create the fonts texture
        s32 len = strlen(characters);
        for(s32 i=0; i<len; i++)
        {
            imp.GenerateCharacter(face, characters[i]);
        }

        // Clean up
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        PRSAFE_DELETE_ARRAY(buffer);
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Draw
/// ---------------------------------------------------------------------------
void prTrueTypeFont::Draw(f32 x, f32 y, const char *fmt, ...)
{
#if defined(ALLOW_FREETYPE)

    char message[MSG_BUFFER_SIZE];

	// Format the output.
    va_list args;
    va_start(args, fmt);        

    #if defined(PLATFORM_BADA)
    vsnprintf(message, sizeof(message), fmt, args);
    #else
    vsprintf(message, fmt, args);
    #endif

    va_end(args);

    Draw(x, y, 1.0f, prColour::White, ALIGN_LEFT, message);

#endif
}


/// ---------------------------------------------------------------------------
/// Draw
/// ---------------------------------------------------------------------------
void prTrueTypeFont::Draw(f32 x, f32 y, float scale, prColour colour, s32 alignment, const char *fmt, ...)
{
#if defined(ALLOW_FREETYPE)

    if (fmt && *fmt)
    {
        char message[MSG_BUFFER_SIZE];

		// Format the output.
        va_list args;
        va_start(args, fmt);        

        #if defined(PLATFORM_BADA)
        vsnprintf(message, sizeof(message), fmt, args);
        #else
        vsprintf(message, fmt, args);
        #endif
        va_end(args);


        // Set alignment
        switch(alignment)
        {
        default:
            prTrace("Unsupported alignment\n");
            break;

        case ALIGN_LEFT:
            break;

        case ALIGN_RIGHT:
            x -= MeasureString(message, scale).x;
            break;

        case ALIGN_CENTER:
            x -= MeasureString(message, scale).x / 2;
            break;
        }


        // Set the colour.
        glColor4f(colour.red, colour.green, colour.blue, colour.alpha);

		glPushMatrix();
		glLoadIdentity();       
		glTranslatef(x, y, 0);
        glScalef(scale, scale, 0.0f);

        // Enable blending
        glEnable(GL_BLEND);
        ERR_CHECK();
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ERR_CHECK();

        // Set states
        glEnableClientState(GL_VERTEX_ARRAY);
        ERR_CHECK();
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        ERR_CHECK();
		    
        // Draw
        s32 len = strlen(message);
        //s32 character;
        f32 lineWidth = 0.0f;

        for (s32 i=0; i<len; i++)
        {
            s32 character = message[i];

            if (character == '#')
            {
                // Tag?
                u32 tag = prTagIsTag(&message[i], colour.alpha);
                if (tag != PRTT_NONE)
                {
                    prTagDoAction();
                    i += prTagGetTagLength();
                }
                else
                {
                    // Draw #
                    prFontGlyph *pGlyph = imp.mpGlyphs[character];
                    if (pGlyph)
                    {
                        pGlyph->Draw();
                        lineWidth += (pGlyph->mAdvance.x); 
                    }
                }
            }
            else if (character == '\n')
            {
                glTranslatef(-lineWidth, (f32)imp.mPointSize, 0);
                lineWidth = 0.0f;
            }
            else if (character != '\r')
            {
                prFontGlyph *pGlyph = imp.mpGlyphs[character];
                if (pGlyph)
                {
                    pGlyph->Draw();
                    lineWidth += (pGlyph->mAdvance.x); 
                }
            }
        }

        // Reset states
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        ERR_CHECK();
        glDisableClientState(GL_VERTEX_ARRAY);
        ERR_CHECK();
        glDisable(GL_BLEND);
        ERR_CHECK();

		glPopMatrix();
        ERR_CHECK();
    }
    
    // Clear texture systems last used ID
    prTextureClearLastID();

#endif
}


/// ---------------------------------------------------------------------------
/// Returns the pixel length of the string.
/// ---------------------------------------------------------------------------
prVector2 prTrueTypeFont::MeasureString(const char *string, float scale)
{
    prVector2 size = prVector2(0, (f32)imp.mPointSize);

#if defined(ALLOW_FREETYPE)

    f32 max = 0.0f;
    s32 len = strlen(string);
    //s32 character;

    for (s32 i=0; i<len; i++)
    {
        s32 character = string[i];

        // Get size.
        if (character == '#')
        {
            // Tag?
            u32 tag = prTagIsTag(&string[i]);
            if (tag != PRTT_NONE)
            {
                i += prTagGetTagLength();
            }
            else
            {
                // Measure
                prFontGlyph *pGlyph = imp.mpGlyphs[character];
                if (pGlyph)
                {
                    size.x += pGlyph->mAdvance.x;
                    if (size.x > max)
                    {
                        max = size.x;
                    }
                }
            }
        }
        else if (character == '\n')
        {
            size.x  = 0.0f;
            size.y += imp.mPointSize;
        }
        else if (character != '\0' && character != '\r')
        {
            prFontGlyph *pGlyph = imp.mpGlyphs[character];
            if (pGlyph)
            {
                size.x += pGlyph->mAdvance.x;
                if (size.x > max)
                {
                    max = size.x;
                }
            }
        }
    }

    // Set max width
    size.x = max;

#endif

    return (size * scale);
}
