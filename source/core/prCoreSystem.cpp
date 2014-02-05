/**
 * prCoreSystem.cpp
 */


#include "prCoreSystem.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// @brief      Constructor.
/// @param      id      - A unique ID which identifies the system.
/// @param      pName   - The name of the system.
/// ---------------------------------------------------------------------------
prCoreSystem::prCoreSystem(u32 id, const char *pName) : m_id  (id)
                                                      , m_name(pName)
{
    prTrace("Init : %*s - ID: %i\n", 24, (pName && *pName) ? pName : "Unnamed", id);
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prCoreSystem::~prCoreSystem()
{
}
