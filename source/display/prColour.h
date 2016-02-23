// File: prColour.h
//      A class represent an RGBA colour
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


#pragma once


#include "../core/prTypes.h"
#include "../core/prMacros.h"


// Typedef: prRGBA
//      A struct to represent RGBA colour
typedef struct prRGBA
{
    Proteus::Core::u8  r;
    Proteus::Core::u8  g;
    Proteus::Core::u8  b;
    Proteus::Core::u8  a;

} prRGBA;


// Value to get floats from bytes
#define RGB_ELE     (1.0f / 255.0f)


// Class: prColour
//      A class represent an RGBA colour
class prColour
{
public:
    // Method: prColour
    //      Plain constructor
    prColour() : red(1.0f), green(1.0f), blue(1.0f), alpha(1.0f)
    {}
    
    // Method: prColour
    //      Float constructor
    //
    // Notes:
    //      Value range should be between 0.0f and 1.0f, but is clamped
    //      to ensure compliance
    //
    // Parameters:
    //      r - The red component
    //      g - The green component
    //      b - The blue component
    //      a - The alpha component
    prColour(Proteus::Core::f32 r, Proteus::Core::f32 g, Proteus::Core::f32 b, Proteus::Core::f32 a = 1.0f)
    {
        red   = PRCLAMP(r, 0.0f, 1.0f);
        green = PRCLAMP(g, 0.0f, 1.0f);
        blue  = PRCLAMP(b, 0.0f, 1.0f);
        alpha = PRCLAMP(a, 0.0f, 1.0f);
    }
    
    // Method: prColour
    //      Byte constructor
    //
    // Notes:
    //      Value range is between 0 and 255 and is clamped by virtue of data size
    //
    // Parameters:
    //      r - The red component
    //      g - The green component
    //      b - The blue component
    //      a - The alpha component
    prColour(Proteus::Core::u8 r, Proteus::Core::u8 g, Proteus::Core::u8 b, Proteus::Core::u8 a = 255)
    {
        red   = RGB_ELE * r;
        green = RGB_ELE * g;
        blue  = RGB_ELE * b;
        alpha = RGB_ELE * a;
    }
    
    // Method: prColour
    //      s32 constructor
    //
    // Notes:
    //      Value range is between 0 and 255, but it is clamped
    //      to ensure compliance
    //
    // Parameters:
    //      r - The red component
    //      g - The green component
    //      b - The blue component
    //      a - The alpha component
    prColour(Proteus::Core::s32 r, Proteus::Core::s32 g, Proteus::Core::s32 b, Proteus::Core::s32 a = 255)
    {
        red   = RGB_ELE * (r & 255);
        green = RGB_ELE * (g & 255);
        blue  = RGB_ELE * (b & 255);
        alpha = RGB_ELE * (a & 255);
    }

    // Method: prColour
    //      Copy constructor.
    prColour(const prColour& colour) : red(colour.red), green(colour.green), blue(colour.blue), alpha(colour.alpha)
    {}

    // Method: RGBA
    //      Converts to the RGBA colour format
    Proteus::Core::u32 RGBA() const;
    
    // Method: BGRA
    //      Converts to the BGRA colour format
    Proteus::Core::u32 BGRA() const;


    // ------------------------------------------------------------------------


    // Operator: +
    //      Operator +
    inline prColour operator + (const prColour& rhs) const;

    // Operator: -
    //      Operator -
    inline prColour operator - (const prColour& rhs) const;


public:
    float   red;
    float   green;
    float   blue;
    float   alpha;    


public:
    static const prColour     White;        ///< Pre-created colour value
    static const prColour     Black;        ///< Pre-created colour value
    static const prColour     Red;          ///< Pre-created colour value
    static const prColour     Green;        ///< Pre-created colour value
    static const prColour     Blue;         ///< Pre-created colour value
    static const prColour     Yellow;       ///< Pre-created colour value
    static const prColour     Cyan;         ///< Pre-created colour value
    static const prColour     Magenta;      ///< Pre-created colour value
    static const prColour     DarkGray;     ///< Pre-created colour value
    static const prColour     Gray;         ///< Pre-created colour value
    static const prColour     LiteGray;     ///< Pre-created colour value
    static const prColour     Brown;        ///< Pre-created colour value
    static const prColour     Orange;       ///< Pre-created colour value
    static const prColour     PastelPink;   ///< Pre-created colour value
    static const prColour     BarneyPurple; ///< Pre-created colour value
};


// Operator +
inline prColour prColour::operator + (const prColour& rhs) const
{
    return prColour(red + rhs.red, green + rhs.green, blue + rhs.blue, alpha + rhs.alpha);
}

// Operator -
inline prColour prColour::operator - (const prColour& rhs) const
{
    return prColour(red - rhs.red, green - rhs.green, blue - rhs.blue, alpha - rhs.alpha);
}


// Function: prSwitchRGBAToBGRA
//      Switches RGBA to BGRA 
//
// Parameters:
//      pData - A pointer to the colour data
//      size  - Size of the colour data
void prSwitchRGBAToBGRA(Proteus::Core::u8 *pData, Proteus::Core::u32 size);
