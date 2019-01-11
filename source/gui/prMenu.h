// File: prMenu.h
//      The standard GUI menu designed to simulate the OS menus. Intended 
//      primarily for game engine tools
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


#include "../prConfig.h"
#include "prWidget.h"
#include "../display/prSpriteManager.h"
#include "../math/prVector2.h"
#include <map>


// Forward declarations
class prBitmapFont;
class prMouse;
class prMessageManager;
class prSprite;


// Namespaces
namespace Proteus {
namespace Gui {


// Forward declarations
class prMenuItem;


// Class: prMenu
//      The standard GUI menu designed to simulate the OS menus. Intended 
//      primarily for game engine tools
class prMenu : public prWidget
{
private:
    // Friends
    friend class prGui;

    // Method: prMenu
    //      Ctor
    prMenu(const char *name, prSpriteManager *pSpriteManager);

    // Method: ~prMenu
    //      Dtor
    ~prMenu();

public:
    // Method: Update
    //      Updates the menu. Default version is not used as only
    //      open and active menus are drawn.
    //
    //      Updated via the menu strip when required
    //
    void Update(f32 dt) override {}

    // Method: Draw
    //      Draws the menu. Default version is not used as only
    //      open and active menus are drawn
    void Draw() override {}

    // Method: DrawMenu
    //      Draws the menu.
    //
    // Returns:
    //      True if a menu item is chosen, false otherwise
    bool DrawMenu(f32 x, f32 y, f32 width);

    // Method: OnPressed
    //      Input handler.
    void OnPressed(const prTouchEvent &e) override {}

    // Method: OnMove
    //      Input handler.
    void OnMove(const prTouchEvent &e) override {}

    // Method: OnReleased
    //      Input handler.
    void OnReleased(const prTouchEvent &e) override {}

    // Method: AddMenuItem
    //      Adds a menu item to the menu
    //
    // Warning:
    //      The menu items ID needs to be unique
    bool AddMenuItem(prMenuItem *pMenuItem);

    // Method: FindMenuItem
    //      Finds a menu item in the menu if it exists
    prMenuItem *FindMenuItem(u32 id);

    // Method: SetText
    //      Sets the menus text. E.g. like 'File'
    void SetText(const char *text);

    // Method: GetText
    //      Gets the menus text
    const char *GetText() const { return mText.Text(); }    

    // Method: GetTextSize
    //      Gets the size of the menus text
    //
    // Returns:
    //      Returns a <prVector2> containing the width and the height of the text
    const Proteus::Math::prVector2 &GetTextSize() const { return mSize; }

    // Method: IsOpened
    //      Is the menu opened and items selectable?
    PRBOOL IsOpened() const { return mOpened; }

    // Method: SetOpened
    //      Opens a menu so items are selectable
    void SetOpened(PRBOOL state) { mOpened = state; }

    // Method: SetGrayFontAsBitmapFont
    //      Sets the font to be used for inactive menu items text
    void SetGrayFontAsBitmapFont(prBitmapFont *pFont) { mpGrayTextBMF = pFont; }

    // Method: SetMessageManager
    //      Sets the message manager which receives the menu items selected ID's
    void SetMessageManager(prMessageManager *pMessageManager) { mpMessageManager = pMessageManager; }


private:
    // Sets the text max width and height
    void AdjustSize(const Proteus::Math::prVector2 &size, prMenuItem *pMenuItem);

    // Tests if a touch is *over* a menu strip item
    bool InMenuItemRect(s32 x, s32 y, s32 width, s32 height, s32 xpos, s32 ypos);


private:
    std::map<u32, prMenuItem *> mItems;                 // Menu items
    prBitmapFont               *mpGrayTextBMF;
    prMouse                    *mpMouse;
    prMessageManager           *mpMessageManager;
    prSprite                   *mpIconSubMenu;
    prString                    mText;
    Proteus::Math::prVector2    mSize;
    s32                         mMaxWidth;
    s32                         mMaxHeight;
    PRBOOL                      mOpened;
};


}}// Namespaces
