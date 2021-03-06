/**
 * prSpriteAnimation.cpp
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


#include "../prConfig.h"


#include "prSpriteAnimation.h"
#include "prSpriteAnimationSequence.h"
#include "prSprite.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSpriteAnimation::prSpriteAnimation(prSprite *sprite) : m_sprite(*sprite)
{
    m_currSequence = nullptr;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSpriteAnimation::~prSpriteAnimation()
{
    std::list<prSpriteAnimationSequence*>::iterator it  = m_sequences.begin();
    std::list<prSpriteAnimationSequence*>::iterator end = m_sequences.end();

    for (; it != end; ++it)
    {
        delete *it;
    }
}


/// ---------------------------------------------------------------------------
/// Updates the sprites animation.
/// ---------------------------------------------------------------------------
void prSpriteAnimation::Update(float dt)
{
    if (m_currSequence)
    {
        m_currSequence->Animate(dt);

    //    if (m_sprite.GetCurrentFrame() != m_currSequence->frameIndex)
    //    {
            m_sprite.SetFrame(m_currSequence->GetFrame());
    //    }
    }
}


/// ---------------------------------------------------------------------------
/// Adds an animation sequence.
/// ---------------------------------------------------------------------------
void prSpriteAnimation::AddSequence(prSpriteAnimationSequence* sequence)
{ 
    PRASSERT(sequence);
    m_sequences.push_back(sequence);
}


/// ---------------------------------------------------------------------------
/// Plays a named animation sequence.
/// ---------------------------------------------------------------------------
void prSpriteAnimation::PlaySequence(const char *name)
{
    PRASSERT(name && *name);
    u32 hash = prStringHash(name);

    std::list<prSpriteAnimationSequence*>::iterator it  = m_sequences.begin();
    std::list<prSpriteAnimationSequence*>::iterator end = m_sequences.end();

    for (; it != end; ++it)
    {
        if ((*it)->GetHash() == hash)
        {
            m_currSequence = *it;
            m_currSequence->Begin();
            m_sprite.SetFrame(m_currSequence->GetFrame());
            return;
        }
    }

    PRWARN("Failed to play animation sequence '%s'", name);
}


/// ---------------------------------------------------------------------------
/// Gets the user data for the current frame.
/// ---------------------------------------------------------------------------
s32 prSpriteAnimation::GetUserDataForCurrentFrame(s32 index) const
{
    PRASSERT(PRBETWEEN(index, 0, 3));

    if (m_currSequence)
    {
        return m_currSequence->GetUserDataForCurrentFrame(index);
    }

    return -1;
}


/// ---------------------------------------------------------------------------
/// Has the current animation stopped.
/// ---------------------------------------------------------------------------
bool prSpriteAnimation::HasAnimationStopped() const
{
    bool result = false;

    if (m_currSequence)
    {
        if (m_currSequence->GetAnimState() == ANIM_STATE_STOPPED)
        {
            result = true;
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Has any animation been played and animation is not in its default state.
/// ---------------------------------------------------------------------------
bool prSpriteAnimation::HasAnimationStarted() const
{
    bool result = false;

    if (m_currSequence)
    {
        if (m_currSequence->GetAnimState() != ANIM_STATE_NONE)
        {
            result = true;
        }
    }

    return result;
}
