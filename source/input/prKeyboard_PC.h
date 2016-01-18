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


#pragma once


#include "prKeyboard.h"


#if defined(PLATFORM_PC)


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

    // Method: KeyboardInjectDown
    //      Engine call. Injects the held down keys
    void KeyboardInjectDown(Proteus::Core::u32 charcode) override;

    // Method: KeyboardInjectPressed
    //      Engine call. Injects the pressed keys
    void KeyboardInjectPressed(Proteus::Core::u32 charcode) override;

    // Method: KeyboardInjectControl
    //      Engine call. Injects the controls keys
    void KeyboardInjectControl(Proteus::Core::u32 charcode) override;
};


#endif
