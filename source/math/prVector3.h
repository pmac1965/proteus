// File: prVector2.h
/*
    Copyright 2014 Paul Michael McNab

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/


#ifndef __PRVECTOR3_H
#define __PRVECTOR3_H


#include "../prConfig.h"


// Class: prVector3
//      Class designed to represent a 3D vector.
class prVector3
{
public:

    static const prVector3 Zero;        // Pre-made value
    static const prVector3 One;         // Pre-made value
    static const prVector3 UnitX;       // Pre-made value
    static const prVector3 UnitY;       // Pre-made value
    static const prVector3 UnitZ;       // Pre-made value


public:
    // Method: prVector3
    //      Constructor.
    prVector3() : x (0.0f) , y (0.0f) , z (0.0f)    
    {
    }

    // Method: prVector3
    //      Parameterised constructor.
    //
    // Parameters:
    //      nx - X coordinate
    //      ny - Y coordinate
    //      nz - Z coordinate
    prVector3(float nx, float ny, float nz) : x (nx) , y (ny) , z (nz)
    {
    }

    // Method: Magnitude
    //      Calculates the magnitude of the vector.
    float Magnitude() const;

    // Method: MagnitudeSquared
    //      Calculates the magnitude of the vector.
    float MagnitudeSquared() const;
    
    // Method: Normalize
    //      Normalise this vector.
    prVector3& Normalize();
    
    // Method: DotProduct
    //      Returns the dot product.
    float DotProduct(const prVector3 &v) const;

    // Method: CrossProduct
    //      Returns the cross product.
    prVector3 CrossProduct(const prVector3 &v) const;

    // Method: ToString
    //      Returns the vector as a string.
    const char *ToString() const;


    // -- Operators --

    
    // Operator +
    inline prVector3 operator + (const prVector3& rhs) const;
    inline prVector3 operator + (const float& rhs) const;

    // Operator -
    inline prVector3 operator - (const prVector3& rhs) const;
    inline prVector3 operator - (const float& rhs) const;

    // Operator *
    inline prVector3 operator * (const prVector3& rhs) const;
    inline prVector3 operator * (const float& rhs) const;

    // Operator /
    inline prVector3 operator / (const prVector3& rhs) const;
    inline prVector3 operator / (const float& rhs) const;

    // Operator +=
    inline prVector3& operator += (const prVector3& rhs);

    // Operator -=
    inline prVector3& operator -= (const prVector3& rhs);

    // Operator *=
    inline prVector3& operator *= (const float val);

    // Unary + operator.
    inline prVector3 operator + () const { return *this; }
    
    // Unary - operator.
    inline prVector3 operator - () const { return prVector3(-x, -y, -z); }


public:

// PC warnings - warning C4201: nonstandard extension used : nameless struct/union
//             - will break if ansi compatibility is enabled.
#if defined(PLATFORM_PC)
#pragma warning( push )
#pragma warning( disable : 4201 )
#endif

    union
    {
        float element[3];
        
        struct
        {                   
            float x;
            float y;
            float z;
        };
    };    

#if defined(PLATFORM_PC)
#pragma warning( pop )
#endif
};


#include "prVector3.inl"


#endif//__PRVECTOR3_H
