/**
 * prLuaDebug.cpp
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


#include "../lua/lua.hpp"


#include "prLuaDebug.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"


namespace 
{
    /// -----------------------------------------------------------------------
    /// The 'trace' function.
    /// -----------------------------------------------------------------------
    int prLuaDebugTrace(lua_State *lua)
    {
        PRASSERT(lua);

        int argc = lua_gettop(lua);
        if (argc == 0)
        {
            prTrace(prLogLevel::LogError, "lua error - trace has no parameters\n");
            return 0;
        }

        for (int i=1; i<=argc; i++)
        {
            switch(lua_type(lua, i))
            {
            case LUA_TNIL:
                prTrace(prLogLevel::LogError, "nil");
                break;

            case LUA_TBOOLEAN:
                {
                    prTrace(prLogLevel::LogError, "%s", (lua_toboolean(lua, i) == 1) ? "true" : "false");
                }
                break;

            case LUA_TNUMBER:
                {
                    prTrace(prLogLevel::LogError, "%s", lua_tostring(lua, i));
                }
                break;

            case LUA_TSTRING:
                {
                    prTrace(prLogLevel::LogError, "%s", lua_tostring(lua, i));
                }
                break;

            default:
                prTrace(prLogLevel::LogError, "%s", lua_typename(lua, i));
                break;
            }
        }

        return 0;
    }
}


/// ---------------------------------------------------------------------------
/// Registers the LUA debug functions
/// ---------------------------------------------------------------------------
void prLuaDebugRegisterDebugFunctions(lua_State *lua)
{
    if (lua)
    {
        lua_pushcclosure(lua, prLuaDebugTrace, 0);
        lua_setglobal(lua, "trace");
    }
}
