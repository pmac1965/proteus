// ----------------------------------------------------------------------------
//
// File: prButton.cpp
//
//      Description     - Contains the GUI button class
//      Author          - Paul Michael McNab.
//      Copyright       - Copyright Paul Michael McNab. All rights reserved.
//
// Disclaimer:
//
//      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//      "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
//      TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//      PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//      CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//      EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//      PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//      PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//      LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//      NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//      SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// ----------------------------------------------------------------------------
/*

#include "prButton.h"
#include "prIButtonListener.h"
#include "../display/sprite.h"
#include "../display/bitmapFont.h"
#include "../math/rect.h"
#include "../debug/prDebug.h"


// ----------------------------------------------------------------------------
// Ctor
// ----------------------------------------------------------------------------
prButton::prButton(const char *name, SpriteManager *pSpriteManager) : prWidget(WT_Button, name, pSpriteManager)
                                                                    , m_textColour(Colour::White)
{
    //m_pressedCallback   = NULL;
    //m_releasedCallback  = NULL;
    m_sprite            = NULL;
    m_font              = NULL;
    m_width             = 0;
    m_height            = 0;
    m_buttonState       = BS_NORMAL;
    m_prevX             = -1;
    m_prevY             = -1;
    m_prIButtonListener = NULL;
    m_textScale         = 1.0f;
}


// ----------------------------------------------------------------------------
// Dtor
// ----------------------------------------------------------------------------
prButton::~prButton()
{
}


// ----------------------------------------------------------------------------
// Updates the button
// ----------------------------------------------------------------------------
void prButton::Update(f32 dt)
{
    if (m_sprite)
    {
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

        // Set position
        m_sprite->pos.x = pos.x;
        m_sprite->pos.y = pos.y;
    }
}


// ----------------------------------------------------------------------------
// Draws the button
// ----------------------------------------------------------------------------
void prButton::Draw()
{
    if (m_sprite)
    {
        // Set position here as well as this can get called before update.
        m_sprite->pos.x = pos.x;
        m_sprite->pos.y = pos.y;
        
        // Draw button
        m_sprite->Draw();

        if (m_font && m_text.Length() > 0)
        {
            //Vector2 size = m_font->MeasureString(m_text.Text(), m_textScale);
            
            f32 x = pos.x + (m_sprite->FrameWidth()  >> 1);
            f32 y = pos.y;// + (m_sprite->FrameHeight() >> 1);
            
            m_font->Draw(x, y, m_textScale, m_textColour, BitmapFont::ALIGN_CENTRE, m_text.Text());
        }
    }
}


// ----------------------------------------------------------------------------
// Gets called when a pressed event occurs
// ----------------------------------------------------------------------------
void prButton::OnPressed(prTouchEvent e)
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

            //TODO("REMOVE THIS");
            //if (m_pressedCallback)
            //    m_pressedCallback(Name());

            if (m_prIButtonListener)
                m_prIButtonListener->OnButtonPressed(Name());
        }
    }
}


// ----------------------------------------------------------------------------
// Gets called when a move event occurs
// ----------------------------------------------------------------------------
void prButton::OnMove(prTouchEvent e)
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


// ----------------------------------------------------------------------------
// Gets called when a released event occurs
// ----------------------------------------------------------------------------
void prButton::OnReleased(prTouchEvent e)
{
    UNUSED(e);

    if (m_sprite && m_enabled)
    {
        m_buttonState = BS_NORMAL;
        m_sprite->SetFrame(BS_NORMAL);

        if (InButtonsRect(m_prevX, m_prevY))
        {
            m_buttonState = BS_SELECTED;
            m_sprite->SetFrame(BS_SELECTED);

            //TODO("REMOVE THIS");
            //if (m_releasedCallback)
            //    m_releasedCallback(Name());

            if (m_prIButtonListener)
                m_prIButtonListener->OnButtonReleased(Name());
        }
    }
}


// ----------------------------------------------------------------------------
// Sets or removes the buttons sprite.
// ----------------------------------------------------------------------------
void prButton::SetSprite(Sprite *pSprite)
{
    if (pSprite)
    {
        m_sprite = pSprite;
        m_width  = pSprite->FrameWidth();
        m_height = pSprite->FrameHeight();
    }
    else
    {
        m_sprite = NULL;
        m_width  = 0;
        m_height = 0;
    }
}


// ----------------------------------------------------------------------------
// Sets or removes the buttons text font.
// ----------------------------------------------------------------------------
void prButton::SetFont(BitmapFont *pFont)
{ 
    m_font = pFont;
}


// ----------------------------------------------------------------------------
// Sets or removes the buttons text.
// ----------------------------------------------------------------------------
void prButton::SetText(const char *text)
{
    m_text.Set(text);
}


//// ----------------------------------------------------------------------------
//// Sets callbacks
//// ----------------------------------------------------------------------------
//void prButton::SetCallbacks(ButtonPressedCallback pcb, ButtonReleasedCallback rcb)
//{
//    m_pressedCallback  = pcb;
//    m_releasedCallback = rcb;
//}


// ----------------------------------------------------------------------------
// Sets callbacks
// ----------------------------------------------------------------------------
void prButton::RegisterListener(prIButtonListener *pListener)
{
    m_prIButtonListener = pListener;
}


// ----------------------------------------------------------------------------
// Private methods below
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// ...
// ----------------------------------------------------------------------------
bool prButton::InButtonsRect(s32 x, s32 y)
{
    ProRect rect = ProRect((s32)pos.y, (s32)pos.x, (s32)(pos.y + m_height), (s32)(pos.x + m_width));

    return rect.PointInside(x, y);
}
*/
