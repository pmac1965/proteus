// File: prActor.h
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


#ifndef __PRACTOR_H
#define __PRACTOR_H


#include "../core/prTypes.h"
#include "../core/prMacros.h"
#include "../math/prVector2.h"


// Class: prActor
//      Actor base class
//
// Notes:
//      More complex actors can use the internal
//      state machine to control the actors.
//      It is however optional
//
// Notes:
//      Actors default to active, visible and on screen
class prActor
{
public:
    // Entity method.
    typedef void (prActor::*prMethod)();

    // An individual entity state.
    typedef struct
    {
        prMethod  entry;
        prMethod  update;
        prMethod  exit;

    } prFsmState;


public:
    // Friends
    friend class prActorManager;

    // Method: prActor
    //      Ctor
    prActor(Proteus::Core::s32 type);

    // Method: ~prActor
    //      Dtor
    virtual ~prActor();


public:
    // Method: Update
    //      Update an actor
    virtual void Update(Proteus::Core::f32 step) = 0;

    // Method: Draw
    //      Draw an actor
    virtual void Draw() {}

    // Method: Init
    //      Optional actor construction
    //
    // Parameters:
    //      data - The data varies according to actor type.
    virtual void Init(void *data) { PRUNUSED(data); }

    // Method: UpdateScreen
    //      Override to determine on screen state.
    //
    // Notes:
    //      Since the engine knows nothing about how you'll render an
    //      actor, you'll need to override this method and set the state
    //      yourself.
    //
    //      The default value is on screen.
    //
    // See Also:
    //      <IsOnscreen>
    virtual void UpdateScreen() {}

    // Method: GetType
    //      Returns user specified type.
    Proteus::Core::s32 GetType() const { return m_type; }

    // Method: SetVisible
    //      Sets visible.
    void SetVisible(bool state) { m_visible = state; }

    // Method: SetActive
    //      Sets active.
    void SetActive(bool state) { m_active = state; }

    // Method: Destroy
    //      Destroy this actor?
    void SetDestroy() { m_destroy = true; }

    // Method: IsOnscreen
    //      Is onscreen?
    //
    // See Also:
    //      <UpdateScreen>
    bool IsOnscreen() const { return m_onScreen; }

    // Method: IsDestroyed
    //      Is destroyed?
    bool IsDestroyed() const { return m_destroy; }

    // Method: IsVisible
    //      Is visible?
    bool IsVisible() const { return m_visible; }

    // Method: IsActive
    //      Is active?
    bool IsActive() const { return m_active; }

    // Method: GetID
    //      Unique ID.
    Proteus::Core::s32 GetID() const { return m_id; }

    // Method: GetState
    //      Entity state.
    Proteus::Core::s32 GetState() const { return m_state; }

    // Method: SetState
    //      Entity state.
    void SetState(Proteus::Core::s32 state) { m_state = state; }

    // Method: SetPriority
    //      Entity priority.
    void SetPriority(Proteus::Core::s32 priority) { m_priority = priority; }

    // Method: GetPriority
    //      Get entity priority.
    Proteus::Core::s32 GetPriority() const { return m_priority; }


public:
    Proteus::Math::prVector2    pos;
    
    Proteus::Core::s32          user0;            // User data for you to do as you please
    Proteus::Core::s32          user1;            // User data for you to do as you please
    Proteus::Core::s32          user2;            // User data for you to do as you please
    Proteus::Core::s32          user3;            // User data for you to do as you please


protected:
    Proteus::Core::s32          m_type;
    Proteus::Core::s32          m_state;
    Proteus::Core::s32          m_priority;
    Proteus::Core::s32          m_id;
    bool                        m_visible;
    bool                        m_active;
    bool                        m_destroy;
    bool                        m_onScreen;


private:

    static int m_baseid;
};


#endif//__PRACTOR_H
