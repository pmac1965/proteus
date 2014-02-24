/**
 * prFps.cpp
 */


#include "prFps.h"
#include "../core/prCore.h"


/// ---------------------------------------------------------------------------
/// Constructor.
/// ---------------------------------------------------------------------------
prFps::prFps() : prCoreSystem(PRSYSTEM_FPS, "prFps")
{
    Reset();
}


/// ---------------------------------------------------------------------------
/// Destructor.
/// ---------------------------------------------------------------------------
prFps::~prFps()
{
}


/// ---------------------------------------------------------------------------
/// Resets the timing data, should be called on game entry, etc.
/// ---------------------------------------------------------------------------
void prFps::Reset()
{
    frames        = 0;
    frameRate     = 0;
    frameRateMin  = 1000000;
    frameRateMax  = -1000000;
}