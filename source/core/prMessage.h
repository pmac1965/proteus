/**
 * @file       prMessage.h
 * @brief      Contains the message class for game and system messaging.
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


#ifndef __PRMESSAGE_H
#define __PRMESSAGE_H


#include "prTypes.h"


/**
 * @brief       The message structure
 */
typedef struct prGameMessage
{
public:

    /**
     * @brief       Default constructor
     */
    prGameMessage()
    {
        type        = 0;
        user0       = 0;
        user1       = 0;
        user2       = 0;
        user3       = 0;
    }

    /** 
     * @brief      Ctor
     *
     * @param      atype - The message type.
     * @param      d0    - Optional data component.
     * @param      d1    - Optional data component.
     * @param      d2    - Optional data component.
     * @param      d3    - Optional data component.
     */
    prGameMessage(u32 atype, u32 d0 = 0, u32 d1 = 0, u32 d2 = 0, u32 d3 = 0)
    {
        type        = atype;
        user0       = d0;
        user1       = d1;
        user2       = d2;
        user3       = d3;
    }

    u32 type;               ///< The message type
    u32 user0;              ///< Data for message use. Will depend on the message
    u32 user1;              ///< Data for message use. Will depend on the message
    u32 user2;              ///< Data for message use. Will depend on the message
    u32 user3;              ///< Data for message use. Will depend on the message

} prGameMessage;


/**
 * @brief      Interface class. Message receivers need to include this class
 * @n          in order to receive messages
 *
 * @see        prMessageManager
 * @see        prGameMessage
 */
class prMessageHandler
{
public:

    /**
     * Ctor
     */
    prMessageHandler() {}

    /**
     * Dtor
     */
    virtual ~prMessageHandler() {}

    /**
     * @brief      Handler for passed messages.
     *
     * @param      msg - A message
     *
     * @note       The message can be targeted or system wide.
     *
     * @return     true if handled.
     * @return     false if NOT handled.
     */
    virtual bool Receive(prGameMessage &msg) = 0;
};


#endif//__PRMESSAGE_H
