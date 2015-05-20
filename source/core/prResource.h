// File: prResource.h
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


#ifndef __PRRESOURCE_H
#define __PRRESOURCE_H


#include "prTypes.h"


#define RESOURCE_FILENAME_SIZE      260


// Class: prResource
//      Class which represents an engine resource.
//
// Notes:
//      All resources need to be derived from this class
class prResource
{
public:
    // Friend
    friend class prResourceManager;


    // Method: prResource
    //      Ctor
    //
    // Parameters:
    //      filename - A char pointer to the filename
    prResource(const char *filename);

    // Method: ~prResource
    //      Dtor
    virtual ~prResource();

    // Method: Filename
    //      Gets the resources filename
    const char *Filename() const { return m_filename; }

    // Method: Filename
    //      Gets the resouces locked status
    bool IsLocked() const { return m_locked; }
    
    // Method: Hash
    //      Gets the resouces hash value
    const Proteus::Core::u32 Hash() const { return m_hash; }

    // Method: Size
    //      Gets the resouces file size
    const Proteus::Core::u32 Size() const { return m_size; }


private:

    // Method: Load
    //      Load resource
    virtual void Load(Proteus::Core::s32 extra) = 0;

    // Method: Unload
    //      Unload resource
    virtual void Unload() = 0;

    // Method: References
    //      Gets the number of references to this resource
    const Proteus::Core::u32 References() const { return m_references; }

    // Method: IncrementReferenceCount
    //      Sets reference count up
    void IncrementReferenceCount() { m_references++; }

    // Method: DecrementReferenceCount
    //      Sets reference count down
    void DecrementReferenceCount() { m_references--; }

    // Method: Lock
    //      Used to lock a resource. 
    //
    // Notes:
    //      A locked resource cannot be deleted with <Unload> when the reference count
    //      reaches zero. Useful for when you need a resource to be loaded permanently
    //
    // Parameters:
    //      state - true or false
    void Lock(bool state) { m_locked = state; }


protected:

    // Method: SetSize
    //      Used to set the resources size.
    //
    // Parameters:
    //      size - The size in bytes
    void SetSize(Proteus::Core::u32 size) { m_size = size; }


private:

    char                m_filename[RESOURCE_FILENAME_SIZE];
    Proteus::Core::u32  m_hash;
    Proteus::Core::u32  m_references;
    Proteus::Core::u32  m_size;
    bool                m_locked;
    bool                m_exp0;
    bool                m_exp1;
    bool                m_exp2;
};


#endif//__PRRESOURCE_H
