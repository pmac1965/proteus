/**
 * @file       prSprite.h
 * @brief      Contains an animated sprite class.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 * @note       A sprite must be created and destroyed using the sprite manager, as
 *             they cannot be used alone
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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


/// Basic 2D sprite entity
class prSprite
{
public:

    /// @enum       prFlipType
    /// @brief      Sprite bitfield flip flags
    /// @see        SetFlip
    /// @see        ClearFlip
    enum prFlipType
    {
        FLIP_NONE      = 0x00000000,        ///< No flips. Default value
        FLIP_LEFTRIGHT = 0x00000001,        ///< Flips sprite on the X axis
        FLIP_UPDOWN    = 0x00000020,        ///< Flips sprite on the Y axis
    };


public:

    /// @brief      Updates the sprites animation.
    /// @param      dt - Delta time
    void Update(float dt);

    /// @brief      Draws the sprite.
    /// @note       This is the normal version used most often
    void Draw();

    /// @brief       Draws the sprite (Batch call)
    /// @note        Batch draw version. Does less state switching
    void BatchDraw();

    /// @brief      Gets the sprite visible state.
    /// @return     true or false
    bool GetVisible() const;

    /// @brief      Sets the sprite visible state.
    /// @param      value - true or false
    void SetVisible(bool value);

    /// @brief      Returns the sprite frame width.
    /// @return     Returns the width of the frame
    s32 GetFrameWidth() const;

    /// @brief      Returns the sprite frame height.
    /// @return     Returns the height of the frame
    s32 GetFrameHeight() const;

    /// @brief      Returns the sprites name.
    /// @return     Returns the sprites name.
    const char *Name() const;

    /// @brief      Sets the frame.
    /// @param      frame - The frame to set
    /// @note       Must be between 0 and the max frame count
    void SetFrame(s32 frame);

    /// @brief      Gets the frame.
    /// @return     Returns the current frame
    s32 GetFrame() const;

    /// @brief      Play an animation.
    /// @param      name - The animations name
    void PlayAnim(const char* name);

    /// @brief      Sets horizontal/vertical flips.
    /// @param      flip - the flips to set
    /// @note       flip is or'd onto flip mask use ClearFlip to remove flips
    /// @see        ClearFlip
    void SetFlip(u32 flip);

    /// @brief      Clears horizontal/vertical flips.
    /// @param      flip - the flips to remove
    /// @see        SetFlip
    void ClearFlip(u32 flip);

    /// @brief      Set scale
    /// @param      scale - The scale to set
    void SetScale(f32 scale);

    /// @brief      Set scale
    /// @param      scaleX - The scale to set
    /// @param      scaleY - The scale to set
    void SetScale(f32 scaleX, f32 scaleY);

    /// @brief      Set scale
    /// @param      x - Receives the X scale
    /// @param      y - Receives the Y scale
    void GetScale(f32 &x, f32 &y) const;
    
    /// @brief      Get scale
    /// @return     Returns the scale
    f32 GetScale() const;

    /// @brief      Get scale
    /// @return     Returns the scale X component
    f32 GetScaleX() const;

    /// @brief      Get scale
    /// @return     Returns the scale Y component
    f32 GetScaleY() const;

    /// @brief      Sets rotation
    /// @param      rot - The rotation value
    void SetRotation(f32 rot);

    /// @brief      Gets rotation
    /// @return     Returns the current rotation
    f32 GetRotation() const;

    /// @brief      User data support.
    /// @param      index - 0 to 3
    /// @param      value - Required value
    void SetUser(s32 index, s32 value);

    /// @brief      User data support.
    /// @param      index - 0 to 3
    /// @return     Returns a user value
    s32 GetUser(s32 index) const;

    /// @brief      Gets the user data for the current frame.
    /// @param      index - The frame index
    /// @return     Returns the value or -1 if it doesn't exist
    s32 GetUserDataForCurrentFrame(s32 index);

    /// @brief      Is this sprite animated?
    /// @return     true or false
    bool IsAnimated() const;

    /// @brief      Has the current animation stopped.
    /// @return     true or false
    bool HasAnimationStopped();

    /// @brief      Has any animation been played and animation is not in its default state.
    /// @return     true or false
    bool HasAnimationStarted();

    /// @brief      Sets the tint colour.
    /// @param      col - The colour
    void SetColour(prColour col);

    /// @brief      Is sprite in a sprite pool
    /// @return     true or false
    bool InPool() const { return (pool != -1); }

    /// @brief      Pool index
    /// @return     The index of the sprite pool
    s32 PoolIndex() const { return pool; }


public:

    prVector2 pos;          ///< Sprite position.

    s32     user0;          ///< This is free for game programmer use.
    s32     user1;          ///< This is free for game programmer use.
    s32     user2;          ///< This is free for game programmer use.
    s32     user3;          ///< This is free for game programmer use.


private:

    s32     pool;           ///< Sprite pool index


private:

    // Friends
    friend class prSpriteManager;
    //friend class SpritePointerPool;


    /// @brief      Ctor.
    /// @param      pTexture    - The sprites texture
    /// @param      name        - The sprites name
    /// @param      frameWidth  - The sprites width
    /// @param      frameHeight - The sprites height
    prSprite(prTexture *pTexture, const char *name, s32 frameWidth, s32 frameHeight);

    /// @brief      Dtor.
    ~prSprite();


private:

    /// @brief      Sets the sprite name
    /// @param      spriteName  - The name
    void SetName(const char *spriteName);

    /// @brief      Adds an animation sequence.
    /// @param      sequence    - The sequence to add
    /// @param      name        - The sequence name
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
