/**
 * @file       prFile.h
 * @brief      Contains a class used to represent a file.
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


#ifndef __PRFILE_H
#define __PRFILE_H


#include "../core/prTypes.h"


// Forward declarations
struct FileImplementation;


/**
 * Class used to represent a file in a cross platform manner
 */
class prFile
{
public:

    friend class prFileManager;

    /**
     * Ctor
     */
    prFile(const char *filename);

    /**
     *  Dtor
     */
    ~prFile();

    /// Gets the size of the file.
    u32 Size() const;

    /// Determines if the file exists
    bool Exists();

    /// Opens a file for reading.
    bool Open();
    
    /// Closes a file
    void Close();
    
    /// Reads data from an open file
    u32 Read(void *pDataBuffer, u32 size);

    /// Seek within an open file.
    void Seek(s32 offset, s32 origin);

    /// Returns the file pointers position.
    s32 Tell() const;

    /// Sets the file pointer back to the start of the file.
    void Rewind();


private:

    // For engine use.
    void Internal_Rewind();
    void Internal_Seek(s32 offset, s32 origin);
    s32  Internal_Tell() const;
    //bool Internal_FileDataLoaded();
    //void Internal_SetFileData(u8 *pData);


    // Stops passing by value and assignment.
    prFile(const prFile&);
    const prFile& operator = (const prFile&);


private:

    // Don't change order.
    FileImplementation  *pImpl;
    FileImplementation  &imp;
};


#endif//__PRFILE_H
