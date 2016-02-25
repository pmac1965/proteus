// File: prMenuItem.h
//      A menu item for the engine menus
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
    explicit prMenuItem(prString text, Proteus::Core::u32 id);
    
    // Method: prMenuItem
    //      Ctor
    explicit prMenuItem(const char *text, Proteus::Core::u32 id);
    
    // Method: prMenuItem
    //      Ctor
    ~prMenuItem()
    {}

    // Method: SetIcon
    //      Sets the icon for the menu item
    void SetIcon(prSprite *pIcon) { mIcon = pIcon; }

    // Method: GetIcon
    //      Gets the icon for the menu item
    prSprite *GetIcon() { return mIcon; }

    // Method: SetCommandKeys
    //      Sets the keyboard command keys for the menu item
    //
    // Parameters:
    //      keys - The keyboard command keys to activate this menu item
    void SetCommandKeys(Proteus::Core::u32 keys) { mKeys = keys; }

    // Method: SetEnabled
    //      Sets the enabled status
    void SetEnabled(Proteus::Core::PRBOOL state) { mEnabled = state; }

    // Method: GetEnabled
    //      Gets the enabled status
    Proteus::Core::PRBOOL GetEnabled() const { return mEnabled; }

    // Method: GetID
    //      Gets the menu items ID
    Proteus::Core::u32 GetID() const { return mId; }

    // Method: GetText
    //      Gets the menu items text
    const char *GetText() const { return mText.Text(); }


private:
    Proteus::Core::u32      mId;
    prString                mText;
    prSprite               *mIcon;
    Proteus::Core::u32      mKeys;
    Proteus::Core::PRBOOL   mEnabled;
};


}}// Namespace
