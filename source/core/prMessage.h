// File: prMessage.h
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


#pragma once


#include "prTypes.h"


// Typedef: prGameMessage
//      The message structure
typedef struct prGameMessage
{
public:
    // Method: prGameMessage
    //      Default constructor
    prGameMessage() : type        (0)
                    , user0       (0)
                    , user1       (0)
                    , user2       (0)
                    , user3       (0)
    {
    }

    // Method: prGameMessage
    //      Constructor
    //
    // Parameters:
    //      atype - The message type.
    //      d0    - Optional data component.
    //      d1    - Optional data component.
    //      d2    - Optional data component.
    //      d3    - Optional data component.
    prGameMessage(u32 atype, u32 d0 = 0, u32 d1 = 0, u32 d2 = 0, u32 d3 = 0)
        : type        (atype)
        , user0       (d0)
        , user1       (d1)
        , user2       (d2)
        , user3       (d3)
    {
    }

    u32 type;               // The message type
    u32 user0;              // Data for message use. Will depend on the message
    u32 user1;              // Data for message use. Will depend on the message
    u32 user2;              // Data for message use. Will depend on the message
    u32 user3;              // Data for message use. Will depend on the message

} prGameMessage;


// Class: prMessageHandler
//      Message receivers need to include this class
//      in order to receive messages
// 
// See Also:
//      <prMessageManager>
//      <prGameMessage>
class prMessageHandler
{
public:
    // Method: prMessageHandler
    //      Ctor
    prMessageHandler() {}

    // Method: ~prMessageHandler
    //      Dtor
    virtual ~prMessageHandler() {}

    // Method: Receive
    //  Handler for passed messages.
    //
    // Parameters:
    //      msg - A message
    //
    // Notes:
    //      The message can be targeted or system wide.
    //
    // Notes:
    //      If true is returned, then the message is no longer 
    //      passed on
    //
    // Returns:
    //      true if handled.
    //      false if NOT handled.
    virtual bool Receive(prGameMessage &msg) = 0;
};
