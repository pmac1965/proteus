// File: prGameTime.h
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


#ifndef __PRGAMETIME_H
#define __PRGAMETIME_H


#include "../core/prTypes.h"


// Forward declarations
struct GameTimeImplementation;


// Class: prGameTimer
//      Cross platform game timing.
class prGameTime
{
public:
    // Method: prGameTime
    //      Ctor
    prGameTime();

    // Method: ~prGameTime
    //      Dtor
    ~prGameTime();

    // Method: Update
    //      Update the game time
    void Update();

    // Method: RunTime
    //      Gets the total runtime in milliseconds
    f32 RunTime() const;

    // Method: ElapsedTime
    //      Gets the elapsed time bewteen 1 frame and the next.
    f32 ElapsedTime() const;


private:
    // Don't change order.
	GameTimeImplementation	*pImpl;
	GameTimeImplementation	&imp;
};


#endif//__PRGAMETIME_H
