/**
 * prVector3.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include <stdio.h>
#include "prVector3.h"


const prVector3 prVector3 :: Zero   (0.0f, 0.0f, 0.0f);
const prVector3 prVector3 :: One    (1.0f, 1.0f, 1.0f);
const prVector3 prVector3 :: UnitX  (1.0f, 0.0f, 0.0f);
const prVector3 prVector3 :: UnitY  (0.0f, 1.0f, 0.0f);
const prVector3 prVector3 :: UnitZ  (0.0f, 0.0f, 1.0f);


// ----------------------------------------------------------------------------
// Returns the points data as a string.
// ----------------------------------------------------------------------------
const char *prVector3::ToString() const
{
    static char buffer[256];

#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%f, %f, %f", x, y, z);
#else
    sprintf(buffer, "%f, %f, %f", x, y, z);
#endif

    return buffer;
}
