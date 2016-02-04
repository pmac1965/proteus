// File: prVersion.h
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


// Function: prGetVersion
//      Acquire engine version number.
//
// Parameters:
//      major    - Variable to receive version number
//      minor    - Variable to receive version number
//      revision - Variable to receive version number
void prGetVersion(int &major, int &minor, int &revision);

// Function: prGetVersionAsString
//      Acquire engine version number as a string.
//
// Returns:
//      A string in the format of "major.minor.revision"
const char *prGetVersionAsString();
