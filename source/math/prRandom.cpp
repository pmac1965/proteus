/**
 * prRandom.cpp
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


#include "../prConfig.h"
#include "prRandom.h"
#include "prMathsUtil.h"


static u64 seed = 0x12345678ABCDEF11LL;


/// ---------------------------------------------------------------------------
/// Sets a new random number seed.
/// ---------------------------------------------------------------------------
void prRandomSetSeed(u32 newSeed)
{
    if (0 == newSeed)
    {
        newSeed = 0x12345678;
    }

    seed = newSeed;
}


/// ---------------------------------------------------------------------------
/// Returns a random number between 0 and max - 1.
/// ---------------------------------------------------------------------------
u32 prRandomNumber(u32 max)
{
    // Seed cannot be zero, ever.
    if (seed == 0)
    {
        seed = 0x12345678ABCDEF11LL;
    }

    // Max must not be zero. It would cause a division by zero error.
    if (max > 0)
    {
        seed  *= 0x12AB89CC;
        seed >>= 16;

        seed *= (seed & 1) ? 0xABCDEF89 : 0xA1B2C3D4;
        seed >>= 16;

        seed *= (seed & 8) ? 0x1234FEDC : 0x2F7A9BC3;
        seed >>= 16;

        return (u32)(seed % max);
    }

    return 0;
}


/// ---------------------------------------------------------------------------
/// Returns a random number between min and max.
/// ---------------------------------------------------------------------------
s32 prRandomNumber(s32 min, s32 max)
{
    if (min > max)
    {
        prSwap(min, max);
    }

    // Needs to be incremented to make the range work.
    max++;

    return min + prRandomNumber(prAbs(max - min));
}


/// ---------------------------------------------------------------------------
/// Returns a random number similar to the C standard lib rand() function.
/// ---------------------------------------------------------------------------
s32 prRand()
{
    return (s32)prRandomNumber(0x7FFF);
}
