// File: prMacros.h
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


#include "prTypes.h"


// ------------------------------------------------------------------------------------------
// Range macros.
// ------------------------------------------------------------------------------------------

/// Macro: PRBETWEEN
///     Used to test if a variable is between the minimum and maximum range specified.
#define PRBETWEEN(var, min, max)   ((var) >= (min)  &&  (var) <= (max))

/// Macro:  PRCLAMP
///     Ensures a variable is between the minimum and maximum range specified.
///
/// Notes:
///     Doesn't work well with unsigned or floating point numbers
#define PRCLAMP(var, min, max)     ((var) < (min) ? (min) : (var) > (max) ? (max) : (var))


// ------------------------------------------------------------------------------------------
// DirectX macros.
// ------------------------------------------------------------------------------------------

/// @def PRSAFE_RELEASE
/// DirectX safe release macro.
#ifndef PRSAFE_RELEASE
#define PRSAFE_RELEASE(p) { if ((p)) { (p)->Release(); (p) = nullptr; } }
#endif


// ------------------------------------------------------------------------------------------
// Bit flag testing macros.
// ------------------------------------------------------------------------------------------

/// @def PRTEST_FLAG
/// Tests if a bit flag is set.
#define PRTEST_FLAG(flag, bit)      ((flag)  &  (bit))

/// @def PRSET_FLAG
/// Sets a bit flag.
#define PRSET_FLAG(flag, bit)       ((flag) |=  (bit))

/// @def PRCLEAR_FLAG
/// Clears a bit flag.
#define PRCLEAR_FLAG(flag, bit)     ((flag) &= ~(bit))

/// @def PRFLIP_FLAG
/// Flips a bit flag.
#define PRFLIP_FLAG(flag, bit)      ((flag) ^=  (bit))


// ------------------------------------------------------------------------------------------
// Text macros.
// ------------------------------------------------------------------------------------------

/// @def PRIS_WHITESPACE
/// Determines if a character is whitespace.
#define PRIS_WHITESPACE(c)    ((c) ==' ' || (c) == '\r' || (c) == '\n' || (c) == '\t')


// ------------------------------------------------------------------------------------------
// Miscellaneous macros.
// ------------------------------------------------------------------------------------------

/// @def PRSAFE_DELETE_ARRAY
/// Allows delete to be called only on valid pointers.
#define PRSAFE_DELETE_ARRAY(ptr)      if ((ptr) != nullptr) {delete [] (ptr); (ptr) = nullptr;}


/// @def PRSAFE_DELETE
/// Allows delete to be called only on valid pointers.
#define PRSAFE_DELETE(ptr)            if ((ptr) != nullptr) {delete (ptr); (ptr) = nullptr;}


/// @def PRSAFE_FREE
/// Allows free to be called only on valid pointers.
#define PRSAFE_FREE(ptr)              if ((ptr) != nullptr) {free((ptr)); (ptr) = nullptr;}


/// @def PRARRAY_SIZE
/// Used to find the size of an array.
#define PRARRAY_SIZE(a)               (sizeof((a)) / sizeof(*(a)))


/// @def PRUNUSED
/// Stops unused variables causing warnings.
#ifndef PRUNUSED
#define PRUNUSED(var)                 ((void)(var))
#endif


// ------------------------------------------------------------------------------------------
// Math macros
// ------------------------------------------------------------------------------------------

/// @def EPSILON
/// EPSILON
#define EPSILON     0.000001f

/// @def PRMAX
/// Calculates the largest number.
#ifndef PRMAX
#define PRMAX(a, b)               (((a) > (b)) ? (a) : (b))
#endif

/// @def PRMIN
/// Calculates the smallest number.
#ifndef PRMIN
#define PRMIN(a, b)               (((a) < (b)) ? (a) : (b))
#endif

/// @def PRMINMIN
/// Calculates the smallest number.
#define PRMINMIN(a,b,c)           ((a) < (b) ? PRMIN((a), (c)) : PRMIN((b), (c)))

/// @def PRMAXMAX
/// Calculates the largest number.
#define PRMAXMAX(a,b,c)           ((a) > (b) ? PRMAX((a), (c)) : PRMAX((b), (c)))

/// @def PRABS
/// Calculates the absolute of a number.
#ifndef PRABS
#define PRABS(a)                  (((a) < 0) ? -(a) : (a))
#endif

/// @def PRFABS
/// Calculates the absolute of a number.
#ifndef PRFABS
#define PRFABS(a)                  (((a) < 0.0f) ? -(a) : (a))
#endif

/// @def PRSAME_SIGNS
/// Determines if two numbers have the same sign.
#define PRSAME_SIGNS(a, b)        (((s32) ((u32)a ^ (u32)b)) >= 0)

/// @def PRCUBE
/// Returns a number cubed.
#define PRCUBE(x)                 ((x) * (x) * (x))

/// @def PRSQR
/// Returns a number squared.
#define PRSQR(x)                  ((x) * (x))

/// @def PRDOUBLE
/// Returns the double of a number.
#define PRDOUBLE(x)               ((x) + (x))

/// @def PRSIGN
/// Returns the sign of a number.
#define PRSIGN(a)                 (((a) < 0) ? -1 : 1)

/// @def PRNUMBITS
/// Returns the number of bits for a type.
#define PRNUMBITS(a)              (sizeof(a) << 3)

/// @def PRLERP
/// Performs linear interpolation.
#define PRLERP(time, from, to)    ( (from) + ( (time) * ((to) - (from)) ) )

/// @def PI
/// PI
#define PI                        3.14159265358979323846f

/// @def PRRAD2DEG
/// Converts radians to degrees.
#define PRRAD2DEG(radians)        ((radians) * (180.0f / PI))

/// @def PRDEG2RAD
/// Converts degrees to radians.
#define PRDEG2RAD(degrees)        ((degrees) * (PI / 180.0f))

/// @def PRSWAP
/// Swaps two numbers.
#define PRSWAP(a, b) \
{                    \
    u64 tmp = b;     \
    b       = a;     \
    a       = tmp;   \
}

/// @def PRFLOOR
/// Gems - Map 't' to the default integer
#define PRFLOOR(t)                ((t) < 0 ? (int) ((t)-1.0) : (int) (t))

/// @def PRROUND
/// Gems - Map 't' to the nearest integer
#define PRROUND(t)                ((t) < 0 ? (int) ((t)-0.5) : (int) ((t)+0.5))

/// @def PRSIZE2
/// Determines if a number is divisible by 2.
#define PRSIZE2(num)              ((((u32)num) & 1)==0)

/// @def PRSIZE4
/// Determines if a number is divisible by 4.
#define PRSIZE4(num)              ((((u32)num) & 3)==0)


// ------------------------------------------------------------------------------------------
// Miscellaneous macros.
// ------------------------------------------------------------------------------------------

/// @def PRROUND_UP
/// Rounds a number up by the specified amount.
#define PRROUND_UP(number, amount)      (((u32)(number) + (amount) - 1)  &  ~((amount) - 1))

/// @def PRROUND_DOWN
/// Rounds a number down by the specified amount.
#define PRROUND_DOWN(number, amount)    ((u32)(number) & ~((amount)-1))

/// @def PREVEN
/// Determines if a number is even or odd.
#define PREVEN(num)                     ((((u32)num) & 1)==0)

/// @def PRALIGNMENT2
/// Determines if the alignment is at a u16 boundary.
#define PRALIGNMENT2(ptr)               ((((u32)ptr) & 1)==0)

/// @def PRALIGNMENT4
/// Determines if the alignment is at a u32 boundary.
#define PRALIGNMENT4(ptr)               ((((u32)ptr) & 3)==0)

/// @def PRSIZE2
/// Determines if a number is divisible by 2.
#define PRSIZE2(num)                    ((((u32)num) & 1)==0)

/// @def PRSIZE4
/// Determines if a number is divisible by 4.
#define PRSIZE4(num)                    ((((u32)num) & 3)==0)

/// @def PRBIT
/// Allows better looking bit setting code.
#define PRBIT(a)                        (1 << (a))

/// @def PRBOOL_TO_STRING
/// Converts a bool to a string.
#define PRBOOL_TO_STRING(b)             ((b) ? "true" : "false")

/// @def PRGB
/// Allows simple calculation of sizes.
#define PRGB(a)                         ((a) * 1024 * 1024 * 1024)

/// @def PRMB
/// Allows simple calculation of sizes.
#define PRMB(a)                         ((a) * 1024 * 1024)

/// @def PRKB
/// Allows simple calculation of sizes.
#define PRKB(a)                         ((a) * 1024)


// ------------------------------------------------------------------------------------------
// Allows for stringizing
// ------------------------------------------------------------------------------------------
/// @def PRSTRINGIZE
/// Allows for stringizing
#ifndef PRSTRINGIZE
#define PRSTRINGIZE(L)                    #L 
#endif

/// @def PRMAKESTRING
/// Allows for stringizing
#ifndef PRMAKESTRING
#define PRMAKESTRING(M, L)                M(L) 
#endif

/// @def PRCURRLINE
/// Allows for stringizing
#ifndef PRCURRLINE
#define PRCURRLINE                        PRMAKESTRING(PRSTRINGIZE, __LINE__) 
#endif

/// @def PRFILE_LINE
/// Allows for stringizing
#ifndef PRFILE_LINE
#define PRFILE_LINE                       __FILE__ " (Line: " PRCURRLINE ")\n" 
#endif


// ------------------------------------------------------------------------------------------
// Miscellaneous PC only macros.
// ------------------------------------------------------------------------------------------
/// @def PRKEY_DOWN
/// Simple key down macro.
#define PRKEY_DOWN(vk_code)             ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
