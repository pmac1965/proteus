// File: prActorManager.h
/**
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


#ifndef __PRACTORMANAGER_H
#define __PRACTORMANAGER_H


#include "../core/prTypes.h"
#include <list>


// Forward references
class prActor;


// Callback
typedef prActor *(*prFactoryCallback)(s32 type);


// Class: prActorManager
//      Actor management class
class prActorManager
{
public:
    // Method: prActorManager
    //      Ctor
    prActorManager();

    // Method: ~prActorManager
    //      Dtor
    ~prActorManager();

    // Method: Registerfactory
    //      Registers a function which creates the games actors.
    //
    // Parameters:
    //      cb - A function pointer
    void Registerfactory(prFactoryCallback cb);

    // Method: Create
    //      Creates an actor
    //
    // Parameters:
    //      type - A user defined type
    //
    // Returns:
    //      An actor pointer or NULL
    prActor *Create(s32 type);

    // Method: Release
    //      Releases the passed actor
    void Release(prActor *actor);

    // Method: ReleaseAll
    //      Releases all the actors
    void ReleaseAll();

    // Method: Update
    //      Updates all the actors
    //
    // Notes:
    //      This is optional
    void Update(f32 time);

    // Method: Draw
    //      Draws the actors
    //
    // Notes:
    //      This is optional. Though actors are drawn in the managers priority
    //      list order
    void Draw();

    // Method: SetPriority
    //      Set the draw order priority.
    void SetPriority(prActor *actor, s32 priority);

    // Method: Count
    //      Gets the number of actors
    s32 Count() const;

    // Method: HowMany
    //      Counts the actors of a specific type
    //
    // Parameters:
    //      type - A user defined type
    u32 HowMany(s32 type);

    // Method: FindByIndex
    //      Finds an actor of a specific type an index.
    //
    // Parameters:
    //      type  - A user defined type
    //      index - 0 to how many you have of the specific type minus 1, as we're zero based
    //
    // See Also:
    //      <HowMany>
    prActor *FindByIndex(s32 type, u32 index);


private:
    // Stops passing by value and assignment.
    prActorManager(const prActorManager&);
    const prActorManager& operator = (const prActorManager&);


private:
    // Data
    std::list<prActor *>    actors;
    prFactoryCallback       callback;
};


#endif//__PRACTORMANAGER_H
