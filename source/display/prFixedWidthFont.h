/**
 * @file        prFixedWidthFont.h
 * @brief       Contains a fixed width font class. This font is a lot
 * @n           faster than the standard bitmap font.
 * @note        This font is intended for debug use as it only supports ASCII
 * @copyright   Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRFIXEDWIDTHFONT_H
#define __PRFIXEDWIDTHFONT_H


#include "../core/prTypes.h"


// Forward declarations
class prTexture;


/// Fixed width font class. This font is a lot
/// faster than the standard bitmap font.
class prFixedWidthFont
{
public:

    // Simple alignment
    enum
    {
        ALIGN_LEFT   = 0x00000000,
        ALIGN_RIGHT  = 0x00000001,
        //ALIGN_CENTER = 0x00000020,
        //ALIGN_CENTRE = ALIGN_CENTER,
    };


public:

    /// Ctor
    /// @param      filename - A texture to load
    /// @param      width    - Width of a character cell
    /// @param      height   - Height of a character  cell
    /// @param      offset   - Optional offset to bring cells closer.
    explicit prFixedWidthFont(const char *filename, u32 width, u32 height, u32 offset);

    /// Ctor
    /// @param      pTexture - A texture to load
    /// @param      width    - Width of a character cell
    /// @param      height   - Height of a character  cell
    /// @param      offset   - Optional offset to bring cells closer.
    explicit prFixedWidthFont(prTexture *pTexture, u32 width, u32 height, u32 offset);

    /// Dtor
    ~prFixedWidthFont();

    /// Draw text
    /// @param      x      - X coordinate
    /// @param      y      - Y coordinate
    /// @param      fmt    - Format string
    /// @param      ...    - Optional parameters
    void Draw(f32 x, f32 y, const char *fmt, ...);

    /// Added scale as the font can look small
    /// @param      scale    - A scale value
    void SetScale(f32 scale) { m_scale = scale; }

    /// Get the width of the passed string.
    /// @param      pText - The string to check
    /// @return     String width in pixels
    f32 PixelWidth(const char *pText) const;

    /// Set the text alignment.
    /// @param      alignment - The required alignment
    void SetAlignment(u32 alignment);


private:

    prTexture  *m_pTexture;
    u32         m_width;
    u32         m_height;
    u32         m_offset;
    s32         m_frameWidth;
    s32         m_frameHeight;
    s32         m_framesAcross;
    s32         m_framesDown;
    s32         m_framesTotal;
    s32         m_frame;
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
