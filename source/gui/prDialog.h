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


// Namespaces
namespace Proteus {
namespace Gui {


// Forward declarations
class prDialogListener;
class prButton;


// Class: prDialog
//      Standard GUI dialog which implements basic dialog functionality
//
// Notes:
//      To get information on button presses a class must implement <prDialogListener>
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
    //
    // Parameters:
    //      dt - Delta time
    void Update(Proteus::Core::f32 dt);

    // Method: Draw
    //      Draw the dialog.
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

    // Method: OnButtonPressed
    //      Callback handler for buttons
    //
    // Notes:
    //      *Do not call*
    void OnButtonPressed(const char *name);

    // Method: OnButtonReleased
    //      Callback handler for buttons
    //
    // Notes:
    //      *Do not call*
    void OnButtonReleased(const char *name);

    // Method: SetBackdrop
    //      Set the dialog backdrop image.
    //
    // Parameters:
    //      backdrop - A *sprite* filename
    //
    // See Also:
    //      <prSprite>
    void SetBackdrop(const char *backdrop);

    // Method: AddButton
    //      Adds a button.
    //
    // Parameters:
    //      pFile - A sprite filename
    //      pName - Buttons name
    //      pText - Buttons text
    //
    // Returns:
    //      Returns a pointer to the button created, or NULL on failure
    prButton *AddButton(const char *pFile, const char *pName, const char *pText);

    // Method: SetTitle
    //      Sets the dialogs title text
    void SetTitle(const char *text, Proteus::Core::f32 scale = 1.0f);

    // Method: SetText
    //      Sets the dialogs body text.
    void SetText(const char *text, Proteus::Core::f32 scale = 1.0f);
    
    // Method: RegisterListener
    //      Set callback listener.
    void RegisterListener(prDialogListener *pListener);

    // Method: GetWidth
    //      Returns the width of the dialogs backdrop
    Proteus::Core::s32 GetWidth() const;
    
    // Method: GetHeight
    //      Returns the height of the dialogs backdrop
    Proteus::Core::s32 GetHeight() const;

    // Method: SetButtonOffset
    //      Sets an offset which is applied to the *buttons* position
    //
    // Parameters:
    //      x - pixels in from side. Moves in X from left/right
    //      y - pixels up from bottom.
    //
    // Notes:
    //      Please ae aware that the buttons will move in different directions
    //      in the X direction, as this is method is intended to bring the buttons
    //      away from the edge
    void SetButtonOffset(Proteus::Core::f32 x, Proteus::Core::f32 y) { m_offsetButtons.x = x;
                                                                       m_offsetButtons.y = y; }

    // Method: SetTitleOffset
    //      Sets an offset which is applied to the *title* text position
    //
    // Parameters:
    //      x - pixels from side
    //      y - pixels from top.
    void SetTitleOffset(Proteus::Core::f32 x, Proteus::Core::f32 y) { m_offsetTitle.x = x;
                                                                      m_offsetTitle.y = y; }

    // Method: SetTextOffset
    //      Sets an offset which is applied to the *body* text position
    //
    // Parameters:
    //      x - pixels from side
    //      y - pixels from top.
    void SetTextOffset(Proteus::Core::f32 x, Proteus::Core::f32 y) { m_offsetText.x = x;
                                                                     m_offsetText.y = y; }

    // Method: ButtonCount
    //      Gets the button count.
    Proteus::Core::s32 ButtonCount() const { return m_buttonCount; }

    // Method: SetTextScale
    //      Sets the dialogs text colour
    void SetTextColour(prColour c) { m_textColour = c; }

    // Method: SetTextScale
    //      Sets the dialogs text scale
    void SetTextScale(Proteus::Core::f32 scale) { m_textScale = scale; }
    
    // Method: SetTextAlignment
    //      Sets the dialogs text alignments
    //
    // Parameters:
    //      title - The title text alignment
    //      body  - The body text alignment
    //
    // Notes:
    //      Valid values are from the <prBitmapFont::prBitmapFontAlign> or <prTrueTypeFont::prTrueTypeFontAlign>
    //      enumerations. The values are interchangeable
    //
    //      The default values are for left aligned text
    //
    // See Also:
    //      <prBitmapFont::prBitmapFontAlign>
    //
    // See Also:
    //      <prTrueTypeFont::prTrueTypeFontAlign>
    void SetTextAlignment(Proteus::Core::s32 title, Proteus::Core::s32 body) { mFontAlignmentTitle = title;
                                                                               mFontAlignmentBody  = body; }
    

private:
    prSprite                *m_spriteBackdrop;
    prDialogListener        *m_pDialogListener;
    prButton                *m_buttons       [DIALOG_MAX_BUTTONS];
    Proteus::Math::prVector2 m_buttonsPos    [DIALOG_MAX_BUTTONS];
    Proteus::Math::prVector2 m_offsetButtons;
    Proteus::Math::prVector2 m_offsetTitle;
    Proteus::Math::prVector2 m_offsetText;
    Proteus::Core::s32       m_buttonCount;
    prString                 m_title;
    prString                 m_text;
    Proteus::Core::f32       m_titleScale;
    Proteus::Core::f32       m_textScale;
    prColour                 m_textColour;
    Proteus::Core::s32       mFontAlignmentTitle;
    Proteus::Core::s32       mFontAlignmentBody;
};


}}// Namespaces


#endif//__PRDIALOG_H
