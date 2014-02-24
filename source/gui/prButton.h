// ----------------------------------------------------------------------------
//
// File: prButton.h
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


#ifndef _PRBUTTON_H
#define _PRBUTTON_H


#include "../../config.h"
#include "prWidget.h"
#include "prGuiListener.h"
#include "../core/prString.h"
#include "../display/colour.h"


// ----------------------------------------------------------------------------
// Button modes.
// ----------------------------------------------------------------------------
enum
{
    BS_NORMAL,
    BS_HOVER,
    BS_SELECTED,
    BS_DISABLED,
};


// ----------------------------------------------------------------------------
// Callbacks
// ----------------------------------------------------------------------------
//typedef bool (*ButtonPressedCallback)(const char *);
//typedef bool (*ButtonReleasedCallback)(const char *);


// ----------------------------------------------------------------------------
// Forward declarations
// ----------------------------------------------------------------------------
class Sprite;
class BitmapFont;
class prIButtonListener;
class SpriteManager;


// ----------------------------------------------------------------------------
// Standard GUI button
// ----------------------------------------------------------------------------
class prButton : public prWidget
{
public:

    prButton(const char *name, SpriteManager *pSpriteManager);
    ~prButton();

    void Update(f32 dt);
    void Draw();

    void OnPressed(prTouchEvent e);
    void OnMove(prTouchEvent e);
    void OnReleased(prTouchEvent e);

    void SetSprite(Sprite *pSprite);
    void SetFont(BitmapFont *pFont);
    void SetText(const char *text);
    //void SetCallbacks(ButtonPressedCallback pcb, ButtonReleasedCallback rcb);

    void RegisterListener(prIButtonListener *pListener);

    s32 GetWidth() const { return m_width; }
    s32 GetHeight() const { return m_height; }

    void SetTextColour(Colour c) { m_textColour = c; }
    void SetTextScale(f32 scale) { m_textScale = scale; }

private:

    bool InButtonsRect(s32 x, s32 y);


private:

    Sprite                 *m_sprite;
    BitmapFont             *m_font;
    prString                m_text;
    s32                     m_buttonState;
    s32                     m_width;
    s32                     m_height;
    s32                     m_prevX;
    s32                     m_prevY;
    //ButtonPressedCallback   m_pressedCallback;
    //ButtonReleasedCallback  m_releasedCallback;

    prIButtonListener      *m_prIButtonListener;
    Colour                  m_textColour;
    f32                     m_textScale;
};


#endif//_PRBUTTON_H
