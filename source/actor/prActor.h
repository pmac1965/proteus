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


#pragma once


#include "../core/prTypes.h"
#include "../core/prMacros.h"
#include "../math/prVector2.h"
#include "../math/prPoint.h"


// Namespaces
namespace Proteus {
namespace Actor {


// Enum: prActorLayer
//      Actor layering
//
// Notes:
//      Actors are sorted by layer, then priority.
//
// ActorLayerBack       - Default at the back
// ActorLayerMiddle0    - Middle layer
// ActorLayerMiddle1    - Middle layer
// ActorLayerMiddle2    - Middle layer
// ActorLayerMiddle3    - Middle layer
// ActorLayerMiddle4    - Middle layer
// ActorLayerMiddle5    - Middle layer
// ActorLayerMiddle6    - Middle layer
// ActorLayerMiddle7    - Middle layer
// ActorLayerMiddle8    - Middle layer
// ActorLayerMiddle9    - Middle layer
// ActorLayerFront      - Highest layer priority
typedef enum prActorLayer
{
    ActorLayerBack,
    ActorLayerMiddle0,    
    ActorLayerMiddle1,
    ActorLayerMiddle2,
    ActorLayerMiddle3,
    ActorLayerMiddle4,
    ActorLayerMiddle5,
    ActorLayerMiddle6,
    ActorLayerMiddle7,
    ActorLayerMiddle8,
    ActorLayerMiddle9,
    ActorLayerFront

} prActorLayer;


// Class: prActor
//      Actor base class
//
// Notes:
//      More complex actors can use the actor
//      state machine to control the actors.
//      It is however optional
//
// Notes:
//      Actors default to active, visible, on screen and on
//      the ActorLayerBack layer
//
// See also:
//      <prActorStateMachine>
class prActor
{
public:
    // Friends
    friend class prActorManager;


    // Method: prActor
    //      Ctor
    //
    // Parameters:
    //      type - A user defined actor identifier
    //
    // Notes:
    //      type should be unique
    explicit prActor(Proteus::Core::s32 type);

    // Method: ~prActor
    //      Dtor
    virtual ~prActor();


    // Method: Update
    //      Update an actor
    //
    // Parameters:
    //      dt - Delta time
    virtual void Update(Proteus::Core::f32 dt) = 0;

    // Method: Draw
    //      Draw an actor
    virtual void Draw() {}

    // Method: Init
    //      Optional actor construction
    //
    // Parameters:
    //      data - The data varies according to actor type.
    virtual void Init(void *data) { PRUNUSED(data); }

    // Method: UpdateOnScreen
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
    virtual void UpdateOnScreen() {}

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
    //      Get actor state.
    //
    // Notes:
    //      Provided for state machine usage
    Proteus::Core::s32 GetState() const { return m_state; }

    // Method: SetState
    //      Set actor state.
    //
    // Notes:
    //      Provided for state machine usage
    void SetState(Proteus::Core::s32 state) { m_state = state; }

    // Method: SetPriority
    //      Actor priority.
    void SetPriority(Proteus::Core::s32 priority) { m_priority = priority; }

    // Method: GetPriority
    //      Get actor priority.
    Proteus::Core::s32 GetPriority() const { return m_priority; }

    // Method: OnCollisionEnter2D
    //      Indicates a collision has started
    //
    // Parameters:
    //      pActor - The colliding actor
    //
    // Return:
    //  A response type. The default value of -1 equals do nothing
    virtual Proteus::Core::s32 OnCollisionEnter2D(prActor *pActor) { return -1; }

    // Method: OnCollisionExit2D
    //      Indicates a collision has ended
    //
    // Parameters:
    //      pActor - The colliding actor
    //
    // Return:
    //  A response type. The default value of -1 equals do nothing
    virtual Proteus::Core::s32 OnCollisionExit2D(prActor *pActor) { return -1; }

    // Method: OnCollisionLinger2D
    //      Indicates a collision is occurring
    //
    // Parameters:
    //      pActor - The colliding actor
    //
    // Return:
    //      A response type. The default value of -1 equals do nothing
    virtual Proteus::Core::s32 OnCollisionLinger2D(prActor *pActor) { return -1; }

    // Method: SetCollisionPosition2D
    //      An overrideable function which allows the collision rectangle start x, y to be
    //      set by an actor before collision detection takes place.
    virtual void SetCollisionPosition2D() { m_colPos.x = -1; m_colPos.y = -1; }

    // Method: DebugDrawCollision2D
    //      An overrideable function which allows the collision to be drawn by the actor
    virtual void DebugDrawCollision2D() {}

    // Method: OnLineIntersect
    //      An overrideable function indicates a collision intersection
    //
    // Parameters:
    //      point - The intersection point
    virtual void OnLineIntersect2D(const Proteus::Math::prPoint2F &point) {}

    // Method: GetActorWidth
    //      Optional call to get the actor width
    //
    // Notes:
    //      The default return value is 0
    //
    // Return:
    //      User defined width
    virtual Proteus::Core::u32 GetActorWidth() const { return 0; }

    // Method: GetActorHeight
    //      Optional call to get the actor height
    //
    // Notes:
    //      The default return value is 0
    //
    // Return:
    //      User defined height
    virtual Proteus::Core::u32 GetActorHeight() const { return 0; }


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
    Proteus::Math::prVector2    m_colPos;    
    bool                        m_visible;
    bool                        m_active;
    bool                        m_destroy;
    bool                        m_onScreen;
    prActorLayer                m_layer;


private:
    static Proteus::Core::s32   m_baseid;
};


}}// Namespaces
