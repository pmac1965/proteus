/**
 * prBackground.cpp
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
  #include "../debug/prException.h"

#elif defined(PLATFORM_IOS)
  #include <OpenGLES/ES1/gl.h>  

#elif defined(PLATFORM_MAC)
  #include <OpenGL/gl.h>

#elif defined(PLATFORM_ANDROID)
  #include <GLES/gl.h>

#elif defined(PLATFORM_LINUX)
  #include <GL/gl.h>
  #include <GL/glu.h>

#else
  #error No platform defined.

#endif


#include "prBackground.h"
#include "prBackgroundLayer.h"
#include "prRenderer.h"
#include "prTexture.h"
#include "../debug/prAssert.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../debug/prAlert.h"
#include "../core/prResourceManager.h"
#include "../core/prStringUtil.h"
#include "../core/prCore.h"
#include "../core/prRegistry.h"
#include "../file/prFileShared.h"
#include "../tinyxml/tinyxml.h"
#include "../display/prOglUtils.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Constructs the background.
/// ---------------------------------------------------------------------------
prBackground::prBackground(const char *filename) : prGameObject()
                                                 , m_colour(prColour::White)
                                                 #if defined(PROTEUS_TOOL)
                                                 , m_filenameBackground(filename)
                                                 #endif
{
    // Game builds will assert if there's no filename
    #if !defined(PROTEUS_TOOL)
    PRASSERT(filename && *filename);
    #endif

    // Init data
    m_texture               = NULL;
    m_width                 = -1;
    m_height                = -1;
    m_type                  = prBackground::UNKNOWN;
    m_correctFileType       = false;
    m_widthHeightSupplied   = false;
    m_exp1                  = false;
    m_exp2                  = false;
    m_pixelHeight           = 0.0f;
    m_pixelWidth            = 0.0f;
    m_scrnWidth             = 0.0f;
    m_scrnHeight            = 0.0f;
    m_v0                    = 0.0f;
    m_u1                    = 0.0f;
    //mVisible                = PRTRUE;

    for (s32 i=0; i<BACKGROUND_MAX_LAYERS;i++)
    {
        mLayers[i] = NULL;
    }

    // Parse the document
    #if defined(PROTEUS_TOOL)
    if (filename && *filename) 
    #endif
    {
        TiXmlDocument* doc = new TiXmlDocument(filename);
        if (doc)
        {
            bool loaded = doc->LoadFile();      
            if (loaded)
            {
                ParseFile(doc);
            }
            else
            {
                PRWARN("Failed to Load background '%s'\n", filename);
                return;
            }

            delete doc;
        }
    }


    #if defined(PROTEUS_TOOL)
    // Texture created elsewhere

    #else
    // Load the texture - filename acquired from the xml
    if (m_filenameTexture.Length() > 0)
    {
        prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
        PRASSERT(pRM);
        m_texture = pRM->Load<prTexture>(m_filenameTexture.Text());
        PRASSERT(m_texture);

        // Texture width
        m_width  = m_texture->GetWidth();
        m_height = m_texture->GetHeight();
        
        // Texture pixel size.
        m_pixelWidth  = 1.0f / m_width;
        m_pixelHeight = 1.0f / m_height;

        // Get draw size if its being forced to a set size
        if (!m_widthHeightSupplied)
        {
            // Assume display size is screen width/height
            prRegistry *pReg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
            PRASSERT(pReg);
            m_scrnWidth  = (float)atof(pReg->GetValue("ScreenWidth"));
            m_scrnHeight = (float)atof(pReg->GetValue("ScreenHeight"));
        }

        // UV Coords pre-calc
        m_v0 = 1.0f - (m_pixelHeight * m_scrnHeight);
        m_u1 =        (m_pixelWidth  * m_scrnWidth);
    }
    else
    {
        PRPANIC("prBackground has no texture.");
    }

    #endif
}


/// ---------------------------------------------------------------------------
/// Dtor.
/// ---------------------------------------------------------------------------
prBackground::~prBackground()
{
    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));
    if (pRM && m_texture)
    {
        pRM->Unload(m_texture);
        m_texture = NULL;
    }

    // Release the layers
    for (s32 i=0; i<BACKGROUND_MAX_LAYERS;i++)
    {
        PRSAFE_DELETE(mLayers[i]);
    }
}


/// ---------------------------------------------------------------------------
/// Draws the background.
/// ---------------------------------------------------------------------------
void prBackground::Draw()
{
    // Visible?
    if (IsVisible())
    {
        //if (mVisible == PRFALSE) {
        //    return;
        //}

        #if !defined(PROTEUS_TOOL)
        PRASSERT(m_texture);
        #endif

        // Draws a single image
        if (m_type == IMAGE)
        {
            if (m_texture)
            {
                f32 width  = (f32)(m_scrnWidth  / 2);
                f32 height = (f32)(m_scrnHeight / 2);

                glPushMatrix();
                ERR_CHECK();

                // Translate to center
                glTranslatef(width, height, 0);
                ERR_CHECK();

                // Move to offset
                glTranslatef(pos.x, pos.y, 0);
                ERR_CHECK();

                // Set scale
                glScalef(m_scrnWidth, m_scrnHeight, 0);
                ERR_CHECK();

                if (m_texture->Bind())
                {
                    prRenderer *pRenderer = static_cast<prRenderer *>(prCoreGetComponent(PRSYSTEM_RENDERER));
                    if (pRenderer)
                    {
                        pRenderer->DrawQuad(0.0f, m_v0, m_u1, 1.0f, m_colour);
                    }
                }

                glPopMatrix();
                ERR_CHECK();
            }
        }
        else
        {
            for (s32 i=0; i<BACKGROUND_MAX_LAYERS; i++)
            {
                if (mLayers[i])
                {
                    mLayers[i]->Draw();
                }
            }
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets a tint colour.
/// ---------------------------------------------------------------------------
void prBackground::SetColour(prColour c)
{
    m_colour = c;
}


/// ---------------------------------------------------------------------------
/// Gets a layer of a tiled background
/// ---------------------------------------------------------------------------
prBackgroundLayer* prBackground::GetLayer(s32 index)
{
    prBackgroundLayer* pLayer = NULL;

    if (PRBETWEEN(index, 0, BACKGROUND_MAX_LAYERS - 1))
    {
        return mLayers[index];
    }

    return pLayer;
}


#if defined(PROTEUS_TOOL)
/// ---------------------------------------------------------------------------
/// Sets the backgrounds texture
/// ---------------------------------------------------------------------------
void prBackground::SetTexture(prTexture* pTex)
{
    m_texture = pTex;
    PRASSERT(m_texture);

    // Texture width
    m_width  = m_texture->GetWidth();
    m_height = m_texture->GetHeight();
        
    // Texture pixel size.
    m_pixelWidth  = 1.0f / m_width;
    m_pixelHeight = 1.0f / m_height;

    // Set draw size
    prRegistry *reg = static_cast<prRegistry *>(prCoreGetComponent(PRSYSTEM_REGISTRY));
    if (reg)
    {
        m_scrnWidth  =  (f32)m_texture->GetWidth();
        m_scrnHeight =  (f32)m_texture->GetHeight();
    }

    // UV Coords pre-calc
    m_v0 = 1.0f - (m_pixelHeight * m_scrnHeight);
    m_u1 =        (m_pixelWidth  * m_scrnWidth);
}


/// ---------------------------------------------------------------------------
/// Saves the background. *Only available on tool builds*
/// ---------------------------------------------------------------------------
bool prBackground::Save()
{
    bool        result  = false;
    const char *type    = NULL;
    const char *name    = "?";


    // We need a file name to save
    if (m_filenameBackground.Length() == 0)
    {
        throw prException("The backgrounds texture filename is null or empty");
    }


    // Set type
    if (m_type == IMAGE)
    {
        type = "image";

        // We need a texture file name to save an image type background
        if (m_filenameTexture.Length() == 0)
        {
            throw prException("The backgrounds texture filename is null or empty");
        }
    }
    else if (m_type == TILEMAP)
    {
        type = "tile";
    }
    else
    {
        throw prException("Unknown background type");
    }


    // Set name?
    if (m_name.Length() > 0)
    {
        name = m_name.Text();
    }


    // Create the doc header
	TiXmlDocument doc;  
 	TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "utf-8", "");  
	doc.LinkEndChild(decl);  


    // Add root
    TiXmlElement *root = new TiXmlElement("background_file");
    root->SetAttribute("version", "1.0.0");
    doc.LinkEndChild( root );


    // Add the background information
    TiXmlElement *element = new TiXmlElement("background");
	root->LinkEndChild(element);
    element->SetAttribute("name", name);
    element->SetAttribute("type", type);


    // Add the details
    if (m_type == IMAGE)
    {
        TiXmlElement *details = new TiXmlElement("texture");
	    element->LinkEndChild(details);

        details->SetAttribute("data", m_filenameTexture.Text());
    }
    else if (m_type == TILEMAP)
    {
        for (s32 i=0; i<BACKGROUND_MAX_LAYERS;i++)
        {
            prBackgroundLayer *pBackgroundLayer = mLayers[i];

            if (pBackgroundLayer)
            {
                // Add layer
                TiXmlElement *layer = new TiXmlElement("layer");

                layer->SetAttribute("tileWidth", (int)(pBackgroundLayer->GetTileWidth()));
                layer->SetAttribute("tileHeight", (int)(pBackgroundLayer->GetTileHeight()));
                layer->SetAttribute("mapWidth", (int)(pBackgroundLayer->GetLayerWidth()));
                layer->SetAttribute("mapHeight", (int)(pBackgroundLayer->GetLayerHeight()));
                layer->SetAttribute("tex", pBackgroundLayer->GetTextureFilename());

	            element->LinkEndChild(layer);

                // Add the row data
                s32  numRows  = pBackgroundLayer->GetLayerHeight();
                s32  numCols  = pBackgroundLayer->GetLayerWidth();
                int *pMapData = pBackgroundLayer->GetMapData();
                s32  rowSize  = (numCols * 12) + 1;                 // 12 == Max int size (10 digits) plus two sysmbols ',-'. Plus 1 for terminating null

                for (s32 rows=0; rows<numRows; rows++)
                {
                    // Add row
                    TiXmlElement *row = new TiXmlElement("row");
	                layer->LinkEndChild(row);

                    // Add char data
                    char *pRow = new char[rowSize];    
                    memset(pRow, 0, rowSize);

                    for (s32 col=0; col<numCols; col++)
                    {
                        char buffer[16];
                        sprintf_s(buffer, "%i", *pMapData++);
                        strcat(pRow, buffer);

                        if (col < (numCols - 1))
                        {
                            strcat(pRow, ",");
                        }
                    }

                    TiXmlText *txt = new TiXmlText(pRow);	        
                    row->LinkEndChild(txt);

                    PRSAFE_DELETE_ARRAY(pRow);
                }
            }
        }
    }


    // Save the file
    doc.SaveFile(m_filenameBackground.Text());


    return result;
}


/// ---------------------------------------------------------------------------
/// Adds a new map layer
/// ---------------------------------------------------------------------------
prBackgroundLayer *prBackground::AddNewLayer(s32 width, s32 height, s32 tileWidth, s32 tileHeight, prTexture *pTexture)
{
    prBackgroundLayer *pLayer = NULL;

    for (s32 i=0; i<BACKGROUND_MAX_LAYERS;i++)
    {
        if (mLayers[i] == NULL)
        {
            pLayer     = new prBackgroundLayer(width, height, tileWidth, tileHeight, pTexture);
            mLayers[i] = pLayer;
            break;
        }
    }

    return pLayer;
}
#endif


/// ---------------------------------------------------------------------------
/// Parses the xml file.
/// ---------------------------------------------------------------------------
void prBackground::ParseFile(TiXmlNode* pParent)
{
    switch (pParent->Type())
    {
    case TiXmlNode::TINYXML_ELEMENT:
        {
            // File data
            if (prStringCompare(pParent->Value(), "background_file") == 0)
            {
                ParseAttribs_File(pParent->ToElement());
            }
            // Background data
            else if (prStringCompare(pParent->Value(), "background") == 0)
            {
                ParseAttribs_Background(pParent->ToElement());
            }
            // Background layers
            else if (prStringCompare(pParent->Value(), "layer") == 0)
            {
                ParseAttribs_Layer(pParent->ToElement());
            }
        }
        break;

    default:
        break;
    } 


    for (TiXmlNode *pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling()) 
    {
        ParseFile(pChild);
    }
}


/// ---------------------------------------------------------------------------
/// Attribute parser - Used to get information from the file like the version
///                    number.
/// ---------------------------------------------------------------------------
void prBackground::ParseAttribs_File(TiXmlElement* pElement)
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
/// Attribute parser - Used to get information about the background like its
///                    name, type, etc.
/// ---------------------------------------------------------------------------
void prBackground::ParseAttribs_Background(TiXmlElement* pElement)
{
    PRASSERT(pElement);
    PRASSERT(m_correctFileType);

    if (pElement && m_correctFileType)
    {
        // Sanity checks
        PRASSERT(pElement->Attribute("name"));
        PRASSERT(pElement->Attribute("type"));


        // Set type and name
        if (prStringCompare(pElement->Attribute("type"), "image") == 0)
        {
            m_type = IMAGE;
            //m_name.Set(pElement->Attribute("name"));
        }
        else if (prStringCompare(pElement->Attribute("type"), "tile") == 0)
        {
            m_type = TILEMAP;
            //m_name.Set(pElement->Attribute("name"));
        }
        else
        {
            #if defined(PROTEUS_TOOL)
                throw prException("Unknown background type");
            #else
                PRPANIC("Unknown background type");
            #endif
        }


        // Load type specific information
        if (m_type == IMAGE)
        {
            // Acquire the data.
            TiXmlHandle root(pElement);
            TiXmlElement *pElem = root.FirstChild("texture").Element();
            if (pElem)
            {
                // Store texture filename
                PRASSERT(pElem->Attribute("data"));
                m_filenameTexture.Set(pElem->Attribute("data"));


                // If we're a tool we cannot use the older .bgd files, they need to be
                // converted as their textures may not be for the PC
                #if defined(PROTEUS_TOOL)
                    char filename[FILE_MAX_FILENAME_SIZE];
                    prStringCopySafe(filename, m_filenameTexture.Text(), sizeof(filename));
                    s32 index = prStringFindLastIndex(filename, '.');
                    if (index > -1)
                    {
                        if (prStringCompareNoCase(&filename[index], ".pvr") == 0)
                        {
                            throw prException("You cannot use this background file, as it contains a .pvr file as the texture");
                        }
                    }
                    else
                    {
                        throw prException("Invalid background file");
                    }

                #else
                    // Tool created background files use the path to the textures image, so we need to
                    // create the path to the .pvr data for games
                    char filename[FILE_MAX_FILENAME_SIZE];
                    prStringCopySafe(filename, m_filenameTexture.Text(), sizeof(filename));
                    prStringReplaceChar(filename, '\\', '/');

                    s32 indexExt = prStringFindLastIndex(filename, '.');
                    s32 indexNme = prStringFindLastIndex(filename, '/');

                    if (indexExt > -1 &&
                        indexNme > -1)
                    {
                        // If not .pvr, create path to .pvr
                        if (prStringCompareNoCase(&filename[indexExt], ".pvr") != 0)
                        {
                            char path[FILE_MAX_FILENAME_SIZE];
                            filename[indexExt] = 0;
                            prStringSnprintf(path, sizeof(path), "data/textures/%s.pvr", &filename[indexNme + 1]);
                            m_filenameTexture.Set(path);
                        }
                    }
                    else
                    {
                        PRPANIC("Invalid background file");
                    }

                #endif

                // Width/height
                const char *width  = pElem->Attribute("width");
                const char *height = pElem->Attribute("height");
                if (width && height)
                {
                    TODO("Deprecated - remove")
                    m_widthHeightSupplied = true;
                    m_scrnWidth           = (f32)atof(width);
                    m_scrnHeight          = (f32)atof(height);
                }
            }
            else
            {
                #if defined(PROTEUS_TOOL)
                    throw prException("The background file has no texture.");
                #else
                    PRPANIC("The background file has no texture.");
                #endif
            }
        }
        else
        {
        }
    }
}


/// ---------------------------------------------------------------------------
/// Attribute parser - Used to get information about the backgrounds layers
/// ---------------------------------------------------------------------------
void prBackground::ParseAttribs_Layer(TiXmlElement* pElement)
{
    PRASSERT(pElement);
    PRASSERT(m_correctFileType);

    if (pElement && m_correctFileType)
    {
        // Sanity checks
        PRASSERT(pElement->Attribute("tileWidth"));
        PRASSERT(pElement->Attribute("tileHeight"));
        PRASSERT(pElement->Attribute("mapWidth"));
        PRASSERT(pElement->Attribute("mapHeight"));
        PRASSERT(pElement->Attribute("tex"));

        const char *tileWidth  = pElement->Attribute("tileWidth");
        const char *tileHeight = pElement->Attribute("tileHeight");
        const char *mapWidth   = pElement->Attribute("mapWidth");
        const char *mapHeight  = pElement->Attribute("mapHeight");
        const char *tex        = pElement->Attribute("tex");

        prTrace(LogError, "ATTRIBS TILED: %s, %s, %s, %s, %s\n", tileWidth, tileHeight, mapWidth, mapHeight, tex);

        // Add a new map layer
/*        for (s32 i=0; i<BACKGROUND_MAX_LAYERS;i++)
        {
            if (mLayers[i] == NULL)
            {
                #if defined(PROTEUS_TOOL)

                    prBackgroundLayer *pLayer = NULL;
                #else
        //        pLayer     = new prBackgroundLayer(width, height, tileWidth, tileHeight, pTexture);

                #endif

                break;
            }
        }//*/


        // Acquire the child rows
        TiXmlHandle   root(pElement);
        TiXmlElement *pElem  = root.FirstChild("row").Element();
        s32           i      = 0;       
        for (; pElem; pElem = pElem->NextSiblingElement())
        {
            prTrace(LogError, "Row %03i: > '%s'\n", i, pElem->GetText());
            i++;
        }
    }
}
