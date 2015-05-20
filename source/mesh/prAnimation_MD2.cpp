/**
 * prAnimation_MD2.cpp
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


#include "prAnimation_MD2.h"
#include "../core/prStringUtil.h"
#include "../debug/prTrace.h"


#if defined(_DEBUG) || defined(DEBUG)
#define MD2_ANIM_DEBUG
#endif


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prAnimation_MD2::prAnimation_MD2(prMD2Frame *pFrames, s32 numFrames)
{
    // Sanity checks.
    PRASSERT(pFrames);
    PRASSERT(numFrames > 0);
    PRASSERT(numFrames < MD2_MAX_FRAMES);


    // Current frame name.
    char name[32] = "0123456789";
    char work[32];

    prMD2Anim *prev = NULL;
    prMD2Anim *anim = NULL;

    for (int i=0; i<numFrames; i++)
    {
        // Get the frame name.
        prStringCopySafe(work, pFrames[i].name, sizeof(work));


        // Isolate the name.
        for (unsigned int j=0; j<sizeof(work); j++)
        {
            if (work[j] >= '0' && work[j] <= '9')
            {
                work[j] = '\0';
                break;
            }
        }


        // Is it a new animation?
        if (prStringCompare(name, work) != CMP_EQUALTO)
        {
            // Set current animation name
            prStringCopySafe(name, work, sizeof(work));

            anim = new prMD2Anim();
            anim->start = i;
            anim->end   = 0;
            anim->hash  = prStringHash(work);
            prStringCopySafe(anim->name, work, sizeof(work));


            // Set last frame of last animation.
            if (prev)
            {
                prev->end = i - 1;
            }


            // Store this animation, so we can set its start/end frames.
            prev = anim;


            // Add to list of animations
            mAnimations.push_back(anim);  //.AddTail(anim);
        }
    }


    // Set last frame of last animation.
    if (anim)
    {
        anim->end = numFrames - 1;
    }

    
    // Init
    mCurrAnimation = ANIMATION_INVALID;
    mStartFrame    = 0;
    mEndFrame      = 0;
    mCurFrame      = 0;
    mStep          = 0.0f;


    // Show animation details.
    #ifdef MD2_ANIM_DEBUG
    std::list<prMD2Anim*>::iterator it  = mAnimations.begin();
    std::list<prMD2Anim*>::iterator end = mAnimations.end();
    for (;it != end; ++it)
    {
        prTrace("Anim '%s': start %i, end %i\n", (*it)->name, (*it)->start,  (*it)->end);
    }
    #endif
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prAnimation_MD2::~prAnimation_MD2()
{
    std::list<prMD2Anim*>::iterator it  = mAnimations.begin();
    std::list<prMD2Anim*>::iterator end = mAnimations.end();
    for (;it != end; ++it)
    {
        PRSAFE_DELETE((*it));
    }
}


/// ---------------------------------------------------------------------------
/// Updates the animation.
/// ---------------------------------------------------------------------------
void prAnimation_MD2::Update(f32 step)
{
    if (mCurrAnimation != ANIMATION_INVALID)
    {
        mStep += step;

        if (mStep > 1.0f)
        {
            mStep -= 1.0f;

            mCurFrame++;

            if (mCurFrame > mEndFrame)
                mCurFrame = mStartFrame;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Plays an animation
/// ---------------------------------------------------------------------------
void prAnimation_MD2::Play(const char *name)
{
    PRASSERT(name && *name);

    //List<MD2Anim*>::Iterator it = m_animations.Begin();
    std::list<prMD2Anim*>::iterator it  = mAnimations.begin();
    std::list<prMD2Anim*>::iterator end = mAnimations.end();

    s32 index = 0;

//    while(it.Okay())
    //while(it != mAnimations.end());//it.Okay())
    for (;it != end; ++it)
    {
        if (prStringCompare(const_cast<char*>(name), (*it)->name) == CMP_EQUALTO)
        {
            mCurrAnimation = index;
            mStartFrame    = (*it)->start;
            mEndFrame      = (*it)->end;
            mCurFrame      = mStartFrame;
            mStep          = 0.0f;
            return;
        }

        ++index;
        //++it;
    }
}


/// ---------------------------------------------------------------------------
/// Plays an animation
/// ---------------------------------------------------------------------------
void prAnimation_MD2::Play(s32 animation)
{
    PRUNUSED(animation);
}


/// ---------------------------------------------------------------------------
/// Stops the current animation.
/// ---------------------------------------------------------------------------
void prAnimation_MD2::Stop()
{
    mCurrAnimation = ANIMATION_INVALID;
    mStep          = 0.0f;
}

