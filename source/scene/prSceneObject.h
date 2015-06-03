// File: prSceneObject.h
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


#ifndef __PRSCENEOBJECT_H
#define __PRSCENEOBJECT_H


//#include "../core/prString.h"
#include "../core/prString.h"
#include "../core/prVertex.h"
#include "../display/prColour.h"
#include "../math/prVector3.h"


// Class: prSceneObject
//      Represents any object within a game scene
class prSceneObject
{
public:
    // Method: prSceneObject
    //      Scene object constructor
    //
    // Parameters:
    //      A name for the object. The name does *not* have
    //      to be unique
    explicit prSceneObject(const char *pName);

    // Method: prSceneObject
    //      Scene object constructor
    //
    // Notes:
    //      The name is set as *"None"*
    prSceneObject();

    // Method: ~prSceneObject
    //      Scene object destructor
    virtual ~prSceneObject();

    // Method: Draw
    //      Draws the scene object
    //
    // Notes:
    //      This method should be called by any parent class wishing to draw
    //      the movement bars
    virtual void Draw();

    // Method: GetID
    //      Returns the unique ID for the scene object.
    Proteus::Core::s32 GetID() const { return mID; }


public:
    // Variable: position
    //      The objects position
    Proteus::Math::prVector3 position;

    // Variable: rotation
    //      The objects rotation
    Proteus::Math::prVector3 rotation;

    // Variable: scale
    //      The objects scale
    Proteus::Math::prVector3 scale;

    // Variable: name
    //      The objects name
    prString    name;

    // Variable: tag
    //      The objects tag
    prString    tag;

    // Variable: colour
    //      The objects colour
    prColour    colour;

    Proteus::Core::PRBOOL      wireframe;
    Proteus::Core::PRBOOL      lit;
    Proteus::Core::PRBOOL      enabled;
    Proteus::Core::PRBOOL      selected;


private:
    static Proteus::Core::s32  sID;                // As names are not unique. The ID will be
    Proteus::Core::s32         mID;
};


#endif//__PRSCENEOBJECT_H
