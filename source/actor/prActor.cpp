/**
 * prActor.cpp
 */


#include "../prConfig.h"
#include "prActor.h"


// Statics
s32 prActor::m_baseid = 0;


/// ---------------------------------------------------------------------------
/// Ctor.
/// ---------------------------------------------------------------------------
prActor::prActor(s32 type) : m_type(type)
{
    m_visible   = true;
    m_active    = true;
    m_destroy   = false;
    m_onScreen  = true;
    m_state     = 0;
    m_priority  = 0;
    m_id        = m_baseid++;
}


/// ---------------------------------------------------------------------------
/// Dtor.
/// ---------------------------------------------------------------------------
prActor::~prActor()
{
}
