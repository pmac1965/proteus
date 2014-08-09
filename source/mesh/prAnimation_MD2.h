// File: prAnimation_MD2.h
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


#ifndef __PRANIMATION_MD2_H
#define __PRANIMATION_MD2_H


#include "prAnimation.h"
#include "prMD2.h"
#include <list>


// Method: prAnimation_MD2
//      Mesh animation class for MD2 files
class prAnimation_MD2 : public prAnimation
{
public:
    // Method: prAnimation_MD2
    //      Constructor
    prAnimation_MD2(prMD2Frame *pFrames, s32 numFrames);

    // Method: ~prAnimation_MD2
    //      Destructor
    ~prAnimation_MD2();

    // Method: Update
    //      Updates the animation
    void Update(f32 step);

    // Method: Play
    //      Plays an animation
    void Play(const char *name);

    // Method: Play
    //      Plays an animation
    void Play(s32 animation);

    // Method: Play
    //      Stops the current animation.
    //
    // Notes:
    //      The animation cannot be resumed.
    //      If you need to resume the animation use pause.
    void Stop();

    // Method: GetFrame
    //      Gets the current frame.
    s32 GetFrame() const { return mCurFrame; }


private:

    std::list<prMD2Anim*>   mAnimations;
    s32                     mStartFrame;
    s32                     mEndFrame;
    s32                     mCurFrame;
    float                   mStep;
};


#endif//__PRANIMATION_MD2_H
