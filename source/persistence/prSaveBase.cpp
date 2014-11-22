/**
 * prSaveBase.cpp
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


#include "prSaveBase.h"
#include "../core/prDefines.h"


/// ---------------------------------------------------------------------------
/// Ctor.
/// ---------------------------------------------------------------------------
prSaveBase::prSaveBase()
{
    m_error     = 0;
    m_folder    = NULL;
    m_filename  = NULL;
    m_saveSata  = NULL;
    m_saveSize  = 0;
    m_loadSize  = NULL;
    m_loadData  = NULL;
}


/// ---------------------------------------------------------------------------
/// Dtor.
/// ---------------------------------------------------------------------------
prSaveBase::~prSaveBase()
{
}


/// ---------------------------------------------------------------------------
/// Save initialisation.
/// ---------------------------------------------------------------------------
void prSaveBase::Init(char *folder, char *filename, void *saveData, s32 saveSize)
{
    PRASSERT(folder);
    PRASSERT(filename);
    PRASSERT(saveData);
    PRASSERT(saveSize > 0);

    m_error     = 0;
    m_folder    = folder;
    m_filename  = filename;
    m_saveSata  = (u8*)saveData;
    m_saveSize  = saveSize;
    m_loadSize  = NULL;
    m_loadData  = NULL;
}


/// ---------------------------------------------------------------------------
/// Load initialisation.
/// ---------------------------------------------------------------------------
void prSaveBase::Init(char *folder, char *filename, void **loadData, s32 *loadSize)
{
    PRASSERT(folder);
    PRASSERT(filename);
    PRASSERT(loadData);
    PRASSERT(loadSize);

    m_error     = 0;
    m_folder    = folder;
    m_filename  = filename;
    m_saveSata  = NULL;
    m_saveSize  = 0;
    m_loadSize  = loadSize;
    m_loadData  = (u8**)loadData;
}
