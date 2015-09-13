// File: prMathsUtil.h
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


#include "../core/prTypes.h"
#include <math.h>


// Namespaces
namespace Proteus {
namespace Math {


// Maths constants
const Proteus::Core::s32   MaxInt32    = 0xFFFFFFFF;                           // Maths constant
const Proteus::Core::f32   MinFloat    = -3.402823466e+38F;                    // Maths constant
const Proteus::Core::f32   MaxFloat    =  3.402823466e+38F;                    // Maths constant
const Proteus::Core::f32   Pi          = 3.14159265358979323846f;              // Maths constant
const Proteus::Core::f32   TwoPi       = Pi * 2;                               // Maths constant
const Proteus::Core::f32   PiHalf      = Pi / 2;                               // Maths constant
const Proteus::Core::f32   Epsilon     = 0.000001f;                            // Maths constant

#ifdef NAN
const Proteus::Core::f32   NaN         = NAN;
#else
const Proteus::Core::f32   NaN         = *(const Proteus::Core::f32*)&MaxInt32;// Maths constant
#endif


// Function: prMax    
//      Calculates the largest number.
template<typename T>
inline T prMax(T a, T b)
{
    return (a > b) ? a : b; 
}

// Function: prMin
//      Calculates the smallest number.
template<typename T>
inline T prMin(T a, T b)
{
    return (a < b) ? a : b;
}

// Function: prAbs
//      Calculates the absolute of a number.
template<typename T>
inline T prAbs(T a)
{
    return (a < 0) ? -a : a;
}

// Function: prClamp
//      Ensures a variable is between the minimum and maximum range specified.
template<typename T>
inline T prClamp(T var, T min, T max)
{
    return ((var) < (min) ? (min) : (var) > (max) ? (max) : (var));
}

// Function: prSwap
//      Swaps two numbers.
template<typename T>
inline void prSwap(T& a, T& b)
{
    T tmp = b;
    b     = a;
    a     = tmp;
}

// Function: prRadsToDegrees
//      Converts radians to degrees.
template<typename T>
void prRadsToDegrees(T& radians)
{
    radians *= (180.0f / Proteus::Math::Pi);
}

// Function: prDegreesToRads
//      Converts degrees to radians.
template<typename T>
void prDegreesToRads(T& degrees)
{
    degrees *= (Proteus::Math::Pi / 180.0f);
}

// Function: prFastMul10
//      Quickly multiplies an integer value by 10.
template<typename T>
inline T prFastMul10(T val)
{
    return ((val << 3) + (val << 1));
}

// Function: prWrap
//      Wraps a number between the min and max range.
template <typename T>
inline T prWrap(T f, T min, T max)
{ 
    return ((f >= min) && (f <= max)) ? f : (f > max ? min + (f - (max + 1)) : (max + 1) - (min - f));    
}

// Function: prRoundToInteger
//      Rounds to integer
inline int prRoundToInteger(double x)
{
    return (int)floor(x + 0.5);
}

// Function: prRoundToFloat
//      Round number n to d decimal points.
inline double prRoundToFloat(double n, unsigned int d)
{
    return floor(n * pow(10., (int)d) + .5) / pow(10., (int)d);
}

// Function: prNextPowerOf2
//      This function gets the first power of 2 >= the int that we pass it.
//      A minimum of 8 is ensured
inline int prNextPowerOf2(int a)
{
    // Ensure a minimum POT of 8
    a = prMax(a, 8);

	int rval = 1;
	
    while(rval < a)
    {
        rval <<= 1;
    }
	
    return rval;
}


// Function: prIsPowerOf2
//      This function tests if a number is power of 2
inline bool prIsPowerOf2(Proteus::Core::s32 size)
{
    bool result = false;
    
    for (Proteus::Core::s32 i = 1; i < 32; i++)
    {
        if (size == (1 << i))
        {
            result = true;
            break;
        }
    }
    
    return result;
}


// Function: prPercentageOfValue
//      Returns the specified percentage of a value.
Proteus::Core::s32 prPercentageOfValue(Proteus::Core::s32 percent, Proteus::Core::s32 value);

// Function: prPercentageOfTotal
//      Calculates 'amount's percentage in relation to 'total'. E.g. (50, 200) == 25% of total.
Proteus::Core::s32 prPercentageOfTotal(Proteus::Core::s32 amount, Proteus::Core::s32 total);

// Function: prRoundFloatDownToInt
//      Converts a float to its integer value without rounding up.
inline int prRoundFloatDownToInt(float var)
{
    int num = (int)var;
    if ((float)num > var)
    {
        num--;
    }
    return num;
}

// Function: prWorldToScreen
//      Converts a world position to a screen position.
void prWorldToScreen(Proteus::Core::f32 wx, Proteus::Core::f32 wy, Proteus::Core::f32 wz, Proteus::Core::f64 &ox, Proteus::Core::f64 &oy, Proteus::Core::f64 &oz);

// Function: prScreenToWorld
//      Converts a screen position to a world position.
void prScreenToWorld(Proteus::Core::s32 sx, Proteus::Core::s32 sy, Proteus::Core::f64 &ox, Proteus::Core::f64 &oy, Proteus::Core::f64 &oz);


}}// Namespaces


// Macro: IS_ZERO(x)
//      Tests if a number is near zero.
//
// Paramers:
//      x - Value to test
//
// Notes:
//      x should be a float/double
#define IS_ZERO(x)      ((x) > -Proteus::Math::Epsilon && (x) < Proteus::Math::Epsilon)
