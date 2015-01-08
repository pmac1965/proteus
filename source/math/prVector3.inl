// ------------------------------------------------------------------------------------------------
//
// prVector3.inl
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
        

#include <math.h>


// Namespaces
namespace Proteus {
namespace Math {


// ------------------------------------------------------------------------------------------------
// Calculates the magnitude of a vector.
// ------------------------------------------------------------------------------------------------
inline f32 prVector3::Length() const
{
    return sqrt((x * x) + (y * y) + (z * z));
}


// ------------------------------------------------------------------------------------------------
// Calculates the magnitude of a vector squared.
// ------------------------------------------------------------------------------------------------
inline f32 prVector3::LengthSquared() const
{
    return ((x * x) + (y * y) + (z * z));
}


// ------------------------------------------------------------------------------------------------
// Normalise this vector.
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::Normalize()
{
    f32 mag = Length();

    if (mag > 0.0f)
    {
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }

    return *this;
}


// ------------------------------------------------------------------------------------------------
// Returns the dot product.
// ------------------------------------------------------------------------------------------------
inline f32 prVector3::DotProduct(const prVector3 &v) const
{
    return ((x * v.x) +
            (y * v.y) +
            (z * v.z));
}


// ------------------------------------------------------------------------------------------------
// Returns the cross product.
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::CrossProduct(const prVector3 &v) const
{
    return prVector3(y * v.z - z * v.y,
                     z * v.x - x * v.z,
                     x * v.y - y * v.x);
}


// ------------------------------------------------------------------------------------------------
// Operator +
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator + (const prVector3& rhs) const
{
    return prVector3(x + rhs.x, y + rhs.y, z + rhs.z);
}

// Operator +
inline prVector3 prVector3::operator + (const f32& rhs) const
{
    return prVector3(x + rhs, y + rhs, z + rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator -
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator - (const prVector3& rhs) const
{
    return prVector3(x - rhs.x, y - rhs.y, z - rhs.z);
}

// Operator -
inline prVector3 prVector3::operator - (const f32& rhs) const
{
    return prVector3(x - rhs, y - rhs, z - rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator *
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator * (const prVector3& rhs) const
{
    return prVector3(x * rhs.x, y * rhs.y, z * rhs.z);
}

// Operator *
inline prVector3 prVector3::operator * (const f32& rhs) const
{
    return prVector3(x * rhs, y * rhs, z * rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator /
// ------------------------------------------------------------------------------------------------
inline prVector3 prVector3::operator / (const prVector3& rhs) const
{
    return prVector3(x / rhs.x, y / rhs.y, z / rhs.z);
}

// Operator /
inline prVector3 prVector3::operator / (const f32& rhs) const
{
    return prVector3(x / rhs, y / rhs, z / rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator +=
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::operator += (const prVector3& rhs)
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    
    return *this;       
}


// ------------------------------------------------------------------------------------------------
// Operator -=
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::operator -= (const prVector3& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    
    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator *=
// ------------------------------------------------------------------------------------------------
inline prVector3& prVector3::operator *= (const f32 val)
{
    x = x * val;
    y = y * val;
    z = z * val;

    return *this;    
}


}}// Namespaces
