/**
 * @file       prFileManager.h
 * @brief      Contains a class used to abstract file management.
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


#ifndef __PRFILEMANAGER_H
#define __PRFILEMANAGER_H


#include "prFileShared.h"
#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations
class prFile;


/// @brief      Class used to abstract file management.
/// @note       
class prFileManager : public prCoreSystem
{
public:
    /// Ctor
    prFileManager();

    /// Dtor
    ~prFileManager();

    /// Register an archive.
    void RegisterArchive(const char *filename);

    /// Returns the filepath for the current platform.
    const char *GetSystemPath(const char *filename);

    /// Let the file system know we're done registering archives.
    void SetRegistrationComplete();

    /// Ready to use?
    bool Ready() const;

    /// How many archives loaded.
    int ArchiveCount() const;

    /// Does the file exist
    bool Exists(const char *filename, u32 &size);

    /// Read a file.
    u32 Read(u8 *pDataBuffer, u32 size, u32 hash);

private:

    char        dataPath[FILE_MAX_FILENAME_SIZE];               // Path to the data directory
    char        path[FILE_MAX_FILENAME_SIZE];                   // Used for returning paths
    u32         count;                                          // Number of registered archives.
    bool        ready;                                          // Indicates all archives registered
    bool        exp2;                                           // Expansion use
    bool        exp1;                                           // Expansion use
    bool        exp0;                                           // Expansion use
    prFile     *pArchiveFile[FILE_ARCHIVES_MAX];                // Archive files
    prArcEntry *pEntries[FILE_ARCHIVES_MAX];                    // Entry tables
    u32         entryCount[FILE_ARCHIVES_MAX];                  // Entry count
    s32         table;                                          // Table in which the last file looked for was found.
    s32         index;                                          // Index of the last file found in its table
    u32         filehash;                                       // Hash of the last file found
};


#if defined(PLATFORM_ANDROID)
/// Sets the APK path.
void SetAPKPath(const char *path);

/// Sets the card path.
void SetCardPath(const char *path);

/// Gets the card path.
const char *GetCardPath();

/// Get access to the android data zip file.
zip *GetAPKArchive();
#endif


#endif//__PRFILEMANAGER_H
