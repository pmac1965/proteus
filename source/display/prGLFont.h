/**
 * @file       prGLFont.h
 * @brief      Contains a basic font class.
 * @note       Intended for PC only used during engine construction/update/testing
 * @n          as its relatively slow.
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


#ifndef __PRGLFONT_H
#define __PRGLFONT_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <windows.h>
#include "../core/prTypes.h"


/// @brief      Class which wraps the OpenGL font system.
/// @note       Intended for PC only use during engine construction/update/testing
/// @n          as its relatively slow, only writes in white and doesn't support
/// @n          non ascii characters. So DON'T use this one
class prGLFont
{
public:

    /// @brief      Constructor.
    /// @param      dc       - A windows device context
    /// @param      fontSize - The required font size
    /// @param      fontName - A font name like 'arial'
    prGLFont(HDC dc, s32 fontSize, const char *fontName);

    /// @brief      Destructor.
    ~prGLFont();

    /// Draws text to the display.
    /// @param      x - X coordinate
    /// @param      y - Y coordinate
    /// @param      text - The text to write
    void Draw(s32 x, s32 y, const char *text);

    /// Draws text to the display.
    /// @param      x - X coordinate
    /// @param      y - Y coordinate
    /// @param      text - The text to write
    void Draw(float x, float y, const char *text);


private:

    /// Sets othographic perspective.
    /// @param      w - Width
    /// @param      h - Height
    void SetOrthographicProjection(float w, float h);

    /// Resets perpective.
    void ResetPerspectiveProjection();


private:

    u32 m_base;
};


#endif//PLATFORM_PC


#endif//__PRGLFONT_H
