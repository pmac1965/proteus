// File: prAnimation.h
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


#ifndef __PRANIMATION_H
#define __PRANIMATION_H


#include "../core/prTypes.h"


#define ANIMATION_INVALID   -1


// Method: prAnimation
//      Mesh animation base class
class prAnimation
{
public:
    // Method: prAnimation
    //      Constructor
    prAnimation();

    // Method: ~prAnimation
    //      Destructor
    virtual ~prAnimation();

    // Method: Update
    //      Updates the animation
    virtual void Update(Proteus::Core::f32 step) = 0;

    // Method: Play
    //      Plays an animation
    virtual void Play(const char *name) = 0;

    // Method: Play
    //      Plays an animation
    virtual void Play(Proteus::Core::s32 animation) = 0;

    // Method: Play
    //      Stops the current animation.
    //
    // Notes:
    //      The animation cannot be resumed.
    //      If you need to resume the animation use pause.
    virtual void Stop() = 0;

    // Method: Play
    //      Pause/unpause an animation.
    void Pause(bool state) { mPause = state; }


protected:

    bool                mPause;
    bool                mExp0;
    bool                mExp1;
    bool                mExp2;

    Proteus::Core::s32  mCurrAnimation;
};


#endif//__PRANIMATION_H
