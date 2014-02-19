/**
 * prSpriteAnimation.cpp
 */


#include "../prConfig.h"


#include "prSpriteAnimation.h"
#include "prSpriteAnimationSequence.h"
#include "prSprite.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSpriteAnimation::prSpriteAnimation(prSprite *sprite) : m_sprite(*sprite)
{
    m_currSequence = NULL;
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
s32 prSpriteAnimation::GetUserDataForCurrentFrame(s32 index)
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
bool prSpriteAnimation::HasAnimationStopped()
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
bool prSpriteAnimation::HasAnimationStarted()
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
