/**
 * prKeyboard_PC.cpp
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


#if defined(PLATFORM_PC)


#include "../debug/prTrace.h"
#include "prKeyboard_PC.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prKeyboard_PC::prKeyboard_PC() : prKeyboard()
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prKeyboard_PC::~prKeyboard_PC()
{
}


/// ---------------------------------------------------------------------------
/// Injects the held down keys
/// ---------------------------------------------------------------------------
void prKeyboard_PC::KeyboardInjectDown(u32 charcode)
{
    if (charcode < PRKEY_BUFFER_SIZE)
    {
        mKeyBuffer[charcode] = 1;
        //prTrace(LogError, "INJECT CHARCODE: %i - %c\n", charcode, charcode);
    }
}


/// ---------------------------------------------------------------------------
/// Engine call. Injects the pressed keys
/// ---------------------------------------------------------------------------
void prKeyboard_PC::KeyboardInjectPressed(u32 charcode)
{
    mLastKey = charcode;
    //prTrace(LogError, "PRESSED CHARCODE: %i - %c\n", charcode, charcode);
}


/// ---------------------------------------------------------------------------
/// Injects the controls keys
/// ---------------------------------------------------------------------------
void prKeyboard_PC::KeyboardInjectControl(u32 charcode)
{
    mControlKeys |= charcode;
}


#endif//PLATFORM_PC
