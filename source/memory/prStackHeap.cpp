/**
 * prStackHeap.cpp
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


#include <string.h>
#include "prStackHeap.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "prMemory.h"


#define HEAP_MIN_SIZE       32          // Keep heap a reasonable minimum size.
#define SPACE_COUNT         6           // Used to format display usage.


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prStackHeap::prStackHeap(u32 size, const char *name) : m_name(name)
{
    // Validate size.
    if (size < HEAP_MIN_SIZE)
    {
        PRPANIC("Requested stack heap size of %i bytes is too small.\nThe minimum size is %i", size, HEAP_MIN_SIZE);
        size = HEAP_MIN_SIZE;
    }


    // We need to round up to 4 bytes to ensure alignment is okay.
    size = PRROUND_UP(size, 4);


    // Create the heap.
    m_heap          = new u8 [size];
    m_heap_size     = size;
    m_start         = (u32)((u64)m_heap);
    m_end           = (u32)((u64)m_heap + size);
    m_index         = 0;
    m_bounds_check  = 0;

    memset(m_stack, 0, sizeof(m_stack));
}


/// ---------------------------------------------------------------------------
/// Destructor
/// ---------------------------------------------------------------------------
prStackHeap::~prStackHeap()
{
    PRSAFE_DELETE(m_heap);
}


/// ---------------------------------------------------------------------------
/// Allocate
/// ---------------------------------------------------------------------------
void* prStackHeap::Allocate(u32 size, const char* func)
{
    if (0 == size)
    {
        PRPANIC("A stack heap cannot allocate a zero sized block.");
        return 0;
    }


    // The minimum size must be 4 in order to permit free list usage, also
    // we want to keep alignment to 4 bytes, so we round up by 4.
    size = PRROUND_UP(size, 4); 


    // Get the actual size we need.
    u32 size_required = size + sizeof(StackNode);


    // Adjust to allow bounds check tags.
    size_required += m_bounds_check;


    // Allocate
    PRASSERT(m_start + size_required < m_end);

    if (m_start + size_required < m_end)
    {
        u8* p;

        // Set the return address.
        if (m_bounds_check)
        {
            p = (u8*)((u64)m_start + sizeof(StackNode) + BOUNDS_CHECK_ADJUST);

            u32* start_tag = (u32*)(p - BOUNDS_CHECK_ADJUST);
            u32* end_tag   = (u32*)(p + size);

            *start_tag = BOUNDS_CHECK_TAG;
            *end_tag   = BOUNDS_CHECK_TAG;
        }
        else
        {
            p = (u8*)((u64)m_start + sizeof(StackNode));
        }


        // Fill in the nodes details
        StackNode& node = *(StackNode*)((u64)m_start);

        node.func = (char*)func;
        node.size = size_required;
        

        // Adjust the start of free memory.
        m_start += size_required;


        return p;
    }

    return 0;
}


/// ---------------------------------------------------------------------------
/// Release
/// ---------------------------------------------------------------------------
void prStackHeap::Release()
{
    if (m_index > 0)
    {
        m_start = m_stack[--m_index];
    }
    else
    {
        // Stack is empty, so reset.
        m_start = (u32)((u64)m_heap);
        m_index = 0;
    }
}


/// ---------------------------------------------------------------------------
/// ReleaseAll
/// ---------------------------------------------------------------------------
void prStackHeap::ReleaseAll()
{
    m_start = (u32)((u64)m_heap);
    m_index = 0;
}


/// ---------------------------------------------------------------------------
/// DisplayUsage
/// ---------------------------------------------------------------------------
void prStackHeap::DisplayUsage(bool full) const
{
#if defined(DEBUG) || defined(_DEBUG)
    
    s32 blocks_used = 0;

    prTrace(prLogLevel::LogError, "\nStack heap: =============================================================================\n");

    if (full)
    {
        StackNode* node = (StackNode*)m_heap;

        while((u32)(u64)node < m_start)
        {
            if (0 == blocks_used) { prTrace(prLogLevel::LogError, "\n"); }

            prTrace
            (
                prLogLevel::LogError,
                "Start: %#16x, Block size: %*i, Data size: %*i, Func: %s\n",   
                (((u8*)node) + sizeof(StackNode) + (m_bounds_check>>1)),
                SPACE_COUNT, node->size,
                SPACE_COUNT, node->size - sizeof(StackNode) - m_bounds_check,
                node->func ? node->func : "Unknown"
            );
            
            node = (StackNode*)((u8*)node + node->size);

            blocks_used++;
        }
    }

    prTrace(prLogLevel::LogError, "\n");
    prTrace(prLogLevel::LogError, "Heap name    : %s\n", (m_name && *m_name) ? m_name : "unnamed");
    prTrace(prLogLevel::LogError, "Marks placed : %i\n", m_index);
    prTrace(prLogLevel::LogError, "Blocks used  : %i\n", blocks_used);
    prTrace(prLogLevel::LogError, "Bounds check : %s\n", m_bounds_check ? "true" : "false"); 
    prTrace(prLogLevel::LogError, "Heap size    : %i K and %i bytes.\n", m_heap_size/1024, m_heap_size%1024);
    prTrace(prLogLevel::LogError, "Used memory  : %i K and %i bytes.\n", (m_heap_size - MemoryFree())/1024, (m_heap_size - MemoryFree())%1024);    
    prTrace(prLogLevel::LogError, "Free memory  : %i K and %i bytes.\n", MemoryFree()/1024, MemoryFree()%1024);
    prTrace(prLogLevel::LogError, "=========================================================================================\n");

#else

    PRUNUSED(full);
    
#endif
}


/// ---------------------------------------------------------------------------
/// MemoryFree
/// ---------------------------------------------------------------------------
u32 prStackHeap::MemoryFree() const
{
    return m_end - m_start;
}


/// ---------------------------------------------------------------------------
/// Mark
/// ---------------------------------------------------------------------------
void prStackHeap::Mark()
{
    PRASSERT(m_index < SH_STACK_SIZE);

    if (m_index < SH_STACK_SIZE)
    {
        m_stack[m_index++] = m_start;
    }
}


/// ---------------------------------------------------------------------------
/// GetMarkCount
/// ---------------------------------------------------------------------------
u32 prStackHeap::GetMarkCount() const
{
    return m_index;
}


/// ---------------------------------------------------------------------------
/// Size
/// ---------------------------------------------------------------------------
u32 prStackHeap::Size() const
{
    return m_heap_size;
}


/// ---------------------------------------------------------------------------
/// IsValidPointer
/// ---------------------------------------------------------------------------
bool prStackHeap::IsValidPointer(void* p)
{
    if (p)
    {
        // Get node address.
        u32 address = (u32)(u64)((u8*)p - sizeof(StackNode) - (m_bounds_check>>1));


        // Out of heap memory range?
        if (address < ((u32)(u64)m_heap) || address >= m_end)
        {
            return false;
        }


        // Is address the same as a node header?
        StackNode* node = (StackNode*)m_heap;

        while((u32)(u64)node < m_start)
        {
            if ((u32)(u64)node == address)
            {
                return true;
            }

            node = (StackNode*)((u8*)node + node->size);
        }
    }

    return false;
}


/// ---------------------------------------------------------------------------
/// BoundsCheckEnable
/// ---------------------------------------------------------------------------
void prStackHeap::BoundsCheckEnable(bool enable)
{
    if (m_start == (u32)((u64)m_heap))
    {
        m_bounds_check = enable ? BOUNDS_CHECK_SIZE : 0;
    }
    else
    {
        PRWARN("You can only enable bounds checking when a stack heap is empty.");
    }
}


/// ---------------------------------------------------------------------------
/// BoundsCheck
/// ---------------------------------------------------------------------------
void prStackHeap::BoundsCheck() const
{
    if (m_bounds_check)
    {
        // Get first node.
        StackNode* node = (StackNode*)m_heap;

        while((u32)(u64)node < m_start)
        {
            u32 *start_tag = (u32*)((u8*)node + sizeof(StackNode));
            u32 *end_tag   = (u32*)((u8*)node + node->size - BOUNDS_CHECK_ADJUST);

            if (*start_tag != BOUNDS_CHECK_TAG)
            {
                PRPANIC("Bounds check failed at block start.");
                return;
            }

            if (*end_tag != BOUNDS_CHECK_TAG)
            {
                PRPANIC("Bounds check failed at block end.");
                return;
            }

            node = (StackNode*)((u8*)node + node->size);
        }
    }
}


/// ---------------------------------------------------------------------------
/// IsBoundsCheckEnabled
/// ---------------------------------------------------------------------------
bool prStackHeap::IsBoundsCheckEnabled() const
{
    return (BOUNDS_CHECK_SIZE == m_bounds_check);
}
