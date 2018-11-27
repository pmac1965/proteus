/**
 * Copyright Paul Michael McNab. All rights reserved.
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


#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "prFile.h"
#include "prFileShared.h"
#include "prFileManager.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"


#if defined(PLATFORM_ANDROID)
#include <android/log.h>
#include <libzip/zip.h>
#include <libzip/zipint.h>
#endif


//#define FILE_LOG_CREATE


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// prFile operation mode.
/// ---------------------------------------------------------------------------
enum prFileMode
{
    FileMode_None,                                      // No file operation has been performed yet!
    FileMode_Normal,                                    // The user opens, reads, closes the file. User also can optionally use their read buffer.
    FileMode_Simple,                                    // The simply calls load. Data is held within file until the file is deleted.
};


/// ---------------------------------------------------------------------------
/// Implementation data
/// ---------------------------------------------------------------------------
typedef struct FileImplementation
{
    // Ctor
    FileImplementation() : filemode     (FileMode_None)
                         , filesize     (0xFFFFFFFF)
                         , filePointer  (0)
    {
        filedata    = nullptr;
        pFile       = nullptr;
        //filemode    = FileMode_None; //cppcheck only triggred these 3?
//        filesize    = 0xFFFFFFFF;
  //      filePointer = 0;
        opened      = false;
        loaded      = false;
        locked      = false;
        inArchive   = false;

        memset(filenameArch, 0, sizeof(filenameArch));
        memset(filenameDisk, 0, sizeof(filenameDisk));
        memset(filenameOrig, 0, sizeof(filenameOrig));

        #if defined(PLATFORM_ANDROID)
        pZipFile    = nullptr;
        bytesRead   = 0;
        #endif
    }
    
    
    // Dtor
    ~FileImplementation()
    {
        PRSAFE_DELETE(filedata);
    }


    // Checks for file errors.
    bool WasThereAFileError()
    {
        bool result = false;

        if (pFile)
        {
            if (ferror(pFile))
            {
                const char *message = strerror(errno);

                clearerr(pFile);
                
                if (message && *message)
                {
                    PRWARN(message);
                }

                result = true;
            }
        }

        return result;
    }


    // Data
    u8         *filedata;                               // Pointer to the files data.
    FILE       *pFile;                                  // Internal file use.
    prFileMode  filemode;                               // Mode of operation.
    char        filenameArch[FILE_MAX_FILENAME_SIZE];   // The files name. (Archive)
    char        filenameDisk[FILE_MAX_FILENAME_SIZE];   // The files name. (With system path added)
    char        filenameOrig[FILE_MAX_FILENAME_SIZE];   // The files name. (Original unaltered)
    u32         filesize;                               // Files size.
    s32         filePointer;                            // prFile position pointer
    bool        opened;                                 // Has the file been opened with Open()
    bool        loaded;                                 // Files loaded/unloaded state.
    bool        locked;                                 // Files locked/unlocked state.    
    bool        inArchive;                              // Is the file in the archive?

    #if defined(PLATFORM_ANDROID)
    zip_file   *pZipFile;
    u32         bytesRead;
    #endif

} FileImplementation;


/// ---------------------------------------------------------------------------
/// Ctor.
/// ---------------------------------------------------------------------------
prFile::prFile(const char *filename) : pImpl (new FileImplementation())
                                     , imp   (*pImpl)
{
    PRASSERT(pImpl);
    PRASSERT(filename && *filename);
    PRASSERT(strlen(filename) < FILE_MAX_FILENAME_SIZE);

    // Ensures name is long enough to skip past 'data/'
    PRASSERT(strlen(filename) > 5);

    // Ensures we have a filemanager
    prFileManager *pFM = static_cast<prFileManager *>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
    PRASSERT(pFM)
    

    if (strlen(filename) < FILE_MAX_FILENAME_SIZE)
    {
        // Creates file names. One for the disk, one for the archive and keeps the original
        prStringCopySafe(imp.filenameArch, filename + 5, FILE_MAX_FILENAME_SIZE);
        prStringCopySafe(imp.filenameDisk, pFM->GetSystemPath(filename), FILE_MAX_FILENAME_SIZE);
        prStringCopySafe(imp.filenameOrig, filename, FILE_MAX_FILENAME_SIZE);

        // Must be lowercase
        prStringToLower(imp.filenameArch);

        // And use forward slashes
        prStringReplaceChar(imp.filenameArch, '\\', '/');
        prStringReplaceChar(imp.filenameDisk, '\\', '/');
        
        //prTrace(LogError, "ARC: %s\n", imp.filenameArch);
        //prTrace(LogError, "DSK: %s\n", imp.filenameDisk);
        //prTrace(LogError, "DRI: %s\n", imp.filenameOrig);
    }
    else
    {
        PRWARN("Filename was too long: %s", filename);
        imp.filenameArch[0] = 0;
        imp.filenameDisk[0] = 0;
        imp.filenameOrig[0] = 0;
    }

#ifdef FILE_LOG_CREATE
    {
        static int c = 0;
        PRLOGD("Created prFile: %i - '%s'\n", c++, filename);
    }
#endif
}


/// ---------------------------------------------------------------------------
/// Dtor.
/// ---------------------------------------------------------------------------
prFile::~prFile()
{
    Close();
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Gets the size of the file.
/// ---------------------------------------------------------------------------
u32 prFile::Size() const
{
    PRASSERT(pImpl);
    return imp.filesize;
}


/// ---------------------------------------------------------------------------
/// Determines if the file exists
/// ---------------------------------------------------------------------------
bool prFile::Exists()
{
    PRASSERT(pImpl);

    // Get filemanager
    prFileManager *pFM = static_cast<prFileManager *>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
    PRASSERT(pFM)

    bool result = false;
    if (imp.filenameDisk[0] != 0 &&
        imp.filenameArch[0] != 0 )
    {
        u32 size = 0;

        // Check archive
        if (pFM->Ready()                     &&
            pFM->ArchiveCount() > 0          &&
            pFM->Exists(imp.filenameArch, size)
           )
        {
            result = true;
        }
        else
        {
#if defined(PLATFORM_ANDROID)
            // Open
            zip* pArchive = prGetAPKArchive();
            if (pArchive)
            {
                prFileManager *pFileManager = static_cast<prFileManager*>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
                PRASSERT(pFileManager);

                imp.pZipFile = zip_fopen(pArchive, pFileManager->GetSystemPath(imp.filenameArch), 0);
                if (imp.pZipFile)
                {
                    result = true;
                    zip_fclose(imp.pZipFile);
                    imp.pZipFile = nullptr;
                }
                else
                {
                    __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Open: Failed to open file: %s\n", pFileManager->GetSystemPath(imp.filenameArch));
                }
            }
            else
            {
                __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get zip archive");
            }

#else
            // Check disk
            imp.pFile = fopen(imp.filenameDisk, "r");

            if (imp.pFile)
            {
                // Errors?
                result = !imp.WasThereAFileError();
                fclose(imp.pFile);
                imp.pFile = nullptr;
            }
            else
            {
                prTrace(prLogLevel::LogError, "File does not exist %s\n", imp.filenameDisk);
            }
#endif
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Opens a file for reading.
/// ---------------------------------------------------------------------------
bool prFile::Open()
{
    PRASSERT(pImpl);


    // Calling open again?
    if (imp.opened)
    {
        return true;
    }


#if defined(PLATFORM_ANDROID)
    // Check mode.
    PRASSERT(imp.filemode != FileMode_Simple, "You cannot open file created by the Load() method.");

    // Open
    zip* pArchive = prGetAPKArchive();
    if (pArchive)
    {
        prFileManager *pFileManager = static_cast<prFileManager*>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
        PRASSERT(pFileManager);

        imp.pZipFile = zip_fopen(pArchive, pFileManager->GetSystemPath(imp.filenameArch), 0);
        if (imp.pZipFile)
        {
            imp.filesize  = imp.pZipFile->bytes_left;
            imp.filemode  = FileMode_Normal;
            imp.opened    = true;
            imp.bytesRead = 0;
        }
        else
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Open: Failed to open file: %s\n", pFileManager->GetSystemPath(imp.filenameArch));
        }
    }
    else
    {
        __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get zip archive");
    }

#else

    // Get filemanager
    prFileManager *pFM = static_cast<prFileManager *>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
    PRASSERT(pFM)

    // Archives registered?
    if (pFM->Ready() && pFM->ArchiveCount() > 0)
    {
        u32 size = 0;

        if (pFM->Exists(imp.filenameArch, size))
        {
            // Okay files open.
            imp.filemode  = FileMode_Normal;
            imp.opened    = true;
            imp.inArchive = true;
            imp.filesize  = size;
            return true;
        }
    }


    // Check mode.
    PRASSERT(imp.filemode != FileMode_Simple, "You cannot open file created by the Load() method.");


    // Open the file?
    imp.pFile = fopen(imp.filenameDisk, "rb");
    if (imp.pFile == nullptr)
    {
        // Show error
        imp.WasThereAFileError();
        prTrace(prLogLevel::LogError, "STD: Failed to open file: %s\n", imp.filenameDisk);
        prTrace(prLogLevel::LogError, "STD: Trying to open file: %s\n", imp.filenameOrig);

        imp.pFile = fopen(imp.filenameOrig, "rb");
        if (imp.pFile == nullptr)
        {
            // Show error
            prTrace(prLogLevel::LogError, "STD: Failed to open file: %s\n", imp.filenameOrig);
            imp.WasThereAFileError();
            return false;
        }
    }


    // Get the files size.
    if (fseek(imp.pFile, 0, SEEK_END) == 0)
    {
        imp.filesize = (u32)ftell(imp.pFile);
        rewind(imp.pFile);       
    }
    else
    {
        // Show error
        imp.WasThereAFileError();
        fclose(imp.pFile);
        imp.pFile = nullptr;
        return false;
    }

#endif

    // Okay files open.
    imp.filemode = FileMode_Normal;
    imp.opened   = true;
    return true;
}


/// ---------------------------------------------------------------------------
/// Closes a file
/// ---------------------------------------------------------------------------
void prFile::Close()
{
    PRASSERT(pImpl);


    // Check mode.
    if (imp.filemode == FileMode_Simple)
    {
        prTrace(prLogLevel::LogError, "You cannot call close on files created by the Load() method\n");
        return;
    }

#if defined(PLATFORM_ANDROID)
    // Close
    if (imp.pZipFile)
    {
        zip_fclose(imp.pZipFile);
        imp.pZipFile  = nullptr;
        imp.bytesRead = 0;
    }

#else
    // Close
    if (imp.pFile)
    {
        fclose(imp.pFile);
        imp.pFile = nullptr;
    }

#endif

    imp.opened   = false;
    imp.filemode = FileMode_None;
}


/// ---------------------------------------------------------------------------
/// Reads data from an open file
/// ---------------------------------------------------------------------------
u32 prFile::Read(void *pDataBuffer, u32 size)
{
    // Sanity checks
    PRASSERT(pImpl        != nullptr);
    PRASSERT(imp.opened   == true);
    PRASSERT(imp.filemode == FileMode_Normal);
    PRASSERT(pDataBuffer  != nullptr);
    PRASSERT(size > 0);

    
    // Get filemanager
    prFileManager *pFM = static_cast<prFileManager *>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
    PRASSERT(pFM)


    size_t bytes = 0xFFFFFFFF;
    if (imp.inArchive)
    {
        bytes = pFM->Read((u8*)pDataBuffer, size, prStringHash(imp.filenameArch));

        #if defined(PLATFORM_ANDROID)
        __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Error: File found in archive");
        #endif
    }
    else
    {
#if defined(PLATFORM_ANDROID)
        // Read from disk.
        PRASSERT(imp.pZipFile != nullptr);
        bytes = zip_fread(imp.pZipFile, pDataBuffer, size);
        imp.bytesRead += bytes;
        //__android_log_print(ANDROID_LOG_ERROR, "Proteus", "Loaded: %s", imp.filenameArch);

#else
        // Read from disk.
        PRASSERT(imp.pFile != nullptr);

        bytes = fread(pDataBuffer, 1, size, imp.pFile);

        //if (bytes != size)
        //{
        //    Trace("Bytes read did not equal sized passed.\n");
        //    Trace("This may not be an error.\n");
        //}

        if (ferror(imp.pFile))
        {
            imp.WasThereAFileError();
        }
#endif
    }


    return (u32)bytes;
}


/// ---------------------------------------------------------------------------
/// Seek within an open file.
/// ---------------------------------------------------------------------------
void prFile::Seek(s32 offset, s32 origin)
{
    // Sanity checks
    PRASSERT(pImpl);

    if (imp.inArchive)
    {
        PRPANIC("Not implemented");
    }
    else
    {
        Internal_Seek(offset, origin);
    }
}


/// ---------------------------------------------------------------------------
/// Returns the file pointers position.
/// ---------------------------------------------------------------------------
s32 prFile::Tell() const
{
    // Sanity checks
    PRASSERT(pImpl);

    if (imp.inArchive)
    {
        PRPANIC("Not implemented");
    }
    else
    {
        return Internal_Tell();
    }

    return -1;
}


/// ---------------------------------------------------------------------------
/// Sets the file pointer back to the start of the file.
/// ---------------------------------------------------------------------------
void prFile::Rewind()
{
    // Sanity checks
    PRASSERT(pImpl);

    if (imp.inArchive)
    {
        PRPANIC("Not implemented");
    }
    else
    {
        return Internal_Rewind();
    }
}


/// ---------------------------------------------------------------------------
/// Rewind to the start of the file
/// ---------------------------------------------------------------------------
void prFile::Internal_Rewind()
{
    PRASSERT(pImpl);
    PRASSERT(imp.pFile);

    rewind(imp.pFile);
}


/// ---------------------------------------------------------------------------
/// Seeks to a position within a file
/// ---------------------------------------------------------------------------
void prFile::Internal_Seek(s32 offset, s32 origin)
{
    PRASSERT(pImpl);
    PRASSERT(PRBETWEEN(origin, PRFILE_SEEK_SET, PRFILE_SEEK_END));

#if defined(PLATFORM_ANDROID)
    PRASSERT(imp.pZipFile != nullptr);

    // Read to simulate seek
    if (origin == PRFILE_SEEK_CUR)
    {
        u8 *pData = new u8[offset];
        imp.bytesRead += zip_fread(imp.pZipFile, pData, offset);
        PRSAFE_DELETE_ARRAY(pData);
    }
    // Reads from start to simulate seek set.
    else if (origin == PRFILE_SEEK_SET)
    {
        zip* pArchive = prGetAPKArchive();
        if (pArchive)
        {
            // Close the old file.
            zip_fclose(imp.pZipFile);
            imp.pZipFile  = nullptr;
            imp.bytesRead = 0;

            prFileManager *pFileManager = static_cast<prFileManager*>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
            PRASSERT(pFileManager);

            // Reopen file to get back to start!
            imp.pZipFile = zip_fopen(pArchive, pFileManager->GetSystemPath(imp.filenameArch), 0);
            if (imp.pZipFile)
            {
                // Seek to the point asked.
                u8 *pData = new u8[offset];
                imp.bytesRead += zip_fread(imp.pZipFile, pData, offset);
                PRSAFE_DELETE_ARRAY(pData);
            }
        }
    }
    else if (origin == PRFILE_SEEK_END)
    {
    }
    else
    {
        PRPANIC("Unknown origin");
    }

#else
    PRASSERT(imp.pFile);
    fseek(imp.pFile, offset, origin);

#endif
}


/// ---------------------------------------------------------------------------
/// Returns the current file pointer position
/// ---------------------------------------------------------------------------
s32 prFile::Internal_Tell() const
{
    PRASSERT(pImpl);

#if defined(PLATFORM_ANDROID)
    return imp.bytesRead;

#else
    PRASSERT(imp.pFile);
    return (s32)ftell(imp.pFile);

#endif
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
/*bool prFile::Internal_FileDataLoaded()
{
    PRASSERT(pImpl);
    return (imp.filedata != nullptr);
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void prFile::Internal_SetFileData(u8 *pData)
{
    PRASSERT(pImpl);
    PRASSERT(pData);

    if (pData)
    {
    }
}*/
