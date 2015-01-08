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


#ifndef __PRCUBE_H
#define __PRCUBE_H


#include "prSceneObject.h"


// Class: prCube
//      A simple geometric primitive for use by the *game editor*
//
// Notes:
//      This class is a scene object, and therefore inherits a great deal
//      of functionality from the scene object
//
// See Also
//      <prSceneObject>
class prCube : public prSceneObject
{
public:
    // Method: prCube
    //      Ctor
    prCube();

    // Method: Draw
    //      Draws this primitive
    void Draw();
};


#endif//__PRCUBE_H
