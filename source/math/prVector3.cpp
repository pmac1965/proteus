/**
 * prVector3.cpp
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


#include <stdio.h>
#include "prVector3.h"


// Namespaces
namespace Proteus {
namespace Math {


// Maths constants 
const prVector3 prVector3 :: Zero   (0.0f, 0.0f, 0.0f);
const prVector3 prVector3 :: One    (1.0f, 1.0f, 1.0f);
const prVector3 prVector3 :: UnitX  (1.0f, 0.0f, 0.0f);
const prVector3 prVector3 :: UnitY  (0.0f, 1.0f, 0.0f);
const prVector3 prVector3 :: UnitZ  (0.0f, 0.0f, 1.0f);


/// ---------------------------------------------------------------------------
/// Returns the points data as a string.
/// ---------------------------------------------------------------------------
const char *prVector3::ToString() const
{
    static char buffer[256];

    sprintf(buffer, "%f, %f, %f", x, y, z);

    return buffer;
}


}}// Namespaces
