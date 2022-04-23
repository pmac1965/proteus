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
#include "../core/prString.h"
#include "../core/prGameObject.h"
#include "../core/prLayer.h"
#include "../core/prTransform.h"
#include "../math/prVector2.h"
#include "../math/prPoint.h"
#include <list>


// Namespaces
namespace Proteus {
namespace Actor {


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
//      the default layer
//
// See also:
//      <prActorStateMachine>
class prActor : public Proteus::Core::prGameObject
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
    explicit prActor(s32 type);

    // Method: ~prActor
    //      Dtor
    virtual ~prActor() {}

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
    s32 GetType() const { return m_type; }

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

    // Method: GetID
    //      Unique ID.
    s32 GetID() const { return m_id; }

    // Method: OnCollisionEnter2D
    //      Indicates a collision has started
    //
    // Parameters:
    //      pActor - The colliding actor
    //
    // Return:
    //  A response type. The default value of -1 equals do nothing
    virtual s32 OnCollisionEnter2D(prActor *pActor) { PRUNUSED(pActor); return -1; }

    // Method: OnCollisionExit2D
    //      Indicates a collision has ended
    //
    // Parameters:
    //      pActor - The colliding actor
    //
    // Return:
    //  A response type. The default value of -1 equals do nothing
    virtual s32 OnCollisionExit2D(prActor *pActor) { PRUNUSED(pActor); return -1; }

    // Method: OnCollisionLinger2D
    //      Indicates a collision is occurring
    //
    // Parameters:
    //      pActor - The colliding actor
    //
    // Return:
    //      A response type. The default value of -1 equals do nothing
    virtual s32 OnCollisionLinger2D(prActor *pActor) { PRUNUSED(pActor); return -1; }

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
    virtual void OnLineIntersect2D(const Proteus::Math::prPoint2F &point) { PRUNUSED(point); }

    // Method: GetActorWidth
    //      Optional call to get the actor width
    //
    // Notes:
    //      The default return value is 0
    //
    // Return:
    //      User defined width
    virtual u32 GetActorWidth() const { return 0; }

    // Method: GetActorHeight
    //      Optional call to get the actor height
    //
    // Notes:
    //      The default return value is 0
    //
    // Return:
    //      User defined height
    virtual u32 GetActorHeight() const { return 0; }

public:
    Proteus::Core::prTransform  transform;      // Transform data
    s32                         user0;          // User data for you to do as you please
    s32                         user1;          // User data for you to do as you please
    s32                         user2;          // User data for you to do as you please
    s32                         user3;          // User data for you to do as you please
    u32                         collision0;     // For passing additional collision info, such as ground type
    u32                         collision1;     // For passing additional collision info
    u32                         collision2;     // For passing additional collision info
    u32                         collision3;     // For passing additional collision info

protected:
    prString                    m_name;
    Proteus::Core::prLayer      m_layer;
    Proteus::Math::prVector2    m_colPos;
    s32                         m_type;
    s32                         m_id;
    bool                        m_destroy;
    bool                        m_onScreen;
    bool                        m_exp0;
    bool                        m_exp1;

    // Add colour

private:
    static s32                  m_baseid;
};


}}// Namespaces
