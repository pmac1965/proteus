/**
 * @file       prResourceManager.h
 * @brief      Contains a resource management class.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRRESOURCEMANAGER_H
#define __PRRESOURCEMANAGER_H


#include <list>
#include "prResource.h"
#include "prCoreSystem.h"


#define RESOURCE_TABLE_SIZE     64


/// @brief      Resource management class
/// @n          There will always be at least one resource manager.
class prResourceManager : public prCoreSystem
{
public:

    /// @brief      Ctor
    prResourceManager();

    /// @brief      Dtor
    ~prResourceManager();

    /// @brief      Loads a resource into the resource manager.
    /// @param      filename - A char pointer to a filename string
    /// @param      locked   - Is this a locked resource?
    /// @return     A resource
    template<typename T>
    T* Load(const char *filename, bool locked = false)
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
            resource->Load();
            resource->Lock(locked);
            Add(resource);
        }

        return resource;
    }

    /// @brief      Creates a resource from data in memory.
    /// @param      name   - A name for the resource
    /// @param      pData  - A pointer to the resource
    /// @param      size   - Size of the resource
    /// @return     A resource
    template<typename T>
    T* LoadFromMemory(const char *name, void *pData, u32 size)
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

    /// @brief      Unloads a resource
    /// @param      resource - A pointer to a resource
    void Unload(prResource *resource);

    /// @brief      Finds a resource by name.
    /// @param      filename - A char pointer to a filename string
    /// @return     A resource or NULL
    prResource *Find(const char *filename);
    
    /// @brief      Debug assist
    void DisplayUsage();

    /// @brief      Gets the number of entries
    /// @return     The number of resources stored
    u32 Count() const;

    /// @brief      Clears the resource manager of all entries
    void Clear();


private:

    /// @brief      Adds a resource
    void Add(prResource *resource);

    /// @brief      Adds a resource
    /*void ClearLocked();*/


private:

    std::list<prResource*>    m_resources[RESOURCE_TABLE_SIZE];
};


#endif//__PRRESOURCEMANAGER_H
