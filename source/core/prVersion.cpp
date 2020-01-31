/**
 * prVersion.cpp
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


#include "../prConfig.h"
#include <cstdio>
#include "prVersion.h"
#include "../debug/prDebug.h"
#include "../debug/prAssert.h"


namespace Proteus {
namespace Core {

    s32 versionMajor    = 2020;     // Year
    s32 versionMinor    = 1;        // Month
    s32 versionRevision = 0;        // Version


    // ---------------------------------------------------------------------------
    // Acquire engine version number.
    // ---------------------------------------------------------------------------
    void prGetVersion(s32 &major, s32 &minor, s32 &revision)
    {
        major    = versionMajor;
        minor    = versionMinor;
        revision = versionRevision;
    }


    // ---------------------------------------------------------------------------
    // Acquire engine version number as a string.
    // ---------------------------------------------------------------------------
    void prGetVersionAsString(char *buffer, s32 bufferSize)
    {
        PRASSERT(buffer && *buffer, "Buffer cannot be null.");
        PRASSERT(bufferSize > 15 && bufferSize < 256, "Invalid buffer size");

        if (buffer && bufferSize > 15 && bufferSize < 256)
        {
#if defined(PLATFORM_PC)
            sprintf_s(buffer, bufferSize, "%i.%i.%i", versionMajor, versionMinor, versionRevision);
#else
            sprintf(buffer, "%i.%i.%i", versionMajor, versionMinor, versionRevision);
#endif
        }
    }

}}
