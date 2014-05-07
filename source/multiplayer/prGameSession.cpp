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
void prGameSession::Update()
{
    if (mpProvider)
    {
        mpProvider->Update();
    }
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
        prTrace("GSS_CONNECT_BEGIN\n");
        break;

    case GSS_CONNECT_FAILED:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_CONNECT_FAILED); }
        prTrace("GSS_CONNECT_FAILED\n");
        break;

    case GSS_CONNECT_OK:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_CONNECT_OK); }
        prTrace("GSS_CONNECT_OK\n");
        break;

    case GSS_TURNED_ON:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_TURNED_ON); }
        prTrace("GSS_TURNED_ON\n");
        break;

    case GSS_DISCOVERY:
        if (mpGameSessionReceiver) { mpGameSessionReceiver->GameSessionStatus((u32)GSS_DISCOVERY); }
        prTrace("GSS_DISCOVERY\n");
        break;

    default:
        prTrace("game session: unknown message: 0x%08x\n", msg);
        break;
    }
}


void prGameSession::SendPacket(prGameSessionPacket &packet)
{
#if defined(PLATFORM_ANDROID)

    prJNI_BTSend((unsigned char *)&packet, sizeof(prGameSessionPacket));

#else
    PRUNUSED(packet);

#endif
}


void prGameSession::ReceivePacket(prGameSessionPacket &packet)
{
    if (mpGameSessionReceiver)
    { 
        mpGameSessionReceiver->PacketReceiver(packet);
    }
}


PRBOOL prGameSession::IsServer()
{
    bool result = false;

#if defined(PLATFORM_ANDROID)

    result = prJNI_BTIsServer();
    
#endif

    return result ? PRTRUE : PRFALSE;
}
