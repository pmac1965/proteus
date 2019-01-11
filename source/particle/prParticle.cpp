/**
 * prParticle.cpp
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


#include "prParticle.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prParticle::prParticle()
{
    Init();
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prParticle::~prParticle()
{
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prParticle::Init()
{
    mLifeTime   = 0.0f;                             // Particles lifetime
    mPos        = Proteus::Math::prVector3::Zero;   // Paricles position
    mColour     = prColour::White;                  // Particles colour (Add colour change)

    //f32         mScaleMin;
    //f32         mScaleMax;

    mVisible    = true;
    mActive     = true;
    mExp1       = false;
    mExp0       = false;
}


/// ---------------------------------------------------------------------------
/// Updates a particle
/// ---------------------------------------------------------------------------
bool prParticle::Update(f32 dt)
{
    if (mActive)
    {
        mPos += (mDirection * mSpeed) * dt;

        mPos += mGravity * dt;

        // Update life timer
        mLifeTime -= dt;
        if (mLifeTime < 0.0f)
        {
            Kill();
        }
    }


    return mActive;
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prParticle::Draw()
{
    if (mVisible)
    {

    }
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prParticle::Kill()
{
    mVisible    = false;
    mActive     = false;
}
