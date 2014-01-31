/**
 * @file       prFileShared.h
 * @brief      Contains shared file system data.
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


#ifndef __PRFILESHARED_H
#define __PRFILESHARED_H


#include "../core/prTypes.h"


// ----------------------------------------------------------------------------
// File seek defines
// ----------------------------------------------------------------------------
#define PRFILE_SEEK_CUR             1
#define PRFILE_SEEK_END             2
#define PRFILE_SEEK_SET             0


// ----------------------------------------------------------------------------
// Misc
// ----------------------------------------------------------------------------
#define FILE_ARCHIVES_MAX           8
#define FILE_MAX_FILENAME_SIZE      260


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


/// @struct     prFileHeader
/// @brief      The standard file header for all binary files made by the proteus engine tools.
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


/// @struct     prChunkHeader
/// @brief      Main header for chunked files
typedef struct prChunkHeader
{
    u32     magic1;         ///< File identifer.
    u32     magic2;         ///< File identifer.
    u32     count;          ///< Number of chunks.
    u32     type;           ///< File type.

} prChunkHeader;


/// @struct     prFileChunk
/// @brief      A chunk header for chunked files.
typedef struct prFileChunk
{
    u32     type;           ///< Chunk type.
    u32     size;           ///< Size of the data. Note this does not include the size of the chunk header.
    u32     entries;        ///< Number of data entries.
    u32     offset;         ///< Offset to the next chunk. Offset is zero for the last chunk.

} prFileChunk;


/// @struct     prSaveHeader
/// @brief      The standard save header for all game save files.
typedef struct prSaveHeader
{
    prSaveHeader()
    {
        magic1   = 0; 
        magic2   = 0; 
        size     = 0;   
        checksum = 0;
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
    u8      exp0;
    u8      exp1;
    char    filename[FILE_MAX_FILENAME_SIZE];

} prArcEntry;


/// @brief      Calculates a checksum.
/// @param      data     - A pointer to the data to checksum
/// @param      datasize - The data size in bytes
/// @return     The checksum value
u32 prCalculateChecksum(u8 *data, u32 datasize);


#endif//__PRFILESHARED_H
