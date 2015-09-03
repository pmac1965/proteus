/**
 * prSpriteManager.cpp
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


// ----------------------------------------------------------------------------
// Platform specifics
// ----------------------------------------------------------------------------
#if defined(PLATFORM_PC)
  #include <Windows.h>
  #include <gl/gl.h>
  #include <gl/glu.h>

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>
  #include <string.h>

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>
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


#include "prSpriteManager.h"
#include "prSprite.h"
#include "prSpriteAnimation.h"
#include "prSpriteAnimationSequence.h"
#include "prOglUtils.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prResourceManager.h"
#include "../core/prStringUtil.h"
#include "../debug/prAssert.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../display/prRenderer.h"
#include "../display/prTexture.h"
#include "../tinyxml/tinyxml.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructs the background manager.
/// ---------------------------------------------------------------------------
prSpriteManager::prSpriteManager() : prCoreSystem(PRSYSTEM_SPRITEMANAGER, "prSpriteManager")
{
    m_correctFileType = false;
    m_sprite          = nullptr;
    m_texture         = nullptr;
    m_exp0            = false;
    m_exp1            = false;
    m_exp2            = false;
    m_pBatchQuads     = nullptr;
    m_numQuads        = 0;
    m_numQuadsAdded   = 0;
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prSpriteManager::~prSpriteManager()
{
    ReleaseAll();
}


/// ---------------------------------------------------------------------------
/// Updates all the active sprites.
/// ---------------------------------------------------------------------------
void prSpriteManager::Update(f32 dt)
{
    if (!m_activeSprites.empty())
    {
        std::list<prActiveSprite>::iterator it  = m_activeSprites.begin();
        std::list<prActiveSprite>::iterator end = m_activeSprites.end();

        for (; it != end; ++it)
        {
            (*it).sprite->Update(dt);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Draws all the visible sprites.
/// ---------------------------------------------------------------------------
void prSpriteManager::Draw()
{
    if (!m_activeSprites.empty())
    {
        std::list<prActiveSprite>::iterator it  = m_activeSprites.begin();
        std::list<prActiveSprite>::iterator end = m_activeSprites.end();

        for (; it != end; ++it)
        {
            // Shall we draw?
            if ((*it).draw)
            {
                (*it).sprite->Draw();
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Creates a sprites.
/// ---------------------------------------------------------------------------
prSprite *prSpriteManager::Create(const char *filename, bool draw)
{
    PRASSERT(filename && *filename);

    // Create sprite.
    m_sprite          = nullptr;
    m_texture         = nullptr;
    m_correctFileType = false;


    Load(filename);


    if (m_sprite && m_texture)
    {
        // Store
        if (!m_activeSprites.empty())
        {
            prActiveSprite as;
            as.sprite  = m_sprite;
            as.texture = m_texture;
            as.draw    = draw;

            TODO("Re-add priority code")

            // Insert sprite according to priority.
/*            std::list<prActiveSprite>::iterator it  = m_activeSprites.begin();
            std::list<prActiveSprite>::iterator end = m_activeSprites.end();
            bool inserted = false;

            for (; it != end; ++it)
            {
                //if ((*it).sprite->GetPriority() >= 0)
                //{
                    m_activeSprites.insert(it, as);
                    inserted = true;
                    break;
                //}
            }

            // Else add last
            if (!inserted)//*/
            {
                m_activeSprites.push_back(as);
            }
        }
        else
        {
            // Push first
            prActiveSprite as;
            as.sprite  = m_sprite;
            as.texture = m_texture;
            as.draw    = draw;
            m_activeSprites.push_back(as);
        }
    }
    else
    {
        PRSAFE_DELETE(m_sprite);
    }

    return m_sprite;
}


//#if defined(PROTEUS_TOOL)
//#if defined(PROTEUS_TOOL) || defined(PLATFORM_PC)
/// ---------------------------------------------------------------------------
/// Creates a sprite.
/// ---------------------------------------------------------------------------
prSprite *prSpriteManager::ToolCreate(prTexture *pTex, s32 width, s32 height)
{
    PRASSERT(pTex);

    if (pTex)
    {
        m_texture = pTex;
        m_sprite  = new prSprite(this, pTex, "sprite", width, height);
    }

    return m_sprite;
}


/// ---------------------------------------------------------------------------
/// Releases a sprite and any asssociated assets.
/// ---------------------------------------------------------------------------
void prSpriteManager::ToolRelease(prSprite *sprite)
{
    if (sprite)
    {
        delete sprite;
    }
}
//#endif


/// ---------------------------------------------------------------------------
/// Releases a sprite and any asssociated assets.
/// ---------------------------------------------------------------------------
void prSpriteManager::Release(prSprite *sprite)
{
    if (sprite)
    {
        if (!m_activeSprites.empty())
        {
            std::list<prActiveSprite>::iterator it  = m_activeSprites.begin();
            std::list<prActiveSprite>::iterator end = m_activeSprites.end();

            for (; it != end; ++it)
            {
                if ((*it).sprite == sprite)
                {
                    PRSAFE_DELETE(sprite);

                    // Release the sprites texture
                    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
                    if (pRM)
                    {
                        pRM->Unload((*it).texture);
                    }            

                    m_activeSprites.erase(it);
                    return;
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Releases all sprites and their asssociated assets. 
/// ---------------------------------------------------------------------------
void prSpriteManager::ReleaseAll()
{
    if (!m_activeSprites.empty())
    {
        std::list<prActiveSprite>::iterator it  = m_activeSprites.begin();
        std::list<prActiveSprite>::iterator end = m_activeSprites.end();

        for (; it != end; ++it)
        {
            PRSAFE_DELETE((*it).sprite);

            // Release the sprites texture
            prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
            if (pRM)
            {
                pRM->Unload((*it).texture);
            }            
        }

        m_activeSprites.clear();
    }
}


/// ---------------------------------------------------------------------------
/// Debug assist code to show all sprites.
/// ---------------------------------------------------------------------------
void prSpriteManager::DisplayUsage()
{
#if defined(DEBUG) || defined(_DEBUG)

    if (!m_activeSprites.empty())
    {
        prTrace(LogError, "Sprites\n");

        std::list<prActiveSprite>::iterator it  = m_activeSprites.begin();
        std::list<prActiveSprite>::iterator end = m_activeSprites.end();
        s32 index = 0;
        for (; it != end; ++index, ++it)
        {
            prTrace(LogError, "Sprite %03i: Name: %s, TexID: %ul\n", index, (*it).sprite->Name(), (*it).texture->GetTexID());
        }

        prTrace(LogError, "Total sprites: %i\n", m_activeSprites.size());
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Starts batch sprite rendering
/// ---------------------------------------------------------------------------
void prSpriteManager::BatchBegin(prSprite *pSprite, s32 batchSize)
{
    PRASSERT(pSprite);

    // Creat batch memory
    if (batchSize > 0) 
    {
        m_pBatchQuads   = new QuadData[(sizeof(QuadData) *  4) * batchSize];        // 4 quads per sprite
        m_pBatchColours = new      f32[(sizeof(f32)      * 16) * batchSize];        // 16 colours per sprite
        m_numQuads      = batchSize;
        m_numQuadsAdded = 0;
        //PRLOGD("Starting batch size %i\n", (sizeof(QuadData) *  4) * batchSize);

        glPushMatrix();
        ERR_CHECK();

        // translate to quad center, the translate for position
        //glTranslatef((float)(m_frameWidth >> 1), (float)(m_frameHeight >> 1), 0);
        //ERR_CHECK();
        //glTranslatef(pSprite->pos.x, pSprite->pos.y, 0);
        //ERR_CHECK();
    }


    // Enable blending
    glEnable(GL_BLEND);
    ERR_CHECK();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ERR_CHECK();


    // Bind the texture for the batch
    pSprite->m_pTexture->Bind();

    // Set states
    glEnableClientState(GL_COLOR_ARRAY);
    ERR_CHECK();
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    ERR_CHECK();
}


/// ---------------------------------------------------------------------------
/// Ends batch sprite rendering
/// ---------------------------------------------------------------------------
void prSpriteManager::BatchEnd()
{
    // Clean batch memory
    if (m_numQuads > 0) 
    {
        if (m_numQuadsAdded > 0)
        {
            //m_numQuadsAdded = 1;

            glVertexPointer(2, GL_FLOAT, sizeof(QuadData), &m_pBatchQuads->x);
            ERR_CHECK();        
            glColorPointer(4, GL_FLOAT, 0,  m_pBatchColours);
            ERR_CHECK();
            glTexCoordPointer(2, GL_FLOAT, sizeof(QuadData), &m_pBatchQuads->u);
            ERR_CHECK();
            glDrawArrays(GL_QUADS, 0, 4 * m_numQuadsAdded);
            ERR_CHECK();
        }

        //PRLOGD("Batch end %i - %i\n", m_numQuadsAdded, 4 * m_numQuadsAdded);

        PRSAFE_DELETE_ARRAY(m_pBatchQuads);
        PRSAFE_DELETE_ARRAY(m_pBatchColours);
        m_numQuads      = 0;
        m_numQuadsAdded = 0;

        glPopMatrix();
        ERR_CHECK();
    }

    // Reset states
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    ERR_CHECK();
    glDisableClientState(GL_COLOR_ARRAY);
    ERR_CHECK();

    // Disable blending
    glDisable(GL_BLEND);
    ERR_CHECK();
}


void prSpriteManager::BatchAdd(f32 *pColours, QuadData* pQuadData)
{
    PRASSERT(m_pBatchQuads);
    PRASSERT(m_pBatchColours);
    PRASSERT(m_numQuads > 0);

    if (m_numQuadsAdded < m_numQuads)
    {

        QuadData* pQuadDest = m_pBatchQuads + (m_numQuadsAdded * 4);
        //PRLOGD("Added quad %i : Base %p. Dest %p. Size %i\n", m_numQuadsAdded, m_pBatchQuads, pQuadDest, (u8*)pQuadDest - (u8*)m_pBatchQuads);


        f32 *pColrDest = m_pBatchColours + (m_numQuadsAdded * 16);
        //PRLOGD("Added cols %i : Base %p. Dest %p. Size %i\n", m_numQuadsAdded, pColours, pColrDest, (u8*)pColrDest - (u8*)m_pBatchColours);

        memcpy(pQuadDest,  pQuadData, sizeof(QuadData) * 4);
        memcpy(pColrDest, pColours, sizeof(f32) * 16);
        //PRLOGD("Added %i : Quads %i. Cols %i.\n", m_numQuadsAdded, sizeof(QuadData) * 4, sizeof(f32) * 16);

        m_numQuadsAdded++;
    }
}


/// ---------------------------------------------------------------------------
/// Loads a sprite file.
/// ---------------------------------------------------------------------------
bool prSpriteManager::Load(const char *filename)
{
    PRASSERT(filename && *filename);

    TiXmlDocument* doc = new TiXmlDocument(filename);
    
    bool result = false;

    if (doc)
    {
        bool loaded = doc->LoadFile();
      
        if (loaded)
        {
            ParseSpriteFile(doc);
            result = true;
        }
        else
        {
            PRWARN("Failed to Load %s\n", filename);
        }

        delete doc;
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Parses a sprite file.
/// ---------------------------------------------------------------------------
void prSpriteManager::ParseSpriteFile(TiXmlNode* pParent)
{
    switch (pParent->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        {
            // File data
            if (strcmp(pParent->Value(), "sprite_file") == 0)
            {
                ParseAttribs_SpriteFile(pParent->ToElement());
            }
            // Sprite data
            else if (strcmp(pParent->Value(), "sprite") == 0)
            {
                ParseAttribs_Sprite(pParent->ToElement());
            }
            // Sequence data
            else if (strcmp(pParent->Value(), "sequence") == 0)
            {
                ParseAttribs_Sequence(pParent->ToElement());
            }

    
            // The first element should set the file type.
            // So lets validate it
            if (m_correctFileType == false)
            {
                PRPANIC("Incorrect file type!");
            }
        }
        break;

    default:
        break;
    } 


    for (TiXmlNode *pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        ParseSpriteFile(pChild);
    }
}


/// ---------------------------------------------------------------------------
/// Attribute parser - Used to get information from the file like the version
/// number.
/// ---------------------------------------------------------------------------
void prSpriteManager::ParseAttribs_SpriteFile(TiXmlElement* pElement)
{
    PRASSERT(pElement);

    if (pElement)
    {
        PRASSERT(pElement->Attribute("version"));
    }

    // Indicate correct file type.
    m_correctFileType = true;
}


/// ---------------------------------------------------------------------------
/// Attribute parser - Used to get information about the sprite like its name,
/// frame width, frame height, etc.
/// ---------------------------------------------------------------------------
void prSpriteManager::ParseAttribs_Sprite(TiXmlElement* pElement)
{
    PRASSERT(pElement);

    if (pElement)
    {
        // Sanity checks
        PRASSERT(pElement->Attribute("name"));
        PRASSERT(pElement->Attribute("width"));
        PRASSERT(pElement->Attribute("height"));


        // Create the animation
        const char *name        = pElement->Attribute("name");
        int         frameWidth  = atoi(pElement->Attribute("width"));
        int         frameHeight = atoi(pElement->Attribute("height"));


        // Acquire the child data.
        TiXmlHandle root(pElement);
        TiXmlElement *pElem  = root.FirstChild("texture").Element();
        if (pElem)
        {
            // Get the sprites texture data
            PRASSERT(pElem->Attribute("data"));

            // Create the sprites texture and the sprite.
            prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
            PRASSERT(pRM)
            m_texture = pRM->Load<prTexture>(pElem->Attribute("data"));
            PRASSERT(m_texture);

            if (m_texture)
            {
                m_sprite = new prSprite(this, m_texture, name, frameWidth, frameHeight);
            }
        }
        else
        {
            PRPANIC("Sprite '%s' has no texture.", name);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Attribute parser
/// ---------------------------------------------------------------------------
void prSpriteManager::ParseAttribs_Sequence(TiXmlElement* pElement)
{
    PRASSERT(pElement);

    if (pElement)
    {
        // Sanity checks
        PRASSERT(pElement->Attribute("name"));
        PRASSERT(pElement->Attribute("anim"));


        // Get animation name.
        const char *name = pElement->Attribute("name");


        // Get animation type
        const char *animType = pElement->Attribute("anim");
        s32 type;

        if (prStringCompare(animType, "loop") == CMP_EQUALTO)
        {
            type = ANIM_TYPE_LOOP;
        }
        else if (prStringCompare(animType, "once") == CMP_EQUALTO)
        {
            type = ANIM_TYPE_ONCE;
        }
        else if (prStringCompare(animType, "yoyo") == CMP_EQUALTO)
        {
            type = ANIM_TYPE_YOYO;
        }
        else
        {
            PRWARN("Unrecognised animation type.");
            type = ANIM_TYPE_NONE;
        }

        
        // Create the animation sequence
        prSpriteAnimationSequence* sequence = new prSpriteAnimationSequence
        (
            name,
            atoi(pElement->Attribute("count")),
            type
        );


        // Add the sequence
        m_sprite->AddSequence(sequence, name);


        // Acquire the child frame data.
        sequence->ParseFrameData(pElement);
    }
}
