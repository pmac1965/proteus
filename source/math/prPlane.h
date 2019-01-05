// File: prPlane.h
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


//#include "prVector3.h"
#include "../core/prGameObject.h"


// Namespaces
namespace Proteus {
namespace Math {


class prPlane : public Proteus::Core::prGameObject// Should be moved, as will be rendered too
{
public:
    prPlane();

    // Should inherit a prGameObject

private:
    // Normal vector of the plane.
//    prVector3   normal;

    // Distance from origin.
//    Proteus::Core::f32  distance;
};


}}// Namespaces
