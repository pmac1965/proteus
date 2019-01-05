// File: prTransform.h
//      The transform struct for game components.
/**
 *  Copyright 2018 Paul Michael McNab
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


#pragma once


#include "../math/prVector3.h"


// Namespaces
namespace Proteus {
namespace Core {


// Struct: prTransform
//      Initial transform structure
//
// Notes:
//      Will be modified to add quaternions.
//
struct /*class*/ prTransform
{
//public:
    prTransform() : position(Proteus::Math::prVector3::Zero)
                  , rotation(Proteus::Math::prVector3::Zero)
                  , scale   (Proteus::Math::prVector3::One)
    {
    }


    void Reset()
    {
        position = Proteus::Math::prVector3::Zero;
        rotation = Proteus::Math::prVector3::Zero;
        scale    = Proteus::Math::prVector3::One;
    }


    //inline void ResetPosition() { position = Proteus::Math::prVector3::Zero; }
    //inline void ResetRotation() { rotation = Proteus::Math::prVector3::Zero; }
    //inline void ResetScale() { scale = Proteus::Math::prVector3::One; }

    Proteus::Math::prVector3    position;
    Proteus::Math::prVector3    rotation;
    Proteus::Math::prVector3    scale;
};


}} // Namespaces