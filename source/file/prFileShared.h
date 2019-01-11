// File: prFileShared.h
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


#include "../prConfig.h"
#include "../core/prTypes.h"


// ----------------------------------------------------------------------------
// File seek defines
// ----------------------------------------------------------------------------
#define PRFILE_SEEK_CUR             1
#define PRFILE_SEEK_END             2
#define PRFILE_SEEK_SET             0


// ----------------------------------------------------------------------------
// File name/path defines
// ----------------------------------------------------------------------------
#define FILE_ARCHIVES_MAX           8
#if defined(PLATFORM_PC)
  #include <Windows.h>
  #define FILE_MAX_FILENAME_SIZE      MAX_PATH
  #define FILE_MAX_FILEPATH_SIZE      MAX_PATH

#elif defined(PLATFORM_LINUX)
  #include <linux/limits.h>
  #define FILE_MAX_FILENAME_SIZE      NAME_MAX
  #define FILE_MAX_FILEPATH_SIZE      PATH_MAX

#else
  #define FILE_MAX_FILENAME_SIZE      256
  #define FILE_MAX_FILEPATH_SIZE      256

#endif


// ----------------------------------------------------------------------------
// Used to convert 4 ascii characters into an identifier.
// ----------------------------------------------------------------------------
#define PRMAKE4(a,b,c,d)    (((a) << 24) +  ((b) << 16) +  ((c) << 8) +  (d))


// ----------------------------------------------------------------------------
// Used to uniquely identify an engine binary file.
// ----------------------------------------------------------------------------
#define PRBIN_MAGIC1        PRMAKE4('P', 'R', 'O', 'T')
#define PRBIN_MAGIC2        PRMAKE4('E', 'U', 'S', '.')


// ----------------------------------------------------------------------------
// Binary file types - For when we allow both types.
// ----------------------------------------------------------------------------
#define PRFILE_TYPE_BG      PRMAKE4('B', 'a', 'c', 'k')
#define PRFILE_TYPE_SP      PRMAKE4('S', 'p', 'r', 't')
#define PRFILE_TYPE_MH      PRMAKE4('M', 'e', 's', 'h')
#define PRFILE_TYPE_SV      PRMAKE4('S', 'a', 'v', 'e')


// Typedef: prFileHeader
//      The standard file header for all binary files made by the proteus engine tools.
typedef struct prFileHeader
{
    u32     magic1;         ///< File identifer.
    u32     magic2;         ///< File identifer.
    u32     size;           ///< File size.
    u32     type;           ///< File type.
    u32     checksum;       ///< The checksum.
    u32     version;        ///< Version.
    u32     exp1;           ///< Expansion
    u32     exp2;           ///< Expansion

} prFileHeader;


// Typedef: prChunkHeader
//      Main header for chunked files
typedef struct prChunkHeader
{
    u32     magic1;         ///< File identifer.
    u32     magic2;         ///< File identifer.
    u32     count;          ///< Number of chunks.
    u32     type;           ///< File type.

} prChunkHeader;


// Typedef: prFileChunk
//      A chunk header for chunked files.
typedef struct prFileChunk
{
    u32     type;           ///< Chunk type.
    u32     size;           ///< Size of the data. Note this does not include the size of the chunk header.
    u32     entries;        ///< Number of data entries.
    u32     offset;         ///< Offset to the next chunk. Offset is zero for the last chunk.

} prFileChunk;


// Typedef: prSaveHeader
//      The standard save header for all game save files.
typedef struct prSaveHeader
{
    prSaveHeader()
        : magic1    (0) // Keep cppcheck happy
        , magic2    (0)
        , size      (0)
        , checksum  (0)
    {
        //magic1   = 0; 
        //magic2   = 0; 
        //size     = 0;   
        //checksum = 0;
    }

    u32     magic1;         ///< File identifer.
    u32     magic2;         ///< File identifer.
    u32     size;           ///< File size.
    u32     checksum;       ///< The checksum

} prSaveHeader;


// ----------------------------------------------------------------------------
// Defines
// ----------------------------------------------------------------------------
#define MAGIC1              PRMAKE4('p', 'r', 'o', 't')         // Used to uniquely identify an archive file.
#define MAGIC2              PRMAKE4('a', 'r', 'c', 'h')         // Used to uniquely identify an archive file.


// ----------------------------------------------------------------------------
// Fat header.
// ----------------------------------------------------------------------------
typedef struct prFatHeader
{
    u32 entries;
    u32 size;
    u32 magic1;
    u32 magic2;

} prFatHeader;


// ----------------------------------------------------------------------------
// Represents an archive entry.
// ----------------------------------------------------------------------------
typedef struct prArcEntry
{
    u32     hash;
    u32     offset;
    u32     filesize;
    u32     compressedSize;
    u8      compressed;
    u8      compressionType;
    u8      accessed;
    u8      exp1;
    char    filename[FILE_MAX_FILENAME_SIZE];

} prArcEntry;


// Function: prCalculateChecksum
//      Calculates a checksum.
//
// Parameters:
//      data     - A pointer to the data to checksum
//      datasize - The data size in bytes
//
// Returns:
//      The checksum value
u32 prCalculateChecksum(u8 *data, u32 datasize);
