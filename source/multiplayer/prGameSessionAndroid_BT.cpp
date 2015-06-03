/**
 * prGameSessionAndroid_BT.cpp
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


#if defined(PLATFORM_ANDROID)


#include "prGameSessionAndroid_BT.h"
#include "prGameSession.h"
#include "prGameSessionProvider.h"
#include "../android/prJNINetwork.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prGameSessionAndroid_BT::prGameSessionAndroid_BT() : prGameSessionProvider(GSP_ANDROID_BT)
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prGameSessionAndroid_BT::~prGameSessionAndroid_BT()
{
}


/// ---------------------------------------------------------------------------
/// Initialises the game session provider
/// ---------------------------------------------------------------------------
void prGameSessionAndroid_BT::Initialise()
{
    prTrace(LogError, "prGameSessionAndroid_BT::Initialise() - start\n");
    prJNI_BTInit();
    prTrace(LogError, "prGameSessionAndroid_BT::Initialise() - done\n");
}


/// ---------------------------------------------------------------------------
/// Updates the game session provider
/// ---------------------------------------------------------------------------
bool prGameSessionAndroid_BT::Update()
{
    return true;
}


/// ---------------------------------------------------------------------------
/// Disconnects a game session
/// ---------------------------------------------------------------------------
void prGameSessionAndroid_BT::Disconnect()
{
    //extern void connectionStop();
    //connectionStop();
}


#endif//PLATFORM_ANDROID
