// File: prQuaternion.h
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


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace Math {


// Class: prQuaternion
//      A quaternion implementation
class prQuaternion
{
public:
    prQuaternion();// : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
    //{}

    prQuaternion(f32 _x, f32 _y, f32 _z, f32 _w) : x(_x), y(_y), z(_x), w(_w)
    {}

    prQuaternion(f32 _x, f32 _y, f32 _z);

	//! Multiplication operator
	//quaternion operator*(const quaternion& other) const;

	//! Multiplication operator with scalar
	//quaternion operator*(f32 s) const;

	//! Multiplication operator with scalar
	prQuaternion& operator*=(f32 s);

	//! Multiplication operator
	//vector3df operator*(const vector3df& v) const;

	//! Multiplication operator
	//quaternion& operator*=(const quaternion& other);


    prQuaternion& Set(f32 _x, f32 _y, f32 _z);

    prQuaternion& Normalise();//f32 _x, f32 _y, f32 _z);


private:
    f32 x, y, z, w;
};


inline prQuaternion::prQuaternion(f32 _x, f32 _y, f32 _z)
{
    Set(_x, _y, -z);
}


//// multiplication operator
//inline quaternion quaternion::operator*(const quaternion& other) const
//{
//	quaternion tmp;
//
//	tmp.W = (other.W * W) - (other.X * X) - (other.Y * Y) - (other.Z * Z);
//	tmp.X = (other.W * X) + (other.X * W) + (other.Y * Z) - (other.Z * Y);
//	tmp.Y = (other.W * Y) + (other.Y * W) + (other.Z * X) - (other.X * Z);
//	tmp.Z = (other.W * Z) + (other.Z * W) + (other.X * Y) - (other.Y * X);
//
//	return tmp;
//}


//// multiplication operator
//inline quaternion quaternion::operator*(f32 s) const
//{
//	return quaternion(s*X, s*Y, s*Z, s*W);
//}


// multiplication operator
inline prQuaternion& prQuaternion::operator*=(f32 s)
{
	x*=s;
	y*=s;
	z*=s;
	w*=s;
	return *this;
}

//// multiplication operator
//inline quaternion& quaternion::operator*=(const quaternion& other)
//{
//	return (*this = other * (*this));
//}



/*inline prQuaternion& prQuaternion::Set(f32 _x, f32 _y, f32 _z)
{
	f64 angle;

	angle = _x * 0.5;
	const f64 sr = sin(angle);
	const f64 cr = cos(angle);

	angle = _y * 0.5;
	const f64 sp = sin(angle);
	const f64 cp = cos(angle);

	angle = _z * 0.5;
	const f64 sy = sin(angle);
	const f64 cy = cos(angle);

	const f64 cpcy = cp * cy;
	const f64 spcy = sp * cy;
	const f64 cpsy = cp * sy;
	const f64 spsy = sp * sy;

	x = (f32)(sr * cpcy - cr * spsy);
	y = (f32)(cr * spcy + sr * cpsy);
	z = (f32)(cr * cpsy - sr * spcy);
	w = (f32)(cr * cpcy + sr * spsy);

	return Normalise();
}//*/


/*inline prQuaternion& prQuaternion::Normalise()
{
	const f32 n = x*x + y*y + z*z + w*w;

	if (n == 1)
		return *this;

	//n = 1.0f / sqrtf(n);
	return (*this *= 1.0f / sqrtf(n));
    //return *this;
}//*/


}}// Namespaces
