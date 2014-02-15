// File: prFixedWidthFont.h
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


#ifndef __PRFIXEDWIDTHFONT_H
#define __PRFIXEDWIDTHFONT_H


#include "../core/prTypes.h"


// Forward declarations
class prTexture;


// Class: prFixedWidthFont
//      Fixed width font class. This font is a lot
//      faster than the standard bitmap font.
//
// Notes:
//      This font is intended for debug use as it only supports ASCII
class prFixedWidthFont
{
public:
    // Enum: prFixedWidthAlignment
    // Simple alignment
    //
    //      FW_ALIGN_LEFT   - Align left
    //      FW_ALIGN_RIGHT  - Align right
    //      FW_ALIGN_CENTER - Align center
    //      FW_ALIGN_CENTRE - Alternate center spelling
    enum prFixedWidthAlignment
    {
        FW_ALIGN_LEFT   = 0x00000000,
        FW_ALIGN_RIGHT  = 0x00000001,
        FW_ALIGN_CENTER = 0x00000020,
        FW_ALIGN_CENTRE = FW_ALIGN_CENTER,
    };


public:

    // Method: prFixedWidthFont
    //      Ctor
    //
    // Parameters:
    //      filename - A texture to load
    //      width    - Width of a character cell
    //      height   - Height of a character  cell
    //      offset   - Optional offset to bring cells closer.
    explicit prFixedWidthFont(const char *filename, u32 width, u32 height, u32 offset);

    // Method: prFixedWidthFont
    //      Ctor
    //
    // Parameters:
    //      pTexture - A texture to load
    //      width    - Width of a character cell
    //      height   - Height of a character  cell
    //      offset   - Optional offset to bring cells closer.
    explicit prFixedWidthFont(prTexture *pTexture, u32 width, u32 height, u32 offset);

    // Method: ~prFixedWidthFont
    //      Dtor
    ~prFixedWidthFont();

    // Method: Draw
    //      Draw text
    //
    // Parameters:
    //      x      - X coordinate
    //      y      - Y coordinate
    //      fmt    - Format string
    //      ...    - Optional parameters
    void Draw(f32 x, f32 y, const char *fmt, ...);

    // Method: SetScale
    //      Sets the font scale
    //
    // Parameters:
    //      scale    - A scale value
    void SetScale(f32 scale) { m_scale = scale; }

    // Method: PixelWidth
    //      Get the width of the passed string.
    //
    // Parameters:
    //      pText - The string to check
    //
    // Returns:
    //      String width in pixels
    f32 PixelWidth(const char *pText) const;

    // Method: SetAlignment
    //      Set the text alignment.
    //
    // Parameters:
    //      alignment - The required alignment
    //
    // See Also:
    //      <prFixedWidthAlignment>
    void SetAlignment(u32 alignment);

    // Method: GetFrameWidth
    //      Get the frame width
    u32 GetFrameWidth() const { return m_frameWidth; }

    // Method: GetFrameHeight
    //      Get the frame height
    u32 GetFrameHeight() const { return m_frameHeight; }

    // Method: GetFrameCount
    //      Get the frame count
    u32 GetFrameCount() const { return m_framesTotal; }


private:

    prTexture  *m_pTexture;
    u32         m_offset;
    s32         m_frameWidth;
    s32         m_frameHeight;
    s32         m_framesAcross;
    s32         m_framesDown;
    s32         m_framesTotal;
    f32         m_fw;
    f32         m_fh;
    f32         m_u0;
    f32         m_u1;
    f32         m_v0;
    f32         m_v1;
    f32         m_scale;
    u32         m_alignment;
};


#endif//__PRFIXEDWIDTHFONT_H
