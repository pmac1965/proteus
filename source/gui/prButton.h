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
//#include "prGuiListener.h"
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
//class BitmapFont;
class prButtonListener;
class prSpriteManager;


// Class: prButton
//      Standard GUI button
class prButton : public prWidget
{
private:
    // Friends
    friend class prGui;

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

    void OnPressed(prTouchEvent e);
    void OnMove(prTouchEvent e);
    void OnReleased(prTouchEvent e);

    void SetSprite(prSprite *pSprite);
    //void SetFont(BitmapFont *pFont);
    void SetText(const char *text);

    void RegisterListener(prButtonListener *pListener);

    s32 GetWidth() const { return m_width; }
    s32 GetHeight() const { return m_height; }

    void SetTextColour(prColour c) { m_textColour = c; }
    void SetTextScale(f32 scale) { m_textScale = scale; }

private:

    bool InButtonsRect(s32 x, s32 y);


private:

    prSprite               *m_sprite;
    //BitmapFont             *m_font;
    prString                m_text;
    s32                     m_buttonState;
    s32                     m_width;
    s32                     m_height;
    s32                     m_prevX;
    s32                     m_prevY;
    prButtonListener       *m_prButtonListener;
    prColour                m_textColour;
    f32                     m_textScale;
};


#endif//__PRBUTTON_H
