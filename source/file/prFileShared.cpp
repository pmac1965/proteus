/**
 * prFileShared.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "prFileShared.h"


/// ---------------------------------------------------------------------------
/// Calculates a checksum.
/// ---------------------------------------------------------------------------
u32 prCalculateChecksum(u8 *data, u32 datasize)
{
    PRASSERT(data);
    PRASSERT(datasize);

    u32 size = 0;
    u32 idx  = 0;

    do
    {
        size += data[idx++];
    }
    while(--datasize > 0);

    return size;
}
