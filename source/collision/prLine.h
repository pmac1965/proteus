// File: prLine.h
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


#ifndef __PRLINE_H
#define __PRLINE_H


#include "../core/prTypes.h"
#include "../math/prPoint.h"


// Method: prLineIntersect
//      Determines the intersection point on an infinite line.
//
// Parameters:
//      x1 - Line 1 start point
//      y1 - Line 1 start point
//      x2 - Line 1 end point
//      y2 - Line 1 end point
//      x3 - Line 2 start point
//      y3 - Line 2 start point
//      x4 - Line 2 end point
//      y4 - Line 2 end point
//      p  - Receives intersection point if it exists
//
// Returns:
//      true if intersects, false otherwise
bool prLineIntersect(Proteus::Core::s32 x1, Proteus::Core::s32 y1, Proteus::Core::s32 x2, Proteus::Core::s32 y2, 
                     Proteus::Core::s32 x3, Proteus::Core::s32 y3, Proteus::Core::s32 x4, Proteus::Core::s32 y4, Proteus::Math::prPoint2F &p);


#endif//__PRLINE_H
