// File: prEditorObject.h
//      This class needs only to exist for editor builds. It allows functions for editing of
//      object names, ID's and the setting of other values
/**
 * Copyright 2016 Paul Michael McNab
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


#include "../core/prString.h"


// Forward declarations
class prGameObject;


#if !defined(PROTEUS_EDITOR)
    // Optional inclusion on game objects
    #define PROT_EDITOR_INCL : prEditorObject

    // Optional inclusion on game objects constructors
    #define PROT_EDITOR_CTOR : prEditorObject(*this)    \
                             , mActive    (PRTRUE)      \
                             , mEnabled   (PRTRUE)      \
                             , mVisible   (PRTRUE)

    //
    #define GAME_OBJ_VAR(type, name) type name;

    //
    #define PROT_EDITOR_FUNC(retval, funcname ) \
        retval funcname();

#else
    // Optional inclusion on game objects
    #define PROT_EDITOR_INCL

    // Optional inclusion on game objects constructors
    #define PROT_EDITOR_CTOR : mActive    (PRTRUE)      \
                             , mEnabled   (PRTRUE)      \
                             , mVisible   (PRTRUE)

    //#define PROT_EDITOR_FUNC() 

#endif


// Class: prEditorObject
//      This class needs only to exist for editor builds. It allows functions for editing of
//      object names, ID's and the setting of other values
class prEditorObject
{
public:
    explicit prEditorObject(prGameObject &gameObject) : mGameObject(gameObject)
													  , mName("")
    {}

    //
    void SetGameObjectName(prString /*aname*/) {}

    //
    prString &GetGameObjectName() { return mName; }// aname) {}


private:
    prGameObject    &mGameObject;
    prString        mName;

    //GAME_OBJ_VAR(int, name)
};
