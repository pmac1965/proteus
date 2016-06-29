// File: prPoint.h
//      Templated classes which represents a simple 2D and 3D points in space
//
// Notes:
//      Contains the pre-defined types
//          - prPoint3
//          - prPoint3F
//          - prPoint2
//          - prPoint2F
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


//#include "../core/prTypes.h"


// Class: TPoint3
//      template for a simple 3D point.
template<typename T>
struct TPoint3
{
public:
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


public:
    // Members are public for faster access.
    T x, y, z;
};


// Class: TPoint2
//      template for a simple 2D point.
template<typename T>
struct TPoint2
{
public:
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


public:
    // Members are public for faster access.
    T x, y;
};


// Namespaces
namespace Proteus {
namespace Math {


// Typedef: prPoint3
//      Simple 3D point with integer x, y, z
typedef TPoint3<Proteus::Core::s32>     prPoint3;

// Typedef: prPoint3F
//      Simple 3D point with float x, y, z
typedef TPoint3<Proteus::Core::f32>     prPoint3F;

// Typedef: prPoint2.
//      Simple 2D point with integer x, y
typedef TPoint2<Proteus::Core::s32>     prPoint2;

// Typedef: prPoint2F.
//      Simple 2D point with float x, y
typedef TPoint2<Proteus::Core::f32>     prPoint2F;


}}// Namespaces
