// File: prSinCos.h
//  This is a pre-generated table, that can be altered on a
//  per game basis to save on sin/cos calculations.
//
//  This is just a starting point.
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
 *
 *  PMAC - Updated Jan 2019 to improve comments.
 */


#pragma once


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace Math {


// Method: prCos
//      Returns cosine. In the range of 0 to 360.
f32 prCos(u32 angle);

// Method: prSin
//      Returns sine. In the range of 0 to 360.
f32 prSin(u32 angle);


}}// Namespaces
