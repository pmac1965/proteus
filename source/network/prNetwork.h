// File: prNetwork.h
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


#ifndef __PRNETWORK_H
#define __PRNETWORK_H


#include "../core/prTypes.h"


// Class: prNetwork
//      Handles network connection
class prNetwork
{
public:
    // Method: prNetwork
    //      Ctor
    prNetwork();

    // Method: ~prNetwork
    //      Dtor
    ~prNetwork();

    // Method: Construct
    //      Construct the class.
    void Construct();

    // Method: Start
    //      Start the network interface.
    void Start();

    // Method: Update
    //      Update the connection status, etc.
    void Update(f32 dt);

    // Method: IsConnected
    //      Do we have a connection to the internet?
    PRBOOL IsConnected();
};


#endif// __PRNETWORK_H
