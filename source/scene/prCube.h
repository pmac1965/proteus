// File: prCube.h
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


//#include "prSceneObject.h"
#include "../core/prGameObject.h"


// Class: prCube
//      A simple geometric primitive for use by the *game editor*
//
// Notes:
//      This class is a game object, and therefore inherits a great deal
//      of functionality from the game object
//
// See Also
//      <prGameObject>
class prCube : public Proteus::Core::prGameObject //: public prSceneObject
{
public:
    // Method: prCube
    //      Ctor
    prCube();

    // Method: Draw
    //      Draws this primitive
    void Draw();
};
