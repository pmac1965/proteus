// File: prMenu.h
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
//#include "prButtonListener.h"
#include "../display/prSpriteManager.h"
//#include "../display/prColour.h"
//#include "../math/prVector2.h"


//// Defines
//#define DIALOG_MIN_BUTTONS  1
//#define DIALOG_MAX_BUTTONS  3

//
//// Forward declarations
//class prSprite;
//class prBitmapFont;
//class prTrueTypeFont;


// Namespaces
namespace Proteus {
namespace Gui {


//// Forward declarations
//class prDialogListener;
//class prButton;


// Class: prMenu
//      Standard GUI menu
//
// Notes:
//      To get information from a menu class must implement <prMenuListener>
//      and register itself as a listener.
//
// See Also:
//      <prMenuListener>
class prMenu : public prWidget//, prButtonListener
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
    //      Updates the menu.
    void Update(Proteus::Core::f32 dt);

    // Method: Draw
    //      Draws the menu.
    void Draw();

    // Method: OnPressed
    //      Input handler.
    void OnPressed(const prTouchEvent &e);

    // Method: OnMove
    //      Input handler.
    void OnMove(const prTouchEvent &e);

    // Method: OnReleased
    //      Input handler.
    void OnReleased(const prTouchEvent &e);

    // Method: SetText
    //      Sets the menus text. E.g. like 'File'
    void SetText(const char *text) { m_text.Set(text); }

    // Method: GetText
    //      Gets the menus text
    const char *GetText() const { return m_text.Text(); }
    

private:
    prString                 m_text;
};


}}// Namespaces
