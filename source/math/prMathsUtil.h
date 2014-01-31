/**
 * @file       prMathsUtil.h
 * @brief      Contains maths functions.
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


#ifndef __PRMATHSUTIL_H
#define __PRMATHSUTIL_H


#include "../core/prTypes.h"
#include <math.h>


// Maths constants
namespace Proteus 
{
    namespace Maths
    {
        const long  MaxInt32    = 0xFFFFFFFF;                           ///< Maths constant
        const float MinFloat    = -3.402823466e+38F;                    ///< Maths constant
        const float MaxFloat    =  3.402823466e+38F;                    ///< Maths constant
        const float Pi          = 3.14159265358979323846f;              ///< Maths constant
        const float TwoPi       = Pi * 2;                               ///< Maths constant
        const float PiHalf      = Pi / 2;                               ///< Maths constant
        const float Epsilon     = 0.000001f;                            ///< Maths constant
        const float NaN         = *(const float*)&MaxInt32;             ///< Maths constant
    };
};


/// @def IS_ZERO
/// Tests if a number is near zero.
#define IS_ZERO(x)      ((x) > -Proteus::Maths::Epsilon && (x) < Proteus::Maths::Epsilon)

            
/// @brief      Calculates the largest number.
template<typename T>
inline T prMax(T a, T b)
{
    return (a > b) ? a : b; 
}

/// @brief      Calculates the smallest number.
template<typename T>
inline T prMin(T a, T b)
{
    return (a < b) ? a : b;
}

/// @brief      Calculates the absolute of a number.
template<typename T>
inline T prAbs(T a)
{
    return (a < 0) ? -a : a;
}

/// @brief      Ensures a variable is between the minimum and maximum range specified.
template<typename T>
inline T prClamp(T var, T min, T max)
{
    return ((var) < (min) ? (min) : (var) > (max) ? (max) : (var));
}

/// @brief      Swaps two numbers.
template<typename T>
inline void prSwap(T& a, T& b)
{
    T tmp = b;
    b     = a;
    a     = tmp;
}

/// @brief      Converts radians to degrees.
template<typename T>
void prRadsToDegrees(T& radians)
{
    radians *= (180.0f / Proteus::Maths::Pi);
}

/// @brief      Converts degrees to radians.
template<typename T>
void prDegreesToRads(T& degrees)
{
    degrees *= (Proteus::Maths::Pi / 180.0f);
}

/// @brief      Quickly multiplies an integer value by 10.
template<typename T>
inline T FastMul10(T val)
{
    return ((val << 3) + (val << 1));
}

/// @brief      Wraps a number between the min and max range.
template <typename T>
inline T prWrap(T f, T min, T max)
{ 
    return ((f >= min) && (f <= max)) ? f : (f > max ? min + (f - (max + 1)) : (max + 1) - (min - f));    
}

/// @brief      Rounds to integer
inline int prRoundToInteger(double x)
{
    return (int)floor(x + 0.5);
}

/// @brief      Round number n to d decimal points.
inline double prRoundToFloat(double n, unsigned int d)
{
    return floor(n * pow(10., (int)d) + .5) / pow(10., (int)d);
}

/// @brief      Returns the specified percentage of a value.
s32 prPercentageOfValue(s32 percent, s32 value);

/// @brief      Calculates 'amount's percentage in relation to 'total'. E.g. (50, 200) == 25% of total.
s32 prPercentageOfTotal(s32 amount, s32 total);

/// @brief      Converts a float to its integer value without rounding up.
inline int prRoundFloatDownToInt(float var)
{
    int num = (int)var;

    if ((float)num > var)
    {
        num--;
    }

    return num;
}

/// @brief      Converts a world position to a screen position.
void prWorldToScreen(f32 wx, f32 wy, f32 wz, f64 &ox, f64 &oy, f64 &oz);

/// @brief      Converts a screen position to a world position.
void prScreenToWorld(s32 sx, s32 sy, f64 &ox, f64 &oy, f64 &oz);


#endif//__PRMATHSUTIL_H
