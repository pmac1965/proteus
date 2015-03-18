// File: prEmitter.h
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


#ifndef __PREMITTER_H
#define __PREMITTER_H


#include "../core/prTypes.h"
#include "../math/prVector3.h"
#include "../display/prColour.h"


// Forward declarations
struct prEffectType;
struct prEmitterDefinition;


// Class: prEmitter
//      Class represents a particle emission point - W.I.P Work in progress
class prEmitter
{
private:
    // We only want the particle manager creating emitters
    friend class prParticleManager;

    // Method: prEmitter 
    //      Ctor
    prEmitter(s32 id, const prEmitterDefinition &ed);

    // Method: prEmitter 
    //      Dtor
    ~prEmitter();

//    void Load(const char *filename);

    // Method: GetID
    //      Get the emitters unique ID.
    s32 GetID() const { return mID; }


public:
    bool Update(f32 dt);
    //void Draw();


    Proteus::Math::prVector3   mPos;           // Position of the emitter

    //std::list<> mParticles;


private:
    // Stop passing by value and assignment.
    prEmitter(const prEmitter&);
    const prEmitter& operator = (const prEmitter&);

    prEffectType   *pEffects;

    s32             mID;                // Unique ID
    f32             mLifetime;          // Lifetime of this emitter as a whole

    bool            mAlive;             //
};


#endif//__PREMITTER_H
