// ------------------------------------------------------------------------------------------------
//
// prMatrix.inl
//
//  Copyright 2014 Paul Michael McNab
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
// ------------------------------------------------------------------------------------------------


// Namespaces
namespace Proteus {
namespace Math {


// ------------------------------------------------------------------------------------------------
// Operator +
// ------------------------------------------------------------------------------------------------
inline prMatrix4 prMatrix4::operator+(const prMatrix4& rhs) const
{
    return prMatrix4(_m[0]  + rhs[0],  _m[1]  + rhs[1],  _m[2]  + rhs[2],  _m[3]  + rhs[3],
                     _m[4]  + rhs[4],  _m[5]  + rhs[5],  _m[6]  + rhs[6],  _m[7]  + rhs[7],
                     _m[8]  + rhs[8],  _m[9]  + rhs[9],  _m[10] + rhs[10], _m[11] + rhs[11],
                     _m[12] + rhs[12], _m[13] + rhs[13], _m[14] + rhs[14], _m[15] + rhs[15]);
}


// ------------------------------------------------------------------------------------------------
// Operator -
// ------------------------------------------------------------------------------------------------
inline prMatrix4 prMatrix4::operator-(const prMatrix4& rhs) const
{
    return prMatrix4(_m[0]  - rhs[0],  _m[1]  - rhs[1],  _m[2]  - rhs[2],  _m[3]  - rhs[3],
                     _m[4]  - rhs[4],  _m[5]  - rhs[5],  _m[6]  - rhs[6],  _m[7]  - rhs[7],
                     _m[8]  - rhs[8],  _m[9]  - rhs[9],  _m[10] - rhs[10], _m[11] - rhs[11],
                     _m[12] - rhs[12], _m[13] - rhs[13], _m[14] - rhs[14], _m[15] - rhs[15]);
}


// ------------------------------------------------------------------------------------------------
// Operator -
// ------------------------------------------------------------------------------------------------
inline prMatrix4 operator-(const prMatrix4& rhs)
{
    return prMatrix4(-rhs[0], -rhs[1], -rhs[2], -rhs[3], -rhs[4], -rhs[5], -rhs[6], -rhs[7], -rhs[8], -rhs[9], -rhs[10], -rhs[11], -rhs[12], -rhs[13], -rhs[14], -rhs[15]);
}


// ------------------------------------------------------------------------------------------------
// Operator +=
// ------------------------------------------------------------------------------------------------
inline prMatrix4& prMatrix4::operator+=(const prMatrix4& rhs)
{
    _m[0]  += rhs[0];   _m[1]  += rhs[1];   _m[2]  += rhs[2];   _m[3]  += rhs[3];
    _m[4]  += rhs[4];   _m[5]  += rhs[5];   _m[6]  += rhs[6];   _m[7]  += rhs[7];
    _m[8]  += rhs[8];   _m[9]  += rhs[9];   _m[10] += rhs[10];  _m[11] += rhs[11];
    _m[12] += rhs[12];  _m[13] += rhs[13];  _m[14] += rhs[14];  _m[15] += rhs[15];
    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator -=
// ------------------------------------------------------------------------------------------------
inline prMatrix4& prMatrix4::operator-=(const prMatrix4& rhs)
{
    _m[0]  -= rhs[0];   _m[1]  -= rhs[1];   _m[2]  -= rhs[2];   _m[3]  -= rhs[3];
    _m[4]  -= rhs[4];   _m[5]  -= rhs[5];   _m[6]  -= rhs[6];   _m[7]  -= rhs[7];
    _m[8]  -= rhs[8];   _m[9]  -= rhs[9];   _m[10] -= rhs[10];  _m[11] -= rhs[11];
    _m[12] -= rhs[12];  _m[13] -= rhs[13];  _m[14] -= rhs[14];  _m[15] -= rhs[15];
    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator *
// ------------------------------------------------------------------------------------------------
inline prMatrix4 prMatrix4::operator*(const prMatrix4& n) const
{
    return prMatrix4(_m[0] * n[0]  + _m[4] * n[1]  + _m[8]  * n[2]  + _m[12] * n[3],
                     _m[1] * n[0]  + _m[5] * n[1]  + _m[9]  * n[2]  + _m[13] * n[3],
                     _m[2] * n[0]  + _m[6] * n[1]  + _m[10] * n[2]  + _m[14] * n[3],
                     _m[3] * n[0]  + _m[7] * n[1]  + _m[11] * n[2]  + _m[15] * n[3],

                     _m[0] * n[4]  + _m[4] * n[5]  + _m[8]  * n[6]  + _m[12] * n[7],
                     _m[1] * n[4]  + _m[5] * n[5]  + _m[9]  * n[6]  + _m[13] * n[7],
                     _m[2] * n[4]  + _m[6] * n[5]  + _m[10] * n[6]  + _m[14] * n[7],
                     _m[3] * n[4]  + _m[7] * n[5]  + _m[11] * n[6]  + _m[15] * n[7],

                     _m[0] * n[8]  + _m[4] * n[9]  + _m[8]  * n[10] + _m[12] * n[11],
                     _m[1] * n[8]  + _m[5] * n[9]  + _m[9]  * n[10] + _m[13] * n[11],
                     _m[2] * n[8]  + _m[6] * n[9]  + _m[10] * n[10] + _m[14] * n[11],
                     _m[3] * n[8]  + _m[7] * n[9]  + _m[11] * n[10] + _m[15] * n[11],

                     _m[0] * n[12] + _m[4] * n[13] + _m[8]  * n[14] + _m[12] * n[15],
                     _m[1] * n[12] + _m[5] * n[13] + _m[9]  * n[14] + _m[13] * n[15],
                     _m[2] * n[12] + _m[6] * n[13] + _m[10] * n[14] + _m[14] * n[15],
                     _m[3] * n[12] + _m[7] * n[13] + _m[11] * n[14] + _m[15] * n[15]);
}


// ------------------------------------------------------------------------------------------------
// Operator *=
// ------------------------------------------------------------------------------------------------
inline prMatrix4& prMatrix4::operator*=(const prMatrix4& rhs)
{
    *this = *this * rhs;
    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator ==
// ------------------------------------------------------------------------------------------------
inline bool prMatrix4::operator==(const prMatrix4& n) const
{
    return (_m[0]  == n[0])  && (_m[1]  == n[1])  && (_m[2]  == n[2])  && (_m[3]  == n[3])  &&
           (_m[4]  == n[4])  && (_m[5]  == n[5])  && (_m[6]  == n[6])  && (_m[7]  == n[7])  &&
           (_m[8]  == n[8])  && (_m[9]  == n[9])  && (_m[10] == n[10]) && (_m[11] == n[11]) &&
           (_m[12] == n[12]) && (_m[13] == n[13]) && (_m[14] == n[14]) && (_m[15] == n[15]);
}


// ------------------------------------------------------------------------------------------------
// Operator !=
// ------------------------------------------------------------------------------------------------
inline bool prMatrix4::operator!=(const prMatrix4& n) const
{
    return (_m[0]  != n[0])  || (_m[1]  != n[1])  || (_m[2]  != n[2])  || (_m[3]  != n[3])  ||
           (_m[4]  != n[4])  || (_m[5]  != n[5])  || (_m[6]  != n[6])  || (_m[7]  != n[7])  ||
           (_m[8]  != n[8])  || (_m[9]  != n[9])  || (_m[10] != n[10]) || (_m[11] != n[11]) ||
           (_m[12] != n[12]) || (_m[13] != n[13]) || (_m[14] != n[14]) || (_m[15] != n[15]);
}


// ------------------------------------------------------------------------------------------------
// Operator []
// ------------------------------------------------------------------------------------------------
inline f32 prMatrix4::operator[](s32 index) const
{
    return _m[index];
}


// ------------------------------------------------------------------------------------------------
// Operator []
// ------------------------------------------------------------------------------------------------
inline f32& prMatrix4::operator[](s32 index)
{
    return _m[index];
}


}}// Namespaces
