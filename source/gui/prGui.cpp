/**
 * prGui.cpp
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


#include "../prConfig.h"
#include "prGui.h"
#include "prButton.h"
#include "prDialog.h"
#include "prMenuStrip.h"
#include "prMenu.h"
#include "prPane.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"


#if defined(PLATFORM_LINUX)
#include <cstring>
#endif


// Namespaces
namespace Proteus {
namespace Gui {


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prGui::prGui()
{
    m_enabled   = PRTRUE;
    m_visible   = PRTRUE;
    m_layer     = 0;
    m_baseLayer = m_layer;
    m_pBmpfont  = nullptr;
    m_pTtfFont  = nullptr;

    prTouch *pTouch = static_cast<prTouch *>(prCoreGetComponent(PRSYSTEM_TOUCH));
    if (pTouch)
    {
        pTouch->RegisterListener(this);
    }
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prGui::~prGui()
{
    Clear();

    prTouch *pTouch = static_cast<prTouch *>(prCoreGetComponent(PRSYSTEM_TOUCH));
    if (pTouch)
    {
        pTouch->UnregisterListener(this);
    }
}


/// ---------------------------------------------------------------------------
/// Create GUI components
/// ---------------------------------------------------------------------------
prWidget *prGui::Create(prWidgetType type, const char *name)
{
    prWidget *widget = nullptr;

    switch(type)
    {
    case WT_Button:
        widget = new prButton(name, &m_spriteManager);
        widget->SetLayer(m_layer);
        widget->SetBMPFont(m_pBmpfont);
        widget->SetTTFFont(m_pTtfFont);
        break;

    case WT_Dialog:
        widget = new prDialog(name, &m_spriteManager);
        widget->SetLayer(++m_layer);
        widget->SetBMPFont(m_pBmpfont);
        widget->SetTTFFont(m_pTtfFont);
        break;

    case WT_MenuStrip:
        widget = new prMenuStrip(name, &m_spriteManager);
        widget->SetLayer(m_layer);
        widget->SetBMPFont(m_pBmpfont);
        widget->SetTTFFont(m_pTtfFont);
        break;

    case WT_Menu:
        widget = new prMenu(name, &m_spriteManager);
        widget->SetLayer(m_layer);
        widget->SetBMPFont(m_pBmpfont);
        widget->SetTTFFont(m_pTtfFont);
        break;

    case WT_Pane:
        widget = new prPane(name, &m_spriteManager);
        widget->SetLayer(m_layer);
        widget->SetBMPFont(m_pBmpfont);
        widget->SetTTFFont(m_pTtfFont);
        break;

    default:
        PRPANIC("Unknown widget type");
        break;
    }

    if (widget)
    {
        m_widgets.push_back(widget);
    }

    return widget;
}


/// ---------------------------------------------------------------------------
/// Calls update on every widget.
/// ---------------------------------------------------------------------------
void prGui::Update(f32 dt)
{
    if (m_enabled)
    {
        auto it  = m_widgets.begin();
        auto end = m_widgets.end();
        for (; it != end;)
        {
            prWidget *pWidget = (*it++);
            PRASSERT(pWidget);
            if (pWidget->GetDestroy())
            {
                // Reduce layer
                if (pWidget->GetType() == WT_Dialog)
                {
                    m_layer--;
                }

                // Remove
                m_widgets.remove(pWidget);
                PRSAFE_DELETE(pWidget);
            }
            else
            {
                // Only update widgets on the active layer.
                if (m_layer == pWidget->GetLayer())
                {
                    if (pWidget->GetActive())
                    {
                        pWidget->Update(dt);
                    }
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Calls draw on every widget.
/// ---------------------------------------------------------------------------
void prGui::Draw()
{
    if (m_visible)
    {
        auto it  = m_widgets.begin();
        auto end = m_widgets.end();
        for (; it != end;)
        {
            prWidget *widget = (*it);
            PRASSERT(widget);
            if (widget->GetVisible())
            {
                widget->Draw();
            }
            ++it;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Removes all the widgets.
/// ---------------------------------------------------------------------------
void prGui::Clear()
{
    auto it  = m_widgets.begin();
    auto end = m_widgets.end();
    for (; it != end;)
    {
        PRSAFE_DELETE(*it);
        ++it;
    }

    m_widgets.clear();

    // Init control data
    m_layer = m_baseLayer;
}


/// ---------------------------------------------------------------------------
/// Finds a name widget.
/// ---------------------------------------------------------------------------
prWidget *prGui::Find(const char *name)
{
    PRASSERT(name && *name);

    auto it  = m_widgets.begin();
    auto end = m_widgets.end();
    for (; it != end; ++it)
    {
        if (strcmp(name, (*it)->GetName()) == 0)
        {
            return *it;
        }
    }

    return nullptr;
}


/// ---------------------------------------------------------------------------
/// Passes on a touch event to every widget.
/// ---------------------------------------------------------------------------
void prGui::InputPressed(const prTouchEvent &e)
{
    if (m_enabled)
    {
        auto it  = m_widgets.begin();
        auto end = m_widgets.end();
        for (; it != end;)
        {
            prWidget *widget = (*it);
            PRASSERT(widget);
            if (widget->GetActive())
            {
                // Only send input to widgets on the active layer.
                if (m_layer == widget->GetLayer())
                {
                    if (widget->GetActive())
                    {
                        widget->OnPressed(e);
                    }
                }
            }
            ++it;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Passes on a touch event to every widget.
/// ---------------------------------------------------------------------------
void prGui::InputReleased(const prTouchEvent &e)
{
    if (m_enabled)
    {
        auto it  = m_widgets.begin();
        auto end = m_widgets.end();
        for (; it != end;)
        {
            prWidget *widget = (*it);
            PRASSERT(widget);
            if (widget->GetActive())
            {
                // Only send input to widgets on the active layer.
                if (m_layer == widget->GetLayer())
                {
                    if (widget->GetActive())
                    {
                        widget->OnReleased(e);
                    }
                }
            }
            ++it;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Passes on a touch event to every widget.
/// ---------------------------------------------------------------------------
void prGui::InputAxis(const prTouchEvent &e)
{
    if (m_enabled)
    {
        auto it  = m_widgets.begin();
        auto end = m_widgets.end();
        for (; it != end;)
        {
            prWidget *widget = (*it);
            PRASSERT(widget);
            if (widget->GetActive())
            {
                // Only send input to widgets on the active layer.
                if (m_layer == widget->GetLayer())
                {
                    if (widget->GetActive())
                    {
                        widget->OnMove(e);
                    }
                }
            }
            ++it;
        }
    }
}


}}// Namespaces
