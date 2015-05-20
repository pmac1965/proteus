// File: prLinkedHeap.h
//      Contains a memory heap manager that uses linked nodes.
//
// Notes:
//      This heap has been tested and rates pretty good in the speed
//      department. It was 17% faster than the nintendo DS memory
//      system. Not tested on other systems. Here for review really
//
// Notes:
//      This class is aimed at small systems with limited speed
//      and memory. Not meant for full blown pc, xbox or playstation
//      games where more advanced memory management would be
//      required
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


#ifndef __PRLINKEDHEAP_H
#define __PRLINKEDHEAP_H


#include "../core/prTypes.h"
#include "prMemory.h"
#include "prAllocator.h"


// Heap states
enum
{
    HEAP_STATE_FREE   = 1,
    HEAP_STATE_NORMAL = 2,
    HEAP_STATE_LOCKED = 3,
    HEAP_STATE_FIXED  = 4
};


// Used to link the memory blocks.
typedef struct LinkNode
{
    struct LinkNode* prev;          // Previous node
    struct LinkNode* next;          // Next node    

    char* func;                     // Allocating function

    Proteus::Core::u32 size   : 24, // Size and status.
                       status : 8;

} LinkNode;


// Type used to debug the heap.
typedef struct AllocInfo
{
    char* name;
    int   count;
    int   size;

} AllocInfo;


// Class: prLinkedHeap
//      Represents a memory heap manager that uses linked nodes.
class prLinkedHeap : public prAllocator
{
public:
    // Method: prLinkedHeap
    //      Constructor.
    //
    // Parameters:
    //      size    - The size of heap required
    //      name    - Optional name of the heap
    prLinkedHeap(Proteus::Core::u32 size, const char *name = 0);

    // Method: prLinkedHeap
    //      Use this constructor if you already have the address and size of the memory you wish to use.
    //
    // Parameters:
    //      start   - The start of your preallocated heap
    //      size    - The size of your preallocated heap
    //      name    - Optional name of the heap
    prLinkedHeap(Proteus::Core::u8 *start, Proteus::Core::u32 size, const char *name = 0);

    // Method: ~prLinkedHeap
    //      Destructor.
    ~prLinkedHeap();

    // Method: Allocate
    //      Allocates memory from the heap.
    //
    // Parameters:
    //      size - Bytes required
    //      name - Optional name for the allocation (Use in memory tracking)
    //
    // Notes:
    //       Use for standard memory allocation.
    void* Allocate(Proteus::Core::u32 size, const char* func = 0);

    // Method: AllocateLocked
    //      Allocates memory from the heap.
    //
    // Parameters:
    //      size - Bytes required
    //      name - Optional name for the allocation (Use in memory tracking)
    //
    // Notes:
    //      Use when you want to give a block some protection against accidental release.
    //      If you release a locked blocked without first unlocking it, then a warning will be issued,
    //      however, the block will still be released.
    void* AllocateLocked(Proteus::Core::u32 size, const char* func = 0);

    // Method: AllocateFixed
    //      Allocates memory from the heap.
    //
    // Parameters:
    //      size - Bytes required
    //      name - Optional name for the allocation (Use in memory tracking)
    //
    // Notes:
    //       Use when you want to stop a block from being released.
    //       You should always allocate fixed blocks first in order to reduce heap fragmentation.
    //       You can only release a fixed block by resetting the heap with ReleaseAll()
    //
    // See Also:
    //      <ReleaseAll>
    void* AllocateFixed(Proteus::Core::u32 size, const char* func = 0);

    // Method: Release
    //      Releases a memory block.
    //
    // Notes:
    //       If your releasing a locked block you must set unlock to true.
    void  Release(void* p, bool unlock);    

    // Method: Release
    //      Releases a memory block.
    void  Release(void* p);    

    // Method: Release
    //      Releases all memory blocks including fixed and locked.
    //
    // Notes:
    //       This method is the only way to release a fixed block.
    void  ReleaseAll();

    // Method: DisplayUsage
    //      Displays information about the heap.
    void DisplayUsage(bool full = false) const;

    // Method: DisplayAllocations
    //      Displays all the allocations by type.
    void DisplayAllocations() const;

    // Method: GetLargestFreeBlock
    //      Returns the size of the largest allocatable block.
    //
    // Notes:
    //      You should deduct the size of a LinkNode from the returned size to acquire the size you can actually allocate.
    Proteus::Core::u32 GetLargestFreeBlock() const;

    // Method: GetLargestFreeBlock
    //      Returns the size of all the used blocks.
    //
    // Notes:
    //      The returned size includes the link node plus the bounds checking tags.
    Proteus::Core::u32 GetSizeOfUsedBlocks() const;

    // Method: GetSizeOfUnusedBlocks
    //      Returns the size of all the unused blocks.
    //
    // Notes:
    //      The returned size includes the link node plus the bounds checking tags.
    Proteus::Core::u32 GetSizeOfUnusedBlocks() const;

    // Method: GetTotalFreeMemory
    //      Returns the total amount of free memory in the heap.
    Proteus::Core::u32 GetTotalFreeMemory() const;

    // Method: BoundsCheckEnable
    //      Enable/disable bounds checking.
    //
    // Notes:
    //      Bounds checking can only be enabled and disabled when the heap is empty.
    void BoundsCheckEnable(bool enable);

    // Method: BoundsCheckEnable
    //      Checks all allocated blocks for accidental overwrite and underwrite.
    //
    // Notes:
    //      If bounds checking is enabled then this method will check that the overwrite tags,
    //      placed at the start and end of each allocated memory block are intact.
    void BoundsCheck() const;

    // Method: BoundsCheckEnable
    //      Returns the bounds checking enabled status.
    bool IsBoundsCheckEnabled() const;

    // Method: Defragment
    //      Defragments the memory heap.
    //
    // Notes:
    //      The time taken to defragment a heap is relative to the number of free allocated blocks.
    //      However, due to the partial defragmentation, the number of free allocated blocks will be significantly reduced.
    //      To be on the safe side, you should only invoke defragment, when you can afford a brief delay.
    //      However, as partial defragmentation occurs every time a block is released you should rarely,
    //      if ever, need to use full defragmentation.
    void Defragment();

    // Method: IsValidPointer
    //      Allows you to test if a pointer is valid for a specific heap.
    //
    // Notes:
    //      Will only return true if the pointer is in the same memory range as,
    //      the heap and it points to the start of a valid block that's being used.
    //      A freed block will return false.
    bool IsValidPointer(void* p);

    // Method: IsPointerInHeap
    //      Tests to see if the pointers address is contained within the heaps bounds.
    bool IsPointerInHeap(void* p);


private:
    // Stop passing by value and assignment.
    prLinkedHeap( const prLinkedHeap& );
    const prLinkedHeap& operator = ( const prLinkedHeap& );


    // Adds a released block to the free list.
    void FreeListAdd(void* p);

    // Looks for a suitable unused block for reallocation.
    LinkNode* FreeListSearch(Proteus::Core::u32 required_size, void*& p);

    // Removes a released block from the free list.
    void FreeListRemove(void* p);

    // Displays all the available blocks in the free list.
    #if defined(_DEBUG) || defined(DEBUG)
    void FreeListShow() const;
    #endif

    // Allocates memory from the heap.
    void* AllocateMemory(Proteus::Core::u32 size, const char* func, Proteus::Core::u32 status);


protected:
    prFreeNode* m_free;                 // Unused, but unreleased blocks.
    prFreeNode* m_last;                 // Unused, but unreleased blocks.

    LinkNode*   m_head;                 // First allocated block.
    LinkNode*   m_tail;                 // Last allocated block.

    const char* m_name;                 // A name used to uniquely identify the heap during debugging.

    Proteus::Core::u8*  m_heap;         // The heap.
    Proteus::Core::u32  m_heap_size;    // The size of the heap.
    Proteus::Core::u32  m_start;        // Start of the unused part of the heap. (Below contains blocks, Above contains no blocks)
    Proteus::Core::u32  m_end;          // The end of the heap.
    Proteus::Core::u32  m_bounds_check; // Bounds check?
    bool                m_user_addr;    // User supplied the heap start address and heap size.
};


#endif//__PRLINKEDHEAP_H
