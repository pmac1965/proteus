/**
 * prVersion.cpp
 */


#include "../prConfig.h"


#include <stdio.h>
#include "prVersion.h"


namespace
{
    int versionMajor    = 2;
    int versionMinor    = 0;
    int versionRevision = 0;
}


/// ---------------------------------------------------------------------------
/// Acquire engine version number.
/// ---------------------------------------------------------------------------
void prGetVersion(int &major, int &minor, int &revision)
{
    major    = versionMajor;
    minor    = versionMinor;
    revision = versionRevision;
}


/// ---------------------------------------------------------------------------
/// Acquire engine version number as a string.
/// ---------------------------------------------------------------------------
const char *prGetVersionAsString()
{
    static char buffer[256];

#if defined(PLATFORM_BADA)
    snprintf(buffer, sizeof(buffer), "%i.%i.%i", versionMajor, versionMinor, versionRevision);
#else
    sprintf(buffer, "%i.%i.%i", versionMajor, versionMinor, versionRevision);
#endif

    return buffer;
}
