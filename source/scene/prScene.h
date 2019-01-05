// File: prScene.h
//      A simple scene graph
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


// Forward declarations
class prGameObject;
class TiXmlNode;
class TiXmlElement;


namespace Scene {

    // Class: prScene
    //      A simple scene graph
    class prScene
    {
    public:
        // Method: prScene
        //      Ctor
        prScene();

        // Method: ~prScene
        //      Dtor
        ~prScene();

        // Method: Load
        //      Loads the scene file specified by the filename
        //
        // Parameters:
        //      pFilename - A scene file
        void Load(const char *pFilename);

        // Method: Unload
        //     Unloads the current scene and frees all the objects
        void Unload();

        // Method: Add
        //      Adds an object to the scene
        //
        // Parameters:
        //      pObject - A game object
        void Add(prGameObject *pObject);

        // Method: Remove
        //      Removes an object from the scene
        //
        // Parameters:
        //      pObject - A game object
        void Remove(prGameObject *pObject);

        // Method: Update
        //      Updates the scene
        void Update();

        // Method: Draw
        //      Draws the scene
        void Draw();

    private:
        // Method: ParseFile
        //      Parses the xml file.
        void ParseFile(TiXmlNode* pParent);
    
        // Method: ParseAttribs_File
        //      Attribute parser used to get information about the file.
        void ParseAttribs_File(TiXmlElement* pElement);

    //private:
    };

}
