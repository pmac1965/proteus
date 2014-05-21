/**
 * prFileSystem.cpp
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


#include "prFileSystem.h"
#include "../prConfig.h"


#if defined(PLATFORM_PC)
  #include <windows.h>

#elif defined(PLATFORM_IOS)
  #include <errno.h>
  #include <string.h>

#elif defined(PLATFORM_MAC)
  #include <errno.h>
  #include <string.h>

#elif defined(PLATFORM_BADA)
  #include <errno.h>
  #include <string.h>

#elif defined(PLATFORM_ANDROID)
  #include <errno.h>
  #include <string.h>

#elif defined(PLATFORM_LINUX)
  #include <errno.h>
  #include <string.h>

#else
  #error Unsupported platform.

#endif


#include <stdio.h>
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"


// Defines 
#define BUFFER_SIZE     512


/// ---------------------------------------------------------------------------
/// Saves a binary file.
/// ---------------------------------------------------------------------------
bool prFileSave(const char* filename, const void *data, u32 size)
{
    PRASSERT(filename && *filename);
    PRASSERT(data);
    PRASSERT(size > 0);

    bool  result = false;

    if (filename && *filename && data && size)
    {
        FILE *fp = fopen(filename, "wb");
        if (fp)
        {
            // Write
            result = (fwrite(data, 1, size, fp) == size);

            if (!result)
            {
                PRPANIC("Failed to write correct number of bytes to file");
                result = false;
            }

            // Errors?
            if (ferror(fp))
            {
                char buffer[BUFFER_SIZE];
                strcpy(buffer, strerror(errno));
                clearerr(fp);
                PRWARN(buffer);
                result = false;
            }

            fclose(fp);
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Determines if the passed file exists.
/// ---------------------------------------------------------------------------
bool prFileExist(const char *filename)
{
    PRASSERT(filename && *filename);

    bool result = false;

    if (filename && *filename)
    {
        FILE *fp = fopen(filename, "r");
        if (fp)
        {
            result = true;

            // Errors?
            if (ferror(fp))
            {
                char buffer[BUFFER_SIZE];
                strcpy(buffer, strerror(errno));
                clearerr(fp);
                PRWARN(buffer);
                result = false;
            }

            fclose(fp);
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Removes read only attributes if required.
/// ---------------------------------------------------------------------------
bool prFileRemoveReadOnly(const char* filename)
{
#if defined(PLATFORM_PC)

    PRASSERT(filename && *filename);

    if (filename && *filename && prFileExist(filename))
    {
        DWORD attributes = GetFileAttributesA(filename);
        if (attributes == 0xFFFFFFFF)
        {
            PRWARN("Failed to acquire attributes.");
            return false;
        }

        // Is a directory?
        if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            PRWARN("Filename is a directory.");
            return false;
        }

        // Not usable? This is because the file should not have these atributes.
        if (attributes & (FILE_ATTRIBUTE_OFFLINE | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY))
        {
            PRWARN("Filename is not usable.");
            return false;
        }

        // If file is read only, then we must remove it.
        if (attributes & FILE_ATTRIBUTE_READONLY)
        {
            if (SetFileAttributesA(filename, FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_ARCHIVE))
            {
                return true;
            }
            else
            {
                prDebugShowLastError();
                return false;
            }
        }
    }

#else

    PRUNUSED(filename);

#endif

    return false;
}


/// ---------------------------------------------------------------------------
/// Deletes a file.
/// ---------------------------------------------------------------------------
bool prFileDelete(const char* filename)
{
#if defined(PLATFORM_PC)

    PRASSERT(filename && *filename);

    if (filename && *filename)
    {
        if (prFileExist(filename))
        {
            if (DeleteFileA(filename))
            {
                return true;
            }
            else
            {
                prDebugShowLastError();
                return false;
            }
        }
    }

#else

    PRUNUSED(filename);

#endif

    return false;
}


/// ---------------------------------------------------------------------------
/// Applies the read only attributes if possible.
/// ---------------------------------------------------------------------------
bool prFileApplyReadOnly(const char* filename)
{
#if defined(PLATFORM_PC)

    PRASSERT(filename && *filename);

    if (filename && *filename && prFileExist(filename))
    {
        DWORD attributes = GetFileAttributesA(filename);
        if (attributes == 0xFFFFFFFF)
        {
            PRWARN("Failed to acquire attributes.");
            return false;
        }

        // Is a directory?
        if (attributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            PRWARN("Filename is a directory.");
            return false;
        }

        // Not usable? This is because the file should not have these atributes.
        if (attributes & (FILE_ATTRIBUTE_OFFLINE | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_TEMPORARY))
        {
            PRWARN("Filename is not usable.");
            return false;
        }

        if (SetFileAttributesA(filename, attributes | FILE_ATTRIBUTE_READONLY))
        {
            return true;
        }
        else
        {
            prDebugShowLastError();
            return false;
        }
    }

#else

    PRUNUSED(filename);

#endif

    return false;
}


/// ---------------------------------------------------------------------------
/// Returns the size of a file or -1 on error.
/// ---------------------------------------------------------------------------
s32 prFileSize(const char *filename)
{
    s32   filesize = -1;

    PRASSERT(filename && *filename);

    if (filename && *filename)
    {
        FILE *fp = fopen(filename, "r");
        if (fp)
        {
            if (fseek(fp, 0, SEEK_END) == 0)
            {
                filesize = (s32)ftell(fp);
                rewind(fp);       
            }
                        
            fclose(fp);
        }
    }

    return filesize;
}


/// ---------------------------------------------------------------------------
/// Loads a binary file into the supplied buffer.
/// ---------------------------------------------------------------------------
bool prFileLoad(const char *filename, u32 filesize, void *buffer, u32 bufferSize)
{
    // Assert invariants
    PRASSERT(filename && *filename);
    PRASSERT(buffer);
    PRASSERT(filesize);
    PRASSERT(bufferSize);
    PRASSERT(bufferSize >= filesize);

    bool  result = false;

    if (filename && *filename && buffer && filesize && bufferSize)
    {
        FILE *fp = fopen(filename, "rb");
        if (fp)
        {
            result = true;

            if (fread(buffer, 1, filesize, fp) != filesize)
            {
                PRPANIC("Failed to read the correct number of bytes");
                result = false;
            }

            // Errors?
            if (ferror(fp))
            {
                char buffer[BUFFER_SIZE];
                strcpy(buffer, strerror(errno));
                clearerr(fp);
                PRWARN(buffer);
                result = false;
            }

            fclose(fp);
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Changes a filenames extension.
/// ---------------------------------------------------------------------------
void prFileChangeExtension(char* filename, const char* extension, s32 bufferSize)
{
    PRASSERT(filename  && *filename);
    PRASSERT(extension && *extension);
    PRUNUSED(bufferSize);

    if (filename && *filename && extension && *extension)
    {
        s32 index = prStringFindLastIndex(filename, '.');

        if (index > -1)
        {
            filename[index] = 0;
        }

        strcat(filename, extension);
    }
}
