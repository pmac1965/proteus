/**
 * @file       prCoreSystem.h
 * @brief      Contains the base class used by all the engines core systems.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 * @note       All engine systems must use this base class as its used to
 * @note       add the systems to the engine core.
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


#ifndef __PRCORESYSTEM_H
#define __PRCORESYSTEM_H


#include "prTypes.h"
#include "prString.h"


/// @brief  Base class used by the engines systems.
/// @note   This class must be used to add any core system to the engine core.
class prCoreSystem
{
public:

    /// @brief      Constructor.
    /// @param      id      - A unique ID which identifies the system.
    /// @param      pName   - The name of the system.
    prCoreSystem(u32 id, const char *pName);

    /// @brief      Destructor.
    virtual ~prCoreSystem();

    /// @brief      Fetches the core systems unique ID.
    /// @return     u32 - The systems unique ID.
    u32 ID() const { return m_id; }

    /// @brief      Fetches the core systems name.
    /// @return     char * - The systems name.
    const char *Name() const { return m_name.Text(); }


private:

    u32         m_id;           ///< The systems unique ID.
    prString    m_name;         ///< The systems name.
};


#endif//__PRCORESYSTEM_H
