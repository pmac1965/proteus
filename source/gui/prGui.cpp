// ----------------------------------------------------------------------------
//
// File: prGui.cpp
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
/*

#include "../../config.h"
#include "prGui.h"
#include "prButton.h"
#include "prText.h"
#include "prImage.h"
#include "prDialog.h"
#include "prScrollBox.h"
#include "../debug/trace.h"
#include "../core/stringUtil.h"


#if defined(PLATFORM_BADA)
#include <FBase.h>
#endif


// ----------------------------------------------------------------------------
// Ctor
// ----------------------------------------------------------------------------
prGui::prGui()
{
    m_enabled = PRTRUE;
    m_visible = PRTRUE;
    PRNS prTouch::GetInstance()->RegisterListener(this);
}


// ----------------------------------------------------------------------------
// Dtor
// ----------------------------------------------------------------------------
prGui::~prGui()
{
    Clear();
    PRNS prTouch::GetInstance()->UnregisterListener(this);
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
prWidget *prGui::Create(WidgetType type, const char *name)
{
    prWidget *widget = NULL;

    switch(type)
    {
    case WT_Button:
        widget = new prButton(name, &m_spriteManager);
        break;

    case WT_Text:
        break;

    case WT_Image:
        break;

    case WT_Dialog:
        {
        widget = new prDialog(name, &m_spriteManager);
            
            // Make all other widgets inactive.
            std::list<prWidget *>::iterator it  = m_widgets.begin();
            std::list<prWidget *>::iterator end = m_widgets.end();
            for (; it != end;)
            {
                (*it)->SetActive(false);
                ++it;
            }
        }
        break;

    case WT_ScrollBox:
        widget = new prScrollBox(name, &m_spriteManager);
        break;

    default:
        PANIC("Unknown widget type");
        break;
    }

    if (widget)
    {
        m_widgets.push_back(widget);
    }

    return widget;
}


// ----------------------------------------------------------------------------
// Calls update on every widget.
// ----------------------------------------------------------------------------
void prGui::Update(f32 dt)
{
    if (m_enabled)
    {
        std::list<prWidget *>::iterator it  = m_widgets.begin();
        std::list<prWidget *>::iterator end = m_widgets.end();
        for (; it != end;)
        {
            prWidget *pWidget = (*it++);

            if (pWidget->GetDestroy())
            {
                // Make all other widgets active, if we're a dialog.
                if (pWidget->Type() == WT_Dialog)
                {
                    std::list<prWidget *>::iterator it2  = m_widgets.begin();
                    std::list<prWidget *>::iterator end2 = m_widgets.end();
                    for (; it2 != end2;)
                    {
                        (*it2)->SetActive(true);
                        ++it2;
                    }
                }

                // Remove
                m_widgets.remove(pWidget);
                SAFE_DELETE(pWidget);
            }
            else
            {
                pWidget->Update(dt);
                //++it;
            }
        }
    }
    //Trace("W: %i\n", m_widgets.size());
}


// ----------------------------------------------------------------------------
// Calls draw on every widget.
// ----------------------------------------------------------------------------
void prGui::Draw()
{
    if (m_visible)
    {
        std::list<prWidget *>::iterator it  = m_widgets.begin();
        std::list<prWidget *>::iterator end = m_widgets.end();
        for (; it != end;)
        {
            (*it)->Draw();
            ++it;
        }
    }
}


// ----------------------------------------------------------------------------
// Removes all the widgets.
// ----------------------------------------------------------------------------
void prGui::Clear()
{
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
    for (; it != end;)
    {
        SAFE_DELETE(*it);
        ++it;
    }

    m_widgets.clear();
}


// ----------------------------------------------------------------------------
// Finds a name widget.
// ----------------------------------------------------------------------------
prWidget *prGui::Find(const char *name)
{
    ASSERT(name && *name);

    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
    for (; it != end; ++it)
    {
        if (strcmp(name, (*it)->Name()) == 0)
        {
            return *it;
        }
    }

    Trace("Failed to find widget: %s\n", name);
    return NULL;
}


// ----------------------------------------------------------------------------
// Passes on a touch event to every widget.
// ----------------------------------------------------------------------------
void prGui::InputPressed(const prTouchEvent &e)
{
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
    for (; it != end;)
    {
        prWidget *widget = (*it);
        if (widget->GetActive())
        {
            widget->OnPressed(e);
        }
        ++it;
    }
}


// ----------------------------------------------------------------------------
// Passes on a touch event to every widget.
// ----------------------------------------------------------------------------
void prGui::InputReleased(const prTouchEvent &e)
{
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
    for (; it != end;)
    {
        prWidget *widget = (*it);
        if (widget->GetActive())
        {
            widget->OnReleased(e);
        }
        ++it;
    }
}


// ----------------------------------------------------------------------------
// Passes on a touch event to every widget.
// ----------------------------------------------------------------------------
void prGui::InputAxis(const prTouchEvent &e)
{
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
    for (; it != end;)
    {
        prWidget *widget = (*it);
        if (widget->GetActive())
        {
            widget->OnMove(e);
        }
        ++it;
    }
}
*/
