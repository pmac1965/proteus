/**
 * prLinkedHeap.cpp
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


#include "prLinkedHeap.h"
#include "prMemoryPool.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prMacros.h"
#include "../core/prList.h"


#define KILOBYTE            1024        // 1K.
#define HEAP_MAX_SIZE_DIFF  256         // Max size a block can differ before realloc is denied.
#define HEAP_MIN_SIZE       32          // Keep heap a reasonable minimum size.
#define SPACE_COUNT         6           // Used to format display usage.


// Local functions
namespace
{
    #if defined(DEBUG) || defined(_DEBUG)
    static const char* HeapState(u32 state)
    {
        switch(state)
        {
        case HEAP_STATE_FREE:   return "Free   ";
        case HEAP_STATE_NORMAL: return "Normal ";
        case HEAP_STATE_LOCKED: return "Locked ";
        case HEAP_STATE_FIXED:  return "Fixed  ";
        default:                return "Unknown";
        }
    }
    #endif
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prLinkedHeap::prLinkedHeap(u32 size, const char *name) : m_name(name)
{
    // Validate size.
    if (size < HEAP_MIN_SIZE)
    {
        PRPANIC("Requested linked heap size of %i bytes is too small. The minimum size is %i", size, HEAP_MIN_SIZE);
    }


    // We need to round up to 4 bytes to ensure alignment is okay.
    size = PRROUND_UP(size, 4);


    // Create the heap.
    m_heap          = new u8 [size];
    m_start         = (u32)(u64)m_heap;
    m_end           = (u32)(u64)m_heap + size;
    m_head          = 0;
    m_tail          = 0;
    m_free          = 0;
    m_last          = 0;
    m_heap_size     = size;
    m_bounds_check  = 0;
    m_user_addr     = false;
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prLinkedHeap::prLinkedHeap(u8 *start, u32 size, const char *name) : m_name(name)
{
    PRASSERT(start);


    // Validate size.
    if (size < HEAP_MIN_SIZE)
    {
        PRPANIC("Requested linked heap size of %i bytes is too small. The minimum size is %i", size, HEAP_MIN_SIZE);
    }


    // Both the start address and the size should be divisible by 4.
    PRASSERT(PRSIZE4((u64)start), "Both the start address and the size should be divisible by 4.");
    PRASSERT(PRSIZE4(size), "Both the start address and the size should be divisible by 4.");


    // Create the heap.
    m_heap          = start;
    m_start         = (u32)(u64)m_heap;
    m_end           = (u32)(u64)m_heap + size;
    m_head          = 0;
    m_tail          = 0;
    m_free          = 0;
    m_last          = 0;
    m_heap_size     = size;
    m_bounds_check  = 0;
    m_user_addr     = true;
}


/// ---------------------------------------------------------------------------
/// Destructor
/// ---------------------------------------------------------------------------
prLinkedHeap::~prLinkedHeap()
{
    ReleaseAll();

    if (m_user_addr == false)
    {
        PRSAFE_DELETE(m_heap);
    }
}


/// ---------------------------------------------------------------------------
/// Allocate
/// ---------------------------------------------------------------------------
void* prLinkedHeap::Allocate(u32 size, const char* func)
{
    return AllocateMemory(size, func, HEAP_STATE_NORMAL);
}


/// ---------------------------------------------------------------------------
/// AllocateLocked
/// ---------------------------------------------------------------------------
void* prLinkedHeap::AllocateLocked(u32 size, const char* func)
{
    return AllocateMemory(size, func, HEAP_STATE_LOCKED);
}


/// ---------------------------------------------------------------------------
/// AllocateFixed
/// ---------------------------------------------------------------------------
void* prLinkedHeap::AllocateFixed(u32 size, const char* func)
{
    return AllocateMemory(size, func, HEAP_STATE_FIXED);
}


/// ---------------------------------------------------------------------------
/// Release
/// ---------------------------------------------------------------------------
void prLinkedHeap::Release(void* p, bool unlock)
{
    if (p)
    {
        // Point to the node.
        LinkNode* node = (LinkNode*)((u8*)p - sizeof(LinkNode) - (m_bounds_check>>1));


        // Ensure we don't attempt to release a node twice.
        if (node->status == HEAP_STATE_FREE)
        {
            PRWARN("Delete called more than once. Block has already been released.");
            return;
        }


        // Does pointer belong to this heap?
        if (!IsPointerInHeap(p))
        {
            PRWARN("Pointer doesn't belong to this heap.");
            return;
        }


        // Warn if locked.
        if (node->status == HEAP_STATE_LOCKED)
        {
            if (!unlock)
            {
                PRWARN("Releasing a locked block.");
            }
        }


        // Warn if fixed.
        if (node->status == HEAP_STATE_FIXED)
        {
            PRWARN("Attempted to release a fixed block.");
            return;
        }


        // Set the state to free.
        node->status = HEAP_STATE_FREE;


        // Add the released block to the free list.
        FreeListAdd(p);

            
        // If its the tail node, then unlink it and return the memory to the heap.
        // Then repeat, just in case the new tail is unused.
        if (node == m_tail)
        {
            while(m_tail && m_tail->status == HEAP_STATE_FREE)
            {
                FreeListRemove( ((u8*)m_tail + sizeof(LinkNode) + (m_bounds_check>>1)) );

                // Return memory to heap.
                m_start -= m_tail->size;

                // Reset tail
                m_tail = m_tail->prev;

                // Any previous nodes left?
                if (m_tail)
                {
                    m_tail->next = 0;
                }
                else
                {
                    m_head = 0;
                }
            }
        }

        // Do a partial defrag by unlink any free blocks that are adjacent to the block being released.
        else
        {
            // Backup to any previous free nodes.
            while(node->prev && node->prev->status == HEAP_STATE_FREE)
            {
                node = node->prev;
            }


            // Unlink and release.
            LinkNode* first_node = node;

            PRASSERT(node->next);

            node = node->next;

            while(node->status == HEAP_STATE_FREE)
            {
                FreeListRemove( ((u8*)node + sizeof(LinkNode) + (m_bounds_check>>1)) );

                first_node->size += node->size;

                // We must set the first nodes next pointer to use the next pointer from the node being unlinked.
                first_node->next = node->next;

                // We must now set the new next nodes prev pointer to the first node.
                node->next->prev = first_node;

                node = node->next;
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Release
/// ---------------------------------------------------------------------------
void prLinkedHeap::Release(void* p)
{
    Release(p, false);
}


/// ---------------------------------------------------------------------------
/// ReleaseAll
/// ---------------------------------------------------------------------------
void prLinkedHeap::ReleaseAll()
{
    m_head  = 0;
    m_tail  = 0;
    m_free  = 0;
    m_last  = 0;
    m_start = (u32)(u64)m_heap;
}


/// ---------------------------------------------------------------------------
/// DisplayUsage
/// ---------------------------------------------------------------------------
void prLinkedHeap::DisplayUsage(bool full) const
{
#if defined(DEBUG) || defined(_DEBUG)

    prTrace("\nLinked heap: ============================================================================\n");

    if (full)
    {
        LinkNode* node = m_head;

        while(node)
        {
            prTrace
            (
                "Addr: %#08x State: %s Block size: %*i Data size: %*i Func: %s\n",
                (u32)(u64)node + sizeof(LinkNode) + (m_bounds_check>>1),
                HeapState(node->status),
                SPACE_COUNT, node->size,
                SPACE_COUNT, node->size - sizeof(LinkNode) - m_bounds_check,
                node->func ? node->func : "Unknown"
            );

            node = node->next;
        }

        prTrace("\n");    
    }

    prTrace("Heap name          : %s\n", (m_name && *m_name) ? m_name : "unnamed");
    prTrace("Bounds check       : %s\n", m_bounds_check ? "true" : "false"); 
    prTrace("Size used blocks   : %iK and %i bytes\n", GetSizeOfUsedBlocks()/KILOBYTE,   GetSizeOfUsedBlocks()%KILOBYTE);
    prTrace("Size unused blocks : %iK and %i bytes\n", GetSizeOfUnusedBlocks()/KILOBYTE, GetSizeOfUnusedBlocks()%KILOBYTE);    
    prTrace("Largest free block : %iK and %i bytes\n", GetLargestFreeBlock()/KILOBYTE,   GetLargestFreeBlock()%KILOBYTE);
    prTrace("Heap size          : %iK and %i bytes\n", m_heap_size/KILOBYTE,             m_heap_size%KILOBYTE);
    prTrace("=========================================================================================\n");


    if (full)
    {
        FreeListShow();
    }

#else

    PRUNUSED(full);
    
#endif
}


/// ---------------------------------------------------------------------------
/// Displays all the allocations by type.
/// ---------------------------------------------------------------------------
void prLinkedHeap::DisplayAllocations() const
{
#if defined(DEBUG) || defined(_DEBUG)

    // The pool will use two allocations (They will appear in the outputted list)
    prMemoryPool<prList<AllocInfo>::prNode> pool(256, "Memory pool (Heap)");


    // Uses stack allocations - does not allocate from the heap.
    prList<AllocInfo> list;
    AllocInfo info;
    bool add;


    LinkNode* node = m_head;

    while(node)
    {
        // Set to add.
        add = true;


        // Set allocation info.
        info.name  = node->func;
        info.size  = node->size;
        info.count = 1;


        // Do we have the allocation stored?
        if (list.Size() > 0)
        {
            prList<AllocInfo>::prIterator it = list.Begin();

            while(it.Okay())
            {
                if ((*it).name == info.name && node->status != HEAP_STATE_FREE)
                {
                    add = false;
                    (*it).count++;
                    (*it).size += info.size;
                    break;
                }

                ++it;
            }
        }


        // Add to list using local pool so we don't affect the heap :)
        if (add)
        {
            prList<AllocInfo>::prNode * pNode = pool.Pop();

            if (pNode)
            {
                // Set node details
                pNode->item.name  = info.name;
                pNode->item.count = info.count;
                pNode->item.size  = info.size;

                list.AddTail(pNode);
            }
        }

        node = node->next;
    }


    prTrace("Allocations by type: ====================================================================\n");

    // Do we have the allocation stored?
    if (list.Size() > 0)
    {
        prList<AllocInfo>::prIterator it = list.Begin();

        while(it.Okay())
        {
            prTrace("Allocations: %*i - Total size: %*i - %s\n", 5, (*it).count, 10, (*it).size, (*it).name);
            ++it;
        }
    }

    prTrace("=========================================================================================\n");
    
#endif
}


/// ---------------------------------------------------------------------------
/// GetLargestFreeBlock
/// ---------------------------------------------------------------------------
u32 prLinkedHeap::GetLargestFreeBlock() const
{
    return m_end - m_start;
}


/// ---------------------------------------------------------------------------
/// GetSizeOfUsedBlocks
/// ---------------------------------------------------------------------------
u32 prLinkedHeap::GetSizeOfUsedBlocks() const
{
    u32 size = 0;

    LinkNode* node = m_head;

    while(node)
    {
        if (node->status != HEAP_STATE_FREE)
        {
            size += node->size;
        }

        node = node->next;
    }

    return size;
}


/// ---------------------------------------------------------------------------
/// GetSizeOfUnusedBlocks
/// ---------------------------------------------------------------------------
u32 prLinkedHeap::GetSizeOfUnusedBlocks() const
{
    u32 size = 0;

    LinkNode* node = m_head;

    while(node)
    {
        if (node->status == HEAP_STATE_FREE)
        {
            size += node->size;
        }

        node = node->next;
    }

    return size;
}


/// ---------------------------------------------------------------------------
/// GetTotalFreeMemory
/// ---------------------------------------------------------------------------
u32 prLinkedHeap::GetTotalFreeMemory() const
{
    return GetLargestFreeBlock() + GetSizeOfUsedBlocks();
}


/// ---------------------------------------------------------------------------
/// BoundsCheckEnable
/// ---------------------------------------------------------------------------
void prLinkedHeap::BoundsCheckEnable(bool enable)
{
    if (m_start == (u32)(u64)m_heap)
    {
        m_bounds_check = enable ? BOUNDS_CHECK_SIZE : 0;
    }
    else
    {
        PRWARN("You can only disable/enable bounds checking when a linked heap is empty.");
    }
}


/// ---------------------------------------------------------------------------
/// BoundsCheck
/// ---------------------------------------------------------------------------
void prLinkedHeap::BoundsCheck() const
{
    if (m_bounds_check)
    {
        // Get first node.
        LinkNode* node = (LinkNode*)m_head;

        while(node)
        {
            u32* start_tag = (u32*)((u8*)node + sizeof(LinkNode));
            u32* end_tag   = (u32*)((u8*)node + node->size - BOUNDS_CHECK_ADJUST);

            if (*start_tag != BOUNDS_CHECK_TAG)
            {
                PRPANIC("Bounds check failed at block start. %s\n", node->func);
            }

            if (*end_tag != BOUNDS_CHECK_TAG)
            {
                PRPANIC("Bounds check failed at block end.");
            }

            node = node->next;
        }
    }
}


/// ---------------------------------------------------------------------------
/// IsBoundsCheckEnabled
/// ---------------------------------------------------------------------------
bool prLinkedHeap::IsBoundsCheckEnabled() const
{
    return (BOUNDS_CHECK_SIZE == m_bounds_check);
}


/// ---------------------------------------------------------------------------
/// Defragment.
/// ---------------------------------------------------------------------------
void prLinkedHeap::Defragment()
{
    LinkNode* node = m_head;
    LinkNode* next_node;
    LinkNode* first_node;

    while(node)
    {
        if (node->status == HEAP_STATE_FREE)
        {
            next_node  = node->next;
            first_node = node;

            while(next_node->status == HEAP_STATE_FREE)
            {
                FreeListRemove( ((u8*)next_node + sizeof(LinkNode) + (m_bounds_check>>1)) );

                first_node->size += next_node->size;

                // We must set the first nodes next pointer to use the next pointer from the node being unlinked.
                first_node->next = next_node->next;

                // We must now set the new next nodes prev pointer to the first node.
                next_node->next->prev = first_node;

                next_node = next_node->next;
            }
        }

        node = node->next;
    }
}


/// ---------------------------------------------------------------------------
/// IsValidPointer
/// ---------------------------------------------------------------------------
bool prLinkedHeap::IsValidPointer(void* p)
{
    if (p)
    {
        // Get node address.
        u32 address = (u32)(u64)p - sizeof(LinkNode) - (m_bounds_check>>1);


        // Out of heap memory range?
        if (address < (u32)(u64)m_heap || address >= m_end)
        {
            return false;
        }


        // Is address the same as a node header?
        LinkNode* node = (LinkNode*)m_head;

        while(node)
        {
            if ((u32)(u64)node == address)
            {
                return (node->status != HEAP_STATE_FREE);
            }

            node = node->next;
        }
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// Tests to see if the pointers address is contained within the heaps bounds.
/// ---------------------------------------------------------------------------
bool prLinkedHeap::IsPointerInHeap(void* p)
{
    bool result = false;

    if (p)
    {
        // As it'll be a pointer to a block subtract block size.
        u32 address = ((u32)(u64)p) - sizeof(LinkNode) - (m_bounds_check>>1);

        // In heap memory range?
        result = (address >= ((u32)(u64)m_heap) && address < m_end);
    }

    return result;
}


// ------------------------------------------------------------------------------------------------
//
// --- PRIVATE
//
// ------------------------------------------------------------------------------------------------


/// ---------------------------------------------------------------------------
/// FreeListAdd
/// ---------------------------------------------------------------------------
void prLinkedHeap::FreeListAdd(void* p)
{
    if (0 == m_free)
    {
        m_free       = (prFreeNode*)p;
        m_last       = (prFreeNode*)p;
        m_free->next = 0;
    }
    else
    {
        m_last       = (prFreeNode*)p;
        m_last->next = 0;
    }
}


/// ---------------------------------------------------------------------------
/// FreeListSearch
/// ---------------------------------------------------------------------------
LinkNode* prLinkedHeap::FreeListSearch(u32 required_size, void*& p)
{
    prFreeNode* freelist = m_free;
    LinkNode* node;

    do
    {
        node = (LinkNode*)((u8*)freelist - sizeof(LinkNode) - (m_bounds_check>>1));

        u32 size = node->size;

        if (size >= required_size)
        {
            // If the node is reasonably close in size, then we'll just use it the way it is.
            if ((size - required_size) <= HEAP_MAX_SIZE_DIFF)
            {
                FreeListRemove(freelist);

                p = freelist;
                return node;
            }
            else
            {
                FreeListRemove(freelist);


                // Create a new block node for the block being sub-divided.
                LinkNode* new_node =  (LinkNode*)((u8*)node + required_size);

                new_node->status = HEAP_STATE_FREE;
                new_node->func   = node->func;
                new_node->size   = node->size - required_size;
                new_node->prev   = node;
                new_node->next   = node->next;
                node->next       = new_node;


                // Set new tags.
                if (m_bounds_check)
                {
                    *(u32*)((u8*)node + required_size - BOUNDS_CHECK_ADJUST) = BOUNDS_CHECK_TAG;
                    *(u32*)((u8*)new_node + sizeof(LinkNode))                = BOUNDS_CHECK_TAG;
                }


                // Add the newly sub-divided block to the free list.
                FreeListAdd( (u8*)new_node + sizeof(LinkNode) + (m_bounds_check>>1) );


                // Set the size of the newly created block.
                node->size = required_size;
                
                
                p = freelist;
                return node;
            }
        }

        freelist = freelist->next;
    }
    while(freelist);


    return 0;
}


/// ---------------------------------------------------------------------------
/// FreeListRemove
/// ---------------------------------------------------------------------------
void prLinkedHeap::FreeListRemove(void* p)
{
    prFreeNode* curr = m_free;
    prFreeNode* prev = NULL;

    do
    {
        if (p == curr)
        {
            // If we are removing the first entry in the free list, then we need only to set
            // the next free block as the first entry in the free list.
            if (p == m_free)
            {
                m_free = m_free->next;
            }

            // If we are removing the last entry in the free list, then we need to unlink the entry and
            // reset the pointer to last entry in the free list.
            else if (p == m_last)
            {
                prev->next = 0;
                m_last     = prev;
            }
            
            // If we get here, then all we need to do is unlink the entry.
            else
            {
                // Added if test because of code analysis warning, but it shouldn't fail,
                // but... better safe than sorry, as shouldn't is not certain
                if (prev) 
                {
                    prev->next = curr->next;
                }
            }

            return;
        }

        prev = curr;
        curr = curr->next;
    }
    while(curr);
}


#if defined(_DEBUG) || defined(DEBUG)
// ------------------------------------------------------------------------------------------------
// FreeListShow
// ------------------------------------------------------------------------------------------------
void prLinkedHeap::FreeListShow() const
{
    if (m_free)
    {
        prTrace("Free list entries: ======================================================================\n");

        prFreeNode* freelist = m_free;

        do
        {
            LinkNode* node = (LinkNode*)((u8*)freelist - sizeof(LinkNode) - (m_bounds_check>>1));

            prTrace("Size : %i, Name: %s, Addr: %08x\n", node->size, node->func ? node->func : "Unknown", freelist);

            freelist = freelist->next;
        }
        while(freelist);

        prTrace("=========================================================================================\n");
    }    
}
#endif


/// ---------------------------------------------------------------------------
/// Allocate
/// ---------------------------------------------------------------------------
void* prLinkedHeap::AllocateMemory(u32 size, const char* func, u32 status)
{
    if (0 == size)
    {
        PRPANIC("Requested memory allocation is tool small.");
    }


    // Get required size.
    size = PRROUND_UP(size, 4);

    u32 required_size = size + sizeof(LinkNode);


    // Adjust to allow bounds check tags.
    required_size += m_bounds_check;


    // Next allocation?
    if (m_head)
    {
        // First, search free list if we have one.
        if (m_free)
        {
            void* p;

            LinkNode* node = FreeListSearch(required_size, p);

            if (node)
            {
                node->func   = const_cast<char*>(func);
                node->status = status;
                return p;
            }
        }


        // Add at heap end.
        {
            PRASSERT(m_start + required_size < m_end);

            if (m_start + required_size < m_end)
            {
                // Set the return address.
                u8* p;

                // Set the return address.
                if (m_bounds_check)
                {
                    p = (u8*)(u64)(m_start + sizeof(LinkNode) + BOUNDS_CHECK_ADJUST);

                    u32* start_tag = (u32*)(p - BOUNDS_CHECK_ADJUST);
                    u32* end_tag   = (u32*)(p + size);

                    *start_tag = BOUNDS_CHECK_TAG;
                    *end_tag   = BOUNDS_CHECK_TAG;
                }
                else
                {
                    p = (u8*)(u64)(m_start + sizeof(LinkNode));
                }


                // Fill in the nodes details
                LinkNode& node = *(LinkNode*)(u64)m_start;

                node.prev   = m_tail;
                node.next   = 0;
                node.func   = const_cast<char*>(func);
                node.status = status;
                node.size   = required_size;
                

                // Adjust the start of free memory.
                m_start += required_size;


                // Link in the new node.
                m_tail->next = &node;
                m_tail       = &node;


                return p;
            }
        }
    }
    // Its the first allocation.
    else
    {
        PRASSERT(m_start + required_size < m_end);

        if (m_start + required_size < m_end)
        {
            // Set the return address.
            u8* p;

            // Set the return address.
            if (m_bounds_check)
            {
                p = (u8*)(u64)(m_start + sizeof(LinkNode) + BOUNDS_CHECK_ADJUST);

                u32* start_tag = (u32*)(p - BOUNDS_CHECK_ADJUST);
                u32* end_tag   = (u32*)(p + size);

                *start_tag = BOUNDS_CHECK_TAG;
                *end_tag   = BOUNDS_CHECK_TAG;
            }
            else
            {
                p = (u8*)(u64)(m_start + sizeof(LinkNode));
            }


            // Fill in the nodes details
            LinkNode& node = *(LinkNode*)(u64)m_start;

            node.prev   = 0;
            node.next   = 0;
            node.func   = const_cast<char*>(func);
            node.status = status;
            node.size   = required_size;
            

            // Adjust the start of free memory.
            m_start += required_size;


            // Link in the first node.
            m_head = &node;
            m_tail = &node;


            return p;
        }
    }

    return 0;
}
