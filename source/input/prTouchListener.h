// File: prTouchListener.h
//      An interface file for game classes that want to receive touch input.
//
// Notes:
//      The handling class needs to register itself with the system class <prTouch>
//      in order to receive notifications
//
//      Class also need to unregister themselves when done
//
// See Also:
//      <prTouch>
//
// See Also:
//      <prTouchEvent>
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


#pragma once


#include "prTouch.h"


// Class: ITouchListener
//      An interface file for classes that want to receive touch input.
class ITouchListener
{
public:
    // Method: InputPressed
    //      Passes on a touch event.
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    virtual void InputPressed(const prTouchEvent &e) = 0;

    // Method: InputReleased
    //      Passes on a touch event.
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    virtual void InputReleased(const prTouchEvent &e) = 0;

    // Method: InputAxis
    //      Passes on a touch event.
    //
    // Parameters:
    //      e - A touch event
    //
    // See Also:
    //      <prTouchEvent>
    virtual void InputAxis(const prTouchEvent &e) = 0;
};
