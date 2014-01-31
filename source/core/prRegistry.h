/**
 * @file       prRegistry.h
 * @brief      Contains the registry class
 * @brief      The registry class stores global data used by the engine,
 * @n          such as screen width and height.
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


#ifndef __PRREGISTRY_H
#define __PRREGISTRY_H


#include "prTypes.h"
#include "prCoreSystem.h"
#include <string>
#include <map>


/// @brief      The registry class stores global data used by the engine,
/// @n          such as screen width and height.
class prRegistry : public prCoreSystem
{
public:

    /// @brief      Ctor.
    prRegistry();

    /// @brief      Ctor.
    virtual ~prRegistry();

    /// @brief      Adds a new key to the registry.
    /// @param      key   - The keys name
    /// @param      value - The keys value
    void AddKey(std::string key, std::string value);
    
    /// @brief      Sets the value of a key in the registry.
    /// @param      key   - The keys name
    /// @param      value - The keys value
    /// @return     true if value set, false otherwise
    bool SetValue(std::string key, std::string value);
    
    /// @brief      Sets the value of a key in the registry.
    /// @param      key   - The keys name
    /// @param      value - The keys value
    /// @return     true if value set, false otherwise
    bool SetValue(std::string key, s32 value);
    
    /// @brief      Gets the value of a key in the registry.
    /// @param      key   - The keys name
    /// @return     The keys value as a string, or
    /// @return     NULL if the key does not exist
    const char *GetValue(std::string key);
    
    /// @brief      Shows all the key/value pairs in the registry.
    /// @note       Only works in debug mode.
    void ShowKeyValuePairs();

private:
    
    // The registry data
    std::map<std::string, std::string>  values;
};


#endif//__PRREGISTRY_H
