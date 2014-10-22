// File: prScene.h
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


#ifndef __PRSCENE_H
#define __PRSCENE_H


#include "../core/prTypes.h"


// Forward declarations
class prSceneNode;


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

    // Method: Add
    //
    void Add(prSceneNode *psn);

    // Method: Remove
    //
    void Remove(prSceneNode *psn);

    //
    void Update(f32 dt);

    //
    //void Render();
};


#endif//__PRSCENE_H
