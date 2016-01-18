/**
 * prKeyboard.cpp
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


#include "prKeyboard.h"


#if (defined(PLATFORM_PC) || defined(PLATFORM_LINUX))


#include <cstring>
#include "../core/prMacros.h"
#include "../debug/prTrace.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prKeyboard::prKeyboard() : prCoreSystem(PRSYSTEM_KEYBOARD, "prKeyboard")
{
    mLastKey            = 0;
    mControlKeys        = 0;
    mPrevControlKeys    = 0;

    memset(mPrevKeyBuffer, 0, sizeof(mKeyBuffer));
    memset(mKeyBuffer,     0, sizeof(mKeyBuffer));
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prKeyboard::~prKeyboard()
{
}


/// ---------------------------------------------------------------------------
/// Updates the keyboard. Called by the engine. Do not call
/// ---------------------------------------------------------------------------
void prKeyboard::Update()
{
    // Store last keys set
    mPrevControlKeys = mControlKeys;
    memcpy(mPrevKeyBuffer, mKeyBuffer, sizeof(mKeyBuffer));

    // Clear old presses
    mLastKey            = 0;
    mControlKeys        = 0;
    memset(mKeyBuffer, 0, sizeof(mKeyBuffer));
}


/// ---------------------------------------------------------------------------
/// Tests if the key passed is currently being held down.
/// ---------------------------------------------------------------------------
bool prKeyboard::IsKeyDown(u32 charcode, u32 ctrlKeys) const
{
    bool result = false;

    if (charcode < PRKEY_BUFFER_SIZE)
    {
        // Just character code
        if (charcode && ctrlKeys == 0)
        {
            result = (mKeyBuffer[charcode] != 0);
        }
        // Just control keys
        else if (ctrlKeys && charcode == 0)
        {            
            result = (PRTEST_FLAG(mControlKeys, ctrlKeys) == ctrlKeys);
        }
        // Character and control keys
        else if (charcode && ctrlKeys)
        {
            result = ((mKeyBuffer[charcode] != 0) &&
                      (PRTEST_FLAG(mControlKeys, ctrlKeys) == ctrlKeys));
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Tests if the key passed has been pressed
/// ---------------------------------------------------------------------------
bool prKeyboard::IsKeyPressed(u32 charcode, u32 ctrlKeys) const
{
    bool result = false;

    if (charcode < PRKEY_BUFFER_SIZE)
    {
        // Just character code
        if (charcode && ctrlKeys == 0)
        {
            //prTrace(LogError, "charcode == %i (%i, %i)\n", charcode, mPrevKeyBuffer[charcode], mKeyBuffer[charcode]);

            result = (mPrevKeyBuffer[charcode] == 0 &&      // Prev must not be same
                      mKeyBuffer[charcode]     != 0);
        }
        // Just control keys
        else if (ctrlKeys && charcode == 0)
        {            
            result = ((PRTEST_FLAG(mPrevControlKeys, ctrlKeys) == 0) &&
                      (PRTEST_FLAG(mControlKeys, ctrlKeys) == ctrlKeys));
        }
    }

    return result;
}


#endif
