/**
 * prRenderer.cpp
 */


#include "prRenderer.h"
#include "../core/prCore.h"
#include "../core/prDefines.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prRenderer::prRenderer() : prCoreSystem(PRSYSTEM_RENDERER, "prRenderer")
{
    m_pWindow    = NULL;
    m_pWatermark = NULL;
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
