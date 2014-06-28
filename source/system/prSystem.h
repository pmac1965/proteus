// File: prSystem.h
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


#ifndef __PRSYSTEM_H
#define __PRSYSTEM_H


#include "../core/prTypes.h"


// Class: prSystem
//      Class that abstracts system calls for single use functions,
//      like vibrate, which may not exist
class prSystem
{
public:
    // Method: prSystem
    //      Constructor.
    prSystem();

    // Method: ~prSystem
    //      Destructor.
    ~prSystem();

    // Method: SleepModeEnable
    //      Enable or disable sleep mode.
    void SleepModeEnable(bool state);

    // Method: Vibrate
    //      Vibrates the devices if possible
    void Vibrate();
};


#endif//__PRSYSTEM_H
