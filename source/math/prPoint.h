// File: prPoint.h
//  A simple group of point structs.
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
 *
 *  Updated Nov 2018 to allow removal of debug code.
 *  Updated Jan 2019 to add operators (+, -, +=, -=)
 */


#pragma once


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace Math {


#if (defined(DEBUG) || defined(_DEBUG))
    const Proteus::Core::s32 POINT_TOSTRING_BUFFERSIZE = 128;
#endif


// Struct: TPoint3
//      template for a simple 3D point.
template<typename T>
struct TPoint3
{
    static const TPoint3 Zero;        // All components set to zero
    static const TPoint3 One;         // All components set to one

    // Method: TPoint3
    //      Constructor.
    TPoint3() : x(0), y(0), z(0)
    {}

    // Method: TPoint3
    //      Parameterised constructor.
    TPoint3(T nx, T ny, T nz) : x(nx), y(ny), z(nz) 
    {}

    // Method: TPoint3
    //      Copy constructor.
    TPoint3(const TPoint3& p) : x(p.x), y(p.y), z(p.z)
    {}

    // Operator ==
    inline bool operator == (const TPoint3& other) const
    {
        return ((x == other.x) &&
                (y == other.y) &&
                (z == other.z));
    }

    // Operator !=
    inline bool operator != (const TPoint3& other) const
    {
        return !(*this == other);
    }
    
    // Operator +
    inline TPoint3& operator + (const TPoint3& rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        z = z + rhs.z;
        return *this;
    }

    // Operator -
    inline TPoint3& operator - (const TPoint3& rhs)
    {
        x = x - rhs.x;
        y = y - rhs.y;
        z = z - rhs.z;
        return *this;
    }

    // Operator +=
    inline void operator += (const TPoint3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
    }

    // Operator -=
    inline void operator -= (const TPoint3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
    }

#if (defined(DEBUG) || defined(_DEBUG))
    // Method: ToString
    //      Debug helper function.
    //
    // Notes:
    //      Method only exists in debug builds
    const char *ToString() const
    {
        static char buffer[POINT_TOSTRING_BUFFERSIZE] = { 0 };
        sprintf(buffer, "%f, %f, %f", x, y, z);
        return buffer;
    }
#endif

    // Members are public by default.
    T x, y, z;
};


// Struct: TPoint2
//      template for a simple 2D point.
template<typename T>
struct TPoint2
{
    static const TPoint2 Zero;        // All components set to zero
    static const TPoint2 One;         // All components set to one

    // Method: TPoint2
    //      Constructor.
    TPoint2() : x(0), y(0)
    {}

    // Method: TPoint2
    //      Parameterised constructor.
    TPoint2(T nx, T ny) : x(nx), y(ny)
    {}

    // Method: TPoint2
    //      Copy constructor.
    TPoint2(const TPoint2& p) : x(p.x), y(p.y)
    {}

    // Operator ==
    inline bool operator == (const TPoint2& other) const
    {
        return ((x == other.x) &&
                (y == other.y));
    }

    // Operator !=
    inline bool operator != (const TPoint2& other) const
    {
        return !(*this == other);
    }

    // Operator +
    inline TPoint2& operator + (const TPoint2& rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        return *this;
    }

    // Operator -
    inline TPoint2& operator - (const TPoint2& rhs)
    {
        x = x - rhs.x;
        y = y - rhs.y;
        return *this;
    }

    // Operator +=
    inline void operator += (const TPoint2& rhs)
    {
        x += rhs.x;
        y += rhs.y;
    }

    // Operator -=
    inline void operator -= (const TPoint2& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
    }

#if (defined(DEBUG) || defined(_DEBUG))
    // Method: ToString
    //      Debug helper function.
    //
    // Notes:
    //      Method only exists in debug builds
    const char *ToString() const
    {
        static char buffer[POINT_TOSTRING_BUFFERSIZE] = { 0 };
        sprintf(buffer, "%f, %f", x, y);
        return buffer;
    }
#endif

    // Members are public by default.
    T x, y;
};


// Typedef: prPoint3
//      Simple 3D point with integer x, y, z
typedef TPoint3<Proteus::Core::s32>     prPoint3;

// Typedef: prPoint3F
//      Simple 3D point with float x, y, z
typedef TPoint3<Proteus::Core::f32>     prPoint3F;

// Typedef: prPoint2
//      Simple 2D point with integer x, y
typedef TPoint2<Proteus::Core::s32>     prPoint2;

// Typedef: prPoint2F
//      Simple 2D point with float x, y
typedef TPoint2<Proteus::Core::f32>     prPoint2F;


}}// Namespaces


// Add inline file
//#include "prPoint.inl"
