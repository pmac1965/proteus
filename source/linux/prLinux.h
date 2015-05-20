// File: prLinux.h
// About:
//      Contains linux helper functions that we need to write in C
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


#ifndef __PRLINUX_H
#define __PRLINUX_H


#include "../core/prTypes.h"


#if defined(__cplusplus)
extern "C" {
#endif


// Function: prStoreArgs
//      Stores the args for internal use
void prLinuxStoreArgs(int argc, const char *args[]);

// Function: prCreateLinuxDisplay
//      Creates a linux window
int prLinuxCreateDisplay(unsigned int width, unsigned int height);

// Function: prLinuxSwapBuffers
//      Displays the last buffer drawn
void prLinuxSwapBuffers();

// Function: prLinuxLoop
//		The linux game loop.
void prLinuxLoop();


#if defined(__cplusplus)
}
#endif


#endif//__PRLINUX_H
