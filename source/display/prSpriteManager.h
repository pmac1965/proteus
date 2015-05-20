// File: prSpriteManager.h
// About:
//      This system class is the only way to correctly create a sprite and
//
//      assign the animation data to that sprite. This class uses the system
//
//      resource manager to ensure textures are only ever loaded once
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


// Struct: prActiveSprite
//      Active sprite
typedef struct prActiveSprite
{
    prSprite  *sprite;            // The sprite.
    prTexture *texture;           // The sprites texture.
    bool       draw;              // Is the sprite manager responsible for drawing?

} prActiveSprite;


// Class: prSpriteManager
//      Manages all sprite creation and destruction.
class prSpriteManager : public prCoreSystem
{
public:

    // Method: prSpriteManager
    //      Constructs the sprite manager.
    prSpriteManager();

    // Method: ~prSpriteManager
    //      Destructor.
    ~prSpriteManager();

    // Method: Update
    //      Updates all the active sprites.
    //
    // Parameters:
    //      dt - Delta time
    void Update(Proteus::Core::f32 dt);

    // Method: Draw
    //      Draws all the visible sprites.
    //
    // Notes:
    //      This is an optional call.
    void Draw();

    // Method: Create
    //      Creates a sprite.
    //
    // Parameters:
    //      filename - The sprite filename
    //      draw     - Should the sprite manager draw routine draw this sprite?
    //
    // Returns:
    //      Returns the created sprite or NULL on failure
    prSprite *Create(const char *filename, bool draw = true);

//#if defined(PROTEUS_TOOL) || defined(PLATFORM_PC)
    // Method: ToolCreate
    //      Creates a sprite.
    //
    // Parameters:
    //      pTex   - A texture pointer
    //      width  - Frame width
    //      height - Frame height
    //
    // Returns:
    //      Returns the created sprite or NULL on failure
    // 
    // Notes:
    //      This call only exists in the tool build
    prSprite *ToolCreate(prTexture *pTex, Proteus::Core::s32 width, Proteus::Core::s32 height);

    // Method: ToolRelease
    //      Releases a sprite and any asssociated assets.
    //
    // Parameters:
    //      sprite - The sprite to release
    // 
    // Notes:
    //      This call only exists in the tool build
    void ToolRelease(prSprite *sprite);
//#endif

    // Method: Release
    //      Releases a sprite and any asssociated assets.
    //
    // Parameters:
    //      sprite - The sprite to release
    void Release(prSprite *sprite);
    
    // Method: ReleaseAll
    //      Releases all sprites and their asssociated assets. 
    void ReleaseAll();

    // Method: DisplayUsage
    //      Debug assist code to show all sprites.
    void DisplayUsage();

    // Method: BatchBegin
    //      Starts batch sprite rendering
    void BatchBegin();
    
    // Method: BatchEnd
    //      Ends batch sprite rendering
    void BatchEnd();


private:

    // Loads a sprite file.
    //
    //      filename - Name of the file to load
    bool Load(const char *filename);

    // Parses a sprite file.
    //
    //      pParent - Pointer to a parent XML node
    void ParseSpriteFile(TiXmlNode* pParent);

    // Parses a sprite file.
    //
    //      pElement - Pointer to an XML element
    void ParseAttribs_SpriteFile(TiXmlElement* pElement);

    // Parses a sprite file.
    //
    //      pElement - Pointer to an XML element
    void ParseAttribs_Sprite(TiXmlElement* pElement);

    // Parses a sprite file.
    //
    //      pElement - Pointer to an XML element
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
