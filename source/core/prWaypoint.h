// File: prWaypoint.h
//      A Basic waypoint
//
// Notes:
//      July 2019 - Added prev/next step through components.
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


namespace Proteus {
namespace Core {

    // Class: prWaypoint
    //      A basic way point
    class prWaypoint
    {
    public:
        // Method: prWaypoint
        //      Constructor.
        prWaypoint() : x(0), y(0), z(0), index(0), m_Prev(nullptr), m_Next(nullptr)
        {}

        // Method: prWaypoint
        //      Constructor.
        //
        // Parameters:
        //      _x     - X coordinate
        //      _y     - Y coordinate
        //      _z     - Z coordinate
        //      _index - Waypoints index
        prWaypoint(s32 _x, s32 _y, s32 _z, s32 _index) : x(_x), y(_y), z(_z), index(_index), m_Prev(nullptr), m_Next(nullptr)
        {}

        // Method: prWaypoint
        //      Copy constructor.
        //
        // Parameters:
        //      a - Waypoint to copy
        prWaypoint(const prWaypoint& a)
        {
            x       = a.x;
            y       = a.y;
            z       = a.z;
            index   = a.index;
            m_Prev  = a.m_Prev;
            m_Next  = a.m_Next;
        }

        // Method: SetPrev
        //      Sets the previous waypoint in the loop.
        //
        // Parameters:
        //      pPrev - The waypoint.
        inline void SetPrev(prWaypoint* pPrev) { PRASSERT(pPrev); m_Prev = pPrev; }

        // Method: SetNext
        //      Sets the next waypoint in the loop.
        //
        // Parameters:
        //      pNext - The waypoint.
        inline void SetNext(prWaypoint* pNext) { PRASSERT(pNext); m_Next = pNext; }

        // Method: GetPrev
        //      Gets the previous waypoint in the loop.
        //
        // Returns:
        //      A waypoint pointer, or null.
        prWaypoint* GetPrev() const { return m_Prev; }

        // Method: GetNext
        //      Gets the next waypoint in the loop.
        //
        // Returns:
        //      A waypoint pointer, or null.
        prWaypoint* GetNext() const { return m_Next; }


    private:
        prWaypoint* m_Prev;
        prWaypoint* m_Next;

    public:
        s32 x;                          // public for fast access
        s32 y;
        s32 z;
        s32 index;
    };
}}
