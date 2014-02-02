/**
 * prMessageManager.cpp
 */

#include "prMessageManager.h"
#include "prCore.h"


/// ---------------------------------------------------------------------------
/// Ctor.
/// ---------------------------------------------------------------------------
prMessageManager::prMessageManager() : prCoreSystem(PRSYSTEM_MESSAGEMANAGER, "prMessageManager")
{
}


/// ---------------------------------------------------------------------------
///  Dtor.
/// ---------------------------------------------------------------------------
prMessageManager::~prMessageManager()
{
}


/// ---------------------------------------------------------------------------
/// Registers a handler with the message manager.
/// ---------------------------------------------------------------------------
void prMessageManager::Register(prMessageHandler *handler)
{
    PRASSERT(handler);

    // Check for duplicate entry.
    std::list<prMessageHandler *>::iterator itr = m_handlers.begin();
    std::list<prMessageHandler *>::iterator end = m_handlers.end();
    for (; itr != end; ++itr)
    {
        if ((*itr) == handler)
        {
            return;
        }
    }

    m_handlers.push_back(handler);
}


/// ---------------------------------------------------------------------------
/// Unregisters a handler with the message manager.
/// ---------------------------------------------------------------------------
void prMessageManager::Unregister(prMessageHandler *handler)
{
    PRASSERT(handler);
    m_handlers.remove(handler);
}


/// ---------------------------------------------------------------------------
/// Instantly sends a message to all registers receivers.
/// ---------------------------------------------------------------------------
void prMessageManager::Send(prGameMessage &msg)
{
    std::list<prMessageHandler *>::iterator itr = m_handlers.begin();
    std::list<prMessageHandler *>::iterator end = m_handlers.end();

    for (; itr != end; ++itr)
    {
        (*itr)->Receive(msg);
    }
}
