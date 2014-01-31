/**
 * @file       prResource.h
 * @brief      This class represents an entry in the resource manager.
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


#ifndef __PRRESOURCE_H
#define __PRRESOURCE_H


#include "prTypes.h"


#define RESOURCE_FILENAME_SIZE      260


/// @brief      Class which represents an engine resource.
/// @n          All renderers will implement the basic core functionality
class prResource
{
public:
    // Friend
    friend class prResourceManager;


    /// @brief      Ctor
    /// @param      filename - A char pointer to the filename
    prResource(const char *filename);

    /// @brief      Dtor
    virtual ~prResource();

    /// @brief      Gets the resource filename
    /// @return     The filename as a char pointer
    const char *Filename() const { return m_filename; }

    /// @brief      Gets the resouces locked status
    /// @return     true or false
    bool IsLocked() const { return m_locked; }
    
    /// @brief      Gets the resouces hash value
    /// @return     The filename as a number
    const u32 Hash() const { return m_hash; }

    /// @brief      Gets the resouces file size
    /// @return     The resouces size
    const u32 Size() const { return m_size; }


private:

    /// @brief      Load resource
    virtual void Load()   = 0;

    /// @brief      Unload resource
    virtual void Unload() = 0;

    /// @brief      Gets the number of references to this resource
    /// @return     The reference count for this resource
    const u32 References() const { return m_references; }

    /// @brief      Sets reference count up
    void IncrementReferenceCount() { m_references++; }

    /// @brief      Sets reference count down
    void DecrementReferenceCount() { m_references--; }

    /// @brief      Used to lock a resource. 
    /// @n          A locked resource cannot be deleted with unload when the reference count
    /// @n          is zero. Useful for when you need a resource to be loaded permanently
    /// @param      state - true or false
    void Lock(bool state) { m_locked = state; }


protected:

    /// @brief      Used to set the resources size.
    /// @param      size - The size in bytes
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
