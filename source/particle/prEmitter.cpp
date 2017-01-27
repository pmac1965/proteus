/**
 * prEmitter.cpp
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


#include "prEmitter.h"
#include "prParticleShared.h"
#include "../debug/prTrace.h"
#include "../core/prDefines.h"
#include "../core/prMacros.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prEmitter::prEmitter(s32 id, const prEmitterDefinition &ed) : mID(id)
{
    prTrace(LogError, "prEmitter::prEmitter - %i\n", mID);

    pEffects = nullptr;
    mAlive   = true;


    // s32 count = 0;//(s32)ed.mEffects.size();
    // if (count >= 0)
    // {
        // //pEffects = new prEffectType[count];
    // }
}


/// ---------------------------------------------------------------------------
/// Destructor
/// ---------------------------------------------------------------------------
prEmitter::~prEmitter()
{
    prTrace(LogError, "prEmitter::~prEmitter - %i\n", mID);

    PRSAFE_DELETE(pEffects);
}


//void prEmitter::Load(const char *filename)
//{
//}


bool prEmitter::Update(f32 dt)
{
    // Reduce lifetime
    if (mAlive)
    {
        mLifetime -= dt;
        mAlive = (mLifetime > 0);
    }

    return mAlive;
}


//void prEmitter::Draw()
//{
//}
