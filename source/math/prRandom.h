// File: prRandom.h
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


#ifndef __PRRANDOM_H
#define __PRRANDOM_H


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace Math {


// Function: prRandomSetSeed
//      Sets a new random number seed.
void prRandomSetSeed(Proteus::Core::u32 newSeed);


// Function: prRandomNumber
//      Returns a random number between 0 and max minus 1.
Proteus::Core::u32 prRandomNumber(Proteus::Core::u32 max);


// Function: prRandomNumber
//      Returns a random number between min and max.
Proteus::Core::s32 prRandomNumber(Proteus::Core::s32 min, Proteus::Core::s32 max);


// Function: prRand
//      Returns a random number similar to the C standard lib rand() function.
Proteus::Core::s32 prRand();


}}// Namespaces


#endif//__PRRANDOM_H
