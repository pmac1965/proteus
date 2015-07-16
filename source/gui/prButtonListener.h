// File: prButtonListener.h
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


#ifndef __PRBUTTONLISTENER_H
#define __PRBUTTONLISTENER_H


#include "../math/prVector2.h"
#include "../debug/prDebug.h"


// Namespaces
namespace Proteus {
namespace Gui {


// Class: prButtonListener
//      The listener for GUI buttons
class prButtonListener
{
public:
    // Method: OnButtonPressed
    //      A method which the handling class needs to implement
    //
    // Parameters:
    //      name - The name of the button pressed
    virtual void OnButtonPressed(const char *name) = 0;

    // Method: OnButtonReleased
    //      A method which the handling class needs to implement
    //
    // Parameters:
    //      name - The name of the button released
    virtual void OnButtonReleased(const char *name) = 0;

    // Method: PreDraw
    //      An optional method which the handling class can chose to implement or ignore.
    //
    // Notes:
    //      This method is called before any drawing takes place, allowing the implementation
    //      to add its own draw code before the button is drawn
    //
    // Parameters:
    //      name - The name of the button being drawn
    //      pos  - The buttons sprites position
    virtual void PreDraw(const char *name, Proteus::Math::prVector2& pos) { PRUNUSED(name); PRUNUSED(pos); }

    // Method: PostDraw
    //      An optional method which the handling class can chose to implement or ignore
    //
    // Notes:
    //      This method is called after all drawing has taken place, allowing the implementation
    //      to add its own draw code after the button is drawn
    //
    // Parameters:
    //      name - The name of the button being drawn
    //      pos  - The buttons sprites position
    virtual void PostDraw(const char *name, Proteus::Math::prVector2& pos) { PRUNUSED(name); PRUNUSED(pos); }

    // Method: SetButtonFrame
    //      An optional method which the handling class can chose to implement or ignore
    //
    // Notes:
    //      This method is called when the button is about to set a sprite frame. You can override
    //      the frame being set and supply your own frame index
    //
    //      Return false to supply your own frame index
    //
    // Parameters:
    //      name     - The name of the button being drawn
    //      frame    - The frame being set. This will be a <prButtonStates> value
    //      newFrame - A new frame. Invalid frame indices will be ignored
    //
    // See Also:
    //      <prButtonStates>
    virtual bool SetButtonFrame(const char *name, Proteus::Core::s32 frame, Proteus::Core::s32 &newFrame) {  PRUNUSED(name);  PRUNUSED(frame);  PRUNUSED(newFrame); return true; }
};


}}// Namespaces


#endif//__PRBUTTONLISTENER_H
