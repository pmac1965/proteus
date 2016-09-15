// File: prFontBase.h
//      Base class for all font types, except the construction
//      font, which is going to be removed.
//
// Engine fonts:
//      <prBitmapFont> is a standard bitmap font.
//
//      <prTrueTypeFont> supports .ttf and .otf type fonts.
//
//      <prFixedWidthFont> is the fastest font type, normally used by debug code.
//
// See Also:
//      <prBitmapFont>
//
// See Also:
//      <prTrueTypeFont>
//
// See Also:
//      <prFixedWidthFont>
/**
 * Copyright 2016 Paul Michael McNab
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


#pragma once


class prFont
{
public:
    // Enum: prFontAlign
    //      Font text alignments
    //
    //  - ALIGN_LEFT
    //  - ALIGN_RIGHT
    //  - ALIGN_CENTER
    //  - ALIGN_CENTRE
    enum prFontAlign
    {
        ALIGN_LEFT   = 0x00000000,
        ALIGN_RIGHT  = 0x00000001,
        ALIGN_CENTER = 0x00000002,
        ALIGN_CENTRE = ALIGN_CENTER
    };

    // Enum: prFontType
    //      The font type
    //
    //  - FONT_BMP (Bitmap font)
    //  - FONT_TTF (True type)
    //  - FONT_FIX (Fixed width bitmap)
    enum prFontType
    {
        FONT_BMP    = 0x00000000,
        FONT_TTF    = 0x00000001,
        FONT_FIX    = 0x00000002
    };


public:
    // Method: prFont
    //      Ctor
    //
    // Parameters:
    //      fontType - The font type
    //
    // See Also:
    //      <prFontType>
    explicit prFont(prFontType fontType) : mFontType(fontType)
    {}

    // Method: ~prFont
    //      Dtor
    ~prFont()
    {}

    // Method: GetFontType
    //      Gets a fonts type
    //
    // Returns:
    //      A <prFontType> enum      
    //
    // See Also:
    //      <prFontType>
    prFontType GetFontType() const { return mFontType; }


private:
    prFontType  mFontType;
};