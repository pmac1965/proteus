/**
 * prMatrix4.cpp
 *
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


#include "prMatrix4.h"
#include "prVector3.h"


// Namespaces
namespace Proteus {
namespace Math {


const f32 DEG2RAD = 3.141593f / 180;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMatrix4::prMatrix4()
{
    Identity();
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMatrix4::prMatrix4(f32 m00, f32 m01, f32 m02, f32 m03,
                     f32 m04, f32 m05, f32 m06, f32 m07,
                     f32 m08, f32 m09, f32 m10, f32 m11,
                     f32 m12, f32 m13, f32 m14, f32 m15)
{
    _m[0]  = m00; _m[1]  = m01; _m[2]  = m02; _m[3]  = m03;
    _m[4]  = m04; _m[5]  = m05; _m[6]  = m06; _m[7]  = m07;
    _m[8]  = m08; _m[9]  = m09; _m[10] = m10; _m[11] = m11;
    _m[12] = m12; _m[13] = m13; _m[14] = m14; _m[15] = m15;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prMatrix4::~prMatrix4()
{
}


/// ---------------------------------------------------------------------------
/// Set the matrix data
/// ---------------------------------------------------------------------------
void prMatrix4::Set(const f32 data[16])
{
    _m[0]   = data[0];
    _m[1]   = data[1];
    _m[2]   = data[2];
    _m[3]   = data[3];
    _m[4]   = data[4];
    _m[5]   = data[5];
    _m[6]   = data[6];
    _m[7]   = data[7];
    _m[ 8]  = data[ 8];
    _m[ 9]  = data[ 9];
    _m[10]  = data[10];
    _m[11]  = data[11];
    _m[12]  = data[12];
    _m[13]  = data[13];
    _m[14]  = data[14];
    _m[15]  = data[15];
}


/// ---------------------------------------------------------------------------
/// Sets the matrix to all zero
/// ---------------------------------------------------------------------------
void prMatrix4::Zero()
{
    _m[0]  = 0.0f; _m[1]  = 0.0f; _m[2]  = 0.0f; _m[3]  = 0.0f;
    _m[4]  = 0.0f; _m[5]  = 0.0f; _m[6]  = 0.0f; _m[7]  = 0.0f;
    _m[8]  = 0.0f; _m[9]  = 0.0f; _m[10] = 0.0f; _m[11] = 0.0f;
    _m[12] = 0.0f; _m[13] = 0.0f; _m[14] = 0.0f; _m[15] = 0.0f;
}


/// ---------------------------------------------------------------------------
/// Sets the matrix to an identity matrix
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::Identity()
{
    _m[0]  = 1.0f; _m[1]  = 0.0f; _m[2]  = 0.0f; _m[3]  = 0.0f;
    _m[4]  = 0.0f; _m[5]  = 1.0f; _m[6]  = 0.0f; _m[7]  = 0.0f;
    _m[8]  = 0.0f; _m[9]  = 0.0f; _m[10] = 1.0f; _m[11] = 0.0f;
    _m[12] = 0.0f; _m[13] = 0.0f; _m[14] = 0.0f; _m[15] = 1.0f;

    return *this;
}


/// ---------------------------------------------------------------------------
/// Translate the matrix by x, y, z
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::Translate(f32 x, f32 y, f32 z)
{
    _m[0] += _m[3] * x;   _m[4] += _m[7] * x;   _m[8] += _m[11] * x;   _m[12] += _m[15] * x;
    _m[1] += _m[3] * y;   _m[5] += _m[7] * y;   _m[9] += _m[11] * y;   _m[13] += _m[15] * y;
    _m[2] += _m[3] * z;   _m[6] += _m[7] * z;   _m[10]+= _m[11] * z;   _m[14] += _m[15] * z;

    return *this;
}


/// ---------------------------------------------------------------------------
/// Translate the matrix by the supplied vector
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::Translate(prVector3& vec)
{
    return Translate(vec.x, vec.y, vec.z);
}


/// ---------------------------------------------------------------------------
/// Scale the matrix by x, y, z
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::Scale(f32 x, f32 y, f32 z)
{
    _m[0] *= x;   _m[4] *= x;   _m[8] *= x;   _m[12] *= x;
    _m[1] *= y;   _m[5] *= y;   _m[9] *= y;   _m[13] *= y;
    _m[2] *= z;   _m[6] *= z;   _m[10]*= z;   _m[14] *= z;

    return *this;
}


/// ---------------------------------------------------------------------------
/// Scale the matrix by the supplied vector
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::Scale(prVector3& vec)
{
    return Scale(vec.x, vec.y, vec.z);
}


/// ---------------------------------------------------------------------------
/// Rotate the matrix by x, y, z
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::Rotate(f32 angle, f32 x, f32 y, f32 z)
{
    f32 c = cosf(angle * DEG2RAD);    // cosine
    f32 s = sinf(angle * DEG2RAD);    // sine
    
    f32 c1 = 1.0f - c;                // 1 - c

    f32 m0 = _m[0],  m4 = _m[4],  m8  = _m[8],  m12 = _m[12],
        m1 = _m[1],  m5 = _m[5],  m9  = _m[9],  m13 = _m[13],
        m2 = _m[2],  m6 = _m[6],  m10 = _m[10], m14 = _m[14];

    // Build rotation matrix
    f32 r0 = x * x * c1 + c;
    f32 r1 = x * y * c1 + z * s;
    f32 r2 = x * z * c1 - y * s;
    f32 r4 = x * y * c1 - z * s;
    f32 r5 = y * y * c1 + c;
    f32 r6 = y * z * c1 + x * s;
    f32 r8 = x * z * c1 + y * s;
    f32 r9 = y * z * c1 - x * s;
    f32 r10= z * z * c1 + c;

    // Multiply rotation matrix
    _m[0]  = r0 * m0  + r4 * m1  + r8  * m2;
    _m[1]  = r1 * m0  + r5 * m1  + r9  * m2;
    _m[2]  = r2 * m0  + r6 * m1  + r10 * m2;
    _m[4]  = r0 * m4  + r4 * m5  + r8  * m6;
    _m[5]  = r1 * m4  + r5 * m5  + r9  * m6;
    _m[6]  = r2 * m4  + r6 * m5  + r10 * m6;
    _m[8]  = r0 * m8  + r4 * m9  + r8  * m10;
    _m[9]  = r1 * m8  + r5 * m9  + r9  * m10;
    _m[10] = r2 * m8  + r6 * m9  + r10 * m10;
    _m[12] = r0 * m12 + r4 * m13 + r8  * m14;
    _m[13] = r1 * m12 + r5 * m13 + r9  * m14;
    _m[14] = r2 * m12 + r6 * m13 + r10 * m14;

    return *this;
}


/// ---------------------------------------------------------------------------
/// Rotate the matrix by the supplied vector
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::Rotate(f32 angle, prVector3& vec)
{
    return Rotate(angle, vec.x, vec.y, vec.z);
}


/// ---------------------------------------------------------------------------
/// Rotate the matrix by the supplied angle
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::RotateX(f32 angle)
{
    f32 c = cosf(angle * DEG2RAD);
    f32 s = sinf(angle * DEG2RAD);

    f32 m1  = _m[1],  m2  = _m[2],
        m5  = _m[5],  m6  = _m[6],
        m9  = _m[9],  m10 = _m[10],
        m13 = _m[13], m14 = _m[14];

    _m[1]  = m1  * c + m2  * -s;
    _m[2]  = m1  * s + m2  *  c;
    _m[5]  = m5  * c + m6  * -s;
    _m[6]  = m5  * s + m6  *  c;
    _m[9]  = m9  * c + m10 * -s;
    _m[10] = m9  * s + m10 *  c;
    _m[13] = m13 * c + m14 * -s;
    _m[14] = m13 * s + m14 *  c;

    return *this;
}


/// ---------------------------------------------------------------------------
/// Rotate the matrix by the supplied angle
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::RotateY(f32 angle)
{
    f32 c = cosf(angle * DEG2RAD);
    f32 s = sinf(angle * DEG2RAD);

    f32 m0  = _m[0],  m2  = _m[2],
        m4  = _m[4],  m6  = _m[6],
        m8  = _m[8],  m10 = _m[10],
        m12 = _m[12], m14 = _m[14];

    _m[0]  = m0  *  c + m2  * s;
    _m[2]  = m0  * -s + m2  * c;
    _m[4]  = m4  *  c + m6  * s;
    _m[6]  = m4  * -s + m6  * c;
    _m[8]  = m8  *  c + m10 * s;
    _m[10] = m8  * -s + m10 * c;
    _m[12] = m12 *  c + m14 * s;
    _m[14] = m12 * -s + m14 * c;

    return *this;
}


/// ---------------------------------------------------------------------------
/// Rotate the matrix by the supplied angle
/// ---------------------------------------------------------------------------
prMatrix4& prMatrix4::RotateZ(f32 angle)
{
    f32 c = cosf(angle * DEG2RAD);
    f32 s = sinf(angle * DEG2RAD);

    f32 m0  = _m[0],  m1  = _m[1],
        m4  = _m[4],  m5  = _m[5],
        m8  = _m[8],  m9  = _m[9],
        m12 = _m[12], m13 = _m[13];

    _m[0]  = m0  * c + m1  * -s;
    _m[1]  = m0  * s + m1  *  c;
    _m[4]  = m4  * c + m5  * -s;
    _m[5]  = m4  * s + m5  *  c;
    _m[8]  = m8  * c + m9  * -s;
    _m[9]  = m8  * s + m9  *  c;
    _m[12] = m12 * c + m13 * -s;
    _m[13] = m12 * s + m13 *  c;

    return *this;
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prMatrix4::Invert()
{
}


}}// Namespaces
