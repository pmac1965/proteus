// File: prVersion.h
//      Engine version number.
//
// Notes:
//      Jul 2019 - Switched to year/month/revision model.
//      Apr 2022 - Switched to year/month/day model.
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


#include "prTypes.h"


namespace Proteus {
namespace Core {

    // Function: prGetVersion
    //      Acquire engine version number.
    //
    // Parameters:
    //      year     - Variable to receive version number
    //      month    - Variable to receive version number
    //      day      - Variable to receive version number
    void prGetVersion(s32 &year, s32 &month, s32 &day);

    // Function: prGetVersionAsString
    //      Acquire engine version number as a string.
    //
    // Notes:
    //      Buffer size should be between 16 and 255 bytes.
    //      Buffer contains a string in the format of "year.month.day"
    void prGetVersionAsString(char* buffer, s32 bufferSize);

}}
