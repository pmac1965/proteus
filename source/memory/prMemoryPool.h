// File: prMemoryPool.h
//      Simple memory pool (Pool of objects)
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


#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"
#include "../core/prTypes.h"
#include "../core/prDefines.h"


// Class: prMemoryPool
//      A simple memory pool implementation.
template<typename T>
class prMemoryPool
{
public:
    // Method: prMemoryPool
    //      Constructor
    //
    // Parameters:
    //      size    - The size of the pool in objects
    //      name    - An optional name for debug purposes
    prMemoryPool(Proteus::Core::s32 size, const char* name = 0) : m_name(name)
    {
        PRASSERT(size > 0);
        Create(size);
    }
    
    // Method: ~prMemoryPool
    //      Destructor.
    ~prMemoryPool()
    {
        Destroy();
    }

    // Method: Pop
    //      Acquires an object from the pool.
    //
    // Returns:
    //      An object or NULL if the pool is empty
    T *Pop()
    {
        if (m_index > 0)
        {
            return m_objects[--m_index];
        }

        prTrace(prLogLevel::LogError, "Failed to pop object from pool\n");
        return 0;
    }
    
    // Method: Push
    //      Returns an object to the pool.
    //
    // Notes:
    //      The object pointer cannot be NULL
    void Push(T *object)
    {
        PRASSERT(object);

        if (object && m_index < m_size)
        {
            m_objects[m_index++] = object;
        }   
        else
        {
            prTrace(LogError, "Failed to push object onto pool\n");
        }
    }

    // Method: Reset
    //      Resets the pool back to its initial state.
    //
    // Notes:
    //      All previously acquired pointers should be considered invalid
    void Reset()
    {
        for (Proteus::Core::s32 i=0; i<m_size; ++i)
        {
            m_objects[i] = &m_pool[i];
        }

        m_index = m_size;
    }

    // Method: GetSize
    //      Returns the size of the pool in objects, *not* bytes.
    Proteus::Core::s32 GetSize() const { return m_size;}
    
    // Method: GetFree
    //      Returns the number of free objects in the pool.
    Proteus::Core::s32 GetFree() const { return m_index;}

    // Method: GetUsed
    //      Returns the number of used objects in the pool.
    Proteus::Core::s32 GetUsed() const { return m_size - m_index;}

    // Method: DisplayUsage
    //      Displays memory pool status information.
    void DisplayUsage() const
    {
        #if defined(_DEBUG) || defined(DEBUG)
    
        prTrace(LogError, "\n");
        prTrace(LogError, "---------------------------------------------------------------\n");
        prTrace(LogError, "Memory pool: (%s)\n", (m_name && *m_name) ? m_name : "unnamed");
        prTrace(LogError, "---------------------------------------------------------------\n");
        prTrace(LogError, "Size in objects: %i\n", GetSize());
        prTrace(LogError, "Size in bytes  : %i\n", GetSize() * sizeof(T));
        prTrace(LogError, "Free objects   : %i\n", GetFree());
        prTrace(LogError, "Used objects   : %i\n", GetUsed());
        prTrace(LogError, "---------------------------------------------------------------\n");
        
        #endif
    }


private:
    // Creates the pool.
    void Create(Proteus::Core::s32 size)
    {
        // Create object pool.
        m_pool = new T [size];
        PRASSERT(m_pool);

        // Create pointer list.
        m_objects = new T* [size];
        PRASSERT(m_objects);

        // Fill in the pointer list.
        for (Proteus::Core::s32 i=0; i<size; ++i)
        {
            m_objects[i] = &m_pool[i];
        }

        m_index = size;
        m_size  = size;
    }

    // Destroys the pool.
    void Destroy()
    {
        PRSAFE_DELETE_ARRAY(m_pool);
        PRSAFE_DELETE_ARRAY(m_objects);
        m_index = 0;
        m_size  = 0;
    }


protected:
    const char         *m_name;     // A name used to uniquely identify a pool during debugging.
    T*                  m_pool;     // The actual memory pool.
    T**                 m_objects;  // Pointers to the objects in the memory pool.
    Proteus::Core::s32  m_index;    // Index of the last usable object. (Range 1 to size)
    Proteus::Core::s32  m_size;     // The size of the memory pool
};
