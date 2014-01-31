/**
 * prRenderer.cpp
 * Contains the renderer base class * Copyright Paul Michael McNab. All rights reserved.
 */


#include "prRenderer.h"
#include "../core/prCore.h"
#include "../core/prDefines.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prRenderer::prRenderer() : prCoreSystem(PRSYSTEM_RENDERER, "prRenderer")
{
    m_pWindow = NULL;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prRenderer::~prRenderer()
{
}


/// ---------------------------------------------------------------------------
/// Sets the window to draw to.
/// ---------------------------------------------------------------------------
void prRenderer::SetWindow(prWindow* window)
{
    PRASSERT(window);
    if (window)
    {
        m_pWindow = window;
    }
}
