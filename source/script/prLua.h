// File: prLua.h
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


#ifndef __PRLUA_H
#define __PRLUA_H


#include "../core/prTypes.h"


struct lua_State;


// Class: prLua
//      Class used to interface with lua and the engines lua functions
//
// Notes:
//      It is completely possible to construct and run a game with only the lua
//      components
class prLua
{
public:
    // Method: prLua
    //      Ctor
    prLua();

    // Method: ~prLua
    //      Dtor
    ~prLua();

    // Method: Run
    //      Runs a script
    //
    // Notes:
    //      This version is for single run scripts
    void Run(const char *filename);


private:
    // Check for any errors after running a script
    void CheckForErrors(lua_State *lua, int status);
};


#endif//__PRLUA_H
