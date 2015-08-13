// File: prTransactionResult.h
//      A simple interface class that allows a handler class to receive IAP notifications
//
/**
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


#pragma once


#include "../core/prTypes.h"


// Namespaces
namespace Proteus {
namespace IAP {


// Enum: prTransactionResultType
//      The result of a transaction
//
// TRANSACTION_PURCHASING   - Starting transaction
// TRANSACTION_PURCHASED    - Transaction completed successfully
// TRANSACTION_FAILED       - Transaction failed
// TRANSACTION_CANCELLED    - Transaction was cancelled
// TRANSACTION_REFUNDED     - Transaction was refunded (Needs to be removed)
// TRANSACTION_RESTORED     - Transaction was restored (Needs to be set at completed)
enum prTransactionResultType
{
    TRANSACTION_PURCHASING,
    TRANSACTION_PURCHASED,
    TRANSACTION_FAILED,
    TRANSACTION_CANCELLED,
    TRANSACTION_REFUNDED,
    TRANSACTION_RESTORED,
};


// Class: prTransactionResult
//      Allows a handler class to receive IAP notifications
class prTransactionResult
{
public:
    // Method: TransactionResult
    //      Lets you know the result of a transaction
    //
    // Parameters:
    //      result - The result of a transaction
    //      id     - Identifier of the item involved
    //
    // See also:
    //      <prTransactionResultType>
    virtual void TransactionResult(Proteus::Core::s32 result, const char *id) = 0;

    // Method: ConnectionStateChanged
    //      Lets you know when the connection state has changed.
    //
    // Parameters:
    //      connected - Connection status
    //
    // Notes:
    //      This is only called if the connection state changes.
    //      Use *UpdateConnection* to get current state
    virtual void ConnectionStateChanged(bool connected) = 0;

    // Method: UpdateConnection
    //      Lets you know the current connection state.
    //
    // Parameters:
    //      connected - Connection status
    virtual void UpdateConnection(bool connected) = 0;
};


}}// Namespaces