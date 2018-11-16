// File: prRegistry.h
// 
// Notes:
//      The registry class stores global data for use by the engine,
//      such as screen width and height.
//
//      These values are used and changed by the engine and they cannot
//      be deleted
//
// Notes:
//      The user is free to add their own key/value pairs, but the name
//      of the key must be unique. Use *ShowKeyValuePairs* to see the
//      default values
//
// Notes:
//      This class is a core system class and it created by default
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


#include "prTypes.h"
#include "prCoreSystem.h"
#include <string>
#include <map>


// Class: prRegistry
//      The registry class stores global data for use by the engine,
//      such as screen width and height.
class prRegistry : public prCoreSystem
{
public:
    // Method: prRegistry
    //      Ctor.
    prRegistry();

    // Method: ~prRegistry
    //      Dtor.
    virtual ~prRegistry() {}

    // Method: AddKey
    //      Adds a new key to the registry
    //
    // Parameters:
    //      key   - The keys name
    //      value - The keys value
    //
    // Returns:
    //      true if added, false otherwise
    bool AddKey(std::string key, std::string value);
    
    // Method: SetValue
    //      Sets the value of a key in the registry.
    //
    // Parameters:
    //      key   - The keys name
    //      value - The keys value
    //
    // Notes:
    //      All entries are stored as stringn
    //
    // Returns:
    //      true if value set, false otherwise
    bool SetValue(std::string key, const std::string &value);
    
    // Method: SetValue
    //      Sets the value of a key in the registry.
    //
    // Parameters:
    //      key   - The keys name
    //      value - The keys value
    //
    // Notes:
    //      All entries are stored as string
    //
    // Returns:
    //      true if value set, false otherwise
    bool SetValue(std::string key, Proteus::Core::s32 value);
    
    // Method: SetValue
    //      Sets the value of a key in the registry.
    //
    // Parameters:
    //      key   - The keys name
    //      value - The keys value
    //
    // Notes:
    //      All entries are stored as string
    //
    // Returns:
    //      true if value set, false otherwise
    bool SetValue(std::string key, Proteus::Core::u32 value);
    
    // Method: SetValue
    //      Sets the value of a key in the registry.
    //
    // Parameters:
    //      key   - The keys name
    //      value - The keys value
    //
    // Notes:
    //      All entries are stored as string
    //
    // Returns:
    //      true if value set, false otherwise
    bool SetValue(std::string key, Proteus::Core::f32 value);
    
    // Method: GetValue
    //      Gets the value of a key in the registry.
    //
    // Parameters:
    //      key   - The keys name
    //
    // Returns:
    //          The keys value as a string, or
    //          nullptr if the key does not exist
    const char *GetValue(std::string key);
    
    // Method: ShowKeyValuePairs
    //      Shows all the key/value pairs in the registry.
    //
    // Notes:
    //      Only works in debug mode.
    void ShowKeyValuePairs();


private:    
    // The registry data
    std::map<std::string, std::string>  values;
};
