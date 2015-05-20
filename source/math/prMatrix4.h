// File: prMatrix4.h
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


#ifndef __PRMATRIX4_H
#define __PRMATRIX4_H


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace Math {


// Forward declarations
class prVector3;


// Class: prMatrix4
//      Represents a 4x4 matrix
class prMatrix4
{
public:
    // Method: prMatrix4
    //      Default constructor. Creates an identity matrix
    prMatrix4();

    // Method: prMatrix4
    //      Constructor
    prMatrix4(Proteus::Core::f32 m00, Proteus::Core::f32 m01, Proteus::Core::f32 m02, Proteus::Core::f32 m03,
              Proteus::Core::f32 m04, Proteus::Core::f32 m05, Proteus::Core::f32 m06, Proteus::Core::f32 m07,
              Proteus::Core::f32 m08, Proteus::Core::f32 m09, Proteus::Core::f32 m10, Proteus::Core::f32 m11,
              Proteus::Core::f32 m12, Proteus::Core::f32 m13, Proteus::Core::f32 m14, Proteus::Core::f32 m15);

    // Method: ~prMatrix4
    //      Dtor
    ~prMatrix4();

    const char *ToString();

    // Method: Set
    //      Set the matrix data
    void Set(const Proteus::Core::f32 data[16]);

    // Method: Zero
    //      Sets the matrix to all zero
    void Zero();

    // Method: Identity
    //      Sets the matrix to an identity matrix
    prMatrix4& Identity();

    // Method: Translate
    //      Translate the matrix by x, y, z
    prMatrix4& Translate(Proteus::Core::f32 x, Proteus::Core::f32 y, Proteus::Core::f32 z);

    // Method: Translate
    //      Translate the matrix by the supplied vector
    prMatrix4& Translate(prVector3& vec);

    // Method: Scale
    //      Scale the matrix by x, y, z
    prMatrix4& Scale(Proteus::Core::f32 x, Proteus::Core::f32 y, Proteus::Core::f32 z);

    // Method: Scale
    //      Scale the matrix by the supplied vector
    prMatrix4& Scale(prVector3& vec);

    // Method: Rotate
    //      Rotate the matrix by x, y, z
    prMatrix4& Rotate(Proteus::Core::f32 angle, Proteus::Core::f32 x, Proteus::Core::f32 y, Proteus::Core::f32 z);

    // Method: Rotate
    //      Rotate the matrix by the supplied vector
    prMatrix4& Rotate(Proteus::Core::f32 angle, prVector3& vec);

    // Method: RotateX
    //      Rotate the matrix by the supplied angle
    prMatrix4& RotateX(Proteus::Core::f32 angle);

    // Method: RotateY
    //      Rotate the matrix by the supplied angle
    prMatrix4& RotateY(Proteus::Core::f32 angle);

    // Method: RotateZ
    //      Rotate the matrix by the supplied angle
    prMatrix4& RotateZ(Proteus::Core::f32 angle);

    //
    void Invert();


    // -- Operators --


    // Operator +
    inline prMatrix4 operator+(const prMatrix4& rhs) const;

    // Operator -
    inline prMatrix4 operator-(const prMatrix4& rhs) const;
    inline prMatrix4 operator-(const prMatrix4& rhs);

    // Operator +=
    inline prMatrix4& operator+=(const prMatrix4& rhs);

    // Operator -=
    inline prMatrix4& operator-=(const prMatrix4& rhs);

    // Operator *
    inline prMatrix4 operator*(const prMatrix4& n) const;

    // Operator *=
    inline prMatrix4& operator*=(const prMatrix4& rhs);

    // Operator ==
    inline bool operator==(const prMatrix4& n) const;

    // Operator !=
    inline bool operator!=(const prMatrix4& n) const;

    // Operator []
    inline Proteus::Core::f32 operator[](Proteus::Core::s32 index) const;

    // Operator []
    inline Proteus::Core::f32& operator[](Proteus::Core::s32 index);


private:
    Proteus::Core::f32 _m[16];
};


}}// Namespaces


// Inline code
#include "prMatrix4.inl"


#endif//__PRMATRIX4_H
