// File: prTrueTypeFont.h
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


#ifndef __PRTRUETYPEFONT_H
#define __PRTRUETYPEFONT_H


#include "../core/prTypes.h"
#include "../math/prVector2.h"
#include "prColour.h"


// Forward declarations.
struct TrueTypeFontImplementation;


// Class: prTrueTypeFont
//      True tyoe font class.
class prTrueTypeFont
{
public:
    // Enum: prTrueTypeFontAlign
    //      Alignments
    //
    //  - ALIGN_LEFT
    //  - ALIGN_RIGHT
    //  - ALIGN_CENTER
    //  - ALIGN_CENTRE
    enum prTrueTypeFontAlign
    {
        ALIGN_LEFT   = 0x00000000,
        ALIGN_RIGHT  = 0x00000001,
        ALIGN_CENTER = 0x00000002,
        ALIGN_CENTRE = ALIGN_CENTER,
    };


public:
    // Method: prTrueTypeFont
    //      Ctor
    prTrueTypeFont();

    // Method: ~prTrueTypeFont
    //      Dtor
    ~prTrueTypeFont();

    // Method: Load
    //      Loads the font data.
    //
    // Parameters:
    //      filename - The font file to load
    //      height   - The fonts required height
    void Load(const char *filename, s32 height);

    // Method: Load
    //      Loads the font data.
    //
    // Notes:
    //      In order to save texture memory you can pass in a list of characters to
    //      create.
    //
    // Parameters:
    //      filename    - The font file to load
    //      height      - The fonts required height
    //      characters  - A list of characters to support
    void Load(const char *filename, s32 height, const char *characters);

    // Method: Draw
    //      Draws a string
    //
    // Parameters:
    //      x   - X position to draw at
    //      y   - Y position to draw at
    //      fmt - A format string
    //      ... - Optional parameters
    void Draw(f32 x, f32 y, const char *fmt, ...);

    // Method: Draw
    //      Draws a string
    //
    // Parameters:
    //      x         - X position to draw at
    //      y         - Y position to draw at
    //      scale     - Text scale
    //      colour    - Tint colour
    //      alignment - Text alignment
    //      fmt       -  A format string
    //      ...       - Optional parameters
    //
    // See Also:
    //      <prTrueTypeFontAlign>
    void Draw(f32 x, f32 y, float scale, prColour colour, s32 alignment, const char *fmt, ...);

    // Method: MeasureString
    //      Returns the pixel size of the string.
    //
    // Notes: 
    //      returns the strings total height and the widest line length if the string is multilined.
    Proteus::Math::prVector2 MeasureString(const char *string, float scale);


private:
    // Stops passing by value and assignment.
    prTrueTypeFont(const prTrueTypeFont&);
    const prTrueTypeFont& operator = (const prTrueTypeFont&);


private:
    // Don't change order.
	TrueTypeFontImplementation	*pImpl;
	TrueTypeFontImplementation	&imp;
};


#endif//__PRTRUETYPEFONT_H
