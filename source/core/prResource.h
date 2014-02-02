/**
 * @file       prResource.h
 * @brief      This class represents an entry in the resource manager.
 */
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


/**
 * @brief      Class which represents an engine resource.
 *
 * @n          All resources need to be derived from this class
 */
class prResource
{
public:
    // Friend
    friend class prResourceManager;


    /**
     * @brief      Ctor
     *
     * @param      filename - A char pointer to the filename
     */
    prResource(const char *filename);

    /**
     * @brief      Dtor
     */
    virtual ~prResource();

    /**
     * @brief      Gets the resource filename
     *
     * @return     The filename as a char pointer
     */
    const char *Filename() const { return m_filename; }

    /**
     * @brief      Gets the resouces locked status
     *
     * @return     true or false
     */
    bool IsLocked() const { return m_locked; }
    
    /**
     * @brief      Gets the resouces hash value
     *
     * @return     The filename as a number
     */
    const u32 Hash() const { return m_hash; }

    /**
     * @brief      Gets the resouces file size
     *
     * @return     The resouces size
     */
    const u32 Size() const { return m_size; }


private:

    /**
     * @brief      Load resource
     */
    virtual void Load()   = 0;

    /**
     * @brief      Unload resource
     */
    virtual void Unload() = 0;

    /**
     * @brief      Gets the number of references to this resource
     *
     * @return     The reference count for this resource
     */
    const u32 References() const { return m_references; }

    /**
     * @brief      Sets reference count up
     */
    void IncrementReferenceCount() { m_references++; }

    /**
     * @brief      Sets reference count down
     */
    void DecrementReferenceCount() { m_references--; }

    /**
     * @brief      Used to lock a resource. 
     * @n          A locked resource cannot be deleted with unload when the reference count
     * @n          is zero. Useful for when you need a resource to be loaded permanently
     *
     * @param      state - true or false
     */
    void Lock(bool state) { m_locked = state; }


protected:

    /**
     * @brief      Used to set the resources size.
     *
     * @param      size - The size in bytes
     */
    void SetSize(u32 size) { m_size = size; }


private:

    char    m_filename[RESOURCE_FILENAME_SIZE];
    u32     m_hash;
    u32     m_references;
    u32     m_size;
    bool    m_locked;
    bool    m_exp0;
    bool    m_exp1;
    bool    m_exp2;
};


#endif//__PRRESOURCE_H
