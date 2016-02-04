// File: prFixedPoint.h
//      Contains code to support C style fixed point math.
/**
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


#pragma once


#include "../core/prTypes.h"


#define FIXED_PRECISION         8
#define FIXED_ONE               (1 << FIXED_PRECISION)


// Macro: S32_TO_FIXED
//      Converts an integer to a fixed point number.
#define S32_TO_FIXED( num )     ((num) << FIXED_PRECISION)

// Macro: FIXED_TO_S32
//      Converts a fx32 fixed point number to an integer.
#define FIXED_TO_S32( num )     ((num) >> FIXED_PRECISION)

// Macro: F32_TO_FIXED
//      Converts a float to a fixed point number.
#define F32_TO_FIXED( num )     ( (fx32) ((num) * FIXED_ONE) )

// Macro: F32_TO_FIXED
//      Converts a fixed point number to a float.
#define FIXED_TO_F32( num )     ( ((f32)(num) / FIXED_ONE) )

// Macro: FIXED_MUL
//      Multiplies a fixed point number.
#define FIXED_MUL(var, mul)     (((var) * (mul)) >> FIXED_PRECISION)

// Macro: FIXED_DIV
//      Divides a fixed point number.
#define FIXED_DIV(var, div)     (((var) << FIXED_PRECISION) / (div))
