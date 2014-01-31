/**
 * @file       prColour.h
 * @brief      Contains a class to represent a colour with
 * @n          red, green, blue and alpha properties.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
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


#ifndef __PRCOLOUR_H
#define __PRCOLOUR_H


#include "../core/prTypes.h"


/// @brief      A struct to represent RGBA colour
typedef struct prRGBA
{
    u8  r;      ///<    The red component
    u8  g;      ///<    The green component
    u8  b;      ///<    The blue component
    u8  a;      ///<    The alpha component

} prRGBA;


/// @brief      A class represent an RGBA colour
class prColour
{
public:

    /// @brief      Ctor
    prColour()
    {
        red   = 1.0f;
        green = 1.0f;
        blue  = 1.0f;
        alpha = 1.0f;
    }
    
    /// @brief      Ctor
    /// @note       Value range should be between 0.0f and 1.0f
    /// @param      r - The red component
    /// @param      g - The green component
    /// @param      b - The blue component
    /// @param      a - The alpha component
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


/// @brief      Switches RGBA to BGRA 
/// @param      pData - A pointer to the colour data
/// @param      size  - Size of the colour data
void prSwitchRGBAToBGRA(u8 *pData, u32 size);


#endif//__PRCOLOUR_H
