// File: prParticleShared.h
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


#include "../core/prTypes.h"
#include <list>
#include <string>


// Struct: prEffectType
//      This struct is used to hold an effect type for a defined emitter
struct prEffectType
{
    Proteus::Core::u32  mHash;              // The identifying hash key
    Proteus::Core::s32  mCount;             // The number of effects
    Proteus::Core::f32  mWaitTime;
    Proteus::Core::f32  mRunTime;
};


// Types
typedef std::list<prEffectType>     prEffectTypeList;


// struct: prEmitterDefinition
//      This is used to hold the definition data of a particle emitter
//
// Notes:
//      Each defined emitter uses one or more effects that are updated at the same time.
//
// Notes:
//      For example. It could store effects called smoulder, burn, explode, smoke
//
//      Each effect has a wait time before it starts.
//      A runtime and a count
struct prEmitterDefinition
{
   
    // Method: prEmitterDefinition
    //      Ctor
    explicit prEmitterDefinition(const char *name);

    // Method: GetHash
    //      Gets the hash for this emitter definition
    Proteus::Core::u32  GetHash() const { return mHash; }

    // Method: GetName
    //      Gets the name for this emitter definition
    const char *GetName() const { return mName.c_str(); }

    // Method: GetNumEffects
    //      Gets the number of effects for this emitter
    Proteus::Core::u32 GetNumEffects() const { return (Proteus::Core::u32)mEffects.size(); }

    prEffectTypeList &GetEffectsList() { return mEffects; }


private:
    Proteus::Core::u32  mHash;
    std::string         mName;
    prEffectTypeList    mEffects;
};
