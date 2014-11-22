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


#ifndef __PRVECTOR2_H
#define __PRVECTOR2_H


#include "../prConfig.h"


// Class: prVector2
//      Class designed to represent a 2D vector.
class prVector2
{
public:

    static const prVector2 Zero;        // Pre-made value
    static const prVector2 One;         // Pre-made value
    static const prVector2 UnitX;       // Pre-made value
    static const prVector2 UnitY;       // Pre-made value


public:
    // Method: prVector2
    //      Constructor.
    prVector2() : x (0.0f) , y (0.0f)    
    {}

    // Method: prVector2
    //      Parameterised constructor.
    //
    // Parameters:
    //      nx - X coordinate
    //      ny - Y coordinate
    prVector2(float nx, float ny) : x (nx) , y (ny)
    {}

    // Method: Magnitude
    //      Calculates the magnitude of the vector.
    float Magnitude() const;

    // Method: MagnitudeSquared
    //      Calculates the magnitude of the vector.
    float MagnitudeSquared() const;
    
    // Method: Normalize
    //      Normalise this vector.
    prVector2& Normalize();

    // Method: ToString
    //      Returns the vector as a string.
    const char *ToString() const;


    // -- Operators --
    

    // Operator +
    inline prVector2 operator + (const prVector2& rhs) const;
    inline prVector2 operator + (const float& rhs) const;

    // Operator -
    inline prVector2 operator - (const prVector2& rhs) const;
    inline prVector2 operator - (const float& rhs) const;

    // Operator *
    inline prVector2 operator * (const prVector2& rhs) const;
    inline prVector2 operator * (const float& rhs) const;

    // Operator /
    inline prVector2 operator / (const prVector2& rhs) const;
    inline prVector2 operator / (const float& rhs) const;

    // Operator +=
    inline prVector2& operator += (const prVector2& rhs);

    // Operator -=
    inline prVector2& operator -= (const prVector2& rhs);

    // Operator *=
    inline prVector2& operator *= (const float val);

    // Unary + operator.
    inline prVector2 operator + () const { return *this; }
    
    // Unary - operator.
    inline prVector2 operator - () const { return prVector2(-x, -y); }


public:


// PC warnings - warning C4201: nonstandard extension used : nameless struct/union
//             - will break if ansi compatibility is enabled.
#if defined(PLATFORM_PC)
#pragma warning( push )
#pragma warning( disable : 4201 )
#endif

    union
    {
        float element[2];
        
        struct
        {                   
            float x;
            float y;
        };
    };    

#if defined(PLATFORM_PC)
#pragma warning( pop )
#endif
};


#include "prVector2.inl"


#endif//__PRVECTOR2_H

