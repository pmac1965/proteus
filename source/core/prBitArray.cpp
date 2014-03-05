/**
 * prBitArray.cpp
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


#include "prBitArray.h"
#include "../core/prMacros.h"
#include "../debug/prDebug.h"
#include "../debug/prAssert.h"


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prBitArray::prBitArray(s32 bitCount)
{
    if (bitCount > 0)
    {
        m_boxes = (bitCount >> 5) + 1;
        m_count = bitCount;
        m_array = new u32 [m_boxes];
        PRASSERT(m_array);

        ClearAll();
    }
    else
    {
        m_boxes = 0;
        m_count = 0;
        m_array = 0;
    }
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prBitArray::~prBitArray()
{
    PRSAFE_DELETE(m_array);
}


/// ---------------------------------------------------------------------------
/// Copy constructor.
/// ---------------------------------------------------------------------------
prBitArray::prBitArray(const prBitArray& bitArray)
{
    m_boxes = bitArray.m_boxes;
    m_count = bitArray.m_count;
    m_array = new u32 [m_boxes];
    PRASSERT(m_array);
    PRASSERT(bitArray.m_array);

    for (s32 i=0; i<m_boxes; i++)
    {
        m_array[i] = bitArray.m_array[i];
    }
}


/// ---------------------------------------------------------------------------
/// Sets the bit at 'index' to 1.
/// ---------------------------------------------------------------------------
void prBitArray::Set(s32 index)
{
    PRASSERT(m_array);
    if (m_count > 0)
    {
        if (PRBETWEEN(index, 0, m_count - 1))
        {
            u32 box	= index >> 5;
            u32 bit	= index & 31;

            m_array[box] |= (1 << bit);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets the bit at 'index' to 0.
/// ---------------------------------------------------------------------------
void prBitArray::Clear(s32 index)
{
    PRASSERT(m_array);
    if (m_count > 0)
    {
        if (PRBETWEEN(index, 0, m_count - 1))
        {
            u32 box	= index >> 5;
            u32 bit	= index & 31;

            m_array[box] &= ~(1 << bit);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Flips the bit at 'index'.
/// ---------------------------------------------------------------------------
void prBitArray::Flip(s32 index)
{
    PRASSERT(m_array);
    if (m_count > 0)
    {
        if (PRBETWEEN(index, 0, m_count - 1))
        {
            u32 box	= index >> 5;
            u32 bit	= index & 31;

            m_array[box] ^= (1 << bit);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets all the bits to 0.
/// ---------------------------------------------------------------------------
void prBitArray::ClearAll()
{
    PRASSERT(m_array);
    if (m_boxes > 0)
    {
        for (s32 i=0; i<m_boxes; i++)
        {
            m_array[i] = 0;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets all the bits to 1.
/// ---------------------------------------------------------------------------
void prBitArray::SetAll()
{
    PRASSERT(m_array);
    if (m_boxes > 0)
    {
        for (s32 i=0; i<m_boxes; i++)
        {
            m_array[i] = 0xFFFFFFFF;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Returns true if the bit at 'index' equals 1, else returns false.
/// ---------------------------------------------------------------------------
bool prBitArray::Test(s32 index) const
{
    PRASSERT(m_array);
    if (m_count > 0)
    {
        if (PRBETWEEN(index, 0, m_count - 1))
        {
            u32 box	= index >> 5;
            u32 bit	= index & 31;

            return ((m_array[box] & (1 << bit)) == (u32)(1 << bit));
        }
    }

    return false;
}
