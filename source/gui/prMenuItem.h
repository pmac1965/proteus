// File: prMenuItem.h
//      A menu item for the engine menus. Intended 
//      primarily for game engine tools
//
// Notes:
//      Once added the menus are resonsible for deleting the menu items, so
//      they can be added and forgotten
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


#include "../core/prString.h"


// Forward declarations
class prSprite;


// Namespaces
namespace Proteus {
namespace Gui {


// Enum: prMenuItemControlKey
//      Optional activation keys
//
// - None
// - Control
// - Shift
// - Alt
enum prMenuItemControlKey
{
    None    = 0x00000000,
    Control = 0x00000001,
    Shift   = 0x00000002,
    Alt     = 0x00000004
};


// Class: prMenuItem
//      A menu item for the engine menus
//
// Notes:
//      Just like windows a text string of "-" will result in a separator
class prMenuItem
{
public:
    // Method: prMenuItem
    //      Ctor
    explicit prMenuItem(prString text, u32 id);
    
    // Method: prMenuItem
    //      Ctor
    explicit prMenuItem(const char *text, u32 id);
    
    // Method: prMenuItem
    //      Ctor
    ~prMenuItem()
    {}

    // Method: SetCommandKeys
    //      Sets the keyboard command keys for the menu item
    //
    // Parameters:
    //      keys      - The keyboard control key to activate this menu item (Ctrl, Alt, Shift)
    //      character - The key to activate this menu item. 'A, Z, Delete, Tab, etc)
    //
    // See Also:
    //      <prMenuItemControlKey>
    void SetCommandKeys(prMenuItemControlKey controlKeys = prMenuItemControlKey::None, u32 character = 0);

    // Method: GotCommandKeys
    //      Do we have a command key sequence?
    PRBOOL GotCommandKeys() const { return mControlKey; }

    // Method: SetIcon
    //      Sets the icon for the menu item
    void SetIcon(prSprite *pIcon) { mIcon = pIcon; }

    // Method: GetIcon
    //      Gets the icon for the menu item
    prSprite *GetIcon() { return mIcon; }

    // Method: SetEnabled
    //      Sets the enabled status
    void SetEnabled(PRBOOL state) { mEnabled = state; }

    // Method: GetEnabled
    //      Gets the enabled status
    PRBOOL GetEnabled() const { return mEnabled; }

    // Method: GetID
    //      Gets the menu items ID
    u32 GetID() const { return mId; }

    // Method: GetText
    //      Gets the menu items text
    const char *GetText() const { return mText.Text(); }

    // Method: GetCommandKeyText
    //      Gets the menu items command key text
    const char *GetCommandKeyText() const { return mCommandKeyText.Text(); }


private:
    u32         mId;
    prString    mText;
    prString    mCommandKeyText;
    prSprite   *mIcon;
    PRBOOL      mControlKey;
    PRBOOL      mEnabled;
};


}}// Namespace
