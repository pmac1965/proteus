// File: prButton.h
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


#ifndef __PRBUTTON_H
#define __PRBUTTON_H


#include "../prConfig.h"
#include "prWidget.h"
#include "../core/prString.h"
#include "../display/prColour.h"


// Button states.
enum
{
    BS_NORMAL,
    BS_HOVER,
    BS_SELECTED,
    BS_DISABLED
};


// Forward declarations
class prSprite;
class prBitmapFont;
class prTrueTypeFont;
class prButtonListener;
class prSpriteManager;


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
    ~prButton();


public:
    // Method: Update
    //      Updates the button
    //
    // Parameters:
    //      dt - Delta time
    void Update(f32 dt);

    // Method: Draw
    //      Draws the button
    void Draw();

    // Method: OnPressed
    //      A touch event handler.
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

    // Method: SetSprite
    //      Sets the buttons sprite
    void SetSprite(prSprite *pSprite);

    // Method: SetFont
    //      Sets the buttons font
    void SetFont(prBitmapFont *pFont);

    // Method: SetTTFFont
    //      Sets the buttons font
    void SetTTFFont(prTrueTypeFont *pFont);

    // Method: SetText
    //      Sets the buttons text
    void SetText(const char *text);

    // Method: RegisterListener
    //      Sets the button message receiver
    void RegisterListener(prButtonListener *pListener);

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

    // ??
    void SetExtend(s32 x) { m_extendX = x; }


private:
    bool InButtonsRect(s32 x, s32 y);


private:
    prSprite               *m_sprite;
    prBitmapFont           *m_font;
    prTrueTypeFont         *m_ttfFont;
    prString                m_text;
    s32                     m_buttonState;
    s32                     m_width;
    s32                     m_height;
    s32                     m_prevX;
    s32                     m_prevY;
    prButtonListener       *m_prButtonListener;
    prColour                m_textColour;
    f32                     m_textScale;
    s32                     m_extendX;
};


#endif//__PRBUTTON_H
