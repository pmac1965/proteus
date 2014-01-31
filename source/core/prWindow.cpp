/**
 * prWindow.cpp
 * Contains the base window class
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include "../prConfig.h"
#include "prWindow.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prWindow::prWindow()
{
    m_width       = 0;
    m_height      = 0;
    m_bits        = 0;
    m_fullScreen  = false;
    m_active      = true;
    m_exp0        = false;
    m_exp1        = false;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prWindow::~prWindow()
{
}
