/**
 * prFileShared.cpp
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
