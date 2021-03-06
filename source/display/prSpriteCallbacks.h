// File: prSpriteCallbacks.h
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


// Class: prSpriteCallbacks
//      Class to include sprite callbacks
class prSpriteCallbacks
{
public:
    // Method: SpriteCurrentFrameIndex
    //      Updates the sprites animation.
    //
    // Parameters:
    //      dt - Delta time
    virtual void SpriteCurrentFrameIndex(const char *name, s32 index) = 0;
};
