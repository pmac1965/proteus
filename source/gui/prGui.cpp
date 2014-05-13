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
#include "../debug/prTrace.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"


#if defined(PLATFORM_BADA)
#include <FBase.h>

#elif defined(PLATFORM_LINUX)
#include <cstring>

#endif


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prGui::prGui()
{
    m_enabled = PRTRUE;
    m_visible = PRTRUE;
    m_layer   = 0;

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
    prWidget *widget = NULL;

    switch(type)
    {
    case WT_Button:
        widget = new prButton(name, &m_spriteManager);
        break;

    case WT_Dialog:
        widget = new prDialog(name, &m_spriteManager);
        widget->SetLayer(++m_layer);
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
        std::list<prWidget *>::iterator it  = m_widgets.begin();
        std::list<prWidget *>::iterator end = m_widgets.end();
        for (; it != end;)
        {
            prWidget *pWidget = (*it++);
            PRASSERT(pWidget);
            if (pWidget->GetDestroy())
            {
                // Reduce layer
                if (pWidget->Type() == WT_Dialog)
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
        std::list<prWidget *>::iterator it  = m_widgets.begin();
        std::list<prWidget *>::iterator end = m_widgets.end();
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
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
    for (; it != end;)
    {
        PRSAFE_DELETE(*it);
        ++it;
    }

    m_widgets.clear();
}


/// ---------------------------------------------------------------------------
/// Finds a name widget.
/// ---------------------------------------------------------------------------
prWidget *prGui::Find(const char *name)
{
    PRASSERT(name && *name);

    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
    for (; it != end; ++it)
    {
        if (strcmp(name, (*it)->Name()) == 0)
        {
            return *it;
        }
    }

    prTrace("Failed to find widget: %s\n", name);
    return NULL;
}


/// ---------------------------------------------------------------------------
/// Passes on a touch event to every widget.
/// ---------------------------------------------------------------------------
void prGui::InputPressed(const prTouchEvent &e)
{
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
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


/// ---------------------------------------------------------------------------
/// Passes on a touch event to every widget.
/// ---------------------------------------------------------------------------
void prGui::InputReleased(const prTouchEvent &e)
{
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
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


/// ---------------------------------------------------------------------------
/// Passes on a touch event to every widget.
/// ---------------------------------------------------------------------------
void prGui::InputAxis(const prTouchEvent &e)
{
    std::list<prWidget *>::iterator it  = m_widgets.begin();
    std::list<prWidget *>::iterator end = m_widgets.end();
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

