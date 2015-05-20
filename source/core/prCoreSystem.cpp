/**
 * prCoreSystem.cpp
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


#include "prCoreSystem.h"
#include "../debug/prTrace.h"


using namespace Proteus::Core;


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
