/**
 * prMemory.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */


// Finding leaks
// http://msdn.microsoft.com/en-us/library/x98tx3cf.aspx


#include "../prConfig.h"


#if defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID)
  #include <cstring>
#else
  #include <memory.h>
#endif


#include "prMemory.h"
#include "../debug/prDebug.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"


#define DEBUG_MEMORY_FUNCTIONS


/// -----------------------------------------------------------------------------------------------
/// Performs memory copying.
/// -----------------------------------------------------------------------------------------------
void prMemoryCopy(void *src, void *dst, u32 size)
{
    PRASSERT(src);
    PRASSERT(dst);
    PRASSERT(size);

#if defined (PLATFORM_PC)
    memcpy_s(dst, size, src, size);
#else
    memcpy(dst, src, size);
#endif
    
    TODO("Needs changed to add dest size as well as source size")
}


/// -----------------------------------------------------------------------------------------------
/// Performs memory clearing.
/// -----------------------------------------------------------------------------------------------
void prMemoryClear(void *addr, u32 size)
{
    PRASSERT(addr);
    PRASSERT(size);
    memset(addr, 0, size);
}


/// -----------------------------------------------------------------------------------------------
/// Performs memory filling.
/// -----------------------------------------------------------------------------------------------
void prMemoryFill8(void *addr, u32 size, u8 data)
{
    PRASSERT(addr);
    PRASSERT(size);


    #ifdef DEBUG_MEMORY_FUNCTIONS
    prTrace("--------------------------\n");
    prTrace("Fill 8\n");
    prTrace("--------------------------\n");
    #endif


    // Fill
    u8 *ptr = (u8*)addr;

    for (u32 i=0; i<size; i++)
    {
        #ifdef DEBUG_MEMORY_FUNCTIONS
        prTrace("%x = %i\n", ptr, data);
        #endif

        *(ptr++) = data;
    }


    #ifdef DEBUG_MEMORY_FUNCTIONS
    prTrace("--------------------------\n");
    prTrace("\n");
    #endif
}


/// -----------------------------------------------------------------------------------------------
/// Performs memory filling.
/// -----------------------------------------------------------------------------------------------
void prMemoryFill16(void *addr, u32 size, u16 data)
{
    PRASSERT(addr);
    PRASSERT(size);
    PRASSERT(PRSIZE2(size));


    #ifdef DEBUG_MEMORY_FUNCTIONS
    prTrace("--------------------------\n");
    prTrace("Fill 16\n");
    prTrace("--------------------------\n");
    #endif


    // Fill
    u16 *ptr = (u16*)addr;

    for (u32 i=0; i<size; i += 2)
    {
        #ifdef DEBUG_MEMORY_FUNCTIONS
        prTrace("%x = %i\n", ptr, data);
        #endif

        *(ptr++) = data;
    }


    #ifdef DEBUG_MEMORY_FUNCTIONS
    prTrace("--------------------------\n");
    prTrace("\n");
    #endif
}


/// -----------------------------------------------------------------------------------------------
/// Performs memory filling.
/// -----------------------------------------------------------------------------------------------
void prMemoryFill32(void *addr, u32 size, u32 data)
{
    PRASSERT(addr);
    PRASSERT(size);
    PRASSERT(PRSIZE4(size));


    #ifdef DEBUG_MEMORY_FUNCTIONS
    prTrace("--------------------------\n");
    prTrace("Fill 32\n");
    prTrace("--------------------------\n");
    #endif


    // Fill
    u32 *ptr = (u32*)addr;

    for (u32 i=0; i<size; i += 4)
    {
        #ifdef DEBUG_MEMORY_FUNCTIONS
        prTrace("%x = %i\n", ptr, data);
        #endif

        *(ptr++) = data;
    }


    #ifdef DEBUG_MEMORY_FUNCTIONS
    prTrace("--------------------------\n");
    prTrace("\n");
    #endif
}
