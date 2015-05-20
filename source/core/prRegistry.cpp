/**
 * prRegistry.cpp
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


#include "prRegistry.h"
#include "prCore.h"
#include "prMacros.h"
#include "prStringUtil.h"
#include "../debug/prTrace.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// @brief      Ctor.
/// ---------------------------------------------------------------------------
prRegistry::prRegistry() : prCoreSystem(PRSYSTEM_REGISTRY, "prRegistry")
{
    // The default system keys.
    static struct DefaultSystemKeys
    {
        const char *key;
        const char *value;
    }
    systemKeys[] = 
    {
        {"ScreenWidth",     "-1"},
        {"ScreenHeight",    "-1"},
        {"FullScreen",      "false"},
        {"Platform",        "Unknown"},
        {"AppName",         "Proteus"},
        {"WindowName",      "Proteus"},
        {"Verbose",         "false"},
        {"LogToFile",       "false"},
        {"UseArchives",     "true"},
        {"Help",            "false"},
        {"Exit",            "false"},
//        {"PhysicalWidth",   "-1"},
//        {"PhysicalHeight",  "-1"},
    };

    for (u32 i=0; i<PRARRAY_SIZE(systemKeys); i++)
    {
        AddKey(systemKeys[i].key, systemKeys[i].value);
    }

    #if defined(PLATFORM_PC)
        SetValue("Platform", "PC");

    #elif defined(PLATFORM_ANDROID)
        SetValue("Platform", "ANDROID");

    #elif defined(PLATFORM_BADA)
        SetValue("Platform", "BADA");

    #elif defined(PLATFORM_IOS)
        SetValue("Platform", "IOS");

    #elif defined(PLATFORM_LINUX)
        SetValue("Platform", "LINUX");

    #else
        SetValue("Platform", "Unknown");

    #endif
}


/// ---------------------------------------------------------------------------
/// @brief      Ctor.
/// ---------------------------------------------------------------------------
prRegistry::~prRegistry()
{
}


/// ---------------------------------------------------------------------------
/// Adds a new key to the registry.
/// ---------------------------------------------------------------------------
void prRegistry::AddKey(std::string key, std::string value)
{
    if (values.find(key) == values.end())
    {
        values.insert
        (
            std::pair<std::string, std::string>(key, value)
        );
    }
}


/// ---------------------------------------------------------------------------
/// Sets the value of a key in the registry.
/// ---------------------------------------------------------------------------
bool prRegistry::SetValue(std::string key, std::string value)
{
    bool result = false;

    std::map<std::string, std::string>::iterator it;
        
    for (it = values.begin(); it != values.end(); ++it)
    {
        if (key.compare((*it).first) == 0)
        {
            (*it).second = value;
            result = true;
            break;
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Sets the value of a key in the registry.
/// ---------------------------------------------------------------------------
bool prRegistry::SetValue(std::string key, s32 value)
{
    bool result = false;

    std::map<std::string, std::string>::iterator it;
        
    for (it = values.begin(); it != values.end(); ++it)
    {
        if (key.compare((*it).first) == 0)
        {
            (*it).second = prStringPrintf("%i", value);
            result = true;
            break;
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Gets the value of a key in the registry.
/// ---------------------------------------------------------------------------
const char *prRegistry::GetValue(std::string key)
{
    std::map<std::string, std::string>::iterator it;
        
    for (it = values.begin(); it != values.end(); ++it)
    {
        if (key.compare((*it).first) == 0)
        {
            return (*it).second.c_str();
        }
    }

    return NULL;
}


/// ---------------------------------------------------------------------------
/// Shows all the key/value pairs in the registry.
/// ---------------------------------------------------------------------------
void prRegistry::ShowKeyValuePairs()
{
#if defined(DEBUG) || defined(_DEBUG)

    if (!values.empty())
    {
        prTrace("System keys:-------------------------------------------------------------------\n");

        std::map<std::string, std::string>::iterator it;
        
        for (it = values.begin(); it != values.end(); ++it)
        {
            prTrace("Key: %*s, Value: %s\n", 20, ((*it).first).c_str(), ((*it).second).c_str());
        }

        prTrace("-------------------------------------------------------------------------------\n");
    }

#endif
}
