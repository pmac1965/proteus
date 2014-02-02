/**
 * @file       prRegistry.h
 * @brief      Contains the registry class
 */
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


#ifndef __PRREGISTRY_H
#define __PRREGISTRY_H


#include "prTypes.h"
#include "prCoreSystem.h"
#include <string>
#include <map>


/**
 * @brief      The registry class stores global data used by the engine,
 * @n          such as screen width and height.
 */
class prRegistry : public prCoreSystem
{
public:

    /**
     * @brief      Ctor.
     */
    prRegistry();

    /**
     * @brief      Dtor.
     */
    virtual ~prRegistry();

    /**
     * @brief      Adds a new key to the registry
     *
     * @param      key   - The keys name
     * @param      value - The keys value
     */
    void AddKey(std::string key, std::string value);
    
    /**
     * @brief      Sets the value of a key in the registry.
     *
     * @param      key   - The keys name
     * @param      value - The keys value
     *
     * @return     true if value set, false otherwise
     */
    bool SetValue(std::string key, std::string value);
    
    /**
     * @brief      Sets the value of a key in the registry.
     *
     * @param      key   - The keys name
     * @param      value - The keys value
     *
     * @return     true if value set, false otherwise
     */
    bool SetValue(std::string key, s32 value);
    
    /**
     * @brief      Gets the value of a key in the registry.
     *
     * @param      key   - The keys name
     *
     * @return     The keys value as a string, or
     * @return     NULL if the key does not exist
     */
    const char *GetValue(std::string key);
    
    /**
     * @brief      Shows all the key/value pairs in the registry.
     *
     * @note       Only works in debug mode.
     */
    void ShowKeyValuePairs();

private:
    
    // The registry data
    std::map<std::string, std::string>  values;
};


#endif//__PRREGISTRY_H
