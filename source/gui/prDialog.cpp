/**
 * prDialog.cpp
 *
 *  Copyright 2014 Paul Michael McNab
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */


#include "prDialog.h"
#include "prButton.h"
#include "prDialogListener.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../display/prSprite.h"
#include "../display/prBitmapFont.h"
#include "../display/prTrueTypeFont.h"
#include <cstring>


// Namespaces
namespace Proteus {
namespace Gui {


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Standard GUI dialog
/// ---------------------------------------------------------------------------
prDialog::prDialog(const char *name, prSpriteManager *pSpriteManager) : prWidget(WT_Dialog, name, pSpriteManager)
                                                                      , m_spriteBackdrop      (nullptr)
                                                                      , m_pDialogListener     (nullptr)
                                                                      , m_buttonCount         (0)
                                                                      , m_titleScale          (1.0f)
                                                                      , m_textScale           (1.0f)
                                                                      , mFontAlignmentTitle   (0)       // 0 == ALIGN_LEFT
                                                                      , mFontAlignmentBody    (0)       // 0 == ALIGN_LEFT
{
    memset(&m_buttons, 0, sizeof(m_buttons));
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prDialog::~prDialog()
{
    for (s32 i=0; i<DIALOG_MAX_BUTTONS; i++)
    {
        PRSAFE_DELETE(m_buttons[i])
    }

    if (m_spriteBackdrop)
    {
        m_pSpriteManager->Release(m_spriteBackdrop);
        m_spriteBackdrop = nullptr;
    }
}


/// ---------------------------------------------------------------------------
/// Update the dialog.
/// ---------------------------------------------------------------------------
void prDialog::Update(f32 dt)
{
    for (s32 i=0; i<m_buttonCount; i++)
    {
        if (m_buttons[i])
        {
            m_buttons[i]->Update(dt);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Draw the dialog.
/// ---------------------------------------------------------------------------
void prDialog::Draw()
{
    // Draw the dialog?
    if (m_spriteBackdrop)
    {
        m_spriteBackdrop->pos = pos;
        m_spriteBackdrop->Draw();

        // Draw any buttons
        for (s32 i=0; i<m_buttonCount; i++)
        {
            if (m_buttons[i])
            {
                Proteus::Math::prVector2 position = pos + m_buttonsPos[i];
                
                // Adjust buttons for offset
                switch(m_buttonCount)
                {
                case 1:
                    position.y -= m_offsetButtons.y;
                    break;

                case 2:
                    if (i == 0)
                    {
                        position.x += m_offsetButtons.x;
                        position.y -= m_offsetButtons.y;
                    }
                    else if (i == 1)
                    {
                        position.x -= m_offsetButtons.x;
                        position.y -= m_offsetButtons.y;
                    }
                    break;
                }

                m_buttons[i]->pos = position;
                m_buttons[i]->Draw();
            }
        }


        // Draw title text.
        {
            f32 titleX = pos.x;
            f32 titleY = pos.y;

            switch(mFontAlignmentTitle)
            {
            // Left
            case 0:
                break;

            // Right
            case 1:
                titleX += m_spriteBackdrop->GetFrameWidth();
                break;

            // Center
            case 2:
                titleX += (m_spriteBackdrop->GetFrameWidth() / 2);                
                break;

            default:
                PRWARN("Invalid text alignment");
                break;
            }

            // Add users offset
            titleX += m_offsetTitle.x;
            titleY += m_offsetTitle.y;

            // Draw title text.
            if (m_pBmpfont && m_title.Length() > 0)
            {
                m_pBmpfont->Draw(titleX, titleY, m_titleScale, m_textColour, mFontAlignmentTitle, m_title.Text());
            }
            else if (m_pTtfFont && m_title.Length() > 0)
            {
                m_pTtfFont->Draw(titleX, titleY, m_titleScale, m_textColour, mFontAlignmentTitle, m_title.Text());
            }
        }

        
        // Draw body text
        {
            f32 bodyX = pos.x;
            f32 bodyY = pos.y;

            switch(mFontAlignmentBody)
            {
            // Left
            case 0:
                bodyY += (m_spriteBackdrop->GetFrameHeight() / 2);      // Center in Y
                break;

            // Right
            case 1:
                bodyX += m_spriteBackdrop->GetFrameWidth();             // Plus width
                bodyY += (m_spriteBackdrop->GetFrameHeight() / 2);      // Center in Y
                break;

            // Center
            case 2:
                bodyX += (m_spriteBackdrop->GetFrameWidth() / 2);       // Center in X
                bodyY += (m_spriteBackdrop->GetFrameHeight() / 2);      // Center in Y
                break;

            default:
                PRWARN("Invalid text alignment");
                break;
            }

            // Add users offset
            bodyX += m_offsetText.x;
            bodyY += m_offsetText.y;

            if (m_pBmpfont && m_text.Length() > 0)
            {
                bodyY -= (m_pBmpfont->MeasureString("Y", m_textScale).y / 2); // Center text in Y
                m_pBmpfont->Draw(bodyX, bodyY, m_textScale, m_textColour, mFontAlignmentBody, m_text.Text());
            }
            else if (m_pTtfFont && m_text.Length() > 0)
            {
                bodyY -= (m_pTtfFont->MeasureString("Y", m_textScale).y / 2); // Center text in Y
                m_pTtfFont->Draw(bodyX, bodyY, m_textScale, m_textColour, mFontAlignmentBody, m_text.Text());
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prDialog::OnPressed(const prTouchEvent &e)
{
    for (s32 i=0; i<m_buttonCount; i++)
    {
        if (m_buttons[i])
        {
            m_buttons[i]->OnPressed(e);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prDialog::OnMove(const prTouchEvent &e)
{
    for (s32 i=0; i<m_buttonCount; i++)
    {
        if (m_buttons[i])
        {
            m_buttons[i]->OnMove(e);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Input handler.
/// ---------------------------------------------------------------------------
void prDialog::OnReleased(const prTouchEvent &e)
{
    for (s32 i=0; i<m_buttonCount; i++)
    {
        if (m_buttons[i])
        {
            m_buttons[i]->OnReleased(e);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Callback handlers.
/// ---------------------------------------------------------------------------
void prDialog::OnButtonPressed(const char *name)
{
    if (m_pDialogListener)
    {
        s32 index = 0;
        for (; index < m_buttonCount; index++)
        {
            if (m_buttons[index])
            {
                if (strcmp(m_buttons[index]->GetName(), name) == 0)
                {
                    break;
                }
            }
        }

        m_destroy = m_pDialogListener->OnDlgButtonPressed(name, index);
    }
}


/// ---------------------------------------------------------------------------
/// Callback handlers.
/// ---------------------------------------------------------------------------
void prDialog::OnButtonReleased(const char *name)
{
    if (m_pDialogListener)
    {
        s32 index = 0;

        // Find index
        for (; index < m_buttonCount; index++)
        {
            if (m_buttons[index])
            {
                if (strcmp(m_buttons[index]->GetName(), name) == 0)
                {
                    break;
                }
            }
        }

        m_destroy = m_pDialogListener->OnDlgButtonReleased(name, index);
    }
}


/// ---------------------------------------------------------------------------
/// Set the dialog backdrop.
/// ---------------------------------------------------------------------------
void prDialog::SetBackdrop(const char *backdrop)
{
    PRASSERT(backdrop && *backdrop);

    // Create backdrop sprite.
    m_spriteBackdrop = m_pSpriteManager->Create(backdrop);
}


/// ---------------------------------------------------------------------------
/// Adds a button.
/// ---------------------------------------------------------------------------
prButton *prDialog::AddButton(const char *pFile, const char *pName, const char *pText)
{
    PRASSERT(pFile && *pFile);
    PRASSERT(pName && *pName);
    PRASSERT(pText && *pText);

    prButton *pButton = nullptr;

    if (m_buttonCount < DIALOG_MAX_BUTTONS)
    {
        // Create the button.
        pButton = new prButton(pName, m_pSpriteManager);
        PRASSERT(pButton);
        
        // Create the button.
        prSprite *pSprite = m_pSpriteManager->Create(pFile);
        PRASSERT(pSprite);
        
        m_buttons[m_buttonCount] = pButton;
        m_buttons[m_buttonCount]->SetSprite(pSprite);
        m_buttons[m_buttonCount]->SetBMPFont(m_pBmpfont);
        m_buttons[m_buttonCount]->SetTTFFont(m_pTtfFont);
        m_buttons[m_buttonCount]->SetText(pText);
        m_buttons[m_buttonCount]->SetLayer(GetLayer());
        m_buttons[m_buttonCount]->RegisterListener(this);


    // Set button positions
    switch(m_buttonCount)
    {
        // 1 button
        case 0:
        {
            s32 center          = m_spriteBackdrop->GetFrameWidth() / 2;
            s32 bottom          = m_spriteBackdrop->GetFrameHeight();
            s32 buttonHeight    = m_buttons[0]->GetHeight();
            s32 buttonWidth     = m_buttons[0]->GetWidth();
            m_buttonsPos[0].x   = (f32)(center - (buttonWidth / 2));
            m_buttonsPos[0].y   = (f32)(bottom - buttonHeight);
        }
        break;
        
        // 2 buttons
        case 1:
        {
            s32 width           = m_spriteBackdrop->GetFrameWidth();
            s32 bottom          = m_spriteBackdrop->GetFrameHeight();
            s32 buttonHeight    = m_buttons[0]->GetHeight();
            s32 buttonWidth     = m_buttons[0]->GetWidth();
            m_buttonsPos[0].x   = 0.0f;
            m_buttonsPos[1].x   = (f32)(width - buttonWidth);
            m_buttonsPos[0].y   = (f32)(bottom - buttonHeight);
            m_buttonsPos[1].y   = (f32)(bottom - buttonHeight);
        }
        break;

        // 3 buttons
        case 2:
        break;
        }

        // And done
        m_buttonCount++;
    }


    return pButton;
}


/// ---------------------------------------------------------------------------
/// Set the dialog title.
/// ---------------------------------------------------------------------------
void prDialog::SetTitle(const char *text, f32 scale)
{
    m_title.Set(text);
    m_titleScale = scale;
}


/// ---------------------------------------------------------------------------
/// Set the dialog text.
/// ---------------------------------------------------------------------------
void prDialog::SetText(const char *text, f32 scale)
{
    m_text.Set(text);
    m_textScale = scale;
}


/// ---------------------------------------------------------------------------
/// Set callback listener.
/// ---------------------------------------------------------------------------
void prDialog::RegisterListener(prDialogListener *pListener)
{
    m_pDialogListener = pListener;
}


/// ---------------------------------------------------------------------------
/// Gets dialog width
/// ---------------------------------------------------------------------------
s32 prDialog::GetWidth() const
{
    s32 size = 0;

    if (m_spriteBackdrop)
    {
        size = m_spriteBackdrop->GetFrameWidth();
    }

    return size;
}
    

/// ---------------------------------------------------------------------------
/// Gets dialog height
/// ---------------------------------------------------------------------------
s32 prDialog::GetHeight() const
{
    s32 size = 0;

    if (m_spriteBackdrop)
    {
        size = m_spriteBackdrop->GetFrameHeight();
    }

    return size;
}


}}// Namespaces
