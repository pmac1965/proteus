/**
 * prColour.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "prColour.h"
#include "../debug/prDebug.h"


using namespace Proteus::Core;


const prColour  prColour::White         (1.0f,   1.0f,   1.0f);
const prColour  prColour::Black         (0.0f,   0.0f,   0.0f);
const prColour  prColour::Red           (1.0f,   0.0f,   0.0f);
const prColour  prColour::Green         (0.0f,   1.0f,   0.0f);
const prColour  prColour::Blue          (0.0f,   0.0f,   1.0f);
const prColour  prColour::Yellow        (1.0f,   1.0f,   0.0f);
const prColour  prColour::Cyan          (0.0f,   1.0f,   1.0f);
const prColour  prColour::Magenta       (1.0f,   0.0f,   1.0f);
const prColour  prColour::DarkGray      (0.250f, 0.250f, 0.250f);
const prColour  prColour::Gray          (0.500f, 0.500f, 0.500f);
const prColour  prColour::LiteGray      (0.750f, 0.750f, 0.750f);
const prColour  prColour::Brown         (0.60f,  0.40f,  0.12f);
const prColour  prColour::Orange        (0.98f,  0.625f, 0.12f);
const prColour  prColour::PastelPink    (0.98f,  0.04f,  0.7f);
const prColour  prColour::BarneyPurple  (0.60f,  0.40f,  0.70f);


/// ---------------------------------------------------------------------------
/// Converts to the RGBA colour format
/// ---------------------------------------------------------------------------
u32 prColour::RGBA() const
{
    u32 colour  = ((u32)red   * 255) << 24;
        colour |= ((u32)green * 255) << 16;
        colour |= ((u32)blue  * 255) << 8;
        colour |= ((u32)alpha * 255);

    return colour;
}


/// ---------------------------------------------------------------------------
/// Converts to the BGRA colour format
/// ---------------------------------------------------------------------------
u32 prColour::BGRA() const
{
    u32 colour  = ((u32)blue  * 255) << 24;
        colour |= ((u32)green * 255) << 16;
        colour |= ((u32)red   * 255) << 8;
        colour |= ((u32)alpha * 255);

    return colour;
}


/// ---------------------------------------------------------------------------
/// Switches RGBA to BGRA 
/// ---------------------------------------------------------------------------
void prSwitchRGBAToBGRA(u8 *pData, u32 size)
{
    // Add size check 
    PRASSERT(pData);
    PRASSERT(size > 0);
    PRASSERT(PRSIZE4(size));

    if (pData && size > 0 && PRSIZE4(size))
    {
        prRGBA *p = (prRGBA *)pData;

        for (; size > 0;)
        {
            u8 r = p->r;
            p->r = p->b;
            p->b = r;
        
            p++;
            size -= 4;
        }
    }
}
