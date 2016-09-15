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
#include "../input/prKeyboard.h"


using namespace Proteus::Core;


/// ---------------------------------------------------------------------------
/// A wrapper between mouse input and the engine.
/// ---------------------------------------------------------------------------
void prLinuxUpdateMouse(int x, int y, unsigned int flags, int pressed)
{
    switch(flags)
    {
    case 1:
        flags = MOUSE_BUTTON_LEFT;
        //prTrace(LogError, "Left: (%i, %i), flags: %i, pressed: %i\n",x,y,flags,pressed ? true : false);
        break;

    case 2:
        flags = MOUSE_BUTTON_MIDDLE;
        //prTrace(LogError, "Mid: (%i, %i), flags: %i, pressed: %i\n",x,y,flags,pressed ? true : false);
        break;

    case 3:
        flags = MOUSE_BUTTON_RIGHT;
        //prTrace(LogError, "Right: (%i, %i), flags: %i, pressed: %i\n",x,y,flags,pressed ? true : false);
        break;

    default:
        flags = 0;
        break;
    }

    prMouse *pMouse = static_cast<prMouse *>(prCoreGetComponent(PRSYSTEM_MOUSE));
    if (pMouse)
    {
        pMouse->SetMouseData(x, y, flags, pressed ? true : false);
        pMouse->SetInRect(true);
    }
}


/// ---------------------------------------------------------------------------
/// A wrapper between keyboard input and the engine
/// ---------------------------------------------------------------------------
void prLinuxUpdateKeyboard(unsigned int keycode, int pressed)
{
	prKeyboard *pKeyboard  = static_cast<prKeyboard *>(prCoreGetComponent(PRSYSTEM_KEYBOARD));
	if (pKeyboard)
	{
		if (pressed)
		{
			pKeyboard->KeyboardInjectPressed(keycode);
		}
		else
		{
			//pKeyboard->K(keycode);
		}
		//pKeyboard->
	}
}

#endif//PLATFORM_LINUX
