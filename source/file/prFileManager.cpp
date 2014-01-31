/**
 * prFileManager.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"


#include <string.h>
#include "prFileManager.h"
#include "prFile.h"
//#include "prFileShared.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../core/prCore.h"
#include "../core/prStringUtil.h"
#include "../zlib/zlib.h"


// ----------------------------------------------------------------------------
// Platform specific includes.
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)  
  #include <windows.h>

#elif defined(PLATFORM_IOS)  
  #include <stdlib.h>
  #include "../iphone/iphone.h"

#elif defined(PLATFORM_BADA)
  #include <stdlib.h>

#elif defined(PLATFORM_ANDROID)
  #include <stdlib.h>
  #include <libzip\zip.h>
  #include <libzip\zipint.h>

  #ifndef MAX_PATH
  #define MAX_PATH    260
  #endif

  static char APKPath[MAX_PATH];
  static char CardPath[MAX_PATH];
  static zip *APKArchive = NULL;

  //#define ANDROID_APK_DEBUG

  // Get access to the android data zip file.
  zip *GetAPKArchive()
  {
      return APKArchive;
  }

#else
  #error No platform defined.

#endif


// ----------------------------------------------------------------------------
// Implementation.
// ----------------------------------------------------------------------------
/*typedef struct FileManagerImplementation
{
    // ------------------------------------------------------------------------
    // Ctor
    // ------------------------------------------------------------------------
    FileManagerImplementation()
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

        // Android doesn't use the archive as the .APK
        // is already zipped! So we'll have one archive only!
        //#if defined(PLATFORM_ANDROID)
        //count        = 1;
        //ready        = true;
        //#endif

        //Trace("CTOR - prFileManager\n");
    }


    // ------------------------------------------------------------------------
    // Dtor
    // ------------------------------------------------------------------------
    ~FileManagerImplementation()
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


    // ------------------------------------------------------------------------
    // This function looks in all archives for the file.
    // Returns the last file found as code assumes later archives will contain updated files
    // ------------------------------------------------------------------------
    bool Exists(u32 hash, u32 &size)
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
                int  lower = 0;
                int  upper = entryCount[i];
                //int  mid;

                // Get table start
                ArcEntry *pStart = pEntries[i];

                while(lower <= upper)
                {
                    int mid = (lower + upper) / 2;
                    //Trace("%i %i %i\n", lower, upper, mid);

                    // Get mid entry
                    ArcEntry *pEntry = pStart;
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
                        //Trace("Found in archive: %s\n", pEntry->filename);
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

} FileManagerImplementation;*/


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
        strcpy(imp.dataPath, GetAppDataPath());

    // PC
    #elif defined(PLATFORM_PC)    
        // If we're using the debugger then use the current directory as it will point to the
        // debug directory path, else use the executables path as the data path.
        if (IsDebuggerPresent() == TRUE)
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
        strcpy(imp.dataPath, "/Home");

    // Bada
    #elif defined(PLATFORM_ANDROID)
        // Copy the app path.
        strcpy(imp.dataPath, "assets");

    #else
        #error No platform defined.

    #endif


    prStringReplaceChar(dataPath, '\\', '/');
    prTrace("App data path: %s\n", dataPath);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prFileManager::~prFileManager()
{
}


// ----------------------------------------------------------------------------
// Register an archive.
// ----------------------------------------------------------------------------
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
                            int  index = -1;
                            for (int i=0; i<FILE_ARCHIVES_MAX; i++)
                            {
                                if (pEntries[i] == NULL && pArchiveFile[i] == NULL)
                                {
                                    index = i;
                                    found = true;
                                    break;
                                }
                            }


                            if (found)
                            {
                                // Open file for data access, store entries and indicate we have an archive open for use.
                                pArchiveFile[index] = new prFile(filenameArc);
                                pEntries    [index] = entries;
                                entryCount  [index] = header.entries;
                                count++;

                                pArchiveFile[index]->Open();

                                prTrace("Found archive: %s\n", filenameArc);

                                /*for(u32 i=0; i<header.entries; i++)
                                {
                                    ArcEntry *pE = imp.pEntries[index];
                                    pE += i;
                                    Trace("File: %s - %x\n", pE->filename, pE->hash);
                                }//*/
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
            PANIC("Error loading APK");
        }
 //       else
 //       {
 //           Trace("Opened APK: %s\n", APKPath);
 //       }

        // Print APK contents
        #if defined(ANDROID_APK_DEBUG)
        if (APKArchive)
        {
            int numFiles = zip_get_num_files(APKArchive);
            for (int i=0; i<numFiles; i++)
            {
                const char *name = zip_get_name(APKArchive, i, 0);
                if (name == NULL)
                {
                    PANIC("Error reading zip file name at index %i : %s", i, zip_strerror(APKArchive));
                }
                Trace("File %i, %s\n", i, name);
            }
        }
        #endif
    }    

#endif
}


// ----------------------------------------------------------------------------
// Returns the filepath for the current platform.
// ----------------------------------------------------------------------------
const char *prFileManager::GetSystemPath(const char *filename)
{
    PRASSERT(filename && *filename);

    // Got a system data path?
    if (dataPath[0] != '\0')
    {
        // Check for buffer overflow.
        #if defined(_DEBUG) || defined(DEBUG)
        {            
            int total = strlen(dataPath) + 1 + strlen(filename);
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
            strcpy(imp.path, imp.dataPath);
            strcat(imp.path, "/");            

            // Remove the passed filename path data.
            char path[FILE_MAX_FILENAME_SIZE];
            strcpy(path, filename);

            StringReplaceChar(path, '\\', '/');

            int index = StringFindLastIndex(path, '/');
            if (index > -1)
            {
                strcat(imp.path, &path[index + 1]);
            }
            else
            {
                strcat(imp.path, path);
            }

        #elif defined(PLATFORM_BADA)
            strcpy(imp.path, "/Home/");
            // Bypass data/
            strcat(imp.path, &filename[5]);

        #elif defined(PLATFORM_ANDROID)
            // Make path
            strcpy(imp.path, "assets/");
            strcat(imp.path, filename);

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


// ----------------------------------------------------------------------------
// Let the file system know we're done registering archives.
// ----------------------------------------------------------------------------
void prFileManager::SetRegistrationComplete()
{
#if !defined(PLATFORM_ANDROID)
    ready = true;
#endif
}


// ----------------------------------------------------------------------------
// Ready to use?
// ----------------------------------------------------------------------------
bool prFileManager::Ready() const
{
    return ready;
}


// ----------------------------------------------------------------------------
// How many archives loaded.
// ----------------------------------------------------------------------------
int prFileManager::ArchiveCount() const
{
    return count;
}


// ----------------------------------------------------------------------------
// Does the file exist
// ----------------------------------------------------------------------------
bool prFileManager::Exists(const char *filename, u32 &size)
{
    PRASSERT(filename && *filename);

#if !defined(PLATFORM_ANDROID)
    
    u32  hash   = prStringHash(filename);
    bool result = false; // imp.Exists(hash, size);
        TODO("Fix")
    if (!result)
    {
        TODO("Put me back")
//        Trace("Failed to find file in archive: %s\n", filename);
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
            Trace("Size %i -> %s\n", file->bytes_left, GetSystemPath(filename));
            Trace("Compressed size %i\n", file->cbytes_left);
            #endif
        }
        else
        {
            Trace("Exists: Failed to open file: %s\n", GetSystemPath(filename));
        }
    }
    else
    {
        Trace("Exist test: No archive\n");
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
        //if (!Exists(hash, filesize))
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

    UNUSED(pDataBuffer);
    UNUSED(size);
    UNUSED(hash);
    return 0xFFFFFFFF;

#endif
}


#if defined(PLATFORM_ANDROID)
// ----------------------------------------------------------------------------
// Read a file.
// ----------------------------------------------------------------------------
/*u32 prFileManager::Read(u8 *pDataBuffer, u32 size, const char *filename)
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
                Trace("size not match");
            }

            filesize = file->bytes_left;
            zip_fread(file, pDataBuffer, size);
            zip_fclose(file);
            //Trace("AND: Loaded file: %s\n", GetSystemPath(filename));
        }
        else
        {
            Trace("Read: Failed to open file: %s\n", GetSystemPath(filename));
        }
    }

    return filesize;
}//*/


// ----------------------------------------------------------------------------
// Sets the APK path for an android device.
// ----------------------------------------------------------------------------
void SetAPKPath(const char *path)
{
    memset(APKPath, 0, sizeof(APKPath));

    if (path && *path)
    {
        StringCopySafe(APKPath, path, sizeof(APKPath));
    }
}


// ----------------------------------------------------------------------------
// Sets the card path for an android device.
// ----------------------------------------------------------------------------
void SetCardPath(const char *path)
{
    memset(CardPath, 0, sizeof(CardPath));

    if (path && *path)
    {
        StringCopySafe(CardPath, path, sizeof(CardPath));
    }
}


// ----------------------------------------------------------------------------
// Sets the card path for an android device.
// ----------------------------------------------------------------------------
const char *GetCardPath()
{
    return CardPath;
}


// ----------------------------------------------------------------------------
// For android we need to put the instance in the class or 
// we end up with multiple versions as the singleton doesn't work 
// correctly for android.
// ----------------------------------------------------------------------------
prFileManager *prFileManager::GetInstance()
{
    static prFileManager *instance = NULL;

    if (instance == NULL)
    {
        instance = new prFileManager();
    }

    return instance;
}
#endif
