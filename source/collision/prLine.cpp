// File: prLine.cpp
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


#include "prLine.h"
#include "../math/prMathsUtil.h"


using namespace Proteus::Math;


/// ---------------------------------------------------------------------------
/// Determines the intersection point on an infinite line.
/// ---------------------------------------------------------------------------
bool prLineIntersect(s32 x1, s32 y1, s32 x2, s32 y2, 
                     s32 x3, s32 y3, s32 x4, s32 y4, Proteus::Math::prPoint2F &p)
{
    s32 d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
    if (d == 0)
        return false;

    s32 xi = ((x3 - x4) * (x1 * y2 - y1 * x2) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
    s32 yi = ((y3 - y4) * (x1 * y2 - y1 * x2) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;

    if (xi < prMin(x1, x2) || xi > prMax(x1, x2))
        return false;

    if (xi < prMin(x3, x4) || xi > prMax(x3, x4))
        return false;

    p.x = (f32)xi;
    p.y = (f32)yi;

    return true;
}
