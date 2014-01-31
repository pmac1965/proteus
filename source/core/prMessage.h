/**
 * @file       prMessage.h
 * @brief      Contains the message class for game and system messaging.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRMESSAGE_H
#define __PRMESSAGE_H


#include "prTypes.h"


/// @struct     prGameMessage
/// @brief      The message structure
typedef struct prGameMessage
{
public:

    /// @brief      Default constructor
    /// @note       Sets all data to zero.            
    prGameMessage()
    {
        type        = 0;
        user0       = 0;
        user1       = 0;
        user2       = 0;
        user3       = 0;
    }

    /// @brief      Ctor
    /// @param      atype - The message type.
    /// @param      d0    - Optional data component.
    /// @param      d1    - Optional data component.
    /// @param      d2    - Optional data component.
    /// @param      d3    - Optional data component.
    prGameMessage(u32 atype, u32 d0 = 0, u32 d1 = 0, u32 d2 = 0, u32 d3 = 0)
    {
        type        = atype;
        user0       = d0;
        user1       = d1;
        user2       = d2;
        user3       = d3;
    }

    u32 type;               ///< The message type
    u32 user0;              ///< Data for message use. Will depend on the message
    u32 user1;              ///< Data for message use. Will depend on the message
    u32 user2;              ///< Data for message use. Will depend on the message
    u32 user3;              ///< Data for message use. Will depend on the message

} prGameMessage;


/// @brief      Interface class. Message receivers need to include this class
/// @n          in order to receive messages
/// @see        prMessageManager
/// @see        prGameMessage
class prMessageHandler
{
public:

    /// @brief      Ctor.
    prMessageHandler() {}

    /// @brief      Dtor.
    virtual ~prMessageHandler() {}

    /// @brief      Handler for passed messages.
    /// @param      msg - A message
    /// @note       The message can be targeted or system wide.
    /// @return     true if handled.
    /// @return     false if NOT handled.
    virtual bool Receive(prGameMessage &msg) = 0;
};


#endif//__PRMESSAGE_H
