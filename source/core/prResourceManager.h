// File: prResourceManager.h
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


#ifndef __PRRESOURCEMANAGER_H
#define __PRRESOURCEMANAGER_H


#include <list>
#include "prResource.h"
#include "prCoreSystem.h"


#define RESOURCE_TABLE_SIZE     64


// Class: prResourceManager
//      Resource management class
//
// Notes:
//      There will always be at least one resource manager, you
//      can however add more if you see fit
class prResourceManager : public prCoreSystem
{
public:

    // Method: prResourceManager
    //      Ctor
    prResourceManager();

    // Method: ~prResourceManager
    //      Dtor
    ~prResourceManager();

    // Method: Load
    //      Loads a resource into the resource manager.
    //
    // Parameters:
    //      filename - A char pointer to a filename string
    //      locked   - Is this a locked resource?
    //
    // Returns:
    //      A resource
    template<typename T>
    T* Load(const char *filename, bool locked = false, Proteus::Core::s32 extra = 0)
    {
        // Created resource?
        prResource *pRes = Find(filename);
        if (pRes)
        {
            pRes->IncrementReferenceCount();
            return (T*)pRes;
        }

        // Create new resource and store it.
        T *resource = new T(filename);
        if (resource)
        {
            resource->Load(extra);
            resource->Lock(locked);
            Add(resource);
        }

        return resource;
    }

    // Method: LoadFromMemory
    //      Creates a resource from data in memory. This is to
    //      allow embedded textures to be used
    //
    // Parameters:
    //      name   - A name for the resource
    //      pData  - A pointer to the resource
    //      size   - Size of the resource
    //
    // Returns:
    //      A resource
    template<typename T>
    T* LoadFromMemory(const char *name, void *pData, Proteus::Core::u32 size)
    {
        // Created resource?
        prResource *pRes = Find(name);
        if (pRes)
        {
            pRes->IncrementReferenceCount();
            return (T*)pRes;
        }

        // Create new resource and store it.
        T *resource = new T(name);
        if (resource)
        {
            resource->LoadFromMemory(pData, size);
            Add(resource);
        }

        return resource;
    }

    // Method: LoadFromRaw
    //      Creates a resource from raw data. This is to
    //      allow loaded images to be converted to textures
    //
    // Parameters:
    //      name   - A name for the resource
    //      pData  - A pointer to the resource
    //      size   - Size of the resource
    //      width  - Width of the raw image
    //      height - Height of the raw iamge
    //
    // Returns:
    //      A resource
    template<typename T>
    T* LoadFromRaw(const char *name, void *pData, Proteus::Core::u32 size, Proteus::Core::u32 width, Proteus::Core::u32 height)
    {
        // Create new resource and store it.
        T *resource = new T(name);
        if (resource)
        {
            resource->LoadFromRaw(pData, size, width, height);
            Add(resource);
        }

        return resource;
    }

    // Method: Unload
    //      Unloads a resource
    //
    // Parameters:
    //      resource - A pointer to a resource
    void Unload(prResource *resource);

    // Method: Find
    //      Finds a resource by name.
    //
    // Parameters:
    //      filename - A char pointer to a filename string
    //
    // Returns:
    //      A resource or NULL
    prResource *Find(const char *filename);
    
    // Method: DisplayUsage
    //      Shows all entries
    void DisplayUsage();

    // Method: Count
    //      Gets the number of entries
    //
    // Returns:
    //      The number of resources stored
    Proteus::Core::u32 Count() const;

    // Method: Clear
    //      Clears the resource manager of all entries
    //
    // Notes:
    //      This will clear locked resources, so beware
    void Clear();


private:

    // Method: Add
    //      Adds a resource
    void Add(prResource *resource);

    /// @brief      Adds a resource
    /*void ClearLocked();*/


private:

    std::list<prResource*>    m_resources[RESOURCE_TABLE_SIZE];
};


#endif//__PRRESOURCEMANAGER_H
