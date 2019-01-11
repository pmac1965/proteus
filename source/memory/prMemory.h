// File: prMemory.h
//      Some of this code is pre-GBA, written before DMA (Direct Memory Acess)
//      
// Notes:
//      Really shouldn't keep is around... but I do
//
// Notes:
//      Not really intended to be used.
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


#include "../core/prTypes.h"


// Testing.
//#define DEBUG_MEMORY


// Bounds checking.
#define BOUNDS_CHECK_SIZE       8           // The size added to an allocated block in order to permit bounds checking.
#define BOUNDS_CHECK_ADJUST     4           // The size if adjust a pointer to get the tag value.
#define BOUNDS_CHECK_TAG        0xDEADC0DE  // The tag value placed at either end of an allocated block.


// Used to maintain a node in a free list.
typedef struct prFreeNode
{
    struct prFreeNode* next;

} prFreeNode;


// Allows function name passing to be removed from the memory code.
#if defined(DEBUG_MEMORY) && (defined(DEBUG) || defined(_DEBUG))
    #define Alloc(size)         Allocate((size), __FUNCTION__)
    #define AllocLocked(size)   AllocateLocked((size), __FUNCTION__)
    #define AllocFixed(size)    AllocateFixed((size), __FUNCTION__)
#else
    #define Alloc(size)         Allocate((size))
    #define AllocLocked(size)   AllocateLocked((size))
    #define AllocFixed(size)    AllocateFixed((size))
#endif


PRNAMESPACE_BEG(Proteus)
PRNAMESPACE_BEG(Memory)


// Function: prMemoryCopy
//      Performs memory copying.
//
// Parameters:
//      src  - A pointer to the source address
//      dsu  - A pointer to the destination address
//      size - The amount to copy in bytes
void prMemoryCopy(void *src, void *dst, u32 size);

// Function: prMemoryClear
//      Performs memory clearing.
//
// Parameters:
//      addr - A pointer to an address
//      size - The size of the pointed to address in bytes
void prMemoryClear(void *addr, u32 size);

// Function: prMemoryFill8
//      Performs memory filling.
//
// Parameters:
//      addr - A pointer to an address
//      size - The size of the pointed to address in bytes
//      data - The data to fill with
void prMemoryFill8(void *addr, u32 size, u8 data);

// Function: prMemoryFill16
//      Performs memory filling.
//
// Parameters:
//      addr - A pointer to an address
//      size - The size of the pointed to address in bytes
//      data - The data to fill with
void prMemoryFill16(void *addr, u32 size, u16 data);

// Function: prMemoryFill32
//      Performs memory filling.
//
// Parameters:
//      addr - A pointer to an address
//      size - The size of the pointed to address in bytes
//      data - The data to fill with
void prMemoryFill32(void *addr, u32 size, u32 data);


PRNAMESPACE_END
PRNAMESPACE_END
