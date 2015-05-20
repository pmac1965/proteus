/**
 * prResourceManager.cpp
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


#include "prResourceManager.h"
#include "prStringUtil.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "prCore.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prResourceManager::prResourceManager() : prCoreSystem(PRSYSTEM_RESOURCEMANAGER, "prResourceManager")
{
    TODO("Perhaps add handles to resource manager? Access pointers by handles?")
    TODO("Perhaps give priorities to resources? for automatic unload/load of resources")
    TODO("Add more binary data types, so Less xml parsing for the bigger games to improve loading times")
    TODO("Add ignore to assert message")
    TODO("Add ignore to warning message")
    TODO("Add clipboard trick - page 111 to 113 gems 1")
    TODO("Complete quaternion")
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prResourceManager::~prResourceManager()
{
    Clear();
}


/// ---------------------------------------------------------------------------
/// Unloads a resource
/// ---------------------------------------------------------------------------
void prResourceManager::Unload(prResource *resource)
{
    PRASSERT(resource);

    if (resource->References() > 1)
    {
        resource->DecrementReferenceCount();
    }
    else
    {
        if (resource->IsLocked())
        {
            return;
        }

        u32 hash = resource->Hash();

        std::list<prResource*>& list = m_resources[hash % RESOURCE_TABLE_SIZE];

        list.remove(resource);

        resource->Unload();

        delete resource;
    }
}


/// ---------------------------------------------------------------------------
/// Finds a resource by name.
/// ---------------------------------------------------------------------------
prResource *prResourceManager::Find(const char *filename)
{
    PRASSERT(filename && *filename);
    
    // Create hash from constant filename format
    char buffer[RESOURCE_FILENAME_SIZE];
    prStringCopySafe(buffer, filename, sizeof(buffer));
    prStringToLower(buffer);
    prStringReplaceChar(buffer, '\\', '/');
    u32 hash = prStringHash(buffer);


    // Search list for entry.
    std::list<prResource*>& list = m_resources[hash % RESOURCE_TABLE_SIZE];
    if (!list.empty())
    {
        std::list<prResource*>::iterator it  = list.begin();
        std::list<prResource*>::iterator end = list.end();

        for (; it != end; ++it)
        {
            prResource* resource = *it;
            PRASSERT(resource);
            if (resource->Hash() == hash)
            {
                return resource;
            }
        }
    }

    return NULL;
}


/// ---------------------------------------------------------------------------
/// Debug assist
/// ---------------------------------------------------------------------------
void prResourceManager::DisplayUsage()
{
#if defined(_DEBUG) || defined(DEBUG)

    prTrace("\nResource Manager: ==============================================================\n");

    u64 size = 0;

    for (s32 i=0; i<RESOURCE_TABLE_SIZE; i++)
    {
        std::list<prResource*>& list = m_resources[i];
        if (list.size() > 0)
        {
            std::list<prResource*>::iterator it  = list.begin();
            std::list<prResource*>::iterator end = list.end();

            for (; it != end; ++it)
            {
                PRASSERT(*it);
                prResource& resource = **it;

                prTrace
                (
                    "Table: %03i, Hash: %08x, Refs %03i, Locked %s, Size: %*i, File: %s\n",
                    i,                                                  // Hash table index
                    resource.Hash(),                                    // Hash key
                    resource.References(),                              // References
                    resource.IsLocked() ? "Yes" : "No ",                // Locked status
                    10, resource.Size(),                                // prResource size.
                    resource.Filename()                                 // Entries file name
                );

                size += resource.Size();
            }
        }
    }

    prTrace("------------------\n");
    prTrace("Number of entries: %i, Total size: %.02fKB or %.02fMB\n", Count(), ((float)size)/1024, ((float)size)/1024/1024);
    prTrace("Note: The total size is approximate\n");
    prTrace("================================================================================\n");

#endif
}


/// ---------------------------------------------------------------------------
/// Counts the number of entries
/// ---------------------------------------------------------------------------
u32 prResourceManager::Count() const
{
    u32 count = 0;

    for (s32 i=0; i<RESOURCE_TABLE_SIZE; i++)
    {
        count += (u32)m_resources[i].size();
    }

    return count;
}


/// ---------------------------------------------------------------------------
/// Clears the resource manager of all entries
/// ---------------------------------------------------------------------------
void prResourceManager::Clear()
{
    prResource *resource;

    for (s32 i=0; i<RESOURCE_TABLE_SIZE; i++)
    {
        std::list<prResource*>& list = m_resources[i];

        if (!list.empty())
        {
            std::list<prResource*>::iterator it  = list.begin();
            std::list<prResource*>::iterator end = list.end();

            for (; it != end; ++it)
            {
                resource = *it;
                PRASSERT(resource);
                resource->Unload();
                delete resource;
            }

            list.clear();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Adds a resource to the manager.
/// ---------------------------------------------------------------------------
void prResourceManager::Add(prResource *resource)
{
    PRASSERT(resource);

    u32 hash = resource->Hash();

    std::list<prResource*>& list = m_resources[hash % RESOURCE_TABLE_SIZE];

    list.push_back(resource);
}
