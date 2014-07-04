// File: prJNISleep.h
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


#ifndef __PRJNISLEEP_H
#define __PRJNISLEEP_H


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)


// Function: prJNI_SleepSetState
//      Set the sleep state
void prJNI_SleepSetState(bool state);


#endif//PLATFORM_ANDROID


#endif//__PRJNISLEEP_H
