/**
 * @file       prVersion.h
 * @brief      Contains code to acquire the engine version number.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
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


#ifndef __PRVERSION_H
#define __PRVERSION_H


/// @brief      Acquire engine version number.
/// @param      major    - Variable to receive version number
/// @param      minor    - Variable to receive version number
/// @param      revision - Variable to receive version number
void prGetVersion(int &major, int &minor, int &revision);

/// @brief      Acquire engine version number as a string.
/// @return     A string in the format of "major.minor.revision"
const char *prGetVersionAsString();


#endif//__PRVERSION_H
