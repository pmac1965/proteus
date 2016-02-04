// File: prVector2.h
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


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "prMathsUtil.h"


// Namespaces
namespace Proteus {
namespace Math {


// Class: prVector2
//      Class designed to represent a 2D vector.
class prVector2
{
public:
    static const prVector2 Zero;        // All components set to zero
    static const prVector2 One;         // All components set to one
    static const prVector2 MinusOne;    // All components set to minus one
    static const prVector2 UnitX;       // X set to one, the rest set to zero
    static const prVector2 UnitY;       // Y set to one, the rest set to zero


public:
#if defined(PROTEUS_OPTIMISE_NO_VECTOR2_INIT)
    // This optimized version does not initialise its members
    prVector2()
    {}
#else
    // Method: prVector2
    //      Constructor.
    prVector2() : x(0.0f), y(0.0f)    
    {}
#endif

    // Method: prVector2
    //      Parameterised constructor.
    //
    // Parameters:
    //      nx - X coordinate
    //      ny - Y coordinate
    prVector2(Proteus::Core::f32 nx, Proteus::Core::f32 ny) : x(nx), y(ny)
    {}

    // Method: prVector2
    //      Copy constructor.
    //
    // Parameters:
    //      copy - The vector to copy
    prVector2(const prVector2& copy) : x(copy.x), y(copy.y)
    {}

    // Method: Length
    //      Calculates the magnitude of the vector.
    Proteus::Core::f32 Length() const;

    // Method: LengthSquared
    //      Calculates the magnitude of the vector squared.
    Proteus::Core::f32 LengthSquared() const;
    
    // Method: Normalize
    //      Normalise this vector.
    prVector2& Normalize();

    // Method: ToString
    //      Returns the vector as a string.
    const char *ToString() const;

    // Method: Equals
    //      Tests for equality with another vector
    //
    // Parameters:
    //      other - The vector to compare
    //
    // Notes:
    //      The difference between the vectors components must be less than Proteus::Math::Epsilon
    inline bool Equals(const prVector2 &other) const { return (IS_ZERO(x - other.x) && IS_ZERO(y - other.y)); }


    // -- Operators --
    

    // Operator +
    inline prVector2 operator + (const prVector2& rhs) const;
    inline prVector2 operator + (const Proteus::Core::f32& rhs) const;

    // Operator -
    inline prVector2 operator - (const prVector2& rhs) const;
    inline prVector2 operator - (const Proteus::Core::f32& rhs) const;

    // Operator *
    inline prVector2 operator * (const prVector2& rhs) const;
    inline prVector2 operator * (const Proteus::Core::f32& rhs) const;

    // Operator /
    inline prVector2 operator / (const prVector2& rhs) const;
    inline prVector2 operator / (const Proteus::Core::f32& rhs) const;

    // Operator +=
    inline prVector2& operator += (const prVector2& rhs);

    // Operator -=
    inline prVector2& operator -= (const prVector2& rhs);

    // Operator *=
    inline prVector2& operator *= (const Proteus::Core::f32 val);

    // Unary + operator.
    inline prVector2 operator + () const { return *this; }
    
    // Unary - operator.
    inline prVector2 operator - () const { return prVector2(-x, -y); }

    // Operator ==
    bool operator == (const prVector2& other) const { return  Equals(other); }

    // Operator !=
    bool operator != (const prVector2& other) const { return !Equals(other); }


public:
// PC warnings - warning C4201: nonstandard extension used : nameless struct/union
//             - will break if ansi compatibility is enabled.
#if defined(PLATFORM_PC)
#pragma warning( push )
#pragma warning( disable : 4201 )
#endif

    union
    {
        Proteus::Core::f32 element[2];
        
        struct
        {                   
            Proteus::Core::f32 x;
            Proteus::Core::f32 y;
        };
    };    

#if defined(PLATFORM_PC)
#pragma warning( pop )
#endif
};


}}// Namespaces


#include "prVector2.inl"
