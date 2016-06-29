// File: prMenuStrip.h
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


#include "prWidget.h"
#include <list>


// Forward declarations
class prRegistry;
class prMouse;


// Namespaces
namespace Proteus {
namespace Gui {


// Forward declarations
class prMenu;


// Class: prMenuStrip
//      Standard GUI menu strip
//
// Notes:
//      This class is simply a container for menus.
//
// See Also:
//      <prMenu>
class prMenuStrip : public prWidget//, prButtonListener
{
private:
    // Friends
    friend class prGui;

    // Method: prMenuStrip
    //      Ctor
    prMenuStrip(const char *name, prSpriteManager *pSpriteManager);

    // Method: ~prMenuStrip
    //      Dtor
    ~prMenuStrip() {}


public:
    // Method: Update
    //      Updates the menu strip.
    void Update(Proteus::Core::f32 dt) override;

    // Method: Draw
    //      Draw the menu strip.
    void Draw() override;

    // Method: OnPressed
    //      Input handler.
    //
    // Notes:
    //      *Do not call*
    void OnPressed(const prTouchEvent &/*e*/) override {}

    // Method: OnMove
    //      Input handler.
    //
    // Notes:
    //      *Do not call*
    void OnMove(const prTouchEvent &/*e*/) override {}

    // Method: OnReleased
    //      Input handler.
    //
    // Notes:
    //      *Do not call*
    void OnReleased(const prTouchEvent &/*e*/) override {}

    // Method: AddMenu
    //      Adds a menu to the menu strip
    void AddMenu(prMenu *pMenu);

    // Closes all the menus
    void SetAllMenusClosed();


private:
    // Tests if a touch is *over* a menu strip item
    bool InMenuRect(Proteus::Core::s32 x, Proteus::Core::s32 y, Proteus::Core::s32 width, Proteus::Core::s32 height, Proteus::Core::s32 xpos, Proteus::Core::s32 ypos);


private:
    std::list<prMenu*>      mMenus;
    prRegistry             *mpRegistry;
    prMouse                *mpMouse;
    Proteus::Core::PRBOOL   mInit;
    Proteus::Core::f32      mStripHeight;
    Proteus::Core::f32      mScreenWidth;
    Proteus::Core::f32      mStartX;
    Proteus::Core::f32      mStartY;
    Proteus::Core::f32      mOpen;
};


}}// Namespaces
