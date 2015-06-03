// File: prKeyboard.h
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


#ifndef __PRKEYBOARD_H
#define __PRKEYBOARD_H


#include "../core/prTypes.h"
#include "../core/prCore.h"
#include "../core/prCoreSystem.h"


#define PRCTRL_KEY_NONE         0x00000000                                              // Control key
#define PRCTRL_KEY_CTRL_LEFT    0x00000001                                              // Control key
#define PRCTRL_KEY_CTRL_RIGHT   0x00000002                                              // Control key
#define PRCTRL_KEY_SHIFT_LEFT   0x00000004                                              // Control key
#define PRCTRL_KEY_SHIFT_RIGHT  0x00000008                                              // Control key
#define PRCTRL_KEY_ALT          0x00000080                                              // Control key
#define PRCTRL_KEY_CONTROL      (PRCTRL_KEY_CTRL_LEFT | PRCTRL_KEY_CTRL_RIGHT)          // Control key
#define PRCTRL_KEY_SHIFT        (PRCTRL_KEY_SHIFT_LEFT | PRCTRL_KEY_SHIFT_RIGHT)        // Control key

#define PRKEY_BUFFER_SIZE       256                                                     // Keyboard buffer size


// Class: prKeyboard
//      Cross platform interface to standard desktop keyboards
class prKeyboard : public prCoreSystem
{
public:
    // Method: prKeyboard
    //      Ctor
    prKeyboard();
    
    // Method: ~prKeyboard
    //      Dtor
    virtual ~prKeyboard();

    // Method: Update
    //      Updates the keyboard. Called by the engine. Do not call
    void Update();

    // Method: IsKeyDown
    //      Tests if the key passed is currently being held down.
    bool IsKeyDown(Proteus::Core::u32 charcode, Proteus::Core::u32 ctrlKeys = PRCTRL_KEY_NONE) const;

    // Method: IsKeyPressed
    //      Tests if the key passed has been pressed
    bool IsKeyPressed(Proteus::Core::u32 charcode, Proteus::Core::u32 ctrlKeys = PRCTRL_KEY_NONE) const;

    // Method: LastKeyPressed
    //      Returns the last key pressed
    Proteus::Core::u32 LastKeyPressed() const { return mLastKey; }

    // Method: KeyboardInjectDown
    //      Engine call. Called by the specific platform implementation.
    //      Injects the held down keys
    virtual void KeyboardInjectDown(Proteus::Core::u32 charcode) = 0;

    // Method: KeyboardInjectPressed
    //      Engine call. Called by the specific platform implementation.
    //      Injects the pressed keys
    virtual void KeyboardInjectPressed(Proteus::Core::u32 charcode) = 0;

    // Method: KeyboardInjectControl
    //      Engine call. Called by the specific platform implementation.
    //      Injects the controls keys
    virtual void KeyboardInjectControl(Proteus::Core::u32 charcode) = 0;


private:
    // Stops passing by value and assignment.
    explicit prKeyboard(const prKeyboard&);
    const prKeyboard& operator = (const prKeyboard&);


protected:
    Proteus::Core::u32 mLastKey;                               // Last key pressed
    Proteus::Core::u32 mControlKeys;                           // Control keys pressed
    Proteus::Core::u32 mPrevControlKeys;                       // Control keys pressed

    Proteus::Core::u32 mPrevKeyBuffer[PRKEY_BUFFER_SIZE];      // Keycodes for pressed buttons
    Proteus::Core::u32 mKeyBuffer    [PRKEY_BUFFER_SIZE];      // Keycodes for pressed buttons
};


#endif//__PRKEYBOARD_H
