/**
 * @file       prSpriteAnimation.h
 * @brief      Contains types for sprite animation support.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
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


#ifndef __PRSPRITEANIMATION_H
#define __PRSPRITEANIMATION_H


#include <list>
#include "../core/prTypes.h"


/// @enum       prAnimState
/// @brief      The status of the animation.
enum prAnimState
{
    ANIM_STATE_NONE,            ///< This is used when no animation exists for a sprite.
    ANIM_STATE_PLAYING,         ///< The animation is playing.
    ANIM_STATE_STOPPED,         ///< The animation has stopped.
    ANIM_STATE_PAUSED           ///< The animation playback has been paused.
};


/// @enum       prAnimDir
/// @brief      Direction that the animation plays in.
enum prAnimDir
{
    ANIM_DIR_NONE,              ///< Used when sprite is not animated.
    ANIM_DIR_FORWARD,           ///< Indicates frame numbers go up on frame change.
    ANIM_DIR_BACKWARD           ///< Indicates frame numbers go down on frame change.
};


/// @enum       prAnimType
/// @brief      The animation type determines what happens when the frame bypasses the start/end frame.
enum prAnimType
{
    ANIM_TYPE_NONE,             ///< Used when sprite is not animated.
    ANIM_TYPE_ONCE,             ///< When we hit the last frame we turn off the animation.
    ANIM_TYPE_LOOP,             ///< When we hit the last frame we restart the animation.
    ANIM_TYPE_YOYO              ///< When we hit the first/last frame we reverse the animation.
};


// Forward references.
class prSprite;
class prSpriteAnimationSequence;


/// @class      prSpriteAnimation
/// @brief      Class to handle sprite animation.
class prSpriteAnimation
{
public:

    /// @brief      Constructor.
    /// @param      sprite - The parent sprite
    prSpriteAnimation(prSprite *sprite);

    /// @brief      Destructor.
    ~prSpriteAnimation();

    /// @brief      Updates the sprites animation.
    /// @param      dt - Delta time
    void Update(float dt);

    /// @brief      Adds an animation sequence.
    /// @param      sequence - An animation sequence
    void AddSequence(prSpriteAnimationSequence* sequence);

    /// @brief      Plays a named animation sequence.
    /// @param      name - Name of the sequence to play
    void PlaySequence(const char *name);

    /// @brief      Gets the user data for the current frame.
    /// @param      index - Index of the user data for this frame (0 to 3)
    /// @return     The user data for a specific frame
    s32 GetUserDataForCurrentFrame(s32 index);

    /// @brief      Has the current animation stopped.
    /// @return     true or false
    bool HasAnimationStopped();

    /// @brief      Has any animation been played and animation is not in its default state.
    /// @return     true or false
    bool HasAnimationStarted();


private:

    prSprite                               &m_sprite;         ///< Reference to the parent sprite.
    std::list<prSpriteAnimationSequence*>   m_sequences;      ///< List of the animation sequences.
    prSpriteAnimationSequence              *m_currSequence;   ///< The current animation sequence.


private:
    
    // Stops passing by value and assignment.
    prSpriteAnimation(const prSpriteAnimation&);
    const prSpriteAnimation& operator = (const prSpriteAnimation&);
};


#endif//__PRSPRITEANIMATION_H
