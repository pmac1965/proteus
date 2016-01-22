/**
 * prActor.cpp
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


#include "../prConfig.h"
#include "prActor.h"


using namespace Proteus::Core;
using namespace Proteus::Math;


// Namespaces
namespace Proteus {
namespace Actor {


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
    m_priority  = 0;
    m_state     = 0;
    m_id        = m_baseid++;
    user0       = 0;
    user1       = 0;
    user2       = 0;
    user3       = 0;
    m_layer     = ActorLayerBack;
    m_colPos    = prVector2::MinusOne;
}


}}// Namespaces
