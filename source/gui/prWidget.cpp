/**
 * prWidget.cpp
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


#include "prWidget.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"
#include "../debug/prDebug.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWidget::prWidget(prWidgetType type, const char *name, prSpriteManager *pSpriteManager) : m_type            (type)
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
    m_visible   = true;
    m_enabled   = true;
    m_animated  = false;
    m_active    = true;
    m_destroy   = false;
    m_hasFocus  = false;
    m_exp1      = false;
    m_exp2      = false;
    m_layer     = 0;
    m_pBmpfont  = nullptr;
    m_pTtfFont  = nullptr;
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prWidget::AddComponent(prWidget *pWidget)
{
    PRUNUSED(pWidget);
}

