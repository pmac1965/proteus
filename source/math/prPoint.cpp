/**
 * prPoint.cpp
 *
 *  Copyright 2014 Paul Michael McNab
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
 *
 *  PMAC - Removed unused code. Reverted date to original code inclusion date
*/


#include "prPoint.h"


// Namespaces
namespace Proteus {
namespace Math {


// Integer versions
const TPoint3<s32> prPoint3 :: Zero   (0, 0, 0);
const TPoint3<s32> prPoint3 :: One    (0, 0, 0);
const TPoint2<s32> prPoint2 :: Zero   (0, 0);
const TPoint2<s32> prPoint2 :: One    (0, 0);

// Float versions
const TPoint3<f32> prPoint3F :: Zero  (0.0f, 0.0f, 0.0f);
const TPoint3<f32> prPoint3F :: One   (0.0f, 0.0f, 0.0f);
const TPoint2<f32> prPoint2F :: Zero  (0.0f, 0.0f);
const TPoint2<f32> prPoint2F :: One   (0.0f, 0.0f);


}} // Namespaces
