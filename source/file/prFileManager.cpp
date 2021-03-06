/**
 * prFileManager.cpp
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


#include "../prConfig.h"


#include <string.h>
#include "prFileManager.h"
#include "prFile.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prCore.h"
#include "../core/prCoreSystem.h"
#include "../core/prStringUtil.h"
#include "../file/prFileShared.h"
#include "../zlib/zlib.h"


// Debug assist
#if defined(_DEBUG) || defined(DEBUG)
//#define FILEMANAGER_DEBUG
//#define FILEMANAGER_SHOW_DATA_PATH
#endif


// Platform specific includes.
#if defined(PLATFORM_PC)  
  #include <windows.h>

#elif defined(PLATFORM_IOS)  
  #include <stdlib.h>
  #include "../ios/prIos.h"

#elif defined(PLATFORM_MAC)
  #include <stdlib.h>

#elif defined(PLATFORM_LINUX)
  #include <stdlib.h>
  #include <unistd.h>

#ifndef MAX_PATH
#define MAX_PATH 256
#endif


#elif defined(PLATFORM_ANDROID)
  #include <stdlib.h>
  #include <libzip\zip.h>
  #include <libzip\zipint.h>

  //#define ANDROID_APK_DEBUG
  //#define ANDROID_APK_DEBUG_SHOW_CONTENTS

  namespace
  {
    char APKPath [FILE_MAX_FILENAME_SIZE] = { '\0' };
    char CardPath[FILE_MAX_FILENAME_SIZE] = { '\0' };
    char SavePath[FILE_MAX_FILENAME_SIZE] = { '\0' };
    zip *APKArchive = nullptr;
  }

#else
  #error No platform defined.

#endif


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Creates a path to the applications data.
/// ---------------------------------------------------------------------------
prFileManager::prFileManager(const char *saveFolder) : prCoreSystem(PRSYSTEM_FILEMANAGER, "prFileManager")
{
    PRASSERT(saveFolder && *saveFolder, "Save location missing");

    // Init data
    count    = 0;
    ready    = false;
    exp0     = false;
    exp1     = false;
    exp2     = false;
    table    = -1;
    index    = -1;
    filehash = 0xFFFFFFFF;

    for (int i=0; i<FILE_ARCHIVES_MAX; i++)
    {
        pArchiveFile[i] = nullptr;
        pEntries[i]     = nullptr;
        entryCount[i]   = 0;
    }

    memset(path, 0, sizeof(path));
    memset(dataPath, 0, sizeof(dataPath));
    memset(mSaveDataPath, 0, sizeof(mSaveDataPath));

    // Copy the save data path
    strcpy(mSaveDataPath, saveFolder);

    // IOS
    #if defined(PLATFORM_IOS)    
        // Copy the app path.
        strcpy(dataPath, prGetAppDataPath());

    // MAC
    #elif defined(PLATFORM_MAC)
        // Copy the app path.
        //strcpy(dataPath, prGetAppDataPath());

    // PC
    #elif defined(PLATFORM_PC)    
        // If we're using the debugger then use the current directory as it will point to the
        // debug directory path, else use the executables path as the data path.
        if (IsDebuggerPresent() != FALSE)
        {
            GetCurrentDirectoryA(FILE_MAX_FILENAME_SIZE, dataPath);
        }
        else
        {
            // Remove the executables name
            GetModuleFileNameA(nullptr, dataPath, FILE_MAX_FILENAME_SIZE);
            int index = prStringFindLastIndex(dataPath, '\\');
            dataPath[index] = 0;
        }

    // Android
    #elif defined(PLATFORM_ANDROID)
        // Copy the app path.
        strcpy(dataPath, "assets");

    // Linux
    #elif defined(PLATFORM_LINUX)
        // Copy the app path.
        char buffer[MAX_PATH];
        getcwd(buffer, MAX_PATH -1);
        strcpy(dataPath, buffer);

    #else
        #error No platform defined.

    #endif

    // Ensure same slashes
    prStringReplaceChar(dataPath, '\\', '/');


    #ifdef FILEMANAGER_SHOW_DATA_PATH
    prTrace(LogError, "App data path: %s\n", dataPath);
    #endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFileManager::~prFileManager()
{
    for (int i=0; i<FILE_ARCHIVES_MAX; i++)
    {
        // Close and delete file
        if (pArchiveFile[i] != nullptr)
        {
            pArchiveFile[i]->Close();
            PRSAFE_DELETE(pArchiveFile[i]);
        }

        // Delete entry table
        PRSAFE_DELETE(pEntries[i]);
    }
}


/// ---------------------------------------------------------------------------
/// Register an archive.
/// ---------------------------------------------------------------------------
void prFileManager::RegisterArchive(const char *filename)
{
#if !defined(PLATFORM_ANDROID)

    #if !defined(REMOVE_EXCEPTIONS)
    try
    #endif
    {
        PRASSERT(filename && *filename);


        // Create the filenames
        char filenameArc[FILE_MAX_FILENAME_SIZE];
        char filenameFat[FILE_MAX_FILENAME_SIZE];

        prStringCopySafe(filenameArc, filename, sizeof(filenameArc));
        prStringCopySafe(filenameFat, filename, sizeof(filenameFat));
        prStringAddString(".arc", filenameArc);
        prStringAddString(".fat", filenameFat);

        
        // Do the files exist?
        prFile *fileArc = new prFile(filenameArc);
        prFile *fileFat = new prFile(filenameFat);
        if (fileArc->Exists() && fileFat->Exists())
        {
            fileArc->Open();
            fileFat->Open();
            u32 sizeArc = fileArc->Size();
            u32 sizeFat = fileFat->Size();

            // Read the fat?
            if (sizeFat > sizeof(prFatHeader))
            {
                prFatHeader header;
                fileFat->Read(&header, sizeof(prFatHeader));

                // Validate the header
                if (header.magic1 == MAGIC1 && header.magic2 == MAGIC2 && header.size == sizeFat)
                {
                    // See if the size matches entry count
                    u32 size = (header.entries * sizeof(prArcEntry)) + sizeof(prFatHeader);

                    if (size == sizeFat)
                    {
                        // Check the archive is larger than 0.
                        if (sizeArc > 0)
                        {
                            // Read the fat table.
                            u32 tableSize = header.entries * sizeof(prArcEntry);

                            prArcEntry *entries = new prArcEntry[header.entries];

                            fileFat->Read(entries, tableSize);


                            // Store table
                            bool found = false;
                            int  idx = 0;
                            for (int i=0; i<FILE_ARCHIVES_MAX; i++)
                            {
                                if (pEntries[i] == nullptr && pArchiveFile[i] == nullptr)
                                {
                                    idx   = i;
                                    found = true;
                                    break;
                                }
                            }


                            if (found)
                            {
                                // Open file for data access, store entries and indicate we have an archive open for use.
                                pArchiveFile[idx] = new prFile(filenameArc);
                                pEntries    [idx] = entries;
                                entryCount  [idx] = header.entries;
                                count++;

                                pArchiveFile[idx]->Open();
                                
                                // Clear the extra data, as we need it to do accessed tracking.
                                for(u32 i=0; i<header.entries; i++)
                                {
                                    prArcEntry *pE = pEntries[idx];
                                    pE += i;
                                    pE->accessed = 0;
                                    pE->exp1     = 0;
                                }

                                #if defined(FILEMANAGER_DEBUG)
                                prTrace(LogError, "Found archive: %s\n", filenameArc);
                                for(u32 i=0; i<header.entries; i++)
                                {
                                    prArcEntry *pE = pEntries[idx];
                                    pE += i;
                                    prTrace(LogError, "File: %s - %x - %s - %s\n", pE->filename, pE->hash, PRBOOL_TO_STRING(pE->accessed), PRBOOL_TO_STRING(pE->exp1));
                                }
                                #endif
                            }
                            else
                            {
                                PRSAFE_DELETE_ARRAY(entries);
                                PRWARN("Too many archives. Unable to store archive.");
                            }
                        }
                        else
                        {
                            PRWARN("Archive has no data. File is zero length: %s", filename);
                        }
                    }
                    else
                    {
                        PRWARN("Archive size does not match entry count.");
                    }
                }
                else
                {
                    PRWARN("Archive file has invalid header: %s", filename);
                }
            }
            else
            {
                PRWARN("Invalid archive file: %s", filename);
            }

            fileArc->Close();
            fileFat->Close();
        }
        else
        {
            prTrace(prLogLevel::LogError, "Failed to locate: %s\n", filenameArc);
        }

        PRSAFE_DELETE(fileArc);
        PRSAFE_DELETE(fileFat);
    }
    #if !defined(REMOVE_EXCEPTIONS)
    catch(...)
    {
        PRWARN("Failed to register archive.");
    }
    #endif

#else

    if (APKArchive == nullptr)
    {
        APKArchive = zip_open(APKPath, 0, nullptr);
        if (APKArchive == nullptr)
        {
            PRPANIC("Error loading APK");
        }
        else
        {
            prTrace(LogError, "Opened APK: %s\n", APKPath);
        }

        // Print the APK's contents
        #if defined(ANDROID_APK_DEBUG) && defined(ANDROID_APK_DEBUG_SHOW_CONTENTS)
        if (APKArchive)
        {
            int numFiles = zip_get_num_files(APKArchive);
            for (int i=0; i<numFiles; i++)
            {
                const char *name = zip_get_name(APKArchive, i, 0);
                if (name == nullptr)
                {
                    PRPANIC("Error reading zip file name at index %i : %s", i, zip_strerror(APKArchive));
                }
                prTrace(LogError, "File %i, %s\n", i, name);
            }
        }
        #endif
    }    

#endif
}


/// ---------------------------------------------------------------------------
/// Returns the filepath for the current platform.
/// ---------------------------------------------------------------------------
const char *prFileManager::GetSystemPath(const char *filename)
{
    PRASSERT(filename && *filename);

    // Got a system data path?
    if (dataPath[0] != '\0')
    {
        // Check for buffer overflow.
        #if defined(_DEBUG) || defined(DEBUG)
        {            
            int total = (int)(strlen(dataPath) + 1 + strlen(filename));
            PRASSERT(total < FILE_MAX_FILENAME_SIZE - 1);
        }
        #endif


        // For PC return the full path
        #if defined(PLATFORM_PC)        
            // Make filename for current system.
            // For PC its the data path + '/' + filepath
            strcpy(path, dataPath);
            strcat(path, "/");            
            strcat(path, filename);

        // For iphone
        #elif defined(PLATFORM_IOS)
            // Make filename for current system.
            // For iphone its the data path + '/' + filename (path data needs to be stripped)
            strcpy(path, dataPath);
            strcat(path, "/");

            // Remove the passed filename path data.
            char tempPath[FILE_MAX_FILENAME_SIZE];
            strcpy(tempPath, filename);

            prStringReplaceChar(tempPath, '\\', '/');
            int index = prStringFindLastIndex(tempPath, '/');
            if (index > -1)
            {
                strcat(path, &tempPath[index + 1]);
            }
            else
            {
                strcat(path, tempPath);
            }

        #elif defined(PLATFORM_ANDROID)
            // Make path
            strcpy(path, "assets/");
            strcat(path, filename);

        #elif defined(PLATFORM_LINUX)
            // Make filename for current system.
            // Its the data path + '/' + filepath
            strcpy(path, dataPath);
            strcat(path, "/");
            strcat(path, filename);
        
        #elif defined(PLATFORM_MAC)
            // Make filename for current system.
            // Its the data path + '/' + filepath
            strcpy(path, dataPath);
            strcat(path, "/");
            strcat(path, filename);

        #else
            #error No platform defined.
        
        #endif
    }
    else
    {
        strcpy(path, filename);
    }


    // Must be lowercase
    #if !defined (PLATFORM_IOS)
    prStringToLower(path);
    #endif


    // And use forward slashes
    prStringReplaceChar(path, '\\', '/');
    return path;
}


/// ---------------------------------------------------------------------------
/// Let the file system know we're done registering archives.
/// ---------------------------------------------------------------------------
void prFileManager::SetRegistrationComplete()
{
#if !defined(PLATFORM_ANDROID)
    ready = true;
#endif
}


/// ---------------------------------------------------------------------------
/// Ready to use?
/// ---------------------------------------------------------------------------
bool prFileManager::Ready() const
{
    return ready;
}


/// ---------------------------------------------------------------------------
/// How many archives loaded.
/// ---------------------------------------------------------------------------
int prFileManager::ArchiveCount() const
{
    return count;
}


/// ---------------------------------------------------------------------------
/// Does the file exist
/// ---------------------------------------------------------------------------
bool prFileManager::Exists(const char *filename, u32 &size)
{
    PRASSERT(filename && *filename);

#if !defined(PLATFORM_ANDROID)
    
    u32  hash   = prStringHash(filename);
    bool result = Exists(hash, size);
    if (!result)
    {
        prTrace(prLogLevel::LogError, "Failed to find file in archive: %s\n", filename);
    }

#else

    bool result = false;

    if (APKArchive)
    {
        zip_file *file = zip_fopen(APKArchive, GetSystemPath(filename), 0);
        if (file)
        {
            result = true;
            size   = file->bytes_left;
            zip_fclose(file);

            #if defined(ANDROID_APK_DEBUG)
            prTrace(LogError, "Size %i -> %s\n", file->bytes_left, GetSystemPath(filename));
            prTrace(LogError, "Compressed size %i\n", file->cbytes_left);
            #endif
        }
        else
        {
            prTrace(LogError, "Exists: Failed to open file: %s\n", GetSystemPath(filename));
        }
    }
    else
    {
        prTrace(LogError, "Exist test: No archive\n");
    }

#endif

    return result;
}


// ----------------------------------------------------------------------------
// Read a file.
// - Decompresses the source buffer into the destination buffer.  sourceLen is
//   the byte length of the source buffer.  Upon entry, destLen is the total size
//   of the destination buffer, which must be large enough to hold the entire
//   uncompressed data.  (The size of the uncompressed data must have been saved
//   previously by the compressor and transmitted to the decompressor by some
//   mechanism outside the scope of this compression library.) Upon exit, destLen
//   is the actual size of the uncompressed buffer.
//   uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
//   enough memory, Z_BUF_ERROR if there was not enough room in the output
//   buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.
// ----------------------------------------------------------------------------
u32 prFileManager::Read(u8 *pDataBuffer, u32 size, u32 hash)
{
#if !defined(PLATFORM_ANDROID)

    PRASSERT(pDataBuffer);
    PRASSERT(size > 0);

    // Do we need to locate the file? Or can we use the last search results?
    if (hash != filehash)
    {
        u32 filesize = 0;
        if (!Exists(hash, filesize))
        {
            PRPANIC("Archive internal read failed. Failed to find file");
        }
    }


    // Get the entry
    PRASSERT(count > 0);
    PRASSERT(table > -1);
    PRASSERT(table < (s32)count);

    prArcEntry *pEntry = pEntries[table];
    PRASSERT(pEntry);
    pEntry += index;

    // Read the file?
    prFile *pFile = pArchiveFile[table];

    // Mark as accessed!
    pEntry->accessed = true;
    //prTrace(LogError, "Accessed '%s'\n", pEntry->filename);

    if (pEntry->compressed)
    {
        u8* pData = (u8*)malloc(pEntry->compressedSize);
        if (pData)
        {
            pFile->Internal_Seek(pEntry->offset, PRFILE_SEEK_SET);
            pFile->Read(pData, pEntry->compressedSize);
            pFile->Internal_Rewind();

            uLong destLen = (uLong)pEntry->filesize;
            int result = uncompress(pDataBuffer, &destLen, pData, pEntry->compressedSize);
            if (result < Z_OK)
            {
                PRWARN("Uncompress failed: %i\n", result);
            }

            free(pData);
        }
        else
        {
            PRPANIC("prFileManager::Read - Memory allocation failed");
        }
    }
    else
    {
        pFile->Internal_Seek(pEntry->offset, PRFILE_SEEK_SET);
        pFile->Read(pDataBuffer, size);
        pFile->Internal_Rewind();
    }

    return pEntry->filesize;

#else

    PRUNUSED(pDataBuffer);
    PRUNUSED(size);
    PRUNUSED(hash);
    return 0xFFFFFFFF;

#endif
}


/// -----------------------------------------------------------------------
/// Displays all files with the access state passed. This
/// allows all files which have and haven't been accessed to
/// be displayed
/// -----------------------------------------------------------------------
void prFileManager::DisplayFiles(bool accessed)
{
    for (int i=0; i<FILE_ARCHIVES_MAX; i++)
    {
        if (pArchiveFile[i] && pEntries[i])
        {
            prTrace(prLogLevel::LogError, "Archive %i\n", i);

            u32 count = entryCount[i];
            if (count)
            {
                prArcEntry *entry = pEntries[i];

                for (u32 j=0; j<count; j++)
                {
                    if (static_cast<u8>(accessed) == entry->accessed)
                    {
                        prTrace(prLogLevel::LogError, "File %i of %i : '%s' - %s\n", j, count, entry->filename, PRBOOL_TO_STRING(entry->accessed));
                    }

                    entry++;
                }
            }
        }
    }
}


// ------------------------------------------------------------------------
// This function looks in all archives for the file.
// Returns the last file found as code assumes later archives will contain
// updated files
// ------------------------------------------------------------------------
bool prFileManager::Exists(u32 hash, u32 &size)
{
    bool result = false;

    table    = -1;
    index    = -1;
    size     = 0xFFFFFFFF;
    filehash = 0xFFFFFFFF;

    if (count > 0)
    {
        for (u32 i=0; i<count; i++)
        {
            s32  lower = 0;
            s32  upper = entryCount[i];

            // Get table start
            prArcEntry *pStart = pEntries[i];

            while(lower <= upper)
            {
                s32 mid = (lower + upper) / 2;
                //prTrace(LogError, "%i %i %i\n", lower, upper, mid);

                // Get mid entry
                prArcEntry *pEntry = pStart;
                pEntry += mid;

                if (hash > pEntry->hash)
                {
                    lower = mid + 1;
                }
                else if (hash < pEntry->hash)
                {
                    upper = mid - 1;
                }
                else
                {
                    index    = mid;
                    table    = i;
                    result   = true;
                    size     = pEntry->filesize;
                    filehash = hash;
                    //prTrace(LogError, "Found in archive: %s\n", pEntry->filename);
                    break;
                }
            }
        }
    }

    //if (!result)
    //{
    //    Trace("Failed to find file in archive: %x\n", hash);
    //}

    return result;
}


#if defined(PLATFORM_ANDROID)
// ----------------------------------------------------------------------------
// Read a file.
// ----------------------------------------------------------------------------
u32 prFileManager::Read(u8 *pDataBuffer, u32 size, const char *filename)
{
    PRASSERT(pDataBuffer);
    PRASSERT(filename);
    PRASSERT(size > 0);

    u32 filesize = 0;

    if (APKArchive && filename && *filename)
    {
        zip_file *file = zip_fopen(APKArchive, GetSystemPath(filename), 0);
        if (file)
        {
            if (size != file->bytes_left)
            {
                prTrace(LogError, "size not match");
            }

            filesize = file->bytes_left;
            zip_fread(file, pDataBuffer, size);
            zip_fclose(file);
            prTrace(LogError, "AND: Loaded file: %s\n", GetSystemPath(filename));
        }
        else
        {
            prTrace(LogError, "Read: Failed to open file: %s\n", GetSystemPath(filename));
        }
    }

    return filesize;
}


/// ---------------------------------------------------------------------------
/// Sets the APK path for an android device.
/// ---------------------------------------------------------------------------
void prSetAPKPath(const char *path)
{
    memset(APKPath, 0, sizeof(APKPath));

    if (path && *path)
    {
        prStringCopySafe(APKPath, path, sizeof(APKPath));
    }
}


/// ---------------------------------------------------------------------------
/// Sets the card path for an android device.
/// ---------------------------------------------------------------------------
void prSetCardPath(const char *path)
{
    memset(CardPath, 0, sizeof(CardPath));

    if (path && *path)
    {
        prStringCopySafe(CardPath, path, sizeof(CardPath));
    }
}


/// ---------------------------------------------------------------------------
/// Sets the save path for an android device.
/// ---------------------------------------------------------------------------
void prSetSavePath(const char *path)
{
    memset(SavePath, 0, sizeof(SavePath));

    if (path && *path)
    {
        prStringCopySafe(SavePath, path, sizeof(SavePath));
    }
}


/// ---------------------------------------------------------------------------
/// Gets the card path for an android device.
/// ---------------------------------------------------------------------------
const char *prGetCardPath()
{
    return CardPath;
}


/// ---------------------------------------------------------------------------
/// Gets the save path for an android device.
/// ---------------------------------------------------------------------------
const char *prGetSavePath()
{
    return SavePath;
}


/// ---------------------------------------------------------------------------
/// Get access to the android data zip file.
/// ---------------------------------------------------------------------------
zip *prGetAPKArchive()
{
    return APKArchive;
}


#endif
