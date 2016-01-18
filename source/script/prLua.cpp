/**
 * prLua.cpp
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


#include "prLua.h"
#include "prLuaDebug.h"
#include "../debug/prTrace.h"
#include "../lua/lua.hpp"
#include "../thread/prThread.h"


//namespace 
//{
//    int printMessage (lua_State *lua)
//    {
//       //assert (lua_isstring (lua,1));
//
//       const char *msg = lua_tostring (lua, 1);
//
//       prTrace(LogError, "LUA: %s", msg);
//
//       // debug output
//       //cout << "script: " << msg << endl;
//       return 0;
//    }
//}


//void report_errors(lua_State *L, int status)
//{
//  if ( status!=0 ) {
//    //std::cerr << "-- " << lua_tostring(L, -1) << std::endl;
//   prTrace(LogError, "%s", lua_tostring(L, -1));
//    lua_pop(L, 1); // remove error message
//  }
//}


int my_function(lua_State *L)
{
  int argc = lua_gettop(L);

  prTrace(LogError, "Arg count %i\n", argc);

  //std::cerr << "-- my_function() called with " << argc
    //<< " arguments:" << std::endl;

  for ( int n=1; n<=argc; ++n ) {
      prTrace(LogError, "%s\n", lua_tostring(L, n));

//    std::cerr << "-- argument " << n << ": "
  //    << lua_tostring(L, n) << std::endl;
  }

  lua_pushnumber(L, 123); // return value
  return 1; // number of return values
}


/// ---------------------------------------------------------------------------
/// This is the thread used to monitor a running script.
/// ---------------------------------------------------------------------------
//u32 PRTHREAD_CALLCONV LuaThreadFunc(void *)
PRTHREAD_RETVAL PRTHREAD_CALLCONV LuaThreadFunc(void *)
{
    return 0;
}


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prLua::prLua()
{
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prLua::~prLua()
{
}


/// ---------------------------------------------------------------------------
/// Runs a script
/// ---------------------------------------------------------------------------
void prLua::Run(const char *filename)
{
    PRASSERT(filename && *filename);

    lua_State *lua = luaL_newstate();
    if (lua)
    {
        // Setup global error functions
        prLuaDebugRegisterDebugFunctions(lua);

        luaL_openlibs(lua);

        lua_register(lua, "my_function", my_function);

        int s = luaL_loadfile(lua, filename);

        //luaL_loadstring(

        if (s == 0)
        {
            // Execute Lua program
            s = lua_pcall(lua, 0, LUA_MULTRET, 0);
        }

        CheckForErrors(lua, s);

        lua_close(lua);        
    }
}


/// ---------------------------------------------------------------------------
/// Runs a script (On a thread)
/// ---------------------------------------------------------------------------
void prLua::RunThreaded(const char *filename)
{
}


/// ---------------------------------------------------------------------------
/// Check for any lua errors 
/// ---------------------------------------------------------------------------
void prLua::CheckForErrors(lua_State *lua, int status)
{
    PRASSERT(lua);

    if (status != 0)
    {        
        prTrace(LogError, "%s\n", lua_tostring(lua, -1));

        // Remove error message
        lua_pop(lua, 1); 
    }
}
