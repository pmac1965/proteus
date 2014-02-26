/**
 * prWidget.cpp
 */


#include "prWidget.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../debug/prDebug.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWidget::prWidget(prWidgetType type, const char *name, prSpriteManager *pSpriteManager) 
    : m_type            (type)
    , m_pSpriteManager  (pSpriteManager)
    , m_colour          (prColour::White)
{
    PRASSERT(pSpriteManager);

    // Set widget name
    const char *theName = "Unnamed widget";

    // New name?
    if (name && *name)
    {
        theName = name;
    }

    m_name.Set(theName);

    // Init data
    m_visible   = false;
    m_enabled   = false;
    m_animated  = false;
    m_active    = true;
    m_destroy   = false;
    m_exp0      = false;
    m_exp1      = false;
    m_exp2      = false;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prWidget::~prWidget()
{
}
