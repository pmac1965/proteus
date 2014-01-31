/**
 * @file       prVector3.h
 * @brief      Contains a class designed to represent a 3D vector.
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


#ifndef __PRVECTOR3_H
#define __PRVECTOR3_H


#include "../prConfig.h"


/// @brief      Class designed to represent a 3D vector.
class prVector3
{
public:

    static const prVector3 Zero;        ///< Pre-made value
    static const prVector3 One;         ///< Pre-made value
    static const prVector3 UnitX;       ///< Pre-made value
    static const prVector3 UnitY;       ///< Pre-made value
    static const prVector3 UnitZ;       ///< Pre-made value


public:

    /// @brief      Ctor
    prVector3() : x (0.0f) , y (0.0f) , z (0.0f)    
    {
    }

    /// @brief      Ctor
    prVector3(float nx, float ny, float nz) : x (nx) , y (ny) , z (nz)
    {
    }

    /// @brief     Calculates the magnitude of the vector.
    float Magnitude() const;

    /// @brief     Calculates the magnitude of the vector squared.
    float MagnitudeSquared() const;
    
    /// @brief     Normalise this vector.
    prVector3& Normalize();
    
    /// @brief     Returns the dot product.
    float DotProduct(const prVector3 &v) const;

    /// @brief     Returns the cross product.
    prVector3 CrossProduct(const prVector3 &v) const;

    /// @brief     Returns the vector as a string.
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
