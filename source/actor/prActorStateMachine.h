// File: prActorStateMachine.h
//      An actor mix in class.
//
// Notes:
//      Actors can add this class to get state
//      machine functionality
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
#include "../debug/prAssert.h"


// Class: prActorStateMachine
//      An actor mix in class.
//
// Notes:
//      Actors can add this class to get state
//      machine functionality
template<typename T>
class prActorStateMachine
{
public:
    // Entity method for the finite state machine.
    typedef void (T::*prMethod)();

    // An individual entity state used in the state machine.
    typedef struct prFsmState
    {
        prMethod  entry;
        prMethod  update;
        prMethod  exit;

    } prFsmState;


public:
    // Method: prActorStateMachine
    //      Constructor
    prActorStateMachine()
    {
        mpStates    = nullptr;
        mStateCurr  = 0xFFFFFFFF;
        mStateCount = -1;
    }


    // Method: StateMachineInitialise
    //      Initialises the state machine
    //
    // Notes:
    //      Initialisation causes the defaults states entry method to
    //      be called. If one exists
    //
    // Parameters:
    //      pStates - The list of states
    //      state   - The default state
    //      count   - The number of states
    void StateMachineInitialise(prFsmState *pStates, Proteus::Core::u32 state, Proteus::Core::u32 count)
    {
        PRASSERT(pStates);
        PRASSERT(count > 0);
        PRASSERT(state < count);
        mpStates    = pStates;
        mStateCurr  = state;
        mStateCount = count;

        // Call default method entry
        prMethod method = mpStates[state].entry;
        if (method)
        {
            (static_cast<T*>(this)->*method)();
        }
    }


    // Method: StateMachineUpdate
    //      Updates the state machine
    void StateMachineUpdate()
    {
        PRASSERT(mStateCurr < mStateCount);
        PRASSERT(mStateCount > 0);
        if (mpStates)
        {
            // Get the update
            prMethod method = mpStates[mStateCurr].update;
            if (method)
            {
                (static_cast<T*>(this)->*method)();        
            }
        }
    }


    // Method: StateMachineSetState
    //      Sets the next state
    void StateMachineSetState(Proteus::Core::u32 state)
    {
        PRASSERT(mStateCurr < mStateCount);
        PRASSERT(mStateCount > 0);
        if (mStateCurr != state)
        {
            // Call old method exit
            prMethod method = mpStates[mStateCurr].exit;
            if (method)
            {
                (static_cast<T*>(this)->*method)();
            }

            // Call new method entry
            method = mpStates[state].entry;
            if (method)
            {
                (static_cast<T*>(this)->*method)();
            }

            mStateCurr = state;
        }
    }


    // Method: GetState
    //      Gets the current state.
    Proteus::Core::u32 GetState() const { return mStateCurr; }


protected:
    prFsmState         *mpStates;
    Proteus::Core::u32  mStateCurr;
    Proteus::Core::u32  mStateCount;
};
