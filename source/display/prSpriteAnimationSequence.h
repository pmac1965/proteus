/**
 * @file       prSpriteAnimationSequence.h
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


#ifndef __PRSPRITEANIMATIONSEQUENCE_H
#define __PRSPRITEANIMATIONSEQUENCE_H


#include "../core/prTypes.h"


// Defines
#define MAX_USER_DATA  4


/// @struct     prAnimSequenceData
/// @brief      Stores the actual animation data for a particular frame.
typedef struct prAnimSequenceData
{
    /// @brief      Default constructor.
    /// @note       Called by default          
    prAnimSequenceData()
    {
        index = 0;
        delay = 0;

        for (int i=0; i<MAX_USER_DATA; i++)
        {
            userData[i] = -1;
        }
    }

        
    s32     index;                      ///< Frame index.
    float   delay;                      ///< delay before showing next frame. (In milliseconds)
    s32     userData[MAX_USER_DATA];    ///< User data for a frame.

} prAnimSequenceData;


// Forward references
class TiXmlElement;


/// @class      prSpriteAnimationSequence
/// @brief      Represents a single animation sequence.
class prSpriteAnimationSequence
{
public:

    /**
     * Ctor.
     *
     * @param      name   - The name of this sequence. Must be unique
     * @param      frames - The number of frames in this animation
     * @param      type   - The animation type
     *
     * @see        prAnimType
     */
    prSpriteAnimationSequence(const char *name, s32 frames, s32 type);

    /**
     * Dtor
     */
    ~prSpriteAnimationSequence();

    /// @brief      Animates a sprite provided that it is not paused.
    /// @param      dt - Delta time
    void Animate(float dt);
    
    /** 
     * Starts the animation sequence.
     */
    void Begin();

    /// @brief      Returns the animations sequences hashed name.
    /// @return     Returns the animations sequences hashed name.
    u32 GetHash() const;

    /// @brief      Returns the current frame.
    /// @return     Returns the current frame.
    s32 GetFrame() const;

    /// @brief      Parses the animation data from an xml file.
    /// @param      pElement - The element to parse
    void ParseFrameData(TiXmlElement* pElement);

    /// @brief      Gets the user data for the current frame.
    /// @param      index - 0 to 3
    /// @returns    The value or -1 if value doesn't exist
    s32 GetUserDataForCurrentFrame(s32 index);

    /// @brief      Returns the animations state.
    /// @return     Returns the animations state.
    /// @see        prAnimState
    s32 GetAnimState() const;


private:

    // Data
    char               *m_pName;                            ///< Name of this sequence.
    prAnimSequenceData *m_pData;                            ///< The sequence data.
    s32                 m_count;                            ///< The number of frames in the sequence.
    u32                 m_hash;                             ///< The hashed name of this anim sequence.
    s32                 m_animationType;                    ///< The animation type. Defaults to none.
    float               m_frameDelay;                       ///< Delay between frames.
    s32                 m_frameIndex;                       ///< The current frame index.
    s32                 m_currentFrame;                     ///< The current frame/delay.
    s32                 m_state;                            ///< The state of the animation playback. 
    s32                 m_dir;                              ///< The animation direction.


private:
    
    // Stops passing by value and assignment.
    prSpriteAnimationSequence(const prSpriteAnimationSequence&);
    const prSpriteAnimationSequence& operator = (const prSpriteAnimationSequence&);
};


#endif//__PRSPRITEANIMATIONSEQUENCE_H
