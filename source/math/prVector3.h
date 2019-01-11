// File: prVector3.h
//  A simple 3D vector class.
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
 *
 *  Modified 2018 to add missing operator=
 */


#pragma once


#include "../prConfig.h"
#include "../core/prTypes.h"
#include "prMathsUtil.h"


// Namespaces
namespace Proteus {
namespace Math {


// Class: prVector3
//      Class designed to represent a 3D vector.
class prVector3
{
public:
    static const prVector3 Zero;        // All components set to zero
    static const prVector3 One;         // All components set to one
    static const prVector3 UnitX;       // X set to one, the rest set to zero
    static const prVector3 UnitY;       // Y set to one, the rest set to zero
    static const prVector3 UnitZ;       // Z set to one, the rest set to zero


public:
#if defined(PROTEUS_OPTIMISE_NO_VECTOR3_INIT)
    // This optimized version does not initialise its members
    // *NOT* recommended, as this may unintended side effects.
    prVector3()
    {}
#else
    // Method: prVector3
    //      Constructor.
    prVector3() : x(0.0f), y(0.0f), z(0.0f)    
    {}
#endif

    // Method: prVector3
    //      Parameterised constructor.
    //
    // Parameters:
    //      nx - X coordinate
    //      ny - Y coordinate
    //      nz - Z coordinate
    prVector3(f32 nx, f32 ny, f32 nz) : x(nx), y(ny), z(nz)
    {}

    // Method: prVector3
    //      Copy constructor.
    //
    // Parameters:
    //      copy - The vector to copy
    prVector3(const prVector3& copy) : x(copy.x), y(copy.y), z(copy.z)
    {}

    // Method: Length
    //      Calculates the magnitude of the vector.
    f32 Length() const;

    // Method: LengthSquared
    //      Calculates the magnitude of the vector squared.
    f32 LengthSquared() const;
    
    // Method: Normalize
    //      Normalise this vector.
    prVector3& Normalize();
    
    // Method: DotProduct
    //      Returns the dot product.
    //
    // Notes:
    //      Must be done on two *unit* vectors
    //
    // Notes:
    //      The dot product is the angle between to *unit* vectors and is normally used in
    //      lighting claculations.
    //
    //      The returned angle is between -1 and 1 and is actually the cosine between
    //      the two vectors
    f32 DotProduct(const prVector3 &v) const;

    // Method: CrossProduct
    //      Returns the cross product.
    //
    // Notes:
    //      Must be done on two *unit* vectors
    //
    // Notes:
    //      The cross product is a vector perpendicular to the plane defined by the two
    //      supplied *unit* vectors.
    //
    //      The order of the *unit* vectors is important. If the *unit* vectors are 
    //      swapped the returned vector will point in the opposite direction
    prVector3 CrossProduct(const prVector3 &v) const;

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
    inline bool Equals(const prVector3 &other) const { return (IS_ZERO(x - other.x) && IS_ZERO(y - other.y) && IS_ZERO(z - other.z)); }


    // -- Operators --

    
    // Operator +
    inline prVector3 operator + (const prVector3& rhs) const;
    inline prVector3 operator + (const f32& rhs) const;

    // Operator -
    inline prVector3 operator - (const prVector3& rhs) const;
    inline prVector3 operator - (const f32& rhs) const;

    // Operator *
    inline prVector3 operator * (const prVector3& rhs) const;
    inline prVector3 operator * (const f32& rhs) const;

    // Operator /
    inline prVector3 operator / (const prVector3& rhs) const;
    inline prVector3 operator / (const f32& rhs) const;

    // Operator +=
    inline prVector3& operator += (const prVector3& rhs);

    // Operator -=
    inline prVector3& operator -= (const prVector3& rhs);

    // Operator *=
    inline prVector3& operator *= (const f32 val);

    // Unary + operator.
    inline prVector3 operator + () const { return *this; }
    
    // Unary - operator.
    inline prVector3 operator - () const { return prVector3(-x, -y, -z); }

    // Operator ==
    bool operator == (const prVector3& other) const { return  Equals(other); }

    // Operator !=
    bool operator != (const prVector3& other) const { return !Equals(other); }

    // Copy Assignment
    prVector3& operator=(const prVector3& v)
    {
        if (this == &v) { return (*this); }
        
        x = v.x;
        y = v.y;
        z = v.z;

        return *this;
    }


public:
// PC warnings - warning C4201: nonstandard extension used : nameless struct/union
//             - will break if ansi compatibility is enabled.
#if defined(PLATFORM_PC)
#pragma warning( push )
#pragma warning( disable : 4201 )
#endif

    union
    {
        f32 element[3];
        
        struct
        {                   
            f32 x;
            f32 y;
            f32 z;
        };
    };    

#if defined(PLATFORM_PC)
#pragma warning( pop )
#endif
};


}}// Namespaces


#include "prVector3.inl"
