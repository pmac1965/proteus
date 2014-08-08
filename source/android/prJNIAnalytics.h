// File: prJNIAnalytics.h
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


#ifndef __PRJNIANALYTICS_H
#define __PRJNIANALYTICS_H


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)


// Function: prJNI_Submit
//      Add an analytics message
void prJNI_Submit(const char *message);


#endif//PLATFORM_ANDROID


#endif//__PRJNIANALYTICS_H