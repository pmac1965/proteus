/**
 * prApplication.cpp
 */


#include "prApplication.h"
#include "prDefines.h"
#include "prCore.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prApplication::prApplication()
{
    m_pWindow = NULL;
    m_running = PRFALSE;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prApplication::~prApplication()
{
    prCoreDestroy();
}
