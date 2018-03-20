// File: prTransform.h
//      The transform class for actor components.
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


// Class: prTransform
//      Initial transform class
//
// Notes:
//      Will be modified to add quaternions.
//
class prTransform
{
public:
    Proteus::Math::prVector3    position;
    Proteus::Math::prVector3    rotation;
    Proteus::Math::prVector3    scale;
};


}} // Namespaces