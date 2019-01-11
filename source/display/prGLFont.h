// File: prGLFont.h
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


#include "../prConfig.h"


#if defined(PLATFORM_PC)


#include <windows.h>
#include "../core/prTypes.h"


// Class: prGLFont
//      Class which wraps the OpenGL font system.
//
// Notes:
//      Intended for PC only use during engine construction/update/testing
//      as its relatively slow, only writes in white and doesn't support
//      non ascii characters. So DON'T use this one
class prGLFont
{
public:
    // Method: prGLFont
    //      Constructor.
    //
    // Parameters:
    //      dc       - A windows device context
    //      fontSize - The required font size
    //      fontName - A font name like 'arial'
    prGLFont(HDC dc, s32 fontSize, const char *fontName);

    // Method: ~prGLFont
    //      Destructor.
    ~prGLFont();

    // Method: Draw
    //      Draws text to the display.
    //
    // Parameters:
    //      x    - X coordinate
    //      y    - Y coordinate
    //      text - The text to write
    void Draw(s32 x, s32 y, const char *text);

    // Method: Draw
    //      Draws text to the display.
    //
    // Parameters:
    //      x    - X coordinate
    //      y    - Y coordinate
    //      text - The text to write
    void Draw(float x, float y, const char *text);


private:

    // Sets othographic perspective.
    void SetOrthographicProjection(float w, float h);

    // Resets perpective.
    void ResetPerspectiveProjection();


private:

    u32 m_base;
    f32 m_red;
    f32 m_green;
    f32 m_blue;
};


#endif//PLATFORM_PC
