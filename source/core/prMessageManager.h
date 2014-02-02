/**
 * @file       prMessageManager.h
 * @brief      Contains the message management class for game and system messaging.
 */
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __PRMESSAGEMANAGER_H
#define __PRMESSAGEMANAGER_H


#include <list>
#include "prMessage.h"
#include "prCoreSystem.h"


/**
 * @brief       The message manager. Allows for global message passing.
 * @note        Can be used locally as well
 */
class prMessageManager : public prCoreSystem
{
public:

    /** 
     * Ctor
     */
    prMessageManager();

    /**
     * Dtor
     */
    virtual ~prMessageManager();

    /**
     * @brief      Registers a handler with the message manager.
     * @param      handler - The handler
     * @note       The passed handler must not be NULL.
     * @n          This will assert in the debug build.
     */
    void Register(prMessageHandler *handler);

    /**
     * @brief      Unregisters a handler with the message manager.
     * @param      handler - The handler
     * @note       The passed handler must not be NULL.
     * @n          This will assert in the debug build.
     */
    void Unregister(prMessageHandler *handler);

    /**
     * @brief      Instantly sends a message to all registered receivers.
     * @param      msg - The message
     */
    void Send(prGameMessage &msg);


private:

    std::list<prMessageHandler*>    m_handlers;
};


#endif//__PRMESSAGEMANAGER_H
