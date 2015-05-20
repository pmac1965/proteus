// File: prSceneNode.h
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


#ifndef __PRSCENENODE_H
#define __PRSCENENODE_H


#include <list>
#include "../core/prTypes.h"


// Class: prSceneNode
//      A simple scene graph node
//
// Notes:
//      This is the base class for other scene node types.
//      It is *NOT* intended to be used on its own
class prSceneNode
{
public:
    // Method: prSceneNode
    //      Ctor
    prSceneNode(prSceneNode *pParent);

    // Method: ~prSceneNode
    //      Dtor
    virtual ~prSceneNode();

    void Update(Proteus::Core::f32 dt);

    //void Render();

    void AddChild(prSceneNode *pNode);

    void RemoveChild(prSceneNode *pNode);

private:
    prSceneNode *mParent;
    std::list<prSceneNode*> mChildren;
};


#endif//__PRSCENENODE_H
