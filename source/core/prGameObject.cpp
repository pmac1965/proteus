/**
 * prAchievementManager.cpp
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


#include "prGameObject.h"


using namespace Proteus::Actor;


// Namespaces
namespace Proteus {
namespace Core {


    /// -----------------------------------------------------------------------
    /// Adds a component to the game object
    /// -----------------------------------------------------------------------
    void prGameObject::AddComponent(Proteus::Actor::prActorComponent *pComponent)
    {
        PRASSERT(pComponent != nullptr, "Null components cannot be added");

        mComponents.push_back(pComponent);
    }


    /// -----------------------------------------------------------------------
    /// Removes a component from the game object
    /// -----------------------------------------------------------------------
    void prGameObject::RemoveComponent(Proteus::Actor::prActorComponent *pComponent)
    {
        PRASSERT(pComponent != nullptr, "Null components cannot be removed");

        mComponents.remove(pComponent);
    }


}}
