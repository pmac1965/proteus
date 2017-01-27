/**
 * prMessageManager.cpp
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
    //std::list<prMessageHandler *>::iterator itr = m_handlers.begin();
    //std::list<prMessageHandler *>::iterator end = m_handlers.end();
    //for (; itr != end; ++itr)
    for (auto it = m_handlers.begin(); it != m_handlers.end(); ++it)
    {
        if ((*it) == handler)
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
/// Instantly sends a message to all registered receivers.
/// ---------------------------------------------------------------------------
void prMessageManager::Send(prGameMessage &msg)
{
    //std::list<prMessageHandler *>::iterator itr = m_handlers.begin();
    //std::list<prMessageHandler *>::iterator end = m_handlers.end();
    //for (; itr != end; ++itr)
    for (auto it = m_handlers.begin(); it != m_handlers.end(); ++it)
    {
        if ((*it)->Receive(msg))
        {
            break;
        }
    }
}
