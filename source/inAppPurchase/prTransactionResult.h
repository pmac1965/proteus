// File: prTransactionResult.h
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


#ifndef __PRTRANSACTIONRESULT_H
#define __PRTRANSACTIONRESULT_H


#include "../core/prTypes.h"


// The result of a transaction
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
    virtual void TransactionResult(Proteus::Core::s32 result, const char *id) = 0;

    // Method: ConnectionStateChanged
    //      Lets you know when the connection state has changed.
    virtual void ConnectionStateChanged(bool connected) = 0;

    // Method: UpdateConnection
    //      Lets you know the current connection state.
    virtual void UpdateConnection(bool connected) = 0;
};


#endif//__PRTRANSACTIONRESULT_H
