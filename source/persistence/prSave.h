// File: prSave.h
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


#ifndef __PRSAVE_H
#define __PRSAVE_H


#include "../core/prTypes.h"


// Forward declarations
struct SaveImplementation;


// Class: prIoResultCallback
//      Handler for IO callbacks
class prIoResultCallback
{
public:
    // Results
    enum
    {
         IO_RESULT_SUCCESS,                        
         IO_RESULT_FAILURE,                        
    };

public:

    // Method: SaveResult
    //      This method needs to be added to a load/save handling class.
    //
    // Parameters:
    //      save - The save result
    virtual void SaveResult(Proteus::Core::s32 result) = 0;

    // Method: LoadResult
    //      This method needs to be added to a load/save handling class.
    //
    // Parameters:
    //      save - The save result
    virtual void LoadResult(Proteus::Core::s32 result) = 0;
};


// Class: prSave
//      A class designed to handle cross platform saving.
class prSave
{
public:
    // Method: prSave
    //      Ctor
    //
    // Parameters:
    //      folder  - A folder to make for the save data
    //
    // Notes:
    //      If the system requires a folder name, then it should be supplied.
    //      Normally this would be the games name. Please remember to change for
    //      different game versions. e.g; "game_lite" or "game_free"
    prSave(const char *folder);

    // Method: ~prSave
    //      Dtor
    ~prSave();

    // Method: Draw
    //      If autosaving is enabled, this will draw the autosave icon.
    void Draw();

    // Method: Update
    //      Updates the save system. Call once per frame.
    void Update();
    
    // Method: StartSave
    //      Starts saving.
    //
    // Parameters:
    //      pData    - A pointer to the save data
    //      size     - The size of the save data
    //      cd       - A callback handler for system messages
    //      filename - The filename for the save
    //
    // Notes:
    //      Makes a copy of the save data. So you can delete it once save has begun.
    void StartSave(void *pData, Proteus::Core::s32 size, prIoResultCallback *cb, const char *filename);

    // Method: StartLoad
    //      Starts loading.
    //
    // Parameters:
    //      ppData    -  A pointer to a pointer to the loaded game save.
    //      pSize     -  The size of the loaded game save
    //      cb        -  A callback handler for system messages
    //      filename  -  The name of the save to look for
    //
    // Notes:
    //      The save system will allocate the buffer, but it is up to the game
    //      to free the buffer.
    //      
    //      As the load may take a while, the passed in pointers MUST not be
    //      local variables.
    void StartLoad(void **ppData, Proteus::Core::s32 *pSize, prIoResultCallback *cb, const char *filename);

    // Method: IsWorking
    //      Are we saving or loading.
    bool IsWorking() const;


private:

    // Report result to user
    void Report(int result);

    // Save
    void BeginSave();
    void WriteSave();
    void CloseSave();

    // Load
    void BeginLoad();
    void ReadLoad();
    void CloseLoad();


private:

    // Stops passing by value and assignment.
    prSave(const prSave&);
    const prSave& operator = (const prSave&);


private:

    // Don't change order.
    SaveImplementation  *pImpl;
    SaveImplementation  &imp;
};


#endif//__PRSAVE_H
