/**
 * prGui.cpp
 */


#include "../prconfig.h"
#include "prGui.h"
#include "prButton.h"
#include "../debug/prTrace.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"


#if defined(PLATFORM_BADA)
#include <FBase.h>
#endif


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prGui::prGui()
{
    m_enabled = PRTRUE;
    m_visible = PRTRUE;

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


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
prWidget *prGui::Create(prWidgetType type, const char *name)
{
    prWidget *widget = NULL;

    switch(type)
    {
    case WT_Button:
        widget = new prButton(name, &m_spriteManager);
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
                //// Make all other widgets active, if we're a dialog.
                //if (pWidget->Type() == WT_Dialog)
                //{
                //    std::list<prWidget *>::iterator it2  = m_widgets.begin();
                //    std::list<prWidget *>::iterator end2 = m_widgets.end();
                //    for (; it2 != end2;)
                //    {
                //        (*it2)->SetActive(true);
                //        ++it2;
                //    }
                //}

                // Remove
                m_widgets.remove(pWidget);
                PRSAFE_DELETE(pWidget);
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
        PRSAFE_DELETE(*it);
        ++it;
    }

    m_widgets.clear();
}


// ----------------------------------------------------------------------------
// Finds a name widget.
// ----------------------------------------------------------------------------
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

