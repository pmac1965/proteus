// File: prFontGlyph.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __PRFONTGLYPH_H
#define __PRFONTGLYPH_H


#include "../core/prTypes.h"
#include "../math/prVector2.h"


// Class: prFontGlyph
//      Represents a glyph from a TTF font
class prFontGlyph
{
public:
    // Method: Ctor
    //      Constructor
    prFontGlyph(Proteus::Core::f32 advanceX, Proteus::Core::f32 advanceY,
                Proteus::Core::f32 offsetX,  Proteus::Core::f32 offsetY,
                Proteus::Core::u32 aChar,    Proteus::Core::u32 texID);

    // Method: Dtor
    //      Destructor
    ~prFontGlyph();

    // Method: SetTextureCoords
    //      Sets the texture coords for the character
    void SetTextureCoords(Proteus::Core::u32 index, Proteus::Core::f32 u, Proteus::Core::f32 v);

    // Method: SetVertexCoords
    //      Sets the vertex coords for the character
    void SetVertexCoords(Proteus::Core::u32 index, Proteus::Core::f32 x, Proteus::Core::f32 y);

    // Method: Draw
    //      Draws the character. This is called by prTrueTypeFont
    void Draw();


public:
    Proteus::Math::prVector2    mAdvance;               // Amount to advance character


private:
    Proteus::Math::prVector2    mOffset;                // X, Y positioning offset
    Proteus::Core::u32          mCharacter;             // This character
    Proteus::Core::u32          mTextureID;             // The texture ID
    Proteus::Core::QuadData     quadData[6];            // Data to draw a simple quad
};


#endif//__PRFONTGLYPH_H
