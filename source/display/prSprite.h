// File: prSprite.h
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


#ifndef __PRSPRITE_H
#define __PRSPRITE_H


#include "prColour.h"
#include "../core/prTypes.h"
#include "../math/prVector2.h"


// Forward declarations
class  prTexture;
class  prSpriteAnimationSequence;
class  prSpriteAnimation;


// Defines
#define MAX_USER_VALUES     4


// Class: prSprite
//      Basic 2D sprite entity
//
// Notes:
//      A sprite must be created and destroyed using the sprite manager, as
//      they cannot be used alone
class prSprite
{
public:
    // enum: prFlipType
    //      Sprite bitfield flip flags
    //
    // See Also:
    //      <SetFlip>
    //
    // See Also:
    //      <ClearFlip>
    enum prFlipType
    {
        FLIP_NONE      = 0x00000000,        ///< No flips. Default value
        FLIP_LEFTRIGHT = 0x00000001,        ///< Flips sprite on the X axis
        FLIP_UPDOWN    = 0x00000020,        ///< Flips sprite on the Y axis
    };


public:
    // Method: Update
    //      Updates the sprites animation.
    //
    // Parameters:
    //      dt - Delta time
    void Update(float dt);

    // Method: Draws
    //      Draws the sprite.
    //
    // Notes:
    //      This is the normal version used most often
    void Draw();

    // Method: BatchDraw
    //      Draws the sprite (Batch call)
    //
    // Notes:
    //      Batch draw version. Does less state switching
    void BatchDraw();

    // Method: GetVisible
    //      Gets the sprite visible state.
    bool GetVisible() const;

    // Method: SetVisible
    //      Sets the sprite visible state.
    //
    // Parameters:
    //      value - true or false
    void SetVisible(bool value);

    // Method: GetFrameWidth
    //      Returns the sprite frame width.
    s32 GetFrameWidth() const;

    // Method: GetFrameHeight
    //      Returns the sprite frame height.
    s32 GetFrameHeight() const;

    // Method: Name
    //      Returns the sprites name.
    const char *Name() const;

    // Method: SetFrame
    //      Sets the frame.
    //
    // Parameters:
    //      frame - The frame to set
    //
    // Notes:
    //      Must be between 0 and the max frame count
    void SetFrame(s32 frame);

    // Method: GetFrame
    //      Gets the current frame.
    s32 GetFrame() const;

    // Method: PlayAnim
    //      Play an animation.
    //
    // Parameters:
    //      name - The animations name
    void PlayAnim(const char* name);

    // Method: SetFlip
    //       Sets horizontal/vertical flips.
    //
    // Parameters:
    //      flip - the flips to set
    //
    // Notes:
    //      flip is or'd onto flip mask use ClearFlip to remove flips
    //
    // See Also:
    //      <ClearFlip>
    void SetFlip(u32 flip);

    // Method: ClearFlip
    //      Clears horizontal/vertical flips.
    //
    // Parameters:
    //      flip - the flips to remove
    //
    // See Also:
    //      <SetFlip>
    void ClearFlip(u32 flip);

    // Method: SetScale
    //      Set scale
    //
    // Parameters:
    //      scale - The scale to set
    void SetScale(f32 scale);

    // Method: SetScale
    //      Set scale
    //
    // Parameters:
    //      scaleX - The scale to set
    //      scaleY - The scale to set
    void SetScale(f32 scaleX, f32 scaleY);

    // Method: GetScale
    //      Get scale
    //
    // Parameters:
    //      x - Receives the X scale
    //      y - Receives the Y scale
    void GetScale(f32 &x, f32 &y) const;
    
    // Method: GetScale
    //      Get scale
    f32 GetScale() const;

    // Method: GetScaleX
    //      Returns the scale X component
    f32 GetScaleX() const;

    // Method: GetScaleY
    //      Returns the scale Y component
    f32 GetScaleY() const;

    // Method: SetRotation
    //      Sets rotation
    //
    // Parameters:
    //      rot - The rotation value
    void SetRotation(f32 rot);

    // Method: GetRotation
    //      Returns the current rotation
    f32 GetRotation() const;

    // Method: SetUser
    //      User data support.
    //
    // Parameters:
    //      index - 0 to 3
    //      value - Required value
    void SetUser(s32 index, s32 value);

    // Method: GetUser
    //      User data support.
    //
    // Parameters:
    //      index - 0 to 3
    //
    // Returns:
    //      Returns a user value
    s32 GetUser(s32 index) const;

    // Method: GetUserDataForCurrentFrame
    //      Gets the user data for the current frame.
    //
    // Parameters:
    //      index - The frame index
    //
    // Returns:
    //      Returns the value or -1 if it doesn't exist
    s32 GetUserDataForCurrentFrame(s32 index) const;

    // Method: IsAnimated
    //      Is this sprite animated?
    bool IsAnimated() const;

    // Method: HasAnimationStopped
    //      Has the current animation stopped.
    bool HasAnimationStopped() const;

    // Method: HasAnimationStarted
    //      Has any animation been played and animation is not in its default state.
    bool HasAnimationStarted() const;

    // Method: SetColour
    //      Sets the tint colour.
    //
    // Parameters:
    //      col - The colour
    void SetColour(prColour col);

    // Method: InPool
    //      Is sprite in a sprite pool?
    bool InPool() const { return (pool != -1); }

    // Method: PoolIndex
    //      Returns pool index
    s32 PoolIndex() const { return pool; }


public:

    prVector2 pos;          // Sprite position.

    s32     user0;          // This is free for game programmer use.
    s32     user1;          // This is free for game programmer use.
    s32     user2;          // This is free for game programmer use.
    s32     user3;          // This is free for game programmer use.


private:

    s32     pool;           // Sprite pool index


private:

    // Friends
    friend class prSpriteManager;
    friend class prSpritePointerPool;


    // Ctor
    //      pTexture    - The sprites texture
    //      name        - The sprites name
    //      frameWidth  - The sprites width
    //      frameHeight - The sprites height
    prSprite(prTexture *pTexture, const char *name, s32 frameWidth, s32 frameHeight);

    // Dtor
    ~prSprite();


private:

    // Sets the sprite name
    //      spriteName  - The name
    void SetName(const char *spriteName);

    // Adds an animation sequence.
    //      sequence    - The sequence to add
    //      name        - The sequence name
    void AddSequence(prSpriteAnimationSequence* sequence, const char *name);


private:

    // Stops passing by value and assignment.
    prSprite(const prSprite&);
    const prSprite& operator = (const prSprite&);


private:

    prTexture          *m_pTexture;
    prSpriteAnimation  *m_animation;
    char               *m_name;
    
    s32                 m_frameWidth;
    s32                 m_frameHeight;
    s32                 m_framesAcross;
    s32                 m_framesDown;
    s32                 m_framesTotal;
    s32                 m_frame;
    u32                 m_flip;

    s32                 m_userData[MAX_USER_VALUES];

    f32                 m_scaleX;
    f32                 m_scaleY;
    f32                 m_rotation;
    f32                 m_u0;
    f32                 m_u1;
    f32                 m_v0;
    f32                 m_v1;
    f32                 m_fw;             // Frame width  (UV coords)
    f32                 m_fh;             // Frame height (UV coords)
    f32                 m_angle;

    bool                m_animated;
    bool                m_visible;
    bool                m_exp0;
    bool                m_exp1;

    prColour            m_colour;
};


#endif//__PRSPRITE_H
