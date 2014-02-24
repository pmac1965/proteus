// File: prActor.h
/*
    Copyright 2014 Paul Michael McNab

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
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
class prActor
{
public:
    // Entity method.
    typedef void (prActor::*prMethod)();

    // An individual entity state.
    typedef struct prFsmState
    {
        prMethod  entry;
        prMethod  update;
        prMethod  exit;
    };


public:
    // Friends
    friend class prActorManager;

    // Method: prActor
    //      Ctor
    prActor(s32 type);

    // Method: ~prActor
    //      Dtor
    virtual ~prActor();


public:
    // Method: Update
    //      Update an actor
    virtual void Update(f32 step) = 0;

    // Method: Draw
    //      Draw an actor
    virtual void Draw() = 0;

    // Method: Init
    //      Optional actor construction
    //
    // Parameters:
    //      data - The data varies according to actor type.
    virtual void Init(void *data) { PRUNUSED(data); }

    // Method: UpdateScreen
    //      Override to determine on screen state.
    //
    // See Also:
    //      <IsOnscreen>
    virtual void UpdateScreen() {}

    // Method: Type
    //      Returns user specified type.
    s32 Type() const { return m_type; }

    // Method: SetVisible
    //      Sets visible.
    void SetVisible(bool state) { m_visible = state; }

    // Method: SetActive
    //      Sets active.
    void SetActive(bool state) { m_active = state; }

    // Method: Destroy
    //      Destroy this actor?
    void Destroy() { m_destroy = true; }

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
    s32 GetID() const { return m_id; }

    // Method: GetState
    //      Entity state.
    s32 GetState() const { return m_state; }

    // Method: SetState
    //      Entity state.
    void SetState(s32 state) { m_state = state; }


public:

    prVector2 pos;


protected:

    s32     m_type;
    s32     m_state;
    s32     m_priority;     /*  Will be removed */
    s32     m_id;

    bool    m_visible;
    bool    m_active;
    bool    m_destroy;
    bool    m_onScreen;


private:

    static int m_baseid;
};


#endif//__PRACTOR_H