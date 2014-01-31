/**
 * prVector2.cpp
 * Copyright Paul Michael McNab. All rights reserved.
 */


#include <stdio.h>
#include "prVector2.h"


const prVector2 prVector2 :: Zero   (0.0f, 0.0f);
const prVector2 prVector2 :: One    (1.0f, 1.0f);
const prVector2 prVector2 :: UnitX  (1.0f, 0.0f);
const prVector2 prVector2 :: UnitY  (0.0f, 1.0f);


/// ---------------------------------------------------------------------------
/// Returns the points data as a string.
/// ---------------------------------------------------------------------------
const char *prVector2::ToString() const
{
    static char buffer[256];

#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%f, %f", x, y);
#else
    sprintf(buffer, "%f, %f", x, y);
#endif

    return buffer;
}
