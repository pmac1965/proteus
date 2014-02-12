// File: prMessageManager.h
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


// Class: prMessageManager
//          The message manager, which allows for global message passing.
//
// Motes:
//      Can be used locally as well
class prMessageManager : public prCoreSystem
{
public:

    // Method: prMessageManager
    //      Ctor
    prMessageManager();

    // Method: ~prMessageManager
    //      Dtor
    virtual ~prMessageManager();

    // Method: Register
    //      Registers a handler with the message manager.
    //
    // Parameters:
    //      handler - The handler
    //
    // Notes:
    //      The passed handler must not be NULL.
    //      This will assert in the debug build.
    void Register(prMessageHandler *handler);

    // Method: Unregister
    //      Unregisters a handler with the message manager.
    //
    // Parameters:
    //      handler - The handler
    //
    // Notes:
    //      The passed handler must not be NULL.
    //      This will assert in the debug build.
    void Unregister(prMessageHandler *handler);

    // Method: Send
    //      Instantly sends a message to all registered receivers.
    //
    // Parameters:
    //      msg - The message
    void Send(prGameMessage &msg);


private:

    std::list<prMessageHandler*>    m_handlers;
};


#endif//__PRMESSAGEMANAGER_H
