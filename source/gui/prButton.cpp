/**
 * prButton.cpp
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


#include "prButton.h"
#include "prButtonListener.h"
#include "../display/prSprite.h"
#include "../display/prBitmapFont.h"
#include "../display/prTrueTypeFont.h"
#include "../display/prSpriteManager.h"
#include "../math/prRect.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prButton::prButton(const char *name, prSpriteManager *pSpriteManager) : prWidget(WT_Button, name, pSpriteManager)
                                                                      , m_textColour(prColour::White)
{
    m_sprite            = NULL;
    m_font              = NULL;
    m_ttfFont           = NULL;
    m_width             = 0;
    m_height            = 0;
    m_buttonState       = BS_NORMAL;
    m_prevX             = -1;
    m_prevY             = -1;
    m_prButtonListener  = NULL;
    m_textScale         = 1.0f;
    m_extendX           = 0;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prButton::~prButton()
{
    //if (m_pSpriteManager && m_sprite)
    //{
    //    prTrace("GUI Button release sprite %s\n", m_sprite->Name());
    //    m_pSpriteManager->Release(m_sprite);
    //}
}


/// ---------------------------------------------------------------------------
/// Updates the button
/// ---------------------------------------------------------------------------
void prButton::Update(f32 dt)
{
    if (m_sprite)
    {
        // Set position
        m_sprite->pos.x = pos.x;
        m_sprite->pos.y = pos.y;

        // Ensure disabled state always appears
        if (!m_enabled)
        {
            m_buttonState = BS_DISABLED;
            m_sprite->SetFrame(BS_DISABLED);
        }

        // Animate?
        if (m_animated)
        {
            m_sprite->Update(dt);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Draws the button
/// ---------------------------------------------------------------------------
void prButton::Draw()
{
    if (m_sprite)
    {
        // Set position here as well as this can get called before update.
        m_sprite->pos.x = pos.x;
        m_sprite->pos.y = pos.y;
        
        // Draw button
        m_sprite->Draw();

        // Draw the font (Bitmap)
        if (m_font && m_text.Length() > 0)
        {
            prVector2 size = m_font->MeasureString(m_text.Text(), m_textScale);
            
            f32 x = pos.x +  (m_sprite->GetFrameWidth()  >> 1);
            f32 y = pos.y + ((m_sprite->GetFrameHeight() >> 1) - (size.y / 2));
            
            m_font->Draw(x, y, m_textScale, m_textColour, prBitmapFont::ALIGN_CENTRE, m_text.Text());
        }

        // Draw the font (TTF)
        else if (m_ttfFont && m_text.Length() > 0)
        {
            prVector2 size = m_ttfFont->MeasureString(m_text.Text(), m_textScale);
            
            f32 x = pos.x +  (m_sprite->GetFrameWidth()  >> 1);
            f32 y = pos.y + ((m_sprite->GetFrameHeight() >> 1) - ((size.y / 2) + (size.y / 10))); // Extra Y is to compensate and bring closer to center
            
            m_ttfFont->Draw(x, y, m_textScale, m_textColour, prTrueTypeFont::ALIGN_CENTRE, m_text.Text());
        }
    }
}


/// ---------------------------------------------------------------------------
/// Gets called when a pressed event occurs
/// ---------------------------------------------------------------------------
void prButton::OnPressed(const prTouchEvent &e)
{
    if (m_sprite && m_enabled)
    {
        m_buttonState = BS_NORMAL;
        m_sprite->SetFrame(BS_NORMAL);

        m_prevX = e.x;
        m_prevY = e.y;

        if (InButtonsRect(e.x, e.y))
        {
            m_buttonState = BS_HOVER;
            m_sprite->SetFrame(BS_HOVER);

            if (m_prButtonListener)
                m_prButtonListener->OnButtonPressed(Name());
        }
    }
}


/// ---------------------------------------------------------------------------
/// Gets called when a move event occurs
/// ---------------------------------------------------------------------------
void prButton::OnMove(const prTouchEvent &e)
{
    if (m_sprite && m_enabled)
    {
        m_buttonState = BS_NORMAL;
        m_sprite->SetFrame(BS_NORMAL);

        m_prevX = e.x;
        m_prevY = e.y;

        if (InButtonsRect(e.x, e.y))
        {
            m_buttonState = BS_HOVER;
            m_sprite->SetFrame(BS_HOVER);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Gets called when a released event occurs
/// ---------------------------------------------------------------------------
void prButton::OnReleased(const prTouchEvent &e)
{
    PRUNUSED(e);

    if (m_sprite && m_enabled)
    {
        m_buttonState = BS_NORMAL;
        m_sprite->SetFrame(BS_NORMAL);

        if (InButtonsRect(m_prevX, m_prevY))
        {
            m_buttonState = BS_SELECTED;
            m_sprite->SetFrame(BS_SELECTED);

            if (m_prButtonListener)
                m_prButtonListener->OnButtonReleased(Name());
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets or removes the buttons sprite.
/// ---------------------------------------------------------------------------
void prButton::SetSprite(prSprite *pSprite)
{
    if (pSprite)
    {
        m_sprite = pSprite;
        m_width  = pSprite->GetFrameWidth();
        m_height = pSprite->GetFrameHeight();
    }
    else
    {
        m_sprite = NULL;
        m_width  = 0;
        m_height = 0;
    }
}


/// ---------------------------------------------------------------------------
/// Sets or removes the buttons text font.
/// ---------------------------------------------------------------------------
void prButton::SetFont(prBitmapFont *pFont)
{ 
    m_font = pFont;
}


/// ---------------------------------------------------------------------------
/// Sets the buttons font
/// ---------------------------------------------------------------------------
void prButton::SetTTFFont(prTrueTypeFont *pFont)
{
    m_ttfFont = pFont;
}


/// ---------------------------------------------------------------------------
/// Sets or removes the buttons text.
/// ---------------------------------------------------------------------------
void prButton::SetText(const char *text)
{
    m_text.Set(text);
}


/// ---------------------------------------------------------------------------
/// Sets callbacks
/// ---------------------------------------------------------------------------
void prButton::RegisterListener(prButtonListener *pListener)
{
    m_prButtonListener = pListener;
}


/// ---------------------------------------------------------------------------
/// Tests for on over
/// ---------------------------------------------------------------------------
bool prButton::InButtonsRect(s32 x, s32 y)
{
    prRect rect = prRect((s32)pos.y, (s32)pos.x, (s32)(pos.y + m_height), (s32)(pos.x + m_width + m_extendX));
    return rect.PointInside(x, y);
}

