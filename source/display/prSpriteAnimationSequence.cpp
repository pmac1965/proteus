/**
 * prSpriteAnimationSequence.cpp
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


#include <string.h>
#include "prSpriteAnimationSequence.h"
#include "prSpriteAnimation.h"
#include "prOglUtils.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../core/prMacros.h"
#include "../core/prStringUtil.h"
#include "../tinyxml/tinyxml.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSpriteAnimationSequence::prSpriteAnimationSequence(const char *name, s32 frames, s32 type) 
{
    PRASSERT(name && *name);
    PRASSERT(frames > 0);

    int len = (int)(strlen(name) + 1);

    m_pName         = new char[len];
    m_pData         = new prAnimSequenceData[ frames ];
    m_count         = frames;
    m_hash          = prStringHash(name);
    m_animationType = type;

    strcpy(m_pName, name);

    m_frameDelay    = 0.0f;   
    m_frameIndex    = 0;   
    m_currentFrame  = 0; 
    m_state         = ANIM_STATE_NONE;        
    m_dir           = 0;          

    PRASSERT(m_pName);
    PRASSERT(m_pData);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSpriteAnimationSequence::~prSpriteAnimationSequence()
{
    PRSAFE_DELETE_ARRAY(m_pName);
    PRSAFE_DELETE_ARRAY(m_pData);
}


/// ---------------------------------------------------------------------------
/// Animates a sprite provided that it is not paused.
/// ---------------------------------------------------------------------------
void prSpriteAnimationSequence::Animate(float dt)
{
    if (m_state == ANIM_STATE_PLAYING)
    {    
        m_frameDelay -= dt;

        if (m_frameDelay < 0.0f)
        {
            // Play once?
            if (m_animationType == ANIM_TYPE_ONCE)
            {
                m_currentFrame++;

                if (m_currentFrame >= m_count)
                {
                    m_state = ANIM_STATE_STOPPED;
                    m_currentFrame--;
                }
                else
                {
                    m_frameIndex  = m_pData[m_currentFrame].index;
                    m_frameDelay += m_pData[m_currentFrame].delay;
                }
            }

            // Looping animation?
            else if (m_animationType == ANIM_TYPE_LOOP)
            {
                m_currentFrame++;

                if (m_currentFrame >= m_count)
                {
                    m_frameIndex   = m_pData[0].index;
                    m_frameDelay   = m_pData[0].delay;
                    m_currentFrame = 0;
                }
                else
                {
                    m_frameIndex  = m_pData[m_currentFrame].index;
                    m_frameDelay += m_pData[m_currentFrame].delay;
                }
            }

            // Yoyo animation?
            else if (m_animationType == ANIM_TYPE_YOYO)
            {
                // Not enough frames to yoyo?
                if (m_count < 2)
                {
                    PRWARN("Invalid animation.");
                    m_state = ANIM_STATE_STOPPED;
                    return;
                }

                m_currentFrame += m_dir;

                if (m_currentFrame >= m_count)
                {
                    m_dir          = -1;
                    m_currentFrame = m_count - 2;
                }                
                else if (m_currentFrame < 0)
                {
                    m_dir          = 1;                    
                    m_currentFrame = 1;
                }

                m_frameIndex   = m_pData[m_currentFrame].index;
                m_frameDelay  += m_pData[m_currentFrame].delay;
            }

            // Unknown.
            else
            {
                m_state = ANIM_STATE_STOPPED;
                PRWARN("Unknown animation type.");
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Starts the animation sequence.
/// ---------------------------------------------------------------------------
void prSpriteAnimationSequence::Begin()
{
    m_frameIndex   = m_pData[0].index;
    m_frameDelay   = m_pData[0].delay;
    m_currentFrame = 0;
    m_state        = ANIM_STATE_PLAYING;
    m_dir          = 1;
}


/// ---------------------------------------------------------------------------
/// Returns the animations sequences hashed name.
/// ---------------------------------------------------------------------------
u32 prSpriteAnimationSequence::GetHash() const
{
    return m_hash;
}


/// ---------------------------------------------------------------------------
/// Returns the current frame.
/// ---------------------------------------------------------------------------
s32 prSpriteAnimationSequence::GetFrame() const
{
    return m_frameIndex;
}


/// ---------------------------------------------------------------------------
/// Parses the animation data from an xml file.
/// ---------------------------------------------------------------------------
void prSpriteAnimationSequence::ParseFrameData(TiXmlElement* pElement)
{
    // Acquire the child frame data.
    TiXmlHandle   root(pElement);
    TiXmlElement *pElem  = root.FirstChild("frame").Element();
    s32           i      = 0;       
    s32           frames = m_count;

    for (; pElem; pElem = pElem->NextSiblingElement())
    {
        PRASSERT(pElem->Attribute("index"));
        PRASSERT(pElem->Attribute("delay"));

        // Get user data.
        const char *user0 = pElem->Attribute("user0");
        const char *user1 = pElem->Attribute("user1");
        const char *user2 = pElem->Attribute("user2");
        const char *user3 = pElem->Attribute("user3");

        if (i < frames)
        {
            m_pData[i].index =        atoi(pElem->Attribute("index"));
            m_pData[i].delay = (float)atoi(pElem->Attribute("delay"));

            // Set user data.
            if (user0) { m_pData[i].userData[0] = atoi(user0); }
            if (user1) { m_pData[i].userData[1] = atoi(user1); }
            if (user2) { m_pData[i].userData[2] = atoi(user2); }
            if (user3) { m_pData[i].userData[3] = atoi(user3); }

            //prTrace(LogError, "User data (%i): %i %i %i %i\n", i, pData[i].userData[0],
            //                                            pData[i].userData[1],
            //                                            pData[i].userData[2],
            //                                            pData[i].userData[3]);
        }
        else
        {
            PRWARN("Sprite frame/index data exceeds specified frame count");
            return;
        }

        //prTrace(LogError, "  Index %i, Delay %.02f\n", pData[i].index, pData[i].delay);
        i++;
    }
}


/// ---------------------------------------------------------------------------
/// Gets the user data for the current frame.
/// ---------------------------------------------------------------------------
s32 prSpriteAnimationSequence::GetUserDataForCurrentFrame(s32 index) const
{
    PRASSERT(PRBETWEEN(index, 0, MAX_USER_DATA - 1));
    return m_pData[m_currentFrame].userData[index];
}


/// ---------------------------------------------------------------------------
/// Returns the animations state.
/// ---------------------------------------------------------------------------
s32 prSpriteAnimationSequence::GetAnimState() const
{
    return m_state;
}
