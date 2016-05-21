// File: prPane.h
//      A GUI pane acts like a dockable container, and can contain
//      multiple GUI components. Created for the game editor and 
//		not suitable for normal game use
/**
 * Copyright 2016 Paul Michael McNab
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
#include "../core/prTypes.h"
#include "../core/prString.h"
#include "../display/prColour.h"


// Forward declarations
class prSprite;


// Namespaces
namespace Proteus {
namespace Gui {


// Class: prPane
//      A GUI pane acts like a dockable container, and can contain
//      multiple GUI components. Created for the game editor and 
//		not suitable for normal game use
class prPane : public prWidget
{
private:
    // Friends
    friend class prGui;

    // Method: prPane
    //      Ctor
    //
    // Parameters:
    //      name           - The name of the pane
    //      pSpriteManager - A pointer to a sprite manager.
    prPane(const char *name, prSpriteManager *pSpriteManager);

    // Method: ~prPane
    //      Dtor
    ~prPane() {}

public:
    // Method: Update
    //      Updates the pane
    //
    // Parameters:
    //      dt - Delta time
    void Update(Proteus::Core::f32 dt) override;

    // Method: Draw
    //      Draws the pane
    void Draw() override;

    // Method: OnPressed
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void OnPressed(const prTouchEvent &e);

    // Method: OnMove
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void OnMove(const prTouchEvent &e);

    // Method: OnReleased
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void OnReleased(const prTouchEvent &e);

    // Method: SetSizeAndPos
    //      Sets the panes size and position relative to screen 0, 0
    //
    // Parameters:
    //      x		- X position
    //      y		- Y position
    //      width	- Pane width
    //      height	- Pane height
    void SetSizeAndPos(Proteus::Core::s32 x, Proteus::Core::s32 y, Proteus::Core::s32 width, Proteus::Core::s32 height) { mXpos = x; mYpos = y; mWidth = width; mHeight = height; }

    // Method: SetTitleText
    //      Sets the panes title text
    //
    // Parameters:
    //      text - The panes name (Optional)
	void SetTitleText(const char *text) { if (text && *text) { mName.Set(text); } }

	void SetIcons(prSprite *pIcon, prSprite *pOptionsIcon) { mpPaneIcon = pIcon; mpOptionsIcon = pOptionsIcon; }

private:
	prString				mName;
    Proteus::Core::s32		mXpos;
    Proteus::Core::s32		mYpos;
    Proteus::Core::s32		mWidth;
    Proteus::Core::s32		mHeight;
	prSprite			   *mpPaneIcon;
	prSprite			   *mpOptionsIcon;
};


}} // Namespaces
