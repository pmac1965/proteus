// File: prSpriteAnimation.h
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


#pragma once


#include <list>
#include "../core/prTypes.h"


// Enum: prAnimState
//      The status of the animation.
//
// ANIM_STATE_NONE        - This is used when no animation exists for a sprite.
// ANIM_STATE_PLAYING     - The animation is playing.
// ANIM_STATE_STOPPED     - The animation has stopped.
// ANIM_STATE_PAUSED      - The animation playback has been paused.
enum prAnimState
{
    ANIM_STATE_NONE,   
    ANIM_STATE_PLAYING,
    ANIM_STATE_STOPPED,
    ANIM_STATE_PAUSED  
};


// Enum: prAnimDir
//      Direction that the animation plays in.
//
// ANIM_DIR_NONE           - Used when sprite is not animated.
// ANIM_DIR_FORWARD        - Indicates frame numbers go up on frame change.
// ANIM_DIR_BACKWARD       - Indicates frame numbers go down on frame change.
enum prAnimDir
{
    ANIM_DIR_NONE,   
    ANIM_DIR_FORWARD,
    ANIM_DIR_BACKWARD
};


// Enum: prAnimType
//      The animation type determines what happens when the frame bypasses the start/end frame.
//
// ANIM_TYPE_NONE          - Used when sprite is not animated.
// ANIM_TYPE_ONCE          - When we hit the last frame we turn off the animation.
// ANIM_TYPE_LOOP          - When we hit the last frame we restart the animation.
// ANIM_TYPE_YOYO          - When we hit the first/last frame we reverse the animation.
enum prAnimType
{
    ANIM_TYPE_NONE,
    ANIM_TYPE_ONCE,
    ANIM_TYPE_LOOP,
    ANIM_TYPE_YOYO 
};


// Forward references.
class prSprite;
class prSpriteAnimationSequence;


// Class: prSpriteAnimation
//      Class to handle sprite animation.
class prSpriteAnimation
{
public:

    // Method: prSpriteAnimation
    //      Constructor.
    //
    // Parameters:
    //      sprite - The parent sprite
    explicit prSpriteAnimation(prSprite *sprite);

    // Method: ~prSpriteAnimation
    //      Destructor.
    ~prSpriteAnimation();

    // Method: Update
    //      Updates the sprites animation.
    //
    // Parameters:
    //      dt - Delta time
    void Update(float dt);

    // Method: AddSequence
    //      Adds an animation sequence.
    //
    // Parameters:
    //      sequence - An animation sequence
    void AddSequence(prSpriteAnimationSequence* sequence);

    // Method: PlaySequence
    //      Plays a named animation sequence.
    //
    // Parameters:
    //      name - Name of the sequence to play
    void PlaySequence(const char *name);

    // Method: GetUserDataForCurrentFrame
    //      Gets the user data for the current frame.
    //
    // Parameters:
    //      index - Index of the user data for this frame (0 to 3)
    //
    // Returns:
    //      The user data for a specific frame
    Proteus::Core::s32 GetUserDataForCurrentFrame(Proteus::Core::s32 index) const;

    // Method: HasAnimationStopped
    //      Has the current animation stopped.
    bool HasAnimationStopped() const;

    // Method: HasAnimationStarted
    //      Has any animation been played and animation is not in its default state.
    bool HasAnimationStarted() const;


private:

    prSprite                               &m_sprite;         // Reference to the parent sprite.
    std::list<prSpriteAnimationSequence*>   m_sequences;      // List of the animation sequences.
    prSpriteAnimationSequence              *m_currSequence;   // The current animation sequence.


private:
    
    // Stops passing by value and assignment.
    prSpriteAnimation(const prSpriteAnimation&);
    const prSpriteAnimation& operator = (const prSpriteAnimation&);
};
