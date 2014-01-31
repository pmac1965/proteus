/**
 * @file       prVector2.h
 * @brief      Contains a class designed to represent a 2D vector.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRVECTOR2_H
#define __PRVECTOR2_H


#include "../prConfig.h"


/// @brief      Class designed to represent a 2D vector.
class prVector2
{
public:

    static const prVector2 Zero;        ///< Pre-made value
    static const prVector2 One;         ///< Pre-made value
    static const prVector2 UnitX;       ///< Pre-made value
    static const prVector2 UnitY;       ///< Pre-made value


public:

    /// @brief     Constructor.
    prVector2() : x (0.0f) , y (0.0f)    
    {}

    /// @brief     Constructor.
    prVector2(float nx, float ny) : x (nx) , y (ny)
    {}

    /// @brief     Calculates the magnitude of the vector.
    float Magnitude() const;

    /// @brief     Calculates the magnitude of the vector squared.
    float MagnitudeSquared() const;
    
    /// @brief     Normalise this vector.
    prVector2& Normalize();

    /// @brief     Returns the vector as a string.
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

