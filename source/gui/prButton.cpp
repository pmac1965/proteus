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
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../core/prStringUtil.h"


// Namespaces
namespace Proteus {
namespace Gui {


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prButton::prButton(const char *name, prSpriteManager *pSpriteManager) : prWidget(WT_Button, name, pSpriteManager)
                                                                      , m_textColour(prColour::White)
{
    m_sprite            = nullptr;
    m_width             = 0;
    m_height            = 0;
    m_buttonState       = BS_NORMAL;
    m_prevX             = -1;
    m_prevY             = -1;
    m_prButtonListener  = nullptr;
    m_textScale         = 1.0f;
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
            SetCurrentFrame(BS_DISABLED);
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
        // Set position here as well as this can get called before updated.
        m_sprite->pos.x = pos.x;
        m_sprite->pos.y = pos.y;
        
        // Pre-draw?
        if (m_prButtonListener)
            m_prButtonListener->PreDraw(GetName(), m_sprite->pos);

        // Draw button
        m_sprite->Draw();

        // Draw the font (Bitmap)
        if (m_pBmpfont && m_text.Length() > 0)
        {
            Proteus::Math::prVector2 size = m_pBmpfont->MeasureString(m_text.Text(), m_textScale);
            
            f32 x = pos.x +  (m_sprite->GetFrameWidth()  >> 1);
            f32 y = pos.y + ((m_sprite->GetFrameHeight() >> 1) - (size.y / 2));
            
            m_pBmpfont->Draw(x, y, m_textScale, m_textColour, prBitmapFont::ALIGN_CENTRE, m_text.Text());
        }

        // Draw the font (TTF)
        else if (m_pTtfFont && m_text.Length() > 0)
        {
            Proteus::Math::prVector2 size = m_pTtfFont->MeasureString(m_text.Text(), m_textScale);
            
            f32 x = pos.x +  (m_sprite->GetFrameWidth()  >> 1);
            f32 y = pos.y + ((m_sprite->GetFrameHeight() >> 1) - ((size.y / 2) + (size.y / 10))); // Extra Y is to bring TTF closer to center
            
            m_pTtfFont->Draw(x, y, m_textScale, m_textColour, prTrueTypeFont::ALIGN_CENTRE, m_text.Text());
        }

        // Post-draw?
        if (m_prButtonListener)
            m_prButtonListener->PostDraw(GetName(), m_sprite->pos);
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
        SetCurrentFrame(BS_NORMAL);

        m_prevX = e.x;
        m_prevY = e.y;

        if (InButtonsRect(e.x, e.y))
        {
            m_buttonState = BS_HOVER;
            SetCurrentFrame(BS_HOVER);

            if (m_prButtonListener)
                m_prButtonListener->OnButtonPressed(GetName());
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
        SetCurrentFrame(BS_NORMAL);

        m_prevX = e.x;
        m_prevY = e.y;

        if (InButtonsRect(e.x, e.y))
        {
            m_buttonState = BS_HOVER;
            SetCurrentFrame(BS_HOVER);
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
        SetCurrentFrame(BS_NORMAL);

        if (InButtonsRect(m_prevX, m_prevY))
        {
            m_buttonState = BS_SELECTED;
            SetCurrentFrame(BS_SELECTED);

            if (m_prButtonListener)
                m_prButtonListener->OnButtonReleased(GetName());
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
        m_sprite = nullptr;
        m_width  = 0;
        m_height = 0;
    }
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
/// Sets the button as selected.
/// ---------------------------------------------------------------------------
void prButton::SetSelected()
{
    m_buttonState = BS_SELECTED;
    SetCurrentFrame(BS_SELECTED);
}


/// ---------------------------------------------------------------------------
/// Tests if a touch is *over* the button
/// ---------------------------------------------------------------------------
bool prButton::InButtonsRect(s32 x, s32 y)
{
    Proteus::Math::prRect<s32> rect = Proteus::Math::prRect<s32>((s32)pos.y, (s32)pos.x, (s32)(pos.y + m_height), (s32)(pos.x + m_width));
    return rect.PointInside(x, y);
}


/// ---------------------------------------------------------------------------
/// Sets the current button frame
/// ---------------------------------------------------------------------------
void prButton::SetCurrentFrame(s32 frame)
{
    if (!m_animated)
    {
        if (m_prButtonListener)
        {
            s32 newFrame = 0;

            if (m_prButtonListener->SetButtonFrame(GetName(), frame, newFrame))
            {
                m_sprite->SetFrame(frame);
            }
            else
            {
                m_sprite->SetFrame(newFrame);
            }
        }
        else
        {
            m_sprite->SetFrame(frame);
        }
    }
}


}}// Namespaces
