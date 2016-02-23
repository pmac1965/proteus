// File: prFps.h
//      Class used to calculate the frame rate.
//
// Notes:
//      This class is a core system class and should be acquired using the core
//      systems component manager.
//
// See Also:
//      <prCoreGetComponent>
//
// Warning:
//      On Android devices extensive or continuous use of this class will
//      eventually cause a thread lock on actual devices, so this class needs
//      to be used with care. At least for the moment
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


#include "../core/prTypes.h"
#include "../core/prCoreSystem.h"


// Class: prFps
//      Class used to calculate the frame rate.
//
// Warning:
//      On Android devices extensive or continuous use of this class will
//      eventually cause a thread lock on actual devices, so this class needs
//      to be used with care. At least for the moment
class prFps : public prCoreSystem
{
public:
    // Method: prFps
    //      Ctor
    prFps();

    // Method: ~prFps
    //      Dtor
    virtual ~prFps() {}

    // Method: Reset
    //      Resets the timing data, should be called on game entry, etc.
    virtual void Reset();

    // Method: Begin
    //      Begins the timing period.
    virtual void Begin() = 0;

    // Method: End
    //      Ends the timing period.
    virtual void End() = 0;

    // Method: FrameRate
    //      Returns the frames per second.
    Proteus::Core::s32 FrameRate() const { return frameRate; }

    // Method: FrameRateMin
    //      Returns the minimum ever frames per second.
    Proteus::Core::s32 FrameRateMin() const { return frameRateMin; }

    // Method: FrameRateMax
    //      Returns the maximum ever frames per second.
    Proteus::Core::s32 FrameRateMax() const { return frameRateMax; }


protected:
    Proteus::Core::s32     frames;
    Proteus::Core::s32     frameRate;
    Proteus::Core::s32     frameRateMin;
    Proteus::Core::s32     frameRateMax;
};
