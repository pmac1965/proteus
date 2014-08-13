// File: prDialog.h
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


#ifndef __PRDIALOG_H
#define __PRDIALOG_H


#include "../prConfig.h"
#include "prWidget.h"
#include "prButtonListener.h"
#include "../display/prSpriteManager.h"
#include "../display/prColour.h"
#include "../math/prVector2.h"


// Defines
#define DIALOG_MIN_BUTTONS  1
#define DIALOG_MAX_BUTTONS  3


// Forward declarations
class prSprite;
class prBitmapFont;
class prTrueTypeFont;
class prDialogListener;
class prButton;


// Class: prDialog
//      Standard GUI dialog
//
// Notes:
//      To get information on button presses a class must implement <prButtonListener>
//      and register itself as a listener.
//
// See Also:
//      <prButtonListener>
class prDialog : public prWidget, prButtonListener
{
private:
    // Friends
    friend class prGui;

    // Method: prDialog
    //      Ctor
    prDialog(const char *name, prSpriteManager *pSpriteManager);

    // Method: ~prDialog
    //      Dtor
    ~prDialog();


public:
    // Method: Update
    //      Updates the dialog.
    void Update(f32 dt);

    // Method: Draw
    //      Draw the dialog.
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

    // Method: SetBackdrop
    //      Set the dialog backdrop image.
    //
    // Parameters:
    //      backdrop - A sprite filename
    void SetBackdrop(const char *backdrop);

    // Method: AddButton
    //      Adds a button.
    //
    // Parameters:
    //      pFile - A sprite filename
    //      pName - Buttons name
    void AddButton(const char *pFile, const char *pName);

    // Method: SetFont
    //      Sets the font for the buttons and the dialog.
    void SetFont(prBitmapFont *pFont);

    // Method: SetTTFFont
    //      Sets the dialogs font
    void SetTTFFont(prTrueTypeFont *pFont);

    // Set the dialog title.
    void SetTitle(const char *text, f32 scale = 1.0f);

    // Set the dialog text.
    void SetText(const char *text, f32 scale = 1.0f);
    
    // Set callback listener.
    void RegisterListener(prDialogListener *pListener);

    // Callback handlers.
    void OnButtonPressed(const char *name);
    void OnButtonReleased(const char *name);

    //
    s32 GetWidth() const;
    
    //
    s32 GetHeight() const;

    // x == pixels in from side, Y == pixels up from bottom.
    void SetButtonOffset(u32 x, u32 y) { m_offsetButtons.x = (f32)x;
                                         m_offsetButtons.y = (f32)y; }

    // x == pixels in from side, Y == pixels down from top.
    void SetTitleOffset(u32 x, u32 y) { m_offsetTitle.x = (f32)x;
                                        m_offsetTitle.y = (f32)y; }

    // x == pixels in from side, Y == pixels down from top.
    void SetTextOffset(u32 x, u32 y) { m_offsetText.x = (f32)x;
                                       m_offsetText.y = (f32)y; }

    // Gets the button count.
    s32 ButtonCount() const { return m_buttonCount; }

    // Colour setter.
    void SetButtonTextColour(prColour c) { m_textColour = c; }
    void SetButtonTextScale(f32 scale) { m_textScale = scale; }
    

private:

    prSprite               *m_spriteBackdrop;
    prBitmapFont           *m_pFont;
    prTrueTypeFont         *m_pttfFont;
    prDialogListener       *m_prIDialogListener;
    prButton               *m_buttons       [DIALOG_MAX_BUTTONS];
    prVector2               m_buttonsPos    [DIALOG_MAX_BUTTONS];
    prVector2               m_offsetButtons;
    prVector2               m_offsetTitle;
    prVector2               m_offsetText;
    s32                     m_buttonCount;
    prString                m_title;
    prString                m_text;
    f32                     m_titleScale;
    f32                     m_textScale;
    prColour                m_textColour;
};


#endif//__PRDIALOG_H
