/**
 * prActorManager.cpp
 */


#include "../prConfig.h"
#include "prActorManager.h"
#include "prActor.h"
#include "../core/prDefines.h"
#include "../core/prMacros.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prActorManager::prActorManager()
{
    callback = NULL;
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
    prActor *actor = NULL;

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
        std::list<prActor *>::iterator it  = actors.begin();
        std::list<prActor *>::iterator end = actors.end();

        for (; it != end; ++it)
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
    std::list<prActor *>::iterator it  = actors.begin();
    std::list<prActor *>::iterator end = actors.end();

    for (; it != end; ++it)
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
    std::list<prActor *> destroy;


    std::list<prActor *>::iterator it  = actors.begin();
    std::list<prActor *>::iterator end = actors.end();
    for (; it != end; ++it)
    {
        prActor *actor = *it;
        if (actor)
        {
            if (actor->m_active)
            {
                actor->Update(time);
            }

            if (actor->m_destroy)
            {
                destroy.push_back(actor);
            }
        }
    }


    // Release actors?
    if (!destroy.empty())
    {
        std::list<prActor *>::iterator it  = destroy.begin();
        std::list<prActor *>::iterator end = destroy.end();     
        for (; it != end; ++it)
        {
            prActor *actor = *it;
            if (actor)
            {
                actors.remove(actor);
                PRSAFE_DELETE(actor);
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Draws the actors
/// ---------------------------------------------------------------------------
void prActorManager::Draw()
{
    std::list<prActor *>::iterator it  = actors.begin();
    std::list<prActor *>::iterator end = actors.end();

    for (; it != end; ++it)
    {
        (*it)->Draw();
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
int prActorManager::Count()
{
    return actors.size();
}
