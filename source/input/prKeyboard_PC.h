// File: prKeyboard_PC.h
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


#ifndef __PRKEYBOARD_PC_H
#define __PRKEYBOARD_PC_H


#include "prKeyboard.h"


// Class: prKeyboard_PC
//      PC Keyboard
class prKeyboard_PC : public prKeyboard
{
public:
    // Method: prKeyboard_PC
    //      Ctor
    prKeyboard_PC();
    
    // Method: ~prKeyboard_PC
    //      Dtor
    ~prKeyboard_PC();

    // Method: LastKeyPressed
    //      Returns the last key pressed
    //u32 LastKeyPressed();

    // Method: KeyboardInjectDown
    //      Engine call. Injects the held down keys
    void KeyboardInjectDown(u32 charcode);

    // Method: KeyboardInjectPressed
    //      Engine call. Injects the pressed keys
    void KeyboardInjectPressed(u32 charcode);

    // Method: KeyboardInjectControl
    //      Engine call. Injects the controls keys
    void KeyboardInjectControl(u32 charcode);
};


#endif//__PRKEYBOARD_PC_H
