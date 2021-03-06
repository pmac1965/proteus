// File: prGameObject.h
//      All entities in a scene are derived from a game object
/**
 * Copyright 2016 Paul Michael McNab
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


#include "prTypes.h"
#include "prString.h"
#include "../editor/prEditorObject.h"
#include "../core/prTransform.h"
#include <list>


// Forward declarations
namespace Proteus {
namespace Actor {
    class prActorComponent;
}}


// Namespaces
namespace Proteus {
namespace Core {


// Class: prGameObject
//      Base class for all objects in a scene
class prGameObject PROT_EDITOR_INCL
{
public:
    // Method: prGameObject
    //      Ctor
    prGameObject() PROT_EDITOR_CTOR
    {}

    // Method: ~prGameObject
    //      Dtor
    virtual ~prGameObject() 
    {}

    // Method: Update
    //      Overrideable update class
    //
    // Notes:
    //      This method is optional
    virtual void Update() {}

    // Method: Draw
    //      Overrideable draw class
    //
    // Notes:
    //      This method is optional
    virtual void Draw() {}

    // Method: IsActive
    //      Determines if this game object is active
    //
    // Notes:
    //      Active game objects are updated if enabled
    inline PRBOOL IsActive() const { return mActive; }

    // Method: IsEnabled
    //      Determines if this game object is enabled
    //
    // Notes:
    //      Enabled game objects are updated if active, and drawn
    //      if visible
    inline PRBOOL IsEnabled() const { return mEnabled; }

    // Method: IsVisible
    //      Determines if this game object is visible
    //
    // Notes:
    //      Visible game objects are drawn
    inline PRBOOL IsVisible() const { return mVisible; }

    // Method: SetActive
    //      Sets a game objects active state
    //
    // Parameters:
    //      active - The new state
    inline void SetActive(PRBOOL active) { mActive = active; }

    // Method: SetEnabled
    //      Sets a game objects enabled state
    //
    // Parameters:
    //      active - The new state
    inline void SetEnabled(PRBOOL active) { mEnabled = active; }

    // Method: SetVisible
    //      Sets a game objects visible state
    //
    // Parameters:
    //      active - The new state
    inline void SetVisible(PRBOOL active) { mVisible = active; }

    // Method: AddComponent
    //      Adds a component to the game object
    //
    // Parameters:
    //      pComponent - The component to add
    void AddComponent(Proteus::Actor::prActorComponent *pComponent);

    // Method: RemoveComponent
    //      Removes a component from the game object
    //
    // Parameters:
    //      pComponent - The component to remove
    void RemoveComponent(Proteus::Actor::prActorComponent *pComponent);

    //
    void FindComponentByName(const char *name) {}

    //
    void FindComponentByType() {}

    //
    void FindComponentByTag(const char *tag) {}

    //
    void DestroyAllComponent() {}

    // serialize out
    virtual bool Save() { return false; }

    // serialize in
    virtual bool Load() { return false; }

    
private:
    prString    mName;
    prString    mTag;
    PRBOOL      mActive;
    PRBOOL      mEnabled;
    PRBOOL      mVisible;

    prTransform mTransform;

    std::list<Proteus::Actor::prActorComponent*>    mComponents;
};


}}// Namespaces
