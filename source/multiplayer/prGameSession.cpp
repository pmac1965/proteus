/**
 * prGameSession.cpp
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


#include "prGameSession.h"
#include "../core/prDefines.h"
#include "../core/prMacros.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"


#if defined(PLATFORM_ANDROID)
#include "../android/prJNINetwork.h"
#endif


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prGameSession::prGameSession()
{
    mpProvider              = NULL;
    mpGameSessionReceiver   = NULL;
    mInitialised            = PRFALSE; 
    mIsAvailable            = PRFALSE; 
    mIsEnabled              = PRFALSE;   
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prGameSession::~prGameSession()
{
    PRSAFE_DELETE(mpProvider);
}


/// ---------------------------------------------------------------------------
/// Initialises the game session
/// ---------------------------------------------------------------------------
void prGameSession::Initialise(prGameSessionReceiver *pReceiver, prGameSessionProvider *pProvider)
{
    PRASSERT(pReceiver);
    PRASSERT(pProvider);
    
    mpGameSessionReceiver   = pReceiver;
    mpProvider              = pProvider;
    mInitialised            = PRTRUE;

    if (mpProvider)
    {
        // Initialise the provider
        // This starts the connect to other device code
        mpProvider->Initialise();

        // Okay tell game connections starting.
        if (pReceiver)
        {
            pReceiver->GameSessionStatus((u32)GSS_CONNECT_BEGIN);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Updates the game session
/// ---------------------------------------------------------------------------
bool prGameSession::Update()
{
    bool result = false;
    
    if (mpProvider)
    {
        result = mpProvider->Update();
    }
    
    return result;
}


/// ---------------------------------------------------------------------------
/// Used by the OS to pass messages back to the game
/// ---------------------------------------------------------------------------
void prGameSession::ReceiveMessage(s32 msg)
{
    switch(msg)
    {
    case GSS_CONNECT_BEGIN:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_CONNECT_BEGIN); }
        prTrace(prLogLevel::LogError, "GSS_CONNECT_BEGIN\n");
        break;

    case GSS_CONNECT_FAILED:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_CONNECT_FAILED); }
        prTrace(prLogLevel::LogError, "GSS_CONNECT_FAILED\n");
        break;

    case GSS_CONNECT_OK:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_CONNECT_OK); }
        prTrace(prLogLevel::LogError, "GSS_CONNECT_OK\n");
        break;

    case GSS_TURNED_ON:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_TURNED_ON); }
        prTrace(prLogLevel::LogError, "GSS_TURNED_ON\n");
        break;

    case GSS_DISCOVERY:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_DISCOVERY); }
        prTrace(prLogLevel::LogError, "GSS_DISCOVERY\n");
        break;

    case GSS_PEER_ID:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_PEER_ID); }
        prTrace(prLogLevel::LogError, "GSS_PEER_ID\n");
        break;

    default:
        prTrace(prLogLevel::LogError, "game session: unknown message: 0x%08x\n", msg);
        break;
    }
}


/// ---------------------------------------------------------------------------
/// Send a data packet to the other device
/// ---------------------------------------------------------------------------
void prGameSession::SendPacket(prGameSessionPacket &packet)
{
#if defined(PLATFORM_ANDROID)

    prJNI_BTSend((unsigned char *)&packet, sizeof(prGameSessionPacket));

#elif defined(PLATFORM_IOS)
    
    extern void connectionSendData(prGameSessionPacket &packet);
    connectionSendData(packet);

#else
    PRUNUSED(packet);

#endif
}


/// ---------------------------------------------------------------------------
/// Receive a data packet from the other device
/// ---------------------------------------------------------------------------
void prGameSession::ReceivePacket(prGameSessionPacket &packet)
{
    if (mpGameSessionReceiver)
    { 
        mpGameSessionReceiver->PacketReceiver(packet);
    }
}


/// ---------------------------------------------------------------------------
/// Determine if we are the client or the server
/// ---------------------------------------------------------------------------
PRBOOL prGameSession::IsServer()
{
#if defined(PLATFORM_ANDROID)

    bool result = prJNI_BTIsServer();

#elif defined(PLATFORM_IOS)

    extern bool isServer();
    bool result = isServer();

#else
    bool result = false;
    
#endif

    return (result ? PRTRUE : PRFALSE);
}


/// ---------------------------------------------------------------------------
/// Disconnects a game session
/// ---------------------------------------------------------------------------
void prGameSession::Disconnect()
{
    if (mpProvider)
    {
        mpProvider->Disconnect();
    }
}
