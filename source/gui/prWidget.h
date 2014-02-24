// ----------------------------------------------------------------------------
//
// File: prWidget.h
//
//      Description     - Contains base GUI widget class
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


#ifndef _PRWIDGET_H
#define _PRWIDGET_H
/*

#include "../../config.h"
#include "../core/types.h"
#include "../core/prString.h"
#include "../input/prTouch.h"
#include "../math/vector2.h"
#include "../display/colour.h"


// GUI widget types.
typedef enum WidgetType
{
    WT_Button,

} WidgetType;


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
class SpriteManager;


// ----------------------------------------------------------------------------
// Base GUI widget class
// ----------------------------------------------------------------------------
class prWidget
{
public:

    prWidget(WidgetType type, const char *name, SpriteManager *pSpriteManager);
    virtual ~prWidget();

    // Get the widget name.
    const char *Name() const { return m_name.Text(); }

    // The widget type.
    int Type() const { return m_type; }

    // Set the render visibility state.
    void SetVisible(bool state) { m_visible = state; }
    bool GetVisible() const     { return m_visible; }

    // Set the enabled state. If disabled, a widget should look disabled.
    void SetEnabled(bool state) { m_enabled = state; }
    bool GetEnabled() const     { return m_enabled; }
    
    // If acive a widget will be updated. Meant for modal dialogs.
    void SetActive(bool state) { m_active = state; }
    bool GetActive() const { return m_active; }
    
    // Set the animated state.
    void SetAnimated(bool state) { m_animated = state; }
    bool GetAnimated() const    { return m_animated; }

    // Destroy this widget.
    bool GetDestroy() const { return m_destroy; }
    void SetDestroy() { m_destroy = true; }

    // Set the widget colour.
    void SetColour(Colour c) { m_colour = c; }

    // Update/draw
    virtual void Update(f32 dt) = 0;
    virtual void Draw() = 0;

    // The touch events
    virtual void OnPressed(prTouchEvent e) = 0;
    virtual void OnMove(prTouchEvent e) = 0;
    virtual void OnReleased(prTouchEvent e) = 0;

private:

    WidgetType  m_type;
    prString    m_name;

protected:

    bool            m_visible;
    bool            m_enabled;
    bool            m_animated;
    bool        m_active;
    bool        m_destroy;
    bool            m_exp0;
    bool        m_exp1;
    bool        m_exp2;

    SpriteManager  *m_pSpriteManager;

    Colour      m_colour;


public:

    Vector2     pos;
};

*/
#endif//_PRWIDGET_H
