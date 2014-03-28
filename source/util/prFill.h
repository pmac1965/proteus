#ifndef __PRFILL
#define __PRFILL


#include "../core/prTypes.h"


typedef bool (*prFillCallback)(u32 y, u32 x, s32 value, void *);


void prFillInit(const u32 width, const u32 height, void *ptr);


void prFillStart(u32 y, u32 x, s32 value, prFillCallback cb);


#endif//__PRFILL
