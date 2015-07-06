/**
 * prActorManager.cpp
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
#include "prActorManager.h"
#include "prActor.h"
#include "../core/prDefines.h"
#include "../core/prMacros.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"


using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Actor {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prActorManager::prActorManager() : callback (nullptr)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prActorManager::~prActorManager()
{
    ReleaseAll();
}


/// ---------------------------------------------------------------------------
/// Register a function which creates the games actors.
/// ---------------------------------------------------------------------------
void prActorManager::Registerfactory(prFactoryCallback cb)
{
    PRASSERT(cb);
    callback = cb;
}


/// ---------------------------------------------------------------------------
/// Creates an actor
/// ---------------------------------------------------------------------------
prActor *prActorManager::Create(s32 type)
{
    prActor *actor = nullptr;

    if (callback)
    {
        actor = callback(type);
        if (actor)
        {
            actors.push_back(actor);
        }
    }

    return actor;
}


/// ---------------------------------------------------------------------------
/// Releases an actor
/// ---------------------------------------------------------------------------
void prActorManager::Release(prActor *actor)
{
    if (actor)
    {
        for (auto it = actors.begin(); it != actors.end(); ++it)
        {
            if ((*it) == actor)
            {
                actors.remove(actor);
                PRSAFE_DELETE(actor);
                return;
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Releases all actors
/// ---------------------------------------------------------------------------
void prActorManager::ReleaseAll()
{
    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        PRSAFE_DELETE(*it);
    }

    actors.clear();
}


/// ---------------------------------------------------------------------------
/// Updates the actors
/// ---------------------------------------------------------------------------
void prActorManager::Update(f32 time)
{
    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        prActor *actor = *it;
        PRASSERT(actor);
        {
            // Active?
            if (actor->m_active)
            {
                actor->Update(time);

                // Destroy?
                if (actor->m_destroy)
                {
                    it = actors.erase(it);
                    PRSAFE_DELETE(actor);
                }
                else
                {
                    // No, actor is still active
                    actor->UpdateOnScreen();
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Draws the actors
/// ---------------------------------------------------------------------------
void prActorManager::Draw()
{
    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        prActor *actor = *it;
        PRASSERT(actor);
        {
            if (actor->m_visible && actor->IsOnscreen())
            {
                (*it)->Draw();
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Set the draw order priority.
/// ---------------------------------------------------------------------------
void prActorManager::SetPriority(prActor *actor, s32 priority)
{
    PRASSERT(actor);

    if (actor)
    {
        bool found = false;

        std::list<prActor *>::iterator it  = actors.begin();
        std::list<prActor *>::iterator end = actors.end();
        for (; it != end; ++it)
        {
            if ((*it) == actor)
            {
                actors.remove(actor);
                found = true;
                break;
            }
        }

        if (found)
        {
            it  = actors.begin();
            end = actors.end();

            actor->m_priority = priority;
            
            for (; it != end; ++it)
            {
                if (priority < (*it)->m_priority)
                {
                    actors.insert(it, actor);
                    return;
                }
            }

            // Add last
            actors.push_back(actor);
        }
        else
        {
            PRWARN("prActorManager::SetPriority - Actor not found");
        }
    }
}


/// ---------------------------------------------------------------------------
/// Gets the number of actors
/// ---------------------------------------------------------------------------
int prActorManager::Count() const
{
    return (int)actors.size();
}



/// ---------------------------------------------------------------------------
/// Counts the actors of a specific type
/// ---------------------------------------------------------------------------
u32 prActorManager::HowMany(s32 type)
{
    u32 count = 0;

    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        if (type == (*it)->m_type)
        {
            count++;
        }
    }

    return count;
}


/// ---------------------------------------------------------------------------
/// Finds an actor of a specific type an index.
/// ---------------------------------------------------------------------------
prActor *prActorManager::FindByIndex(s32 type, u32 index)
{
    prActor *actor = NULL;
    u32 count = 0;

    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        if (type == (*it)->m_type)
        {
            if (count == index)
            {
                actor = (*it);
                break;
            }

            count++;
        }
    }

    return actor;
}


}}// Namespaces
