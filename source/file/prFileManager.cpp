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
 *
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
#include "../zlib/zlib.h"


// Debug assist
#if defined(_DEBUG) || defined(DEBUG)
//#define FILEMANAGER_DEBUG
#endif


// Platform specific includes.
#if defined(PLATFORM_PC)  
  #include <windows.h>

#elif defined(PLATFORM_IOS)  
  #include <stdlib.h>
  #include "../ios/prIos.h"

#elif defined(PLATFORM_MAC)
  #include <stdlib.h>
  //#include "../ios/prIos.h"

#elif defined(PLATFORM_BADA)
  #include <stdlib.h>

#elif defined(PLATFORM_LINUX)
  #include <stdlib.h>
  #include <unistd.h>

  #ifndef MAX_PATH
  #define MAX_PATH    256
  #endif

#elif defined(PLATFORM_ANDROID)
  #include <stdlib.h>
  #include <libzip\zip.h>
  #include <libzip\zipint.h>

  #ifndef MAX_PATH
  #define MAX_PATH    260
  #endif

  #define ANDROID_APK_DEBUG
  //#define ANDROID_APK_DEBUG_SHOW_CONTENTS

  namespace
  {
    char APKPath [MAX_PATH] = { '\0' };
    char CardPath[MAX_PATH] = { '\0' };
    char SavePath[MAX_PATH] = { '\0' };
    zip *APKArchive = NULL;
  }

#else
  #error No platform defined.

#endif


/// ---------------------------------------------------------------------------
/// Creates a path to the applications data.
/// ---------------------------------------------------------------------------
prFileManager::prFileManager() : prCoreSystem(PRSYSTEM_FILEMANAGER, "prFileManager")
{
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
        pArchiveFile[i] = NULL;
        pEntries[i]     = NULL;
        entryCount[i]   = 0;
    }

    memset(path, 0, sizeof(path));
    memset(dataPath, 0, sizeof(dataPath));

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
            GetModuleFileNameA(NULL, dataPath, FILE_MAX_FILENAME_SIZE);
            int index = prStringFindLastIndex(dataPath, '\\');
            dataPath[index] = 0;
        }

    // Bada
    #elif defined(PLATFORM_BADA)
        // Copy the app path.
        strcpy(dataPath, "/Home");

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


    prStringReplaceChar(dataPath, '\\', '/');
    //prTrace("App data path: %s\n", dataPath);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFileManager::~prFileManager()
{
    for (int i=0; i<FILE_ARCHIVES_MAX; i++)
    {
        // Close and delete file
        if (pArchiveFile[i] != NULL)
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
                            int  idx = -1;
                            for (int i=0; i<FILE_ARCHIVES_MAX; i++)
                            {
                                if (pEntries[i] == NULL && pArchiveFile[i] == NULL)
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

                                #if defined(FILEMANAGER_DEBUG)
                                prTrace("Found archive: %s\n", filenameArc);
                                for(u32 i=0; i<header.entries; i++)
                                {
                                    prArcEntry *pE = pEntries[idx];
                                    pE += i;
                                    prTrace("File: %s - %x\n", pE->filename, pE->hash);
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
            prTrace("Failed to locate: %s\n", filenameArc);
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

    if (APKArchive == NULL)
    {
        APKArchive = zip_open(APKPath, 0, NULL);
        if (APKArchive == NULL)
        {
            PRPANIC("Error loading APK");
        }
        else
        {
            prTrace("Opened APK: %s\n", APKPath);
        }

        // Print the APK's contents
        #if defined(ANDROID_APK_DEBUG) && defined(ANDROID_APK_DEBUG_SHOW_CONTENTS)
        if (APKArchive)
        {
            int numFiles = zip_get_num_files(APKArchive);
            for (int i=0; i<numFiles; i++)
            {
                const char *name = zip_get_name(APKArchive, i, 0);
                if (name == NULL)
                {
                    PRPANIC("Error reading zip file name at index %i : %s", i, zip_strerror(APKArchive));
                }
                prTrace("File %i, %s\n", i, name);
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

        #elif defined(PLATFORM_BADA)
            strcpy(path, "/Home/");
            // Bypass data/
            strcat(path, &filename[5]);

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
        prTrace("Failed to find file in archive: %s\n", filename);
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
            prTrace("Size %i -> %s\n", file->bytes_left, GetSystemPath(filename));
            prTrace("Compressed size %i\n", file->cbytes_left);
            #endif
        }
        else
        {
            prTrace("Exists: Failed to open file: %s\n", GetSystemPath(filename));
        }
    }
    else
    {
        prTrace("Exist test: No archive\n");
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
    if (pEntry->compressed)
    {
        u8* pData = (u8*)malloc(pEntry->compressedSize);
        if (pData)
        {
            pFile->Internal_Seek(pEntry->offset, PRFILE_SEEK_SET);
            pFile->Read(pData, pEntry->compressedSize);
            pFile->Internal_Rewind();

            u32 destLen = pEntry->filesize;
            int result = uncompress(pDataBuffer, (uLong*)&destLen, pData, pEntry->compressedSize);
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


// ------------------------------------------------------------------------
// This function looks in all archives for the file.
// Returns the last file found as code assumes later archives will contain updated files
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
                //prTrace("%i %i %i\n", lower, upper, mid);

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
                    //prTrace("Found in archive: %s\n", pEntry->filename);
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
                prTrace("size not match");
            }

            filesize = file->bytes_left;
            zip_fread(file, pDataBuffer, size);
            zip_fclose(file);
            prTrace("AND: Loaded file: %s\n", GetSystemPath(filename));
        }
        else
        {
            prTrace("Read: Failed to open file: %s\n", GetSystemPath(filename));
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
