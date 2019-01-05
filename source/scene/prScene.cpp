/**
 * prScene.cpp
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


#include "prScene.h"
#include "../core/prStringUtil.h"
#include "../tinyxml/tinyxml.h"


namespace Scene
{

    /// ---------------------------------------------------------------------------
    /// Constructor
    /// ---------------------------------------------------------------------------
    prScene::prScene()
    {

    }


    /// ---------------------------------------------------------------------------
    /// Destructor
    /// ---------------------------------------------------------------------------
    prScene::~prScene()
    {
        Unload();
    }


    /// ---------------------------------------------------------------------------
    /// Loads the scene file specified by the filename
    /// ---------------------------------------------------------------------------
    void prScene::Load(const char *pFilename)
    {
        // Parse the document
        if (pFilename && *pFilename) 
        {
            TiXmlDocument* doc = new TiXmlDocument(pFilename);
            if (doc)
            {
                bool loaded = doc->LoadFile();      
                if (loaded)
                {
                    ParseFile(doc);
                }
                else
                {
                    PRWARN("Failed to Load scene '%s'\n", pFilename);
                    return;
                }

                delete doc;
            }
        }
    }


    /// ---------------------------------------------------------------------------
    /// Unloads the current scene and frees all the objects
    /// ---------------------------------------------------------------------------
    void prScene::Unload()
    {
    }


    /// ---------------------------------------------------------------------------
    /// Adds an object to the scene
    /// ---------------------------------------------------------------------------
    void prScene::Add(prGameObject *pObject)
    {
    }


    /// ---------------------------------------------------------------------------
    /// Removes an object from the scene
    /// ---------------------------------------------------------------------------
    void prScene::Remove(prGameObject *pObject)
    {
    }


    /// ---------------------------------------------------------------------------
    /// Updates the scene
    /// ---------------------------------------------------------------------------
    void prScene::Update()
    {
    }


    /// ---------------------------------------------------------------------------
    /// Draws the scene
    /// ---------------------------------------------------------------------------
    void prScene::Draw()
    {
    }


    /// ---------------------------------------------------------------------------
    /// Parses the xml file.
    /// ---------------------------------------------------------------------------
    void prScene::ParseFile(TiXmlNode* pParent)
    {
        switch (pParent->Type())
        {
        case TiXmlNode::TINYXML_ELEMENT:
            {
                // File data
                if (prStringCompare(pParent->Value(), "scene_file") == 0)
                {
                    ParseAttribs_File(pParent->ToElement());
                }
                // Background data
                else if (prStringCompare(pParent->Value(), "background") == 0)
                {
                    //ParseAttribs_Background(pParent->ToElement());
                }
                // Background layers
                else if (prStringCompare(pParent->Value(), "layer") == 0)
                {
                    //ParseAttribs_Layer(pParent->ToElement());
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
    void prScene::ParseAttribs_File(TiXmlElement* pElement)
    {
        PRASSERT(pElement);
        if (pElement)
        {
            PRASSERT(pElement->Attribute("version"));
        }

        // Indicate correct file type.
        //m_correctFileType = true;
    }

}
