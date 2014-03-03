// File: prTouch.h
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


#ifndef __PRTOUCH_H
#define __PRTOUCH_H


#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Forward declarations
struct TouchImplementation;
class  ITouchListener;


// Defines.
#define MAX_TOUCHPOINTS     32              // The number of simultaneous touch events which can be handled. 


// Struct: prTouchEvent
//      This describes a single touch event.
//
//       type       - Type of event. 
//       x          - Absolute position.
//       y          - Absolute position.
//       rx         - Relative movement amount.
//       ry         - Relative movement amount.
//       index      - This events index. 0 to MAX_TOUCHPOINTS - 1
//       id         - This events ID.
//
// See Also:
//      <prTouchEventTypes>
typedef struct prTouchEvent
{
    s32 type;  
    s32 x;     
    s32 y;     
    s32 rx;    
    s32 ry;    
    s32 index; 
    u32 id;    

} prTouchEvent;


// Class: prTouch
//      Class designed to handle all touch input.
//
// Notes:
//      This class is created, updated and destroyed by the engine.
//      To use this class you just need to register a listener
//      with the touch system.
//
// Notes:
//      Please remember to unregister the listener when your done.
//
// See Also:
//      <ITouchListener>
class prTouch : public prCoreSystem
{
public:
    // Enum : prTouchEventTypes
    //      The touch event types
    //
    //  - A touch event has ended.
    //  - A touch event has begun.
    //  - A current touch event has changed position.
    enum prTouchEventTypes
    {
        Released,
        Pressed,
        Axis
    };


public:
    // Method: prTouch
    //      Ctor
    prTouch();

    // Method: ~prTouch
    //      Dtor
    ~prTouch();

    // Method: Update
    //      Updates the touch input system.
    //
    // Notes:
    //      *This method is called by the game engine. Do not call*
    void Update();

    // Method: TouchCount
    //      Returns the number of simultaneous touch events occurring.
    s32 TouchCount() const;

    // Method: RegisterListener
    //      Registers a listener with the touch system.
    //
    // Parameters:
    //      listener - The listener to add.
    void RegisterListener(ITouchListener *listener);

    // Method: UnregisterListener
    //      Unregisters a listener with the touch system.
    //
    // Parameters:
    //      listener - The listener to remove.
    void UnregisterListener(ITouchListener *listener);

    // Method: ClearListeners
    //      Removes all listeners in the touch system.
    void ClearListeners();

    // Method: TouchBegin
    //      Class update method.
    //
    // Notes:
    //      *This method is called by the game engine. Do not call*
    void TouchBegin(u32 id, float x, float y);

    // Method: TouchMove
    //      Class update method.
    //
    // Notes:
    //      *This method is called by the game engine. Do not call*
    void TouchMove(u32 id, float x, float y);

    // Method: TouchEnd
    //      Class update method.
    //
    // Notes:
    //      *This method is called by the game engine. Do not call*
    void TouchEnd(u32 id, float x, float y);

    // Method: GetListenerCount
    //      Gets the listener count.
    s32 GetListenerCount() const;


private:

    // Don't change order.
    TouchImplementation *pImpl;
    TouchImplementation &imp;
};


#endif//__PRTOUCH_H
