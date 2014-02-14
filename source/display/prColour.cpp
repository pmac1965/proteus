/**
 * prColour.cpp
 */


#include "prColour.h"
#include "../debug/prDebug.h"


const prColour  prColour::White         (1.0f, 1.0f, 1.0f);
const prColour  prColour::Black         (   0,    0,    0);
const prColour  prColour::Red           (1.0f,    0,    0);
const prColour  prColour::Green         (   0, 1.0f,    0);
const prColour  prColour::Blue          (   0,    0, 1.0f);
const prColour  prColour::Yellow        (1.0f, 1.0f,    0);
const prColour  prColour::Cyan          (   0, 1.0f, 1.0f);
const prColour  prColour::Magenta       (1.0f,    0, 1.0f);

const prColour  prColour::DarkGray      (0.250f, 0.250f, 0.250f);
const prColour  prColour::Gray          (0.500f, 0.500f, 0.500f);
const prColour  prColour::LiteGray      (0.750f, 0.750f, 0.750f);


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
