// File: prSpriteAnimationSequence.h
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


#include "../core/prTypes.h"


// Defines
#define MAX_USER_DATA  4


// Struct: prAnimSequenceData
//      Stores the actual animation data for a particular frame.
typedef struct prAnimSequenceData
{
    // Method: prAnimSequenceData
    //      Default constructor.
    prAnimSequenceData()
    {
        index = 0;
        delay = 0;

        for (int i=0; i<MAX_USER_DATA; i++)
        {
            userData[i] = -1;
        }
    }

        
    Proteus::Core::s32 index;                      // Frame index.
    Proteus::Core::f32 delay;                      // delay before showing next frame. (In milliseconds)
    Proteus::Core::s32 userData[MAX_USER_DATA];    // User data for a frame.

} prAnimSequenceData;


// Forward references
class TiXmlElement;


// Class: prSpriteAnimationSequence
//      Represents a single animation sequence.
class prSpriteAnimationSequence
{
public:

    // Method: prSpriteAnimationSequence
    //      Ctor
    //
    // Parameters:
    //      name   - The name of this sequence. Must be unique
    //      frames - The number of frames in this animation
    //      type   - The animation type
    //
    // See Also:
    //      <prAnimType>
    prSpriteAnimationSequence(const char *name, Proteus::Core::s32 frames, Proteus::Core::s32 type);

    // Method: ~prSpriteAnimationSequence
    //      Dtor
    ~prSpriteAnimationSequence();

    // Method: Animate
    //      Animates a sprite provided that it is not paused.
    //
    // Parameters:
    //      dt - Delta time
    void Animate(float dt);
    
    // Method: Begin
    //      Starts the animation sequence.
    void Begin();

    // Method: GetHash
    //      Returns the animations sequences hashed name.
    Proteus::Core::u32 GetHash() const;

    // Method: GetFrame
    //      Returns the current frame.
    Proteus::Core::s32 GetFrame() const;

    // Method: ParseFrameData
    //      Parses the animation data from an xml file.
    //
    // Parameters:
    //      pElement - The element to parse
    void ParseFrameData(TiXmlElement* pElement);

    // Method: GetUserDataForCurrentFrame
    //      Gets the user data for the current frame.
    //
    // Parameters:
    //      index - 0 to 3
    //
    // Returns:
    //      The value or -1 if value doesn't exist
    Proteus::Core::s32 GetUserDataForCurrentFrame(Proteus::Core::s32 index) const;

    // Method: GetAnimState
    //      Returns the animations state.
    //
    // See Also:
    //      <prAnimState>
    Proteus::Core::s32 GetAnimState() const;


private:

    // Data
    char               *m_pName;                            // Name of this sequence.
    prAnimSequenceData *m_pData;                            // The sequence data.
    Proteus::Core::s32  m_count;                            // The number of frames in the sequence.
    Proteus::Core::u32  m_hash;                             // The hashed name of this anim sequence.
    Proteus::Core::s32  m_animationType;                    // The animation type. Defaults to none.
    float               m_frameDelay;                       // Delay between frames.
    Proteus::Core::s32  m_frameIndex;                       // The current frame index.
    Proteus::Core::s32  m_currentFrame;                     // The current frame/delay.
    Proteus::Core::s32  m_state;                            // The state of the animation playback. 
    Proteus::Core::s32  m_dir;                              // The animation direction.


private:
    
    // Stops passing by value and assignment.
    prSpriteAnimationSequence(const prSpriteAnimationSequence&);
    const prSpriteAnimationSequence& operator = (const prSpriteAnimationSequence&);
};
