/**
 * @file        prSpriteManager.h
 * @brief       Contains the sprite manager class.
 * @copyright   Copyright Paul Michael McNab. All rights reserved.
 * @note        This system class is the only way to correctly create a sprite and
 * @n           assign the animation data to the sprite. This class uses the system
 * @n           resource manager to ensure textures are only ever loaded once
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


#ifndef __PRSPRITEMANAGER_H
#define __PRSPRITEMANAGER_H


#include <list>
#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations.
class prSprite;
class TiXmlNode;
class TiXmlElement;
class prTexture;



// Active sprite
typedef struct prActiveSprite
{
    prSprite  *sprite;            // The sprite.
    prTexture *texture;           // The sprites texture.
    bool       draw;              // Is the sprite manager responsible for drawing?

} prActiveSprite;


/**
 * Manages all sprite creation and destruction.
 */
class prSpriteManager : public prCoreSystem
{
public:

    /**
     * Constructs the sprite manager.
     */
    prSpriteManager();

    /**
     * Destructor.
     */
    ~prSpriteManager();

    /**
     * Updates all the active sprites.
     *
     * @param   dt - Delta time
     */
    void Update(f32 dt);

    /** 
     * Draws all the visible sprites.
     *
     * @note    This is an optional call.
     */
    void Draw();

    /**
     * Creates a sprite.
     *
     * @param   filename - The sprite filename
     * @param   draw     - Should the sprite manager draw routine draw this sprite?
     *
     * @return  Returns the created sprite or NULL on failure
     */
    prSprite *Create(const char *filename, bool draw = true);

#if defined(PROTEUS_TOOL) || defined(PLATFORM_PC)
    /**
     * Creates a sprite.
     *
     * @param   pTex   - A texture pointer
     * @param   width  - Frame width
     * @param   height - Frame height
     *
     * @return  Returns the created sprite or NULL on failure
     * 
     * @note    This call only exists in the tool build
     */
    prSprite *ToolCreate(prTexture *pTex, s32 width, s32 height);

    /**
     * Releases a sprite and any asssociated assets.
     *
     * @param   sprite - The sprite to release
     * 
     * @note    This call only exists in the tool build
     */
    void ToolRelease(prSprite *sprite);
#endif

    /**
     * Releases a sprite and any asssociated assets.
     *
     * @param   sprite - The sprite to release
     */
    void Release(prSprite *sprite);
    
    /**
     * Releases all sprites and their asssociated assets. 
     */
    void ReleaseAll();

    /**
     * Debug assist code to show all sprites.
     */
    void DisplayUsage();

    /**
     * Starts batch sprite rendering
     */
    void BatchBegin();
    
    /**
     * Ends batch sprite rendering
     */
    void BatchEnd();


private:

    /**
     * Loads a sprite file.
     *
     * @param   filename - Name of the file to load
     *
     * @return  true if loaded, false otherwise
     */
    bool Load(const char *filename);

    /**
     * Parses a sprite file.
     *
     * @param   pParent - Pointer to a parent XML node
     */
    void ParseSpriteFile(TiXmlNode* pParent);

    /**
     * Parses a sprite file.
     *
     * @param   pElement - Pointer to an XML element
     */
    void ParseAttribs_SpriteFile(TiXmlElement* pElement);

    /**
     * Parses a sprite file.
     *
     * @param   pElement - Pointer to an XML element
     */
    void ParseAttribs_Sprite(TiXmlElement* pElement);

    /**
     * Parses a sprite file.
     *
     * @param   pElement - Pointer to an XML element
     */
    void ParseAttribs_Sequence(TiXmlElement* pElement);


private:

    // Stops passing by value and assignment.
    prSpriteManager(const prSpriteManager&);
    const prSpriteManager& operator = (const prSpriteManager&);


private:

    bool                        m_correctFileType;
    bool                        m_exp0;
    bool                        m_exp1;
    bool                        m_exp2;
    prSprite                   *m_sprite;
    prTexture                  *m_texture;
    std::list<prActiveSprite>   m_activeSprites;
};


#endif//__PRSPRITEMANAGER_H
