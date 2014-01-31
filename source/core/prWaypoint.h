/**
 * @file       prWaypoint.h
 * @brief      Contains a basic waypoint.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRWAYPOINT_H
#define __PRWAYPOINT_H


#include "prTypes.h"


/// A basic way point
class prWaypoint
{
public:

    /// @brief      Constructor.
    prWaypoint()
    {
        x     = 0;
        y     = 0;
        index = 0;
    }
    
    /// @brief      Constructor.
    /// @param      _x     - X coordinate
    /// @param      _y     - Y coordinate
    /// @param      _index - Waypoints index
    prWaypoint(s32 _x, s32 _y, s32 _index)
    {
        x     = _x;
        y     = _y;
        index = _index;
    }

    /// @brief      Copy constructor.
    /// @param      a - Waypoint to copy
    prWaypoint(const prWaypoint &a)
    {
        x     = a.x;
        y     = a.y;
        index = a.index;
    }


private:
    
    s32 x;
    s32 y;
    s32 index;
};


#endif//__PRWAYPOINT_H
