/**
 * prNetwork.cpp
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


#include "prNetwork.h"
#include "../core/prMacros.h"


/// ---------------------------------------------------------------------------
/// Ctor. 
/// ---------------------------------------------------------------------------
prNetwork::prNetwork()
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prNetwork::~prNetwork()
{
}


/// ---------------------------------------------------------------------------
/// Construct the class.
/// ---------------------------------------------------------------------------
void prNetwork::Construct()
{
}


/// ---------------------------------------------------------------------------
/// Star the network interface.
/// ---------------------------------------------------------------------------
void prNetwork::Start()
{
}


/// ---------------------------------------------------------------------------
/// Update the connection status, etc.
/// ---------------------------------------------------------------------------
void prNetwork::Update(f32 dt)
{
    PRUNUSED(dt);
}


/// ---------------------------------------------------------------------------
/// Do we have a connection to the internet?
/// ---------------------------------------------------------------------------
PRBOOL prNetwork::IsConnected()
{
    PRBOOL result = PRFALSE;

#if defined(PLATFORM_IOS)
    extern bool DoWeHaveAConnection();
    if (DoWeHaveAConnection())
    {
        result = PRTRUE;
    }

#elif defined(PLATFORM_PC)
    result = PRTRUE;

#elif defined(PLATFORM_BADA)
    result = PRTRUE;

#elif defined(PLATFORM_ANDROID)
    result = PRTRUE;
    
#elif defined(PLATFORM_LINUX)
    result = PRTRUE;
    
#elif defined(PLATFORM_MAC)
    result = PRTRUE;

#else
    #error Unsupported platform

#endif

    return result;
}
