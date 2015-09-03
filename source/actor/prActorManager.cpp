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
#include "../debug/prDebug.h"


using namespace Proteus::Core;


// Namespaces
namespace Proteus {
namespace Actor {


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prActorManager::prActorManager()
#ifdef NEW_ACTORMANAGER
                                 : mCallback    (nullptr)
                                 , mActors      (nullptr)
                                 , mActorTypes  (0)
#else
                                 : callback (nullptr)
#endif
{
    TODO("Modify to add create/release for actor pools - via handler class")
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prActorManager::~prActorManager()
{
    ReleaseAll();

#ifdef NEW_ACTORMANAGER

    for (s32 i=0; i<mActorTypes; i++)
    {
        prList<prActor *> *pActors = mActors[i];
        PRASSERT(pActors);
        PRSAFE_DELETE(pActors);
    }

    PRSAFE_DELETE_ARRAY(mActors);

#endif
}


/// ---------------------------------------------------------------------------
/// Register a function which creates the games actors.
/// ---------------------------------------------------------------------------
void prActorManager::Registerfactory(prFactoryCallback cb, s32 actorTypes)
{
    PRASSERT(cb);
    PRASSERT(actorTypes > 0);

#ifdef NEW_ACTORMANAGER
    // Create an array of actor lists. One for each type
    mActors = new prList<prActor *> *[actorTypes];

    for (s32 i=0; i<actorTypes; i++)
    {
        mActors[i] = new prList<prActor *>();
    }

    mCallback   = cb;
    mActorTypes = actorTypes;

#else
    callback = cb;

#endif
}


/// ---------------------------------------------------------------------------
/// Creates an actor
/// ---------------------------------------------------------------------------
prActor *prActorManager::Create(s32 type)
{
#ifdef NEW_ACTORMANAGER
    PRASSERT(mCallback);
    PRASSERT(mActorTypes > 0);
    PRASSERT(type >= 0 && type < mActorTypes);

    prList<prActor *> *pActors = mActors[type];
    PRASSERT(pActors);

    // Create the actor
    prActor *actor = mCallback(type);
    if (actor)
    {
        pActors->AddTail(actor);
    }    

    return actor;

#else
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

#endif
}


/// ---------------------------------------------------------------------------
/// Releases an actor
/// ---------------------------------------------------------------------------
void prActorManager::Release(prActor *actor)
{
#ifdef NEW_ACTORMANAGER
    PRASSERT(mActors);

    if (actor)
    {
        actor->m_type;

        prList<prActor *> *pActors = mActors[actor->m_type];
        PRASSERT(pActors);

        auto it = pActors->Begin();    
        while(it.Okay())
        {
            if ((*it) == actor)
            {
                pActors->Remove(actor);
                PRSAFE_DELETE(actor);
                return;
            }
            ++it;
        }
    }

#else
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

#endif
}


/// ---------------------------------------------------------------------------
/// Releases all actors
/// ---------------------------------------------------------------------------
void prActorManager::ReleaseAll()
{
#ifdef NEW_ACTORMANAGER
    for (s32 i=0; i<mActorTypes; i++)
    {
        prList<prActor *> *pActors = mActors[i];
        PRASSERT(pActors);

        auto it = pActors->Begin();
        while(it.Okay())
        {
            PRSAFE_DELETE(*it);
            ++it;
        }

        pActors->Clear();
    }

#else
    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        PRSAFE_DELETE(*it);
    }

    actors.clear();

#endif
}


/// ---------------------------------------------------------------------------
/// Updates the actors
/// ---------------------------------------------------------------------------
void prActorManager::Update(f32 time)
{
#ifdef NEW_ACTORMANAGER
    for (s32 i=0; i<mActorTypes; i++)
    {
        prList<prActor *> *pActors = mActors[i];
        PRASSERT(pActors);

        if (pActors->Size() > 0)
        {
            prActor *pDes = nullptr;

            auto it = pActors->Begin();
            while(it.Okay())
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
                            it = pActors->Remove(actor);
                            PRSAFE_DELETE(actor);
                            continue;
                        }
                        else
                        {
                            // No, actor is still active
                            actor->UpdateOnScreen();
                        }
                    }
                }
                ++it;
            }
        }
    }

#else
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
#endif
}


/// ---------------------------------------------------------------------------
/// Draws the actors
/// ---------------------------------------------------------------------------
void prActorManager::Draw()
{
#ifdef NEW_ACTORMANAGER
    for (s32 i=0; i<mActorTypes; i++)
    {
        prList<prActor *> *pActors = mActors[i];
        PRASSERT(pActors);

        auto it = pActors->Begin();
        while(it.Okay())
        {
            prActor *actor = *it;
            PRASSERT(actor);
            if (actor->m_visible && actor->IsOnscreen())
            {
                (*it)->Draw();
            }            

            ++it;
        }
    }
#else
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
#endif
}


/// ---------------------------------------------------------------------------
/// Set the draw order priority.
/// ---------------------------------------------------------------------------
void prActorManager::SetPriority(prActor *actor, s32 priority)
{
#ifdef NEW_ACTORMANAGER

#else
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
#endif
}


/// ---------------------------------------------------------------------------
/// Gets the number of actors
/// ---------------------------------------------------------------------------
int prActorManager::Count() const
{
#ifdef NEW_ACTORMANAGER
    return 0;

#else
    return (int)actors.size();

#endif
}



/// ---------------------------------------------------------------------------
/// Counts the actors of a specific type
/// ---------------------------------------------------------------------------
u32 prActorManager::HowMany(s32 type)
{
    u32 count = 0;

#ifdef NEW_ACTORMANAGER

    prList<prActor *> *pActors = mActors[type];
    return (u32)pActors->Size();

#else
    for (auto it = actors.begin(); it != actors.end(); ++it)
    {
        if (type == (*it)->m_type)
        {
            count++;
        }
    }

#endif

    return count;
}


/// ---------------------------------------------------------------------------
/// Finds an actor of a specific type an index.
/// ---------------------------------------------------------------------------
prActor *prActorManager::FindByIndex(s32 type, u32 index)
{
    prActor *actor = NULL;
 
#ifdef NEW_ACTORMANAGER
    PRASSERT(mActorTypes > 0);
    PRASSERT(type >= 0 && type < mActorTypes);

    u32 count = 0;
    prList<prActor *> *pActors = mActors[type];

    auto it = pActors->Begin();
    while(it.Okay())
    {
        if (count == index)
        {
            actor = (*it);
            break;
        }

        ++count;
        ++it;
    }

#else
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

#endif

    return actor;
}


}}// Namespaces
