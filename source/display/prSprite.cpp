/**
 * prSprite.cpp
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


// Platform specifics
#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>
  #include <string.h>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <string.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>
  #include <string.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <string.h>

#else
  #error No platform defined.

#endif


#include "prRenderer.h"
#include "prSprite.h"
#include "prSpriteAnimation.h"
#include "prSpriteManager.h"
#include "prOglUtils.h"
#include "../core/prMacros.h"
#include "../core/prCore.h"
#include "../display/prTexture.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"


//using namespace Proteus::Core;


// Defines
#define DEFAULT_SCALE                   1.0f


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prSprite::prSprite(prSpriteManager *pSpriteManager, prTexture *pTexture, const char *name, s32 frameWidth, s32 frameHeight) : m_colour(prColour::White)
{
    PRASSERT(pTexture);
    PRASSERT(name && *name);

    mpRenderer      = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
    m_pTexture      = pTexture;
    mpSpriteManager = pSpriteManager;
    m_animation     = nullptr;
    m_name          = nullptr;
    m_frameWidth    = frameWidth;
    m_frameHeight   = frameHeight;
    m_framesAcross  = pTexture->GetWidth()  / frameWidth;
    m_framesDown    = pTexture->GetHeight() / frameHeight;
    m_framesTotal   = m_framesAcross * m_framesDown;
    m_frame         = 0;

    m_pw = 1.0f / pTexture->GetWidth();     // Get pixel width/height
    m_ph = 1.0f / pTexture->GetHeight();
    m_fw = m_pw * frameWidth;               // Set frame width/height in pixels
    m_fh = m_ph * frameHeight;

    // User values
    user0 = 0;
    user1 = 0;
    user2 = 0;
    user3 = 0;
    pool  = -1;

    m_flip      = prSprite::FLIP_NONE;
    m_scaleX    = DEFAULT_SCALE;
    m_scaleY    = DEFAULT_SCALE;
    m_rotation  = 0.0f;
    m_u0        = 0.0f;
    m_u1        = 0.0f;
    m_v0        = 0.0f;
    m_v1        = 0.0f;
    m_angle     = 0.0f;
    m_visible   = true;
    m_animated  = false;
    m_exp0      = false;
    m_exp1      = false;
    m_priority  = 0;
    
    debug       = SPRITE_DBG_NONE;

    SetName(name);
    SetFrame(m_frame);
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSprite::~prSprite()
{
    PRSAFE_DELETE(m_name);
    PRSAFE_DELETE(m_animation);
}


/// ---------------------------------------------------------------------------
/// Updates the sprite.
/// ---------------------------------------------------------------------------
void prSprite::Update(float dt)
{
    if (m_animation && m_animated)
    {
        m_animation->Update(dt);
    }

    #if (defined(_DEBUG) || defined(DEBUG))
    if ((debug & SPRITE_DBG_SHOW_ANIM_FRAMES) == SPRITE_DBG_SHOW_ANIM_FRAMES)
    {
        PRLOGD("Frame '%i'\n", m_frame);
    }
    #endif
}


/// ---------------------------------------------------------------------------
/// Draws the sprite.
/// ---------------------------------------------------------------------------
void prSprite::Draw()
{
    PRASSERT(m_pTexture);
    if (m_visible)
    {
        glEnable(GL_BLEND);
        ERR_CHECK();

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        ERR_CHECK();

            glPushMatrix();
            ERR_CHECK();
            
            // translate to quad center, then translate for position
            //glTranslatef((float)(m_frameWidth >> 1), (float)(m_frameHeight >> 1), 0);
            //ERR_CHECK();
            //glTranslatef(pos.x, pos.y, 0);
            glTranslatef(pos.x + (float)(m_frameWidth >> 1), pos.y + (float)(m_frameHeight >> 1), 0);
            ERR_CHECK();

            float width  = (GLfloat)(m_frameWidth  * m_scaleX);
            float height = (GLfloat)(m_frameHeight * m_scaleY);

            // Set flips
            if ((m_flip & FLIP_LEFTRIGHT) == FLIP_LEFTRIGHT)
            {
                width = -width;
            }

            if ((m_flip & FLIP_UPDOWN) == FLIP_UPDOWN)
            {
                height = -height;
            }

            glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
            ERR_CHECK();
            glScalef(width, height, 0);
            ERR_CHECK();

            m_pTexture->Bind();

            PRASSERT(mpRenderer)
            mpRenderer->DrawQuad(m_u0, m_v0, m_u1, m_v1, m_colour);
            
            glPopMatrix();
            ERR_CHECK();

        glDisable(GL_BLEND);
        ERR_CHECK();
    }
}


/// ---------------------------------------------------------------------------
/// Draws the sprite.
/// ---------------------------------------------------------------------------
void prSprite::BatchDraw()
{
    PRASSERT(m_pTexture);
    if (m_visible)
    {
        // Render
        if (mpSpriteManager->BatchAvailable())
        {
            float width  = (GLfloat)(m_frameWidth  * m_scaleX);
            float height = (GLfloat)(m_frameHeight * m_scaleY);

            // Set flips
            if ((m_flip & FLIP_LEFTRIGHT) == FLIP_LEFTRIGHT)
            {
                width = -width;
            }

            if ((m_flip & FLIP_UPDOWN) == FLIP_UPDOWN)
            {
                height = -height;
            }

            //glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
            //ERR_CHECK();
            //glScalef(width, height, 0);
            //ERR_CHECK();

            f32 x = pos.x;
            f32 y = pos.y;

            width  /= 2;
            height /= 2;

            f32 colors[] =
            {
                m_colour.red, m_colour.green, m_colour.blue, m_colour.alpha,
                m_colour.red, m_colour.green, m_colour.blue, m_colour.alpha,
                m_colour.red, m_colour.green, m_colour.blue, m_colour.alpha,
                m_colour.red, m_colour.green, m_colour.blue, m_colour.alpha
            };

            QuadData quadData[] =
            {
                {x + -width, y +  height, m_u0, m_v0, },
                {x + -width, y + -height, m_u0, m_v1, },
                {x +  width, y +  height, m_u1, m_v0, },
                {x +  width, y + -height, m_u1, m_v1, },
            };

            mpSpriteManager->BatchAdd(colors, quadData);
        }
        else
        {
            glPushMatrix();
            ERR_CHECK();
            
                // translate to quad center, the translate for position
                //glTranslatef((float)(m_frameWidth >> 1), (float)(m_frameHeight >> 1), 0);
                //ERR_CHECK();
                glTranslatef(pos.x + (float)(m_frameWidth >> 1), pos.y + (float)(m_frameHeight >> 1), 0);
                ERR_CHECK();

                float width  = (GLfloat)(m_frameWidth  * m_scaleX);
                float height = (GLfloat)(m_frameHeight * m_scaleY);

                // Set flips
                if ((m_flip & FLIP_LEFTRIGHT) == FLIP_LEFTRIGHT)
                {
                    width = -width;
                }

                if ((m_flip & FLIP_UPDOWN) == FLIP_UPDOWN)
                {
                    height = -height;
                }

                glRotatef(m_rotation, 0.0f, 0.0f, 1.0f);
                ERR_CHECK();
                glScalef(width, height, 0);
                ERR_CHECK();

                // Render - Using old method
                PRASSERT(mpRenderer)
                mpRenderer->BatchDrawQuad(m_u0, m_v0, m_u1, m_v1, m_colour);
            
            glPopMatrix();
            ERR_CHECK();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Returns the sprites name.
/// ---------------------------------------------------------------------------
const char *prSprite::Name() const
{
    return m_name;
}


/// ---------------------------------------------------------------------------
/// Adds an animation sequence.
/// ---------------------------------------------------------------------------
void prSprite::AddSequence(prSpriteAnimationSequence* sequence, const char *name)
{
    PRASSERT(sequence);
    PRASSERT(name && *name);

    if (m_animation == NULL)
    {
        #if (defined(_DEBUG) || defined(DEBUG))
        if ((debug & SPRITE_DBG_SHOW_SEQ_NAMES) == SPRITE_DBG_SHOW_SEQ_NAMES)
        {
            PRLOGD("Add sequence: '%s' to '%s'\n", name, Name());
        }
        #endif

        m_animation = new prSpriteAnimation(this);
        m_animated  = true;
    }

    PRASSERT(m_animation);
    m_animation->AddSequence(sequence);
}


/// ---------------------------------------------------------------------------
/// Sets the frame.
/// ---------------------------------------------------------------------------
void prSprite::SetFrame(s32 frame)
{
    if (PRBETWEEN(frame, 0, m_framesTotal - 1))
    {
        // Set the frame source rect position indices
        s32 x = frame % m_framesAcross;
        s32 y = frame / m_framesAcross;

        m_u0 = x * m_fw;
        m_u1 = m_u0 + m_fw;

        m_v0 = 1.0f - ((y * m_fh) + m_fh);
        m_v1 = m_v0 + m_fh;

        // Left/right
        m_u0 += (m_pw / 2);             // Add half pixel to stop blurring.
        m_u1 -= (m_pw / 2);             // Sub half pixel to stop blurring.

        // Top/bottom
        m_v0 += (m_ph / 2);             // Add half pixel to stop blurring
        m_v1 -= (m_ph / 2);             // Sub half pixel to stop blurring

        // Set frame
        m_frame = frame;
    }
    #if (defined(_DEBUG) || defined(DEBUG))
    else
    {
        if ((debug & SPRITE_DBG_SHOW_ANIM_FRAMES) == SPRITE_DBG_SHOW_ANIM_FRAMES)
        {
            PRLOGE("Attempted to set an invalid sprite frame index %i. Texture: %s\n", frame, m_pTexture->Filename());
        }
    }
    #endif
}


/// ---------------------------------------------------------------------------
/// Play an animation.
/// ---------------------------------------------------------------------------
void prSprite::PlayAnim(const char* name)
{
    PRASSERT(name && *name);

    if (m_animation && m_animated)
    {
        m_animation->PlaySequence(name);

        #if (defined(_DEBUG) || defined(DEBUG))
        if ((debug & SPRITE_DBG_SHOW_ANIM_NAMES) == SPRITE_DBG_SHOW_ANIM_NAMES)
        {
            PRLOGD("Play anim '%s'\n", name);
        }
        #endif
    }
}


/// ---------------------------------------------------------------------------
/// Sets horizontal/vertical flips.
/// ---------------------------------------------------------------------------
void prSprite::SetFlip(u32 flip)
{
    m_flip |= flip;
}


/// ---------------------------------------------------------------------------
/// Clears horizontal/vertical flips.
/// ---------------------------------------------------------------------------
void prSprite::ClearFlip(u32 flip)
{
    PRCLEAR_FLAG(m_flip, flip);
}


/// ---------------------------------------------------------------------------
/// Set scale
/// ---------------------------------------------------------------------------
void prSprite::SetScale(f32 scale)
{
    m_scaleX = scale;
    m_scaleY = scale;
}


/// ---------------------------------------------------------------------------
/// Set scale
/// ---------------------------------------------------------------------------
void prSprite::SetScale(f32 scaleX, f32 scaleY)
{
    m_scaleX = scaleX;
    m_scaleY = scaleY;
}


/// ---------------------------------------------------------------------------
/// Get scale
/// ---------------------------------------------------------------------------
void prSprite::GetScale(f32 &x, f32 &y) const
{
    x = m_scaleX;
    y = m_scaleY;
}


/// ---------------------------------------------------------------------------
/// Get scale
/// ---------------------------------------------------------------------------
f32 prSprite::GetScale() const
{
    return ((m_scaleX + m_scaleY) / 2);
}


/// ---------------------------------------------------------------------------
/// Get scale
/// ---------------------------------------------------------------------------
f32 prSprite::GetScaleX() const
{
    return m_scaleX;
}


/// ---------------------------------------------------------------------------
/// Get scale
/// ---------------------------------------------------------------------------
f32 prSprite::GetScaleY() const
{
    return m_scaleY;
}


///// ---------------------------------------------------------------------------
///// Set rotation
///// ---------------------------------------------------------------------------
//void prSprite::SetRotation(f32 rot)
//{
//    m_rotation = rot;
//}
//
//
///// ---------------------------------------------------------------------------
///// Get rotation
///// ---------------------------------------------------------------------------
//f32 prSprite::GetRotation() const
//{
//    return m_rotation;
//}


/// ---------------------------------------------------------------------------
/// User data support.
/// ---------------------------------------------------------------------------
void prSprite::SetUser(s32 index, s32 value)
{
    PRASSERT(PRBETWEEN(index, 0, MAX_USER_VALUES));
    m_userData[index] = value;
}


/// ---------------------------------------------------------------------------
/// User data support.
/// ---------------------------------------------------------------------------
s32 prSprite::GetUser(s32 index) const
{
    PRASSERT(PRBETWEEN(index, 0, MAX_USER_VALUES));
    return m_userData[index];
}


/// ---------------------------------------------------------------------------
/// Gets the user data for the current frame.
/// ---------------------------------------------------------------------------
s32 prSprite::GetUserDataForCurrentFrame(s32 index) const
{
    if (m_animation)
    {
        return m_animation->GetUserDataForCurrentFrame(index);
    }

    return -1;
}


/// ---------------------------------------------------------------------------
/// Is this sprite animated?
/// ---------------------------------------------------------------------------
bool prSprite::IsAnimated() const
{
    bool result = false;

    if (m_animation && m_animated)
    {
        result = true;
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Has the current animation stopped.
/// ---------------------------------------------------------------------------
bool prSprite::HasAnimationStopped() const
{
    bool result = false;

    if (m_animation && m_animated)
    {
        result = m_animation->HasAnimationStopped();
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Has any animation been played and animation is not in its default state.
/// ---------------------------------------------------------------------------
bool prSprite::HasAnimationStarted() const
{
    bool result = false;

    if (m_animation && m_animated)
    {
        result = m_animation->HasAnimationStarted();
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Sets the tint colour.
/// ---------------------------------------------------------------------------
void prSprite::SetColour(prColour col)
{
    m_colour = col;
}


/// ---------------------------------------------------------------------------
/// Sets the animations name.
/// ---------------------------------------------------------------------------
void prSprite::SetName(const char *spriteName)
{
    PRASSERT(spriteName && *spriteName);
    int len = (int)(strlen(spriteName) + 1);
    m_name = new char[len];
    strcpy(m_name, spriteName);
}
