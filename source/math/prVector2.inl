// ------------------------------------------------------------------------------------------------
//
// prVector2.inl
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


// ------------------------------------------------------------------------------------------------
// Calculates the magnitude of a vector.
// ------------------------------------------------------------------------------------------------
inline float prVector2::Magnitude() const
{
    return sqrt((x * x) + (y * y));
}


// ------------------------------------------------------------------------------------------------
// Calculates the magnitude of a vector squared.
// ------------------------------------------------------------------------------------------------
inline float prVector2::MagnitudeSquared() const
{
    return ((x * x) + (y * y));
}


// ------------------------------------------------------------------------------------------------
// Normalise this vector.
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::Normalize()
{
    float mag = Magnitude();

    if (mag > 0.0f)
    {
        x = x / mag;
        y = y / mag;
    }

    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator +
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator + (const prVector2& rhs) const
{
    return prVector2(x + rhs.x, y + rhs.y);
}

// Operator +
inline prVector2 prVector2::operator + (const float& rhs) const
{
    return prVector2(x + rhs, y + rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator -
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator - (const prVector2& rhs) const
{
    return prVector2(x - rhs.x, y - rhs.y);
}

// Operator -
inline prVector2 prVector2::operator - (const float& rhs) const
{
    return prVector2(x - rhs, y - rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator *
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator * (const prVector2& rhs) const
{
    return prVector2(x * rhs.x, y * rhs.y);
}

// Operator *
inline prVector2 prVector2::operator * (const float& rhs) const
{
    return prVector2(x * rhs, y * rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator /
// ------------------------------------------------------------------------------------------------
inline prVector2 prVector2::operator / (const prVector2& rhs) const
{
    return prVector2(x / rhs.x, y / rhs.y);
}

// Operator /
inline prVector2 prVector2::operator / (const float& rhs) const
{
    return prVector2(x / rhs, y / rhs);
}


// ------------------------------------------------------------------------------------------------
// Operator +=
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::operator += (const prVector2& rhs)
{
    x += rhs.x;
    y += rhs.y;
    
    return *this;       
}


// ------------------------------------------------------------------------------------------------
// Operator -=
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::operator -= (const prVector2& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    
    return *this;
}


// ------------------------------------------------------------------------------------------------
// Operator *=
// ------------------------------------------------------------------------------------------------
inline prVector2& prVector2::operator *= (const float val)
{
    x = x * val;
    y = y * val;

    return *this;
}
