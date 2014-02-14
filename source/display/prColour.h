// File: prColour.h
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


#ifndef __PRCOLOUR_H
#define __PRCOLOUR_H


#include "../core/prTypes.h"


// Typedef: prRGBA
//      A struct to represent RGBA colour
typedef struct prRGBA
{
    u8  r;
    u8  g;
    u8  b;
    u8  a;

} prRGBA;


// Class: prColour
//      A class represent an RGBA colour
class prColour
{
public:
    // Method: prColour
    //      Ctor
    prColour()
    {
        red   = 1.0f;
        green = 1.0f;
        blue  = 1.0f;
        alpha = 1.0f;
    }
    
    // Method: prColour
    //      Ctor
    //
    // Notes:
    //      Value range should be between 0.0f and 1.0f
    //
    // Parameters:
    //      r - The red component
    //      g - The green component
    //      b - The blue component
    //      a - The alpha component
    prColour(f32 r, f32 g, f32 b, f32 a = 1.0f)
    {
        red   = r;
        green = g;
        blue  = b;
        alpha = a;
    }


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
};


// Function: prSwitchRGBAToBGRA
//      Switches RGBA to BGRA 
//
// Parameters:
//      pData - A pointer to the colour data
//      size  - Size of the colour data
void prSwitchRGBAToBGRA(u8 *pData, u32 size);


#endif//__PRCOLOUR_H
