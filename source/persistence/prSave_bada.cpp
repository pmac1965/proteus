/**
 * prSave_bada.cpp
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


#if defined(PLATFORM_BADA)


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "prSave_bada.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../file/prFileSystem.h"
#include "../file/prFileShared.h"
#include "../persistence/prEncryption.h"


/// ---------------------------------------------------------------------------
/// Implementation data.
/// ---------------------------------------------------------------------------
typedef struct SaveBadaImplementation
{
    SaveBadaImplementation()
    {
        pFile    = NULL;
        filesize = 0;
    }

    FILE   *pFile;
    s32     filesize;

} SaveBadaImplementation;


/// ---------------------------------------------------------------------------
/// Gets the save/load path.
/// ---------------------------------------------------------------------------
void GetSaveLoadPath(char *pBuffer)
{
    if (pBuffer)
    {
        strcpy(pBuffer, "/Home");
    }
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSaveBada::prSaveBada() : pImpl (new SaveBadaImplementation())
                         , imp   (*pImpl)
{
    // Write startup info?
    #if defined(_DEBUG) || defined(DEBUG)
    {
        char path[FILE_MAX_FILENAME_SIZE];
        GetSaveLoadPath(path);
        prTrace("Save path: %s\n", path);
    }
    #endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSaveBada::~prSaveBada()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Start a save.
/// ---------------------------------------------------------------------------
bool prSaveBada::SaveBegin()
{
    PRASSERT(pImpl);
    bool result = false;

    if (!ErrorOccurred())
    {
        // Create filename and path
        char filename[FILE_MAX_FILENAME_SIZE];

        GetSaveLoadPath(filename);
        strcat(filename, "/");
        strcat(filename, m_filename);

        // Create the file?
        imp.pFile = fopen(filename, "wb");
        if (imp.pFile == NULL)
        {
            prTrace("SaveBegin: Failed to create file: %s\n", filename);
            SetError(-1);
            return result;
        }

        result = true;
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Update the save
/// ---------------------------------------------------------------------------
bool prSaveBada::SaveUpdate()
{
    PRASSERT(pImpl);
    bool result = false;

    if (!ErrorOccurred())
    {
        // Create header.
        prSaveHeader header;
        header.magic1   = PRMAKE4('p','r','o','t');
        header.magic2   = PRMAKE4('s','a','v','e');
        header.size     = m_saveSize + sizeof(prSaveHeader);
        header.checksum = prCalculateChecksum((u8 *)m_saveSata, m_saveSize);


        // Write header
        prEncrypt((u8*)&header, sizeof(prSaveHeader));
        int bytes = fwrite(&header, 1, sizeof(prSaveHeader), imp.pFile);
        if (bytes != sizeof(prSaveHeader))
        {
            prTrace("A:SaveUpdate - Bytes written did not match save size\n");
        }

        // Write data.
        prEncrypt(m_saveSata, m_saveSize);
        bytes = fwrite(m_saveSata, 1, m_saveSize, imp.pFile);
        if (bytes != m_saveSize)
        {
            prTrace("B:SaveUpdate - Bytes written did not match save size\n");
        }


        // Errors?
        if (ferror(imp.pFile))
        {
            prTrace("SaveUpdate - %s\n", strerror(errno));
            clearerr(imp.pFile);
            SetError(-1);
            fclose(imp.pFile);
            imp.pFile = NULL;
        }
        else
        {
            result = true;
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Finishes the save.
/// ---------------------------------------------------------------------------
bool prSaveBada::SaveEnd()
{
    PRASSERT(pImpl);
    bool result = false;

    if (!ErrorOccurred())
    {
        if (imp.pFile)
        {
            fclose(imp.pFile);
            imp.pFile = NULL;
        }

        result = true;
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Starts a load
/// ---------------------------------------------------------------------------
bool prSaveBada::LoadBegin()
{
    PRASSERT(pImpl);
    bool result = false;

    if (!ErrorOccurred())
    {
        // Create filename and path
        char filename[FILE_MAX_FILENAME_SIZE];
        GetSaveLoadPath(filename);
        strcat(filename, "/");
        strcat(filename, m_filename);

        imp.filesize = 0;

        // Exists?
        if (prFileExist(filename))
        {
            // Open the file.
            imp.pFile = fopen(filename, "rb");
            if (imp.pFile == NULL)
            {
                prTrace("LoadBegin: Failed to open file: %s\n", filename);
                SetError(-1);
                return result;
            }
            else
            {
                // Get file size
                if (fseek(imp.pFile, 0, SEEK_END) == 0)
                {
                    imp.filesize = ftell(imp.pFile);
                    rewind(imp.pFile);       
                }

                // Check size
                if (imp.filesize == 0)
                {
                    prTrace("File is empty. Load cancelled: %s\n", filename);
                    SetError(-1);
                    return result;
                }

                if ((u32)imp.filesize < sizeof(prSaveHeader))
                {
                    prTrace("File is too small. Load cancelled: %s\n", filename);
                    SetError(-1);
                    return result;
                }


                // All okay?
                if (!ErrorOccurred())
                {
                    result = true;
                }
            }
        }
        else
        {
            prTrace("File doesn't exist. Load cancelled: %s\n", filename);
            SetError(-1);
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Updates a load 
/// ---------------------------------------------------------------------------
bool prSaveBada::LoadUpdate()
{
    PRASSERT(pImpl);
    PRASSERT(m_loadData);
    PRASSERT(m_loadSize);

    bool result = false;

    if (!ErrorOccurred())
    {
        u32 loadsize = imp.filesize - sizeof(prSaveHeader);

        *m_loadData = new u8[loadsize];
        *m_loadSize = loadsize;

        if (*m_loadData)
        {
            prSaveHeader header;

            // Read the header.
            if (fread(&header, 1, sizeof(prSaveHeader), imp.pFile) != sizeof(prSaveHeader))
            {
                if (ferror(imp.pFile))
                {
                    prTrace("LoadUpdate: fread error: %s\n", strerror(errno));
                    clearerr(imp.pFile);
                }

                SetError(-1);
                return result;
            }


            // Decrypt header
            prDecrypt((u8 *)&header, sizeof(prSaveHeader));


            // Read the save.        
            if (fread(*m_loadData, 1, loadsize, imp.pFile) != loadsize)
            {
                if (ferror(imp.pFile))
                {
                    prTrace("LoadUpdate: fread error: %s\n", strerror(errno));
                    clearerr(imp.pFile);
                }

                SetError(-1);
                return result;
            }
                
                
            // All okay?
            if (!ErrorOccurred())
            {
                // Decrypt.
                prDecrypt((u8 *)*m_loadData, loadsize);

                // Validate header.
                if (header.magic1   == PRMAKE4('p','r','o','t') &&
                    header.magic2   == PRMAKE4('s','a','v','e') &&
                    header.size     == (u32)imp.filesize      &&
                    header.checksum == prCalculateChecksum((u8 *)*m_loadData, loadsize))
                {
                    // Decrypt.
                }
                else
                {
                    prTrace("Invalid save header\n");
                    SetError(-1);
                    return result;
                }
            }


            // Was the save file bad?
            if (ErrorOccurred())
            {
                fclose(imp.pFile);
                imp.pFile = NULL;
                PRSAFE_DELETE_ARRAY(*m_loadData);
            }
            else
            {
                result = true;
            }
        }
        else
        {
            prTrace("Unable to allocate buffer to load save file.\n");
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Finishes a load
/// ---------------------------------------------------------------------------
bool prSaveBada::LoadEnd()
{
    PRASSERT(pImpl);
    bool result = false;

    if (!ErrorOccurred())
    {
        if (imp.pFile)
        {
            fclose(imp.pFile);
            imp.pFile = NULL;
        }

        result = true;
    }

    return result;
}


#endif
