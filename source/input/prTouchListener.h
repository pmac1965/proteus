// File: prTouchListener.h
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


#ifndef __PRITOUCHLISTENER_H
#define __PRITOUCHLISTENER_H


#include "prTouch.h"


// Class: ITouchListener
//      Interface file for classes which want to receive touch input.
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


#endif//__PRITOUCHLISTENER_H
