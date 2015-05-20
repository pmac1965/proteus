// File: prAllocator.h
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


#ifndef __PRALLOCATOR_H
#define __PRALLOCATOR_H


#include "../core/prTypes.h"


// Class: prAllocator
//      Interface for all prAllocators.
class prAllocator
{
public:
    // Method: Allocate
    //      Allocates memory from an prAllocator
    virtual void *Allocate(Proteus::Core::u32 size, const char* func = 0) = 0;

    // Method: Release
    //      Releases memory to an prAllocator.
    virtual void Release(void *p) = 0;
};


#endif//__PRALLOCATOR_H
