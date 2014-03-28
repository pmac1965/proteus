// File: prSaveBase.h
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


#ifndef __PRSAVEBASE_H
#define __PRSAVEBASE_H


#include "../core/prTypes.h"


// Class: prSaveBase
//      Saving base class
class prSaveBase
{
public:
    // Method: prSaveBase
    //      Ctor
    prSaveBase();

    // Method: ~prSaveBase
    //      Dtor
    virtual ~prSaveBase();

    // Method: Init
    //      Save initialisation.
    void Init(char *folder, char *filename, void  *saveData, s32  saveSize);

    // Method: Init
    //      Load initialisation.
    void Init(char *folder, char *filename, void **loadData, s32 *loadSize);

    // Method: ErrorOccurred
    //      Checks for errors.
    bool ErrorOccurred() const { return (m_error != 0); }

    // Method: SetError
    //      Sets an error.
    void SetError(s32 error) { m_error = error; }

    // Method: SaveBegin
    //      Required save state
    virtual bool SaveBegin()  = 0;

    // Method: SaveUpdate
    //      Required save state
    virtual bool SaveUpdate() = 0;

    // Method: SaveEnd
    //      Required save state
    virtual bool SaveEnd()    = 0;

    // Method: LoadBegin
    //      Required load state
    virtual bool LoadBegin()  = 0;

    // Method: LoadUpdate
    //      Required load state
    virtual bool LoadUpdate() = 0;

    // Method: LoadEnd
    //      Required load state
    virtual bool LoadEnd()    = 0;

    // Method: Draw
    //      Draws when saving is occuring
    virtual void Draw() {}


protected:

    s32     m_error;
    char   *m_folder;
    char   *m_filename;
    u8     *m_saveSata;
    s32     m_saveSize;
    s32    *m_loadSize;
    u8    **m_loadData;
};


#endif//__PRSAVEBASE_H
