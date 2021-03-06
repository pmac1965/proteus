// File: prButton.h
//      Standard GUI button
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
#include "../core/prString.h"
#include "../display/prColour.h"


// Forward declarations
class prSprite;
class prSpriteManager;


// Namespaces
namespace Proteus {
namespace Gui {


// Enum: prButtonStates
//      Button states.
//
// - BS_NORMAL   (Sprite frame 0)
// - BS_HOVER    (Sprite frame 1)
// - BS_SELECTED (Sprite frame 2)
// - BS_DISABLED (Sprite frame 3)
//
enum prButtonStates
{
    BS_NORMAL,
    BS_HOVER,
    BS_SELECTED,
    BS_DISABLED
};


// Forward declarations
class prButtonListener;


// Class: prButton
//      Standard GUI button
//
// See Also:
//      <prButtonListener>
//      <prTouchEvent>
class prButton : public prWidget
{
private:
    // Friends
    friend class prGui;
    friend class prDialog;

    // Method: prButton
    //      Ctor
    //
    // Parameters:
    //      name           - The name of the button
    //      pSpriteManager - A pointer to a sprite manager.
    prButton(const char *name, prSpriteManager *pSpriteManager);

    // Method: ~prButton
    //      Dtor
    ~prButton() {}


public:
    // Method: Update
    //      Updates the button
    //
    // Parameters:
    //      dt - Delta time
    void Update(f32 dt) final override;

    // Method: Draw
    //      Draws the button
    void Draw() final override;

    // Method: OnPressed
    //      A touch event handler.
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void OnPressed(const prTouchEvent &e) final override;

    // Method: OnMove
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void OnMove(const prTouchEvent &e) final override;

    // Method: OnReleased
    //      A touch event handler
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    void OnReleased(const prTouchEvent &e) final override;

    // Method: SetSprite
    //      Sets the buttons sprite
    void SetSprite(prSprite *pSprite);

    // Method: SetText
    //      Sets the buttons text
    void SetText(const char *text);

    // Method: RegisterListener
    //      Sets the button message receiver
    void RegisterListener(prButtonListener *pListener);

    // Method: SetSelected
    //      Sets the button as selected.
    //
    // Notes:
    //      Used to highlight a specific button
    void SetSelected();

    // Method: GetWidth
    //      Gets the button width
    s32 GetWidth() const { return m_width; }

    // Method: GetHeight
    //      Gets the button height
    s32 GetHeight() const { return m_height; }

    // Method: SetTextColour
    //      Sets the button text colour
    void SetTextColour(prColour c) { m_textColour = c; }

    // Method: SetTextScale
    //      Sets the button text scale
    void SetTextScale(f32 scale) { m_textScale = scale; }


private:
    // Tests if a touch is *over* the button
    bool InButtonsRect(s32 x, s32 y);

    // Sets the current button frame
    void SetCurrentFrame(s32 frame);


private:
    prSprite           *m_sprite;
    prString            m_text;
    s32                 m_buttonState;
    s32                 m_width;
    s32                 m_height;
    s32                 m_prevX;
    s32                 m_prevY;
    prButtonListener   *m_prButtonListener;
    prColour            m_textColour;
    f32                 m_textScale;
};


}}// Namespaces
