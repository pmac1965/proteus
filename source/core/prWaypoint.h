// File: prWaypoint.h
//      A Basic waypoint
//
// Notes:
//      Jul 2019 - Added prev/next step through components.
//      Mar 2020 - Added inline to methods, made index private.
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
//#include "../debug/prDebug.h"


namespace Proteus {
namespace Core {

    // Class: prWaypoint
    //      A basic way point
    template<typename T>
    class prWaypoint
    {
    public:
        // Method: prWaypoint
        //      Constructor.
        prWaypoint() : x(0), y(0), z(0), m_index(0), m_Prev(nullptr), m_Next(nullptr)
        {}

        // Method: prWaypoint
        //      Constructor.
        //
        // Parameters:
        //      _x     - X coordinate
        //      _y     - Y coordinate
        //      _z     - Z coordinate
        //      _index - Waypoints index
        prWaypoint(s32 _x, s32 _y, s32 _z, s32 _index) : x(_x), y(_y), z(_z), m_index(_index), m_Prev(nullptr), m_Next(nullptr)
        {}

        // Method: GetIndex
        //      Gets the waypoints index.
        inline s32 GetIndex() const { return m_index; }

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
        inline prWaypoint* GetPrev() const { return m_Prev; }

        // Method: GetNext
        //      Gets the next waypoint in the loop.
        //
        // Returns:
        //      A waypoint pointer, or null.
        inline prWaypoint* GetNext() const { return m_Next; }

    private:
        // Stops passing by value and assignment.
        prWaypoint(const prWaypoint&);
        const prWaypoint& operator = (const prWaypoint&);

    private:
        prWaypoint* m_Prev;
        prWaypoint* m_Next;
        s32         m_index;

    public:
        T x;                          // public for fast access
        T y;
        T z;
    };
}}
