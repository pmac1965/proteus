/**
 * prAccelerometer.cpp
 *
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


#include "prAccelerometer.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"
#include "../debug/prAssert.h"
#include "../core/prCore.h"
#include "../core/prCoreSystem.h"


//using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// Implementation data
/// ---------------------------------------------------------------------------
typedef struct AccelerometerImplementation
{
    AccelerometerImplementation()
    {
        cx = 0.0f;
        cy = 0.0f;
        cz = 0.0f;
        px = 0.0f;
        py = 0.0f;
        pz = 0.0f;
        dx = 0.0f;
        dy = 0.0f;
        dz = 0.0f;
    }
    
    f32 cx, cy, cz;
    f32 px, py, pz;
    f32 dx, dy, dz;
    
} AccelerometerImplementation;


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prAccelerometer::prAccelerometer() : prCoreSystem(PRSYSTEM_ACCELEROMETER, "prAccelerometer")
                                   , pImpl (new AccelerometerImplementation())
                                   , imp   (*pImpl)
{
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prAccelerometer::~prAccelerometer()
{
    PRSAFE_DELETE(pImpl);
}


/// ---------------------------------------------------------------------------
/// Update
/// ---------------------------------------------------------------------------
void prAccelerometer::Update(f32 x, f32 y, f32 z)
{
    if (pImpl)
    {
        imp.px = imp.cx;
        imp.py = imp.cy;
        imp.pz = imp.cz;

        imp.cx = x;
        imp.cy = y;
        imp.cz = z;
        
        imp.dx = imp.cx - imp.px;
        imp.dy = imp.cy - imp.py;
        imp.dz = imp.cz - imp.pz;        
    }
}
 

/// ---------------------------------------------------------------------------
/// Get accelerometer values.
/// ---------------------------------------------------------------------------
void prAccelerometer::GetActual(f32 &x, f32 &y, f32 &z) const
{
    if (pImpl)
    {
        x = imp.cx;
        y = imp.cy;
        z = imp.cz;
    }
}

    
/// ---------------------------------------------------------------------------
/// Get accelerometer values.
/// ---------------------------------------------------------------------------
void prAccelerometer::GetDifference(f32 &x, f32 &y, f32 &z) const
{
    if (pImpl)
    {
        x = imp.dx;
        y = imp.dy;
        z = imp.dz;
    }
}
