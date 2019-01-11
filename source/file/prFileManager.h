// File: prFileManager.h
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


#include "prFileShared.h"
#include "../prConfig.h"
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations
class prFile;
struct zip;


// Class: prFileManager
//      Class used to abstract file management.
class prFileManager : public prCoreSystem
{
public:
    // Method: prFileManager
    //      Ctor
    explicit prFileManager(const char *saveFolder);

    // Method: ~prFileManager
    //      Dtor
    ~prFileManager();

    // Method: RegisterArchive
    //      Registers an archive.
    void RegisterArchive(const char *filename);

    // Method: GetSystemPath
    //      Returns the filepath for the current platform.
    //
    // Parameters:
    //      filename - A filename to convert
    const char *GetSystemPath(const char *filename);

    // Method: SetRegistrationComplete
    //      Let the file system know we're done registering archives.
    void SetRegistrationComplete();

    // Method: Ready
    //      Ready to use?
    bool Ready() const;

    // Method: ArchiveCount
    //      How many archives loaded?
    int ArchiveCount() const;

    // Method: Exists
    //      Does the file exist?
    bool Exists(const char *filename, u32 &size);

    // Method: Read
    //      Read a file.
    u32 Read(u8 *pDataBuffer, u32 size, u32 hash);

#if defined(PLATFORM_ANDROID)
    // Method: Read
    //      Read a file.
    u32 Read(u8 *pDataBuffer, u32 size, const char *filename);
#endif

    // Method: DisplayFiles
    //      Displays all files with the access state passed. This
    //      allows all files which have and haven't been accessed to
    //      be displayed, so you can trim unused data if required
    //
    // Notes:
    //      For any build which uses the archives. (PC, LINUX, IOS, MAC)
    //
    // Notes:
    //      Only works if archives are loaded and being used as this method uses
    //      the archive entries
    void DisplayFiles(bool accessed);

    // Method: GetSaveDataPath()
    //      Gets the path to the folder that all game save data is loaded from
    //      and save to.
    const char *GetSaveDataPath() const { return mSaveDataPath; }


private:
    // This function looks in all registered archives for the file.
    // Returns the last file found as code assumes later archives
    // will contain updated files
    bool Exists(u32 hash, u32 &size);


private:
    char        mSaveDataPath[FILE_MAX_FILENAME_SIZE];          // Path to the save data directory (Used by multiple systems)
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
// Method: prSetAPKPath
//      Sets the APK path.
//
// Notes:
//      *Android only*
void prSetAPKPath(const char *path);

// Method: prSetCardPath
//      Sets the card path.
//
// Notes:
//      *Android only*
void prSetCardPath(const char *path);

// Method: prSetSavePath
//      Sets the save path for an android device.
//
// Notes:
//      *Android only*
void prSetSavePath(const char *path);

// Method: prGetCardPath
//      Gets the card path.
//
// Notes:
//      *Android only*
const char *prGetCardPath();

// Method: prGetSavePath
//      Gets the save path for an android device.
//
// Notes:
//      *Android only*
const char *prGetSavePath();

// Method: prGetAPKArchive
//      Get access to the android data zip file.
//
// Notes:
//      *Android only*
zip *prGetAPKArchive();
#endif
