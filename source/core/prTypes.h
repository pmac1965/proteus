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


#ifndef __PRTYPES_H
#define __PRTYPES_H


#include "../prConfig.h"
#include "../debug/prAssert.h"


// Namespaces for C++ code
#if defined(__cplusplus)
  #define PRNAMESPACE_BEG(name) namespace name {
  #define PRNAMESPACE_END       }

// No namespaces for C code
#else
  #define PRNAMESPACE_BEG(name)
  #define PRNAMESPACE_END

#endif


// Namespaces
PRNAMESPACE_BEG(Proteus)
PRNAMESPACE_BEG(Core)


// Basic types - PC
#if defined(PLATFORM_PC)
    typedef   signed char           s8;
    typedef unsigned char           u8;
    typedef   signed short          s16;
    typedef unsigned short          u16;
    typedef   signed int            s32;
    typedef unsigned int            u32;
    typedef   signed __int64        s64;
    typedef unsigned __int64        u64;
    typedef float                   f32;
    typedef double                  f64;

// Basic types - iphone/mac
#elif (defined(PLATFORM_IOS) || defined(PLATFORM_MAC))
    typedef   signed char           s8;
    typedef unsigned char           u8;
    typedef   signed short          s16;
    typedef unsigned short          u16;
    typedef   signed int            s32;
    typedef unsigned int            u32;
    typedef   signed long long      s64;
    typedef unsigned long long      u64;
    typedef float                   f32;
    typedef double                  f64;

// Basic types - android, linux, tizen
#elif (defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX))
    typedef   signed char           s8;
    typedef unsigned char           u8;
    typedef   signed short          s16;
    typedef unsigned short          u16;
    typedef   signed int            s32;
    typedef unsigned int            u32;
    typedef   signed long long      s64;
    typedef unsigned long long      u64;
    typedef float                   f32;
    typedef double                  f64;

#else
    #error Platform not defined

#endif


// Volatile versions.
typedef volatile s8         vs8;
typedef volatile u8         vu8;
typedef volatile s16        vs16;
typedef volatile u16        vu16;
typedef volatile s32        vs32;
typedef volatile u32        vu32;
typedef volatile s64        vs64;
typedef volatile u64        vu64;
typedef volatile f32        vf32;
typedef volatile f64        vf64;


// Character types
typedef char                char8;
typedef wchar_t             char16;


// Fixed point types
typedef s16                 fx16;
typedef s32                 fx32;
typedef volatile fx16       vfx16;
typedef volatile fx32       vfx32;


// Boolean
typedef u32                 PRBOOL;
#define PRTRUE              1
#define PRFALSE             0


// Type for drawing quads
typedef struct QuadData
{
	f32 x, y;
	f32 u, v;

}  QuadData;


// Sanity checks
PRCOMPILER_ASSERT(sizeof(s8) == 1);
PRCOMPILER_ASSERT(sizeof(u8) == 1);
PRCOMPILER_ASSERT(sizeof(s16) == 2);
PRCOMPILER_ASSERT(sizeof(u16) == 2);
PRCOMPILER_ASSERT(sizeof(s32) == 4);
PRCOMPILER_ASSERT(sizeof(u32) == 4);
PRCOMPILER_ASSERT(sizeof(s64) == 8);
PRCOMPILER_ASSERT(sizeof(u64) == 8);
PRCOMPILER_ASSERT(sizeof(f32) == 4);
PRCOMPILER_ASSERT(sizeof(f64) == 8);


PRNAMESPACE_END
PRNAMESPACE_END // Namespaces


#endif//__PRTYPES_H
