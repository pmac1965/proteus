// File: prOglConfig.h
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


// Helpers to allow us to move to opengl 2, as  a base minimum
#define PROGL_VERSION_11        // Base opengl 1.1
#define PROGL_VERSION_20        // Opengl (es) 2.0 (Pretty similar to 3.3)
#define PROGL_VERSION_30        // Opengl 3.0


// GLES?
#if (defined(PLATFORM_ANDROID) ||defined(PLATFORM_ANDROID))
#define PROGL_GLES              // Is ios or android? If so, then GLES
#endif
