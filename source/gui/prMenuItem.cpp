// File: prMenuItem.cpp
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


#include "prMenuItem.h"
#include "../core/prMacros.h"
#include "../input/prKeys.h"
#include "../debug/prTrace.h"


//using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Gui {


// Support for keys
struct MenuKeys
{
    u32   key;
    char *text;

}
menuKeys[] = 
{
#if defined(PLATFORM_PC)
    { PRKEY_A,      "A" }, 
    { PRKEY_B,      "B" },
    { PRKEY_C,      "C" },
    { PRKEY_D,      "D" },
    { PRKEY_E,      "E" },
    { PRKEY_F,      "F" },
    { PRKEY_G,      "G" },
    { PRKEY_H,      "H" },
    { PRKEY_I,      "I" },
    { PRKEY_J,      "J" },
    { PRKEY_K,      "K" },
    { PRKEY_L,      "L" },
    { PRKEY_M,      "M" },
    { PRKEY_N,      "N" },
    { PRKEY_O,      "O" },
    { PRKEY_P,      "P" },
    { PRKEY_Q,      "Q" },
    { PRKEY_R,      "R" },
    { PRKEY_S,      "S" },
    { PRKEY_T,      "T" },
    { PRKEY_U,      "U" },
    { PRKEY_V,      "V" },
    { PRKEY_W,      "W" },
    { PRKEY_X,      "X" },
    { PRKEY_Y,      "Y" },
    { PRKEY_Z,      "Z" },
    { PRKEY_DEL,    "DEL"},

#else
    { 0, nullptr },

#endif
};


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenuItem::prMenuItem(prString text, u32 id) : mText(text), mId(id)
{
    mIcon       = nullptr;
    mEnabled    = PRTRUE;
    mControlKey = PRFALSE;
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMenuItem::prMenuItem(const char *text, u32 id) : mText(text), mId(id)
{
    mIcon       = nullptr;
    mEnabled    = PRTRUE;
    mControlKey = PRFALSE;
}


/// ---------------------------------------------------------------------------
/// Sets the keyboard command keys for the menu item
/// ---------------------------------------------------------------------------
void prMenuItem::SetCommandKeys(prMenuItemControlKey controlKeys, u32 character) 
{
    // Reset
    mCommandKeyText.Set("");
    mControlKey = PRTRUE;

    // Set command keys
    if (controlKeys & prMenuItemControlKey::Control)
    {
        mCommandKeyText.Append("Ctrl+");
    }
    if (controlKeys & prMenuItemControlKey::Shift)
    {
        mCommandKeyText.Append("Shift+");
    }
    if (controlKeys & prMenuItemControlKey::Alt)
    {
        mCommandKeyText.Append("Alt+");
    }

    // Set key
    for(s32 i=0; i<PRARRAY_SIZE(menuKeys); i++)
    {
        if (character == menuKeys[i].key)
        {
            mCommandKeyText.Append(menuKeys[i].text);
            break;
        }
    }
}


}}// Namespaces
