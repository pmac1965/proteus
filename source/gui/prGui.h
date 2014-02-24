// ----------------------------------------------------------------------------
//
// File: prGui.h
//
//      Description     - Contains ...
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


#ifndef _PRGUI_H
#define _PRGUI_H


#include "prWidget.h"
#include "../input/prITouchListener.h"
//#include "../core/resourceManager.h"
#include "../display/spriteManager.h"
#include <list>


// ----------------------------------------------------------------------------
// Container for GUI widgets
// ----------------------------------------------------------------------------
class prGui : public ITouchListener
{
public:

    prGui();
    ~prGui();

    prWidget *Create(WidgetType type, const char *name);

    void Update(f32 dt);
    void Draw();
    void Clear();

    void SetEnable(PRBOOL state) { m_enabled = state; }
    void SetVisible(PRBOOL state) { m_visible = state; }

    PRBOOL IsEnabled() const { return m_enabled; }
    PRBOOL IsVisible() const { return m_enabled; }

    prWidget *Find(const char *name);

    // Passes on a touch event.
    void InputPressed(const prTouchEvent &e);

    // Passes on a touch event.
    void InputReleased(const prTouchEvent &e);

    // Passes on a touch event.
    void InputAxis(const prTouchEvent &e);


private:

    std::list<prWidget *>   m_widgets;
    SpriteManager           m_spriteManager;
    PRBOOL                  m_enabled;
    PRBOOL                  m_visible;
};


#endif//_PRGUI_H
