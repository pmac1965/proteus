/**
* prCoreSystem.cpp
* Contains the base class used by all the engines core systems.
* Copyright Paul Michael McNab. All rights reserved.
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
    prTrace("Init : %s - ID: %i\n", (pName && *pName) ? pName : "Unnamed", id);
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prCoreSystem::~prCoreSystem()
{
}
