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


#ifndef __PRPARTICLESHARED_H
#define __PRPARTICLESHARED_H


#include "../core/prTypes.h"
#include <list>


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
typedef std::list<prEffectType>                                 prEffectTypeList;
typedef std::list<prEffectType>::iterator                       prEffectTypeListIt;


// struct: prEmitterDefinition
//      This is used to hold the definition data of a particle emitter
struct prEmitterDefinition
{
    explicit prEmitterDefinition(const char *name);
    prEffectTypeList    mEffects;
    Proteus::Core::u32  mHash;
};


#endif//__PRPARTICLESHARED_H
