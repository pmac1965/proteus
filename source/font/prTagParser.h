// File: prTagParser.h
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


// Enum: prTagType
//      Tag return types
//
// PRTT_NONE    - No tag found
// PRTT_SET_RGB - Set colour using system colour. Usage #set_rgb<0.0,1.0,0.0>#
//
// Notes:
//      Please use American/UK English style floating point numbers in your xml
//      files
enum prTagType
{
    PRTT_NONE,
    PRTT_SET_RGB,
};


// Method: prIsTag
//      Checks if a # in a string message indicates a tag
//
// Returns:
//      A value indicating tag type
//
// See Also:
//      <prTagType>
Proteus::Core::u32 prTagIsTag(const char *pString, Proteus::Core::f32 _a = 1.0f);

// Method: prTagDoAction
//      Performs the action set by the last tag found
void prTagDoAction();

// Method: prTagGetTagLength
//      Returns the length of the last tag found
Proteus::Core::s32 prTagGetTagLength();
