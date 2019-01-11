// File: prRandom.h
//      A simple random number generator.
//
// Notes:
//      Not all devices come with the full set of C/C++ libraries.
//      So many libraries/functions, such as rand(), can be missing
//      or just not work.
//
// Notes:
//      This engine can be used on more than just the big platforms,
//      so some code has its uses in certain cases. It depends on the project!
//
//      I have coded for platforms that didn't support bytes, or hard disks,
//      and came with RAM in the kilobytes and all the data was in switchable ROM banks.      
//
//      So don't make assumptions, some code isn't, 'just in case' code.
//
//      This code has been used in two SPG240 games.
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


// Function: prRandomSetSeed
//      Sets a new random number seed.
//
// Parameters:
//      newSeed = The random number generators seed.
void prRandomSetSeed(u32 newSeed);

// Function: prRandomNumber
//      Returns a random number between 0 and max minus 1.
//
// Parameters:
//      max = The highest value required.
u32 prRandomNumber(u32 max);

// Function: prRandomNumber
//      Returns a random number between min and max.
//
// Parameters:
//      min = The lowest value required.
//      max = The highest value required.
s32 prRandomNumber(s32 min, s32 max);

// Function: prRand
//      Returns a random number similar to the C standard lib rand() function.
s32 prRand();


}}// Namespaces
