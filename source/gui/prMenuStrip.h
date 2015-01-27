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


#ifndef __PRMENUSTRIP_H
#define __PRMENUSTRIP_H


#include "prWidget.h"
#include <list>


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
    ~prMenuStrip();


public:
    // Method: Update
    //      Updates the menu strip.
    void Update(f32 dt);

    // Method: Draw
    //      Draw the menu strip.
    void Draw();

    // Method: OnPressed
    //      Input handler.
    //
    // Notes:
    //      *Do not call*
    void OnPressed(const prTouchEvent &e);

    // Method: OnMove
    //      Input handler.
    //
    // Notes:
    //      *Do not call*
    void OnMove(const prTouchEvent &e);

    // Method: OnReleased
    //      Input handler.
    //
    // Notes:
    //      *Do not call*
    void OnReleased(const prTouchEvent &e);

    // Method: AddMenu
    //      Adds a menu to the menu strip
    void AddMenu(prMenu *pMenu);


private:
    std::list<prMenu*>  mMenus;

    f32 mScreenWidth;
};


}}// Namespaces


#endif//__PRMENUSTRIP_H
