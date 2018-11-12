// File: prStackHeap.h
//      Contains a stack type memory heap manager
//
// Notes:
//      This class is aimed at small systems with limited speed
//      and memory. Not meant for full blown pc, xbox or playstation
//      games where more advanced memory management would be
//      required. Though I've seen an implementation in 
//      a PSP project I worked on
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
#include "prAllocator.h"


// Used to set the maximum number of memory marks that can be placed
#define SH_STACK_SIZE  32


//  Used to store information on a stack based memory block
typedef struct StackNode
{
    Proteus::Core::u32  size;
    char*               func;

} StackNode;


// Class: prStackHeap
//      Represents a stack based memory heap manager.
class prStackHeap : public Proteus::Memory::prAllocator
{
public:
    // Method: prStackHeap
    //      Constructor.
    //
    // Parameters:
    //      size    - The size of heap required
    //      name    - Optional name of the heap
    prStackHeap(Proteus::Core::u32 size, const char *name = 0);

    // Method: ~prStackHeap
    //      Destructor.
    ~prStackHeap();

    // Method: Allocate
    //      Allocates memory. Returns null on failure.
    //
    // Parameters:
    //      size - Bytes required
    //      name - Optional name for the allocation (Use in memory tracking)
    void* Allocate(Proteus::Core::u32 size, const char* func = 0);

    // Method: Release
    //      Restores the last memory mark placed, releasing all memory allocated after the mark was placed.
    void Release();

    // Method: Release
    //      Does nothing.
    //
    // Notes:
    //      This function is included in order to allow the stack heap to be used as an allocator.
    void Release(void *) {}

    // Method: ReleaseAll
    //      Releases all memory marks, releasing all memory.
    void ReleaseAll();

    // Method: DisplayUsage
    //      Displays information about the heap.
    //
    // Notes:
    //       If full mode is used information on every block is also displayed.
    void DisplayUsage(bool full = false) const;

    // Method: MemoryFree
    //      Returns the amount of memory free in the heap.
    Proteus::Core::u32 MemoryFree() const;

    // Method: Mark
    //      Marks the current end of the heap, allowing it to be restored later.
    void Mark();

    // Method: GetMarkCount
    //      Returns the number of marks placed.
    Proteus::Core::u32 GetMarkCount() const;

    // Method: Size
    //      Returns the heap size.
    Proteus::Core::u32 Size() const;

    // Method: IsValidPointer
    //      Allows you to test if a pointer is valid for a specific heap.
    //
    // Notes:
    //      Will only return true if the pointer is in the same memory range as
    //      the heap and it points to the start of a valid block.
    bool IsValidPointer(void* p);

    // Method: BoundsCheckEnable
    //      Enable/disable bounds checking.
    //
    // Notes:
    //      Bounds checking can only be enabled and disabled when the heap is empty.
    void BoundsCheckEnable(bool enable);

    // Method: BoundsCheck
    //      Checks all allocated blocks for accidental overwrite and underwrite.
    //
    // Notes:
    //      If bounds checking is enabled then this method will check that the overwrite tags,
    //      placed at the start and end of each allocated memory block are intact.
    void BoundsCheck() const;

    // Method: BoundsCheck
    //      Returns the bounds checking enabled status.
    bool IsBoundsCheckEnabled() const;


private:
    // Stop passing by value and assignment.
    prStackHeap(const prStackHeap&);
    const prStackHeap& operator = (const prStackHeap&);


protected:
    Proteus::Core::u8   *m_heap;                 // The heap.
    Proteus::Core::u32   m_heap_size;            // The size of the heap.
    Proteus::Core::u32   m_start;                // Start of the unused part of the heap. (Below contains blocks, Above contains no blocks)
    Proteus::Core::u32   m_end;                  // The end of the heap.
    Proteus::Core::s32   m_index;                // The stack index
    Proteus::Core::u32   m_stack[SH_STACK_SIZE]; // The stack.
    Proteus::Core::u32   m_bounds_check;         // Bounds check?
    const char          *m_name;                 // A name used to uniquely identify the heap during debugging.
};
