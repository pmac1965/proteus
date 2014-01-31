/**
 * @file       prMessageManager.h
 * @brief      Contains the message management class for game and system messaging.
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


#ifndef __PRMESSAGEMANAGER_H
#define __PRMESSAGEMANAGER_H


#include <list>
#include "prMessage.h"
#include "prCoreSystem.h"


/// @brief      The message manager. Allows for global message passing.
/// @n          Can be used locally as well
class prMessageManager : public prCoreSystem
{
public:

    /// @brief      Ctor.
    prMessageManager();

    /// @brief      Ctor.
    virtual ~prMessageManager();

    /// @brief      Registers a handler with the message manager.
    /// @param      handler - The handler
    /// @note       The passed handler must not be NULL.
    /// @n          This will assert in the debug build.
    void Register(prMessageHandler *handler);

    /// @brief      Unregisters a handler with the message manager.
    /// @param      handler - The handler
    /// @note       The passed handler must not be NULL.
    /// @n          This will assert in the debug build.
    void Unregister(prMessageHandler *handler);

    /// @brief      Instantly sends a message to all registers receivers.
    /// @param      msg - The message
    void Send(prGameMessage &msg);


private:

    std::list<prMessageHandler*>    m_handlers;
};


#endif//__PRMESSAGEMANAGER_H
