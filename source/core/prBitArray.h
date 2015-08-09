// File: prBitArray.h
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


// Class: prBitArray
//      Class that allows huge arrays of bit sized flags.
class prBitArray
{
public:
    // Method: prBitArray
    //      Constructor.
    explicit prBitArray(Proteus::Core::s32 bitCount);

    // Method: ~prBitArray
    //      Destructor.
    ~prBitArray();

    // Method: prBitArray
    //      Copy constructor.
    prBitArray(const prBitArray& bitArray);

    // Method: Set
    //      Sets the bit at 'index' to 1.
    //
    // Parameters:
    //      index - A value between 0 to max bits - 1
    void Set(Proteus::Core::s32 index);

    // Method: Clear
    //      Sets the bit at 'index' to 0.
    //
    // Parameters:
    //      index - A value between 0 to max bits - 1
    void Clear(Proteus::Core::s32 index);

    // Method: Flip
    //      Flips the bit at 'index'.
    //
    // Parameters:
    //      index - A value between 0 to max bits - 1
    void Flip(Proteus::Core::s32 index);

    // Method: ClearAll
    //      Sets all the bits to 0.
    void ClearAll();

    // Method: SetAll
    //      Sets all the bits to 1.
    void SetAll();

    // Method: Test
    //      Returns true if the bit at 'index' equals 1, else returns false.
    //
    // Parameters:
    //      index - A value between 0 to max bits - 1
    bool Test(Proteus::Core::s32 index) const;

    // Method: Count
    //      Returns the number of bits in the array
    Proteus::Core::u32 Count() const { return m_count; }


protected:
    Proteus::Core::s32  m_boxes;
    Proteus::Core::s32  m_count;
    Proteus::Core::u32 *m_array;
};
