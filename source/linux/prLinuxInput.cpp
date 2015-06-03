/**
 * prLinuxInput.cpp
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


#if defined(PLATFORM_LINUX)


#include "prLinuxInput.h"
#include "../core/prCore.h"
#include "../debug/prTrace.h"
#include "../input/prMouse.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// A wrapper between input and the engine.
/// ---------------------------------------------------------------------------
void prLinuxUpdateMouse(int x, int y, unsigned int flags)
{
    static int c=0;
//    prTrace(LogError, "%i, %i - %i\n",x,y,flags);

    switch(flags)
    {
    case 1:
        flags = MOUSE_BUTTON_LEFT;
        prTrace(LogError, "Left %i - %i\n",c++, flags);
        break;

    case 2:
        flags = MOUSE_BUTTON_MIDDLE;
        prTrace(LogError, "Mid: %i - %i\n",c++, flags);
        break;

    case 3:
        flags = MOUSE_BUTTON_RIGHT;
        prTrace(LogError, "Right %i - %i\n",c++, flags);
        break;

    default:
        flags = 0;
        break;
    }

    prMouse *pMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
    if (pMouse)
    {
        pMouse->SetMouseData(x, y, flags, true);
        pMouse->SetInRect(true);
    }
}


#endif//PLATFORM_LINUX
