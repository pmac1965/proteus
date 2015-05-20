/**
 * prFontGlyph.cpp
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


#include "prFontGlyph.h"


#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <cstring>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <cstring>

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include <cstring>

#elif defined(PLATFORM_BADA)
  #include <FGraphicsOpengl.h>
  #include <cstring>
  using namespace Osp::Graphics::Opengl;

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>
  #include <cstring>

#else
  #error No platform defined.

#endif


#include "../core/prCore.h"
#include "../display/prOglUtils.h"
#include "../display/prRenderer.h"
#include "../debug/prTrace.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prFontGlyph::prFontGlyph(f32 advanceX, f32 advanceY, f32 offsetX, f32 offsetY, u32 aChar, u32 texID)
   
    : mAdvance(advanceX, advanceY)
    , mOffset(offsetX, offsetY)
    , mCharacter(aChar)
    , mTextureID(texID)
{
    //prTrace("(%i): adv %f %f\n", aChar, advanceX, advanceY);
    memset(quadData, 0, sizeof(quadData));
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFontGlyph::~prFontGlyph()
{
}


/// ---------------------------------------------------------------------------
/// Sets the texture coords for the character
/// ---------------------------------------------------------------------------
void prFontGlyph::SetTextureCoords(u32 index, f32 u, f32 v)
{
    if (index < 6)
    {
        quadData[index].u = u;
        quadData[index].v = v;
    }
}


/// ---------------------------------------------------------------------------
/// Sets the vertex coords for the character
/// ---------------------------------------------------------------------------
void prFontGlyph::SetVertexCoords(u32 index, f32 x, f32 y)
{
    if (index < 6)
    {
        quadData[index].x = x;
        quadData[index].y = y;
    }
}


/// ---------------------------------------------------------------------------
/// Draws this glyph
/// ---------------------------------------------------------------------------
void prFontGlyph::Draw()
{
    // Move to offset
    glTranslatef(mOffset.x, mOffset.y, 0);

    // Bind texture for character
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    ERR_CHECK();

    // Draw
    glVertexPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->x);
    ERR_CHECK();
    glTexCoordPointer(2, GL_FLOAT, sizeof(QuadData), &quadData->u);
    ERR_CHECK();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    ERR_CHECK();

    // Undo offset
    glTranslatef(-mOffset.x, -mOffset.y, 0);

    // Advance
    glTranslatef(mAdvance.x, mAdvance.y, 0);
}
