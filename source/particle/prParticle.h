// File: prParticle.h
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


#ifndef __PRPARTICLE_H
#define __PRPARTICLE_H


#include "../core/prTypes.h"
#include "../math/prVector3.h"
#include "../display/prColour.h"


// Class: prParticle
//      Class represents a single particle
class prParticle
{
public:
    // Method: prParticle
    //      Constructor.
    prParticle();

    // Method: ~prParticle
    //      Destructor.
    ~prParticle();

    void Init();

    // Method: Update
    //      Updates a particle
    bool Update(f32 dt);

    // Method: Draw
    //      Draws a particle
    void Draw();

    // Method: Kill
    //      Kills a particle
    void Kill();

    bool IsActive() const { return mActive; }

    bool IsVisible() const { return mVisible; }


private:

    f32         mLifeTime;                  // Particles lifetime
    prVector3   mPos;                       // Particles position
    prVector3   mGravity;                   // Particles gravity
    prVector3   mDirection;                 // Particles direction
    prVector3   mSpeed;                     // Particles speed
    prColour    mColour;                    // Particles colour (Add colour change)

    //f32         mScaleMin;
    //f32         mScaleMax;

    bool        mVisible;
    bool        mActive;
    bool        mExp1;
    bool        mExp0;
};


#endif//__PRPARTICLE_H
