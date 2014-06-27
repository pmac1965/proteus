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


#if defined(PLATFORM_PC)


// FreeType Headers
#include <ft2build.h>
#include <freetype.h>
#include <ftglyph.h>
#include <ftoutln.h>
#include <fttrigon.h>


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
#include "../file/prFile.h"
#include "../file/prFileShared.h"
#include "../tinyxml/tinyxml.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../core/prCore.h"
#include "../core/prResourceManager.h"
#include "../utf8proc/utf8proc.h"
#include "../display/prTexture.h"
#include "../display/prRenderer.h"
#include "../display/prOglUtils.h"


// Defines
#define MSG_BUFFER_SIZE     1024


// Type for drawing quad.
/*typedef struct QuadData
{
	float x, y;
	float u, v;

}  QuadData;//*/


// Implementation data.
typedef struct TrueTypeFontImplementation
{
    // ------------------------------------------------------------------------
    // Ctor
    // ------------------------------------------------------------------------
    TrueTypeFontImplementation()
    {
        h        = 0;
        textures = NULL;
        listBase = 0;
    }

    
    // ------------------------------------------------------------------------
    // Dtor
    // ------------------------------------------------------------------------
    ~TrueTypeFontImplementation()
    {
        PRSAFE_DELETE(textures);
    }

    float   h;          // Holds the height of the font.
    GLuint *textures;   // Holds the texture id's 
    GLuint  listBase;   // Holds the first display list id


} TrueTypeFontImplementation;


///This function gets the first power of 2 >= the
///int that we pass it.
inline int next_p2 ( int a )
{
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}


/// Create a display list coresponding to the give character.
void make_dlist(FT_Face face, char ch, GLuint list_base, GLuint *tex_base )
{
	//The first thing we do is get FreeType to render our character
	//into a bitmap.  This actually requires a couple of FreeType commands:

	// Load the glyph for our character
	if (FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT))
    {
		prTrace("FT_Load_Glyph failed\n");
        return;
    }

	// Move the faces glyph into a glyph object.
    FT_Glyph glyph;
    if (FT_Get_Glyph( face->glyph, &glyph ))
    {
		prTrace("FT_Get_Glyph failed\n");
        return;
    }

	// Convert the glyph to a bitmap.
	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1);
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

	// This reference will make accessing the bitmap easier
	FT_Bitmap& bitmap = bitmap_glyph->bitmap;

	// Use our helper function to get the widths of
	// the bitmap data that we will need in order to create
	// our texture.
	int width  = next_p2( bitmap.width );
	int height = next_p2( bitmap.rows );

	// Allocate memory for the texture data.
	GLubyte* expanded_data = new GLubyte[ 2 * width * height];

	// Here we fill in the data for the expanded bitmap.
	// Notice that we are using two channel bitmap (one for
	// luminocity and one for alpha), but we assign
	// both luminocity and alpha to the value that we
	// find in the FreeType bitmap. 
	// We use the ?: operator so that value which we use
	// will be 0 if we are in the padding zone, and whatever
	// is the the Freetype bitmap otherwise.
	for(int j = 0; j < height; j++)
    {
		for(int i=0; i < width; i++)
        {
			expanded_data[2 * (i + j * width)    ] = 255;
            expanded_data[2 * (i + j * width) + 1] = (i >= bitmap.width || j >= bitmap.rows) ? 0 : bitmap.buffer[i + bitmap.width * j];
		}
	}


	// Now we just setup some texture paramaters.
    glBindTexture(GL_TEXTURE_2D, tex_base[ch]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Here we actually create the texture itself, notice
	// that we are using GL_LUMINANCE_ALPHA to indicate that
	// we are using 2 channel data.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, expanded_data);

	// With the texture created, we don't need to expanded data anymore
    delete [] expanded_data;

	// So now we can create the display list
	glNewList(list_base+ch,GL_COMPILE);

	glBindTexture(GL_TEXTURE_2D,tex_base[ch]);

	glPushMatrix();

	//first we need to move over a little so that
	//the character has the right amount of space
	//between it and the one before it.
	glTranslatef(bitmap_glyph->left, 0, 0);

	//Now we move down a little in the case that the
	//bitmap extends past the bottom of the line 
	//(this is only true for characters like 'g' or 'y'.
	glTranslatef(0, bitmap_glyph->top-bitmap.rows, 0);

	//Now we need to account for the fact that many of
	//our textures are filled with empty padding space.
	//We figure what portion of the texture is used by 
	//the actual character and store that information in 
	//the x and y variables, then when we draw the
	//quad, we will only reference the parts of the texture
	//that we contain the character itself.
	float	x=(float)bitmap.width / (float)width,
			y=(float)bitmap.rows / (float)height;

	//Here we draw the texturemaped quads.
	//The bitmap that we got from FreeType was not 
	//oriented quite like we would like it to be,
	//so we need to link the texture to the quad
	//so that the result will be properly aligned.
	glBegin(GL_QUADS);
	//glTexCoord2d(0,0); glVertex2f(0,bitmap.rows);
	//glTexCoord2d(0,y); glVertex2f(0,0);
	//glTexCoord2d(x,y); glVertex2f(bitmap.width,0);
	//glTexCoord2d(x,0); glVertex2f(bitmap.width,bitmap.rows);
	glTexCoord2d(x,y); glVertex2f(bitmap.width, bitmap.rows);
	glTexCoord2d(x,0); glVertex2f(bitmap.width, 0);
	glTexCoord2d(0,0); glVertex2f(0, 0);
	glTexCoord2d(0,y); glVertex2f(0, bitmap.rows);
	glEnd();
	glPopMatrix();
	glTranslatef(face->glyph->advance.x >> 6 ,0,0);


	//increment the raster position as if we were a bitmap font.
	//(only needed if you want to calculate text length)
	//glBitmap(0,0,0,0,face->glyph->advance.x >> 6,0,NULL);

	// Finish the display list
	glEndList();

    FT_Done_Glyph(glyph);
}



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
    PRASSERT(filename && *filename);
    PRASSERT(pImpl);

    // Create And initilize the FreeType Font Library.
    FT_Library library;
    
    if (FT_Init_FreeType( &library ))
    {
        prTrace("FT_Init_FreeType failed\n");
        return;
    }
 
    // Load the font
    FT_Face face;
    if (FT_New_Face(library, filename, 0, &face))
    {
        prTrace("FT_New_Face failed. Tthere is probably a problem with your font file\n");
        return;
    }

    prTrace("NUM GLYPHS: %i\n", face->num_glyphs);

    // For some reason, FreeType measures font size
    // in terms of 1/64ths of pixels. To make a font
    // 'height' pixels high, we need to request a size of height * 64.
    FT_Set_Char_Size(face, height << 6, height << 6, 96, 96);
 
    // Here We Ask OpenGL To Allocate Resources For
    // All The Textures And Display Lists Which We
    // Are About To Create. 
    imp.textures = new GLuint[128];
	imp.h = (f32)height;

    //GLuint elementbuffer;
    //glGenBuffers(1, &elementbuffer);

    imp.listBase=glGenLists(128);
    glGenTextures( 128, imp.textures );
 
    // This Is Where We Actually Create Each Of The Fonts Display Lists.
    for(unsigned char i=0;i<128;i++)
    {
        make_dlist(face, i, imp.listBase, imp.textures);
    }

    // Clean up
    FT_Done_Face(face);
    FT_Done_FreeType(library);
}


/// ---------------------------------------------------------------------------
/// Draw
/// ---------------------------------------------------------------------------
void prTrueTypeFont::Draw(f32 x, f32 y, const char *fmt, ...)
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

    Draw(x, y, 1.0f, prColour::White, ALIGN_LEFT, message);
}


/// ---------------------------------------------------------------------------
/// Draw
/// ---------------------------------------------------------------------------
void prTrueTypeFont::Draw(f32 x, f32 y, float scale, prColour colour, s32 alignment, const char *fmt, ...)
{
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

        // Set the colour.
        glColor4f(colour.red, colour.green, colour.blue, colour.alpha);


	    glListBase(imp.listBase);

	float modelview_matrix[16];	
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview_matrix);

	//This is where the text display actually happens.
	//For each line of text we reset the modelview matrix
	//so that the line's text will start in the correct position.
	//Notice that we need to reset the matrix, rather than just translating
	//down by h. This is because when each character is
	//draw it modifies the current matrix so that the next character
	//will be drawn immediatly after it.  
	//for(int i=0;i<10;i++)
    {
        // http://www.glprogramming.com/red/chapter07.html
		

		glPushMatrix();
		glLoadIdentity();       
		glTranslatef(x, y, 0);
        glScalef(scale, scale, 0.0f);
		glMultMatrixf(modelview_matrix);

	//  The commented out raster position stuff can be useful if you need to
	//  know the length of the text that you are creating.
	//  If you decide to use it make sure to also uncomment the glBitmap command
	//  in make_dlist().
	//	glRasterPos2f(0,0);
		glCallLists(strlen(message), GL_UNSIGNED_BYTE, message);
	//	float rpos[4];
	//	glGetFloatv(GL_CURRENT_RASTER_POSITION ,rpos);
	//	float len=x-rpos[0];

		glPopMatrix();
	}

    }
}


/// ---------------------------------------------------------------------------
/// Returns the pixel length of the string.
/// ---------------------------------------------------------------------------
prVector2 prTrueTypeFont::MeasureString(const char *string, float scale)
{    
    return prVector2::Zero;
}


#endif
