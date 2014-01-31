/**
 * @file       prTypes.h
 * @brief      Contains the engine types.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRTYPES_H
#define __PRTYPES_H


#include "../prConfig.h"
#include "../debug/prAssert.h"


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

// Basic types - iphone
#elif defined(PLATFORM_IOS)
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

// Basic types - bada
#elif defined(PLATFORM_BADA)
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

// Basic types - android
#elif defined(PLATFORM_ANDROID)
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

// Basic types - android
#elif defined(PLATFORM_LINUX)
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


#endif//__PRTYPES_H
