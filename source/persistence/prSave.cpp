/**
 * prSave.cpp
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


#include <stdlib.h>
#include <string.h>
#include "prSave.h"
#include "prSaveBase.h"
#include "../file/prFile.h"
#include "../file/prFileManager.h"
#include "../core/prCore.h"
#include "../core/prDefines.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../debug/prAssert.h"
#include "../file/prFileShared.h"


#if defined(PLATFORM_PC)
  #include "prSave_pc.h"

#elif defined(PLATFORM_IOS)
  #include "prSave_ios.h"

#elif defined(PLATFORM_ANDROID)
  #include "prSave_android.h"

#elif defined(PLATFORM_LINUX)
  #include "prSave_linux.h"

#elif defined(PLATFORM_MAC)
  #include "prSave_mac.h"

#else
  #error Unsupported platform.
#endif


//using namespace Proteus::Core;


// prSave modes
enum
{
    SAVE_MODE_NONE,
    SAVE_MODE_BEGIN_SAVE,
    SAVE_MODE_WRITE_SAVE,
    SAVE_MODE_CLOSE_SAVE,
    SAVE_MODE_BEGIN_LOAD,
    SAVE_MODE_READ_LOAD,
    SAVE_MODE_CLOSE_LOAD,
    SAVE_MODE_COUNT,
};


// Implementation
typedef struct SaveImplementation
{
    // Ctor
    SaveImplementation()
    {
        pSave       = nullptr;
        pSaveData   = nullptr;

        Reset();
        memset(folder, 0, sizeof(folder));

        #if defined(PLATFORM_PC)
          pSave = new prSavePC();

        #elif defined(PLATFORM_IOS)
          pSave = new prSaveIos();

        #elif defined(PLATFORM_ANDROID)
          pSave = new prSaveAndroid();

        #elif defined(PLATFORM_LINUX)
          pSave = new prSaveLinux();
        
        #elif defined(PLATFORM_MAC)
          pSave = new prSaveMac();

        #else
          #error Unsupported platform.

        #endif
    }

    
    // Dtor
    ~SaveImplementation()
    {
        PRSAFE_DELETE(pSave);
    }
    
    
    // Copy constructor - Stops warning
    SaveImplementation(const SaveImplementation&);


    // Resets back to normal after save/load operation
    void Reset()
    {
        PRSAFE_FREE(pSaveData);

        mode        = SAVE_MODE_NONE;
        saveSize    = -1;
        pLoadSize   = nullptr;
        ppLoadData  = nullptr;
        callback    = nullptr;

        memset(filename, 0, sizeof(filename));
    }


    s32                 mode;                               // Operating mode.
    s32                 saveSize;                           // Saved data size.
    s32                *pLoadSize;                          // Loaded data size.
    void               *pSaveData;                          // Data to save.
    void              **ppLoadData;                         // Loaded data.
    prIoResultCallback *callback;                           // Reporting callback
    char                filename[FILE_MAX_FILENAME_SIZE];   // filename.
    char                folder  [FILE_MAX_FILENAME_SIZE];   // folder name.
    prSaveBase         *pSave;                              // Platform specific functions.

} SaveImplementation;


/// ---------------------------------------------------------------------------
/// Constructor 
/// ---------------------------------------------------------------------------
prSave::prSave()  : pImpl (new SaveImplementation())
                  , imp   (*pImpl)
{
    PRASSERT(pImpl);

    // Get the file manager, so we can get save data folder/path
    prFileManager *pFileManager = static_cast<prFileManager *>(prCoreGetComponent(PRSYSTEM_FILEMANAGER));
    PRASSERT(pFileManager);
    prStringCopySafe(imp.folder, pFileManager->GetSaveDataPath(), sizeof(imp.folder));
}


/// ---------------------------------------------------------------------------
/// Destructor 
/// ---------------------------------------------------------------------------
prSave::~prSave()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Draws the saving image 
/// ---------------------------------------------------------------------------
void prSave::Draw()
{
    PRASSERT(pImpl);
    PRASSERT(imp.pSave);
    imp.pSave->Draw();
}


/// ---------------------------------------------------------------------------
/// Updates the IO task
/// ---------------------------------------------------------------------------
void prSave::Update()
{
    PRASSERT(pImpl);
    PRASSERT(imp.pSave);

    switch(imp.mode)
    {
    case SAVE_MODE_NONE:
        break;

    case SAVE_MODE_BEGIN_SAVE:
        BeginSave();
        break;

    case SAVE_MODE_WRITE_SAVE:
        WriteSave();
        break;

    case SAVE_MODE_CLOSE_SAVE:
        CloseSave();
        break;

    case SAVE_MODE_BEGIN_LOAD:
        BeginLoad();
        break;

    case SAVE_MODE_READ_LOAD:
        ReadLoad();
        break;

    case SAVE_MODE_CLOSE_LOAD:
        CloseLoad();
        break;
    }
}


/// ---------------------------------------------------------------------------
/// Start saving.
/// ---------------------------------------------------------------------------
void prSave::StartSave(void *pData, s32 size, prIoResultCallback *cb, const char *filename)
{
    PRASSERT(pImpl);
    PRASSERT(pData);
    PRASSERT(size > 0);
    PRASSERT(cb);
    PRASSERT(filename && *filename);
    PRASSERT(imp.pSave);


    // Working?
    if (imp.mode != SAVE_MODE_NONE)
    {
        prTrace(prLogLevel::LogError, "Error: The save system is currently saving. Cannot restart.\n");
        return;
    }


    // Create copy of the save data.
    imp.pSaveData = malloc(size);
    if (imp.pSaveData == nullptr)
    {
        prTrace(prLogLevel::LogError, "Error: Unable to allocate enough memory for the save data.\n");
        return;
    }
    else
    {
        // prSave copy
        memcpy(imp.pSaveData, pData, size);
    }


    // Start
    imp.callback  = cb;
    imp.saveSize  = size;
    imp.mode      = SAVE_MODE_BEGIN_SAVE;

    prStringCopySafe(imp.filename, filename, sizeof(imp.filename));

    imp.pSave->Init(imp.folder, imp.filename, imp.pSaveData, imp.saveSize);
}


/// ---------------------------------------------------------------------------
/// Start loading.
/// ---------------------------------------------------------------------------
void prSave::StartLoad(void **ppData, s32 *pSize, prIoResultCallback *cb, const char *filename)
{
    PRASSERT(pImpl);
    PRASSERT(ppData);
    PRASSERT(pSize);
    PRASSERT(filename && *filename);

    // Working?
    if (imp.mode != SAVE_MODE_NONE)
    {
        prTrace(prLogLevel::LogError, "Error: The current save system operation has not completed.\n");
        return;
    }

    imp.callback   = cb;
    imp.pLoadSize  = pSize;
    imp.ppLoadData = ppData;
    prStringCopySafe(imp.filename, filename, sizeof(imp.filename));

    // Start
    imp.mode = SAVE_MODE_BEGIN_LOAD;

    imp.pSave->Init(imp.folder, imp.filename, ppData, pSize);
}


/// ---------------------------------------------------------------------------
/// Report result to user
/// ---------------------------------------------------------------------------
void prSave::Report(int result)
{
    PRASSERT(pImpl);

    // Call the report callback.
    if (imp.callback)
    {
        if (PRBETWEEN(imp.mode, SAVE_MODE_BEGIN_SAVE, SAVE_MODE_CLOSE_SAVE))
        {
            imp.callback->SaveResult(result);
        }
        else
        {
            imp.callback->LoadResult(result);
        }
    }

    // And reset.
    imp.Reset();
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prSave::BeginSave()
{
    PRASSERT(pImpl);

    if (imp.pSave)
    {
        if (!imp.pSave->SaveBegin())
        {
            // Check for error
            if (imp.pSave->ErrorOccurred())
            {
                Report(prIoResultCallback::IO_RESULT_FAILURE);
            }
        }
        else
        {
            // Else move on
            imp.mode = SAVE_MODE_WRITE_SAVE;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prSave::WriteSave()
{
    PRASSERT(pImpl);

    if (imp.pSave)
    {
        if (!imp.pSave->SaveUpdate())
        {
            // Check for error
            if (imp.pSave->ErrorOccurred())
            {
                Report(prIoResultCallback::IO_RESULT_FAILURE);
            }
        }
        else
        {
            // Else move on
            imp.mode = SAVE_MODE_CLOSE_SAVE;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prSave::CloseSave()
{
    PRASSERT(pImpl);

    if (imp.pSave)
    {
        if (!imp.pSave->SaveEnd())
        {
            // Check for error
            if (imp.pSave->ErrorOccurred())
            {
                Report(prIoResultCallback::IO_RESULT_FAILURE);
            }
        }
        else
        {
            // And done.
            Report(prIoResultCallback::IO_RESULT_SUCCESS);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prSave::BeginLoad()
{
    PRASSERT(pImpl);

    if (imp.pSave)
    {
        if (!imp.pSave->LoadBegin())
        {
            // Check for error
            if (imp.pSave->ErrorOccurred())
            {
                Report(prIoResultCallback::IO_RESULT_FAILURE);
            }
        }
        else
        {
            // Else move on
            imp.mode = SAVE_MODE_READ_LOAD;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prSave::ReadLoad()
{
    PRASSERT(pImpl);

    if (imp.pSave)
    {
        if (!imp.pSave->LoadUpdate())
        {
            // Check for error
            if (imp.pSave->ErrorOccurred())
            {
                Report(prIoResultCallback::IO_RESULT_FAILURE);
            }
        }
        else
        {
            // Else move on
            imp.mode = SAVE_MODE_CLOSE_LOAD;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prSave::CloseLoad()
{
    PRASSERT(pImpl);

    if (imp.pSave)
    {
        if (!imp.pSave->LoadEnd())
        {
            // Check for error
            if (imp.pSave->ErrorOccurred())
            {
                Report(prIoResultCallback::IO_RESULT_FAILURE);
            }
        }
        else
        {
            // And done.
            Report(prIoResultCallback::IO_RESULT_SUCCESS);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Doing some work?
/// ---------------------------------------------------------------------------
bool prSave::IsWorking() const
{
    PRASSERT(pImpl);
    return (imp.mode != SAVE_MODE_NONE);
}
