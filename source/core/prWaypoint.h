// File: prWaypoint.h
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


#include "prTypes.h"


// Class: prWaypoint
//      A basic way point
class prWaypoint
{
public:
    // Method: prWaypoint
    //      Constructor.
    prWaypoint() : x(0), y(0), index(0)
    {
    }
    
    // Method: prWaypoint
    //      Constructor.
    //
    // Parameters:
    //      _x     - X coordinate
    //      _y     - Y coordinate
    //      _index - Waypoints index
    prWaypoint(Proteus::Core::s32 _x, Proteus::Core::s32 _y, Proteus::Core::s32 _index)  : x(_x), y(_y), index(_index)
    {
    }

    // Method: prWaypoint
    //      Copy constructor.
    //
    // Parameters:
    //      a - Waypoint to copy
    prWaypoint(const prWaypoint &a)
    {
        x     = a.x;
        y     = a.y;
        index = a.index;
    }


public:    
    Proteus::Core::s32 x;              // public for fast access
    Proteus::Core::s32 y;
    Proteus::Core::s32 index;
};
