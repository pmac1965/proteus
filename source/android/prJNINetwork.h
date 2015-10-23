// File: prJNINetwork.h
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


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)


// Function: prJNI_BTInit
//      Initialises bluetooth
void prJNI_BTInit();

// Function: prJNI_BTSend
//      Sends data via bluetooth.
void prJNI_BTSend(unsigned char *bytes, unsigned int arraySize);

// Function: prJNI_BTIsServer
//      Determines if current machine is the server.
bool prJNI_BTIsServer();


#endif//PLATFORM_ANDROID
