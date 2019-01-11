// File: prGameSession.h
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


#include "../core/prTypes.h"
#include "prGameSessionProvider.h"


// Game Pession Providers
enum
{
    GSP_ANDROID_BT,             // Android bluetooth
    GSP_IOS_GAMESESSION,        // IOS game session
};



// Game Session Status
enum
{
    GSS_CONNECT_BEGIN   = 0x64210001,           // Connecting has begun
    GSS_CONNECT_FAILED  = 0x64210002,           // Connecting failed
    GSS_CONNECT_OK      = 0x64210003,           // Connecting succeeded
    GSS_TURNED_ON       = 0x64210004,           // Bluetooth needs to be turned on. You must restart connect
    GSS_DISCOVERY       = 0x64210005,           // Bluetooth is discovering devices. You must restart connect
    GSS_PEER_ID         = 0x64210006,           // Send the peer ID. Allows you to decide things, like who goes first
};


// Class: prGameSessionPacket
//      This class is incomplete as yet - TO BE COMPLETED
class prGameSessionPacket
{
public:
    u32 size;
    u32 hash;
    u32 type;
    u32 index;
};


// Class: prGameSessionReceiver
//      This class should be derived by any class wishing to know the status
//      of the current game session
class prGameSessionReceiver
{
public:
    // Method: GameSessionStatus
    //      Receives the current status of a game session
    virtual void GameSessionStatus(u32 status) = 0;

    // Method: PacketReceiver
    //      Receive a data packet from the other device
    virtual void PacketReceiver(prGameSessionPacket &packet) = 0;
};


// Class: prGameSession
//      Cross platform interface for multiplayer connectivity using bluetooth, or
//      other types of close range communication
class prGameSession
{
public:
    // Method: prGameSession
    //      Ctor
    prGameSession();

    // Method: ~prGameSession
    //      Dtor
    ~prGameSession();

    // Method: Initialise
    //      Initialises the game session
    //
    // Parameters:
    //      pProvider - A pointer to the provider
    //
    // Notes:
    //      This determines if the technology is available, etc
    //
    //      Call when required as this may bring up a dialog dependant on the platform
    //
    //      This class will clean up the provider
    void Initialise(prGameSessionReceiver *pReceiver, prGameSessionProvider *pProvider);

    // Method: Update
    //      Updates the game session
    bool Update();

    // Method: IsAvailable
    //      Is the technology available? For example does a device have bluetooth
    PRBOOL IsAvailable() const { return mIsAvailable; }

    // Method: IsEnabled
    //      Is the technology turned on?
    PRBOOL IsEnabled() const { return mIsEnabled; }

    // Method: ReceiveMessage
    //      Used by the OS to pass messages back to the game
    //
    // Notes:
    //      The messages are of type. Game Session Status
    //
    //      Some of which are platform specific 
    void ReceiveMessage(s32 msg);

    // Method: SendPacket
    //      Send a data packet to the other device
    void SendPacket(prGameSessionPacket &packet);

    // Method: ReceivePacket
    //      Receive a data packet from the other device
    void ReceivePacket(prGameSessionPacket &packet);

    // Method: IsServer
    //      Determine if we are the client or the server
    PRBOOL IsServer();
    
    // Method: GetProvider
    //      Gets the current provider
    prGameSessionProvider *GetProvider() const { return mpProvider; }
    
    // Method: Disconnect
    //      Disconnects a game session
    void Disconnect();


private:
    prGameSessionProvider  *mpProvider;             // The provider
    prGameSessionReceiver  *mpGameSessionReceiver;  // Notification class.
    PRBOOL                  mInitialised;           // Has game session been initialised.
    PRBOOL                  mIsAvailable;           // Is the technology available? For example does a device have bluetooth
    PRBOOL                  mIsEnabled;             // Is the technology turned on?
};
