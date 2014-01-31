/**
 * @file       prPoint.h
 * @brief      Contains some simple point classes.
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


#ifndef __PRPOINT_H
#define __PRPOINT_H


#include "../core/prTypes.h"


// template for a simple 3D point.
template<typename T>
struct TPoint3
{
public:

    /// Constructor.
    TPoint3() : x(0), y(0), z(0)
    {}

    /// Constructor.
    TPoint3(T nx, T ny, T nz) : x(nx), y(ny), z(nz) 
    {}

    /// Copy Constructor.
    TPoint3(const TPoint3& p) : x(p.x), y(p.y), z(p.z)
    {}


public:

    // Members are public for faster access.
    T x, y, z;
};


// template for a simple 2D point.
template<typename T>
struct TPoint2
{
public:

    /// Constructor.
    TPoint2() : x(0), y(0)
    {}

    /// Constructor.
    TPoint2(T nx, T ny) : x(nx), y(ny)
    {}

    /// Copy Constructor.
    TPoint2(const TPoint2& p) : x(p.x), y(p.y)
    {}


public:

    // Members are public for faster access.
    T x, y;
};


/// @typedef    prPoint3
/// @brief      Simple 3D point with integer x, y, z
typedef TPoint3<s32>     prPoint3;

/// @typedef    prPoint3F
/// @brief      Simple 3D point with float x, y, z
typedef TPoint3<f32>     prPoint3F;

/// @typedef    prPoint2.
/// @brief      Simple 2D point with integer x, y
typedef TPoint2<s32>     prPoint2;

/// @typedef    prPoint2F.
/// @brief      Simple 2D point with float x, y
typedef TPoint2<f32>     prPoint2F;


#endif//__PRPOINT_H
