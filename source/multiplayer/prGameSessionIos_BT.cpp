/**
 * prGameSessionIos_BT.cpp
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


#if defined(PLATFORM_IOS)


#include "prGameSessionIos_BT.h"
#include "prGameSession.h"
#include "prGameSessionProvider.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prGameSessionIos_BT::prGameSessionIos_BT() : prGameSessionProvider(GSP_IOS_GAMESESSION)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prGameSessionIos_BT::~prGameSessionIos_BT()
{
}


/// ---------------------------------------------------------------------------
/// Initialises the game session provider
/// ---------------------------------------------------------------------------
void prGameSessionIos_BT::Initialise()
{
    extern void connectionStart();
    connectionStart();
}


/// ---------------------------------------------------------------------------
/// Updates the game session provider
/// ---------------------------------------------------------------------------
bool prGameSessionIos_BT::Update()
{
    extern bool isStillConnected();
    return isStillConnected();
}


/// ---------------------------------------------------------------------------
/// Disconnects a game session
/// ---------------------------------------------------------------------------
void prGameSessionIos_BT::Disconnect()
{
    extern void connectionStop();
    connectionStop();
}


#endif//PLATFORM_IOS
