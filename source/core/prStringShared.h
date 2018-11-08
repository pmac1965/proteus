// File: prStringShared.h
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


// Enum:   prStringResult
//      String comparison results
//
// Returns:
//      CMP_ERROR       - An error occurred
//      CMP_LESSTHAN    - Less than
//      CMP_EQUALTO     - Equal to
//      CMP_GREATERTHAN - Greater than
//enum class prStringResult : Proteus::Core::s32
enum prStringResult
{
    CMP_ERROR       = -2,
    CMP_LESSTHAN    = -1,
    CMP_EQUALTO     =  0,
    CMP_GREATERTHAN =  1
};
