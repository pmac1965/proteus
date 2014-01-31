/**
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


#ifndef __PROTEUS_H
#define __PROTEUS_H


#include "prConfig.h"

#include "Box2D/Box2D.h"
#include "tinyxml/tinyxml.h"
#include "tool/AntTweakBar.h"
#include "zlib/zlib.h"

#include "core/prApplication.h"
#include "core/prApplication_PC.h"
#include "core/prCore.h"
#include "core/prCoreSystem.h"
#include "core/prDefines.h"
#include "core/prMacros.h"
#include "core/prMessage.h"
#include "core/prMessageManager.h"
#include "core/prRegistry.h"
#include "core/prString.h"
#include "core/prStringShared.h"
#include "core/prStringUtil.h"
#include "core/prTypes.h"
#include "core/prVersion.h"
#include "core/prVertex.h"
#include "core/prWaypoint.h"
#include "core/prWindow.h"
#include "core/prWindowProcedure.h"
#include "core/prWindow_PC.h"
#include "debug/prAssert.h"
#include "debug/prConsoleWindow.h"
#include "debug/prDebug.h"
#include "debug/prTrace.h"
#include "display/prColour.h"
#include "display/prOglUtils.h"
#include "display/prRenderer.h"
#include "display/prRenderer_DX9.h"
#include "display/prRenderer_GL11.h"
#include "display/prRenderer_GL20.h"
#include "input/prMouse.h"
#include "math/prMathsUtil.h"
#include "math/prPoint.h"
#include "math/prVector2.h"
#include "math/prVector3.h"


// Include the PC libraries
#if defined(PLATFORM_PC)

    // OpenGL libs
    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")

    // DirectX
    #pragma comment(lib, "dxguid.lib")
    #pragma comment(lib, "dinput8.lib")
    #pragma comment(lib, "dxerr.lib")

    // OpenAL libs
    #if defined(SOUND_ALLOW)
        #pragma comment(lib, "OpenAl32.lib")
    #endif

    // Tools
    #if defined(PROTEUS_USE_ANT_TWEAK_BAR)
        #pragma comment(lib, "AntTweakBar.lib")
    #endif

    // Engine libs
    #if defined(PROTEUS_TOOL)
        #pragma comment(lib, "proteus_t.lib")
    #else
        #if defined(_DEBUG)
            #pragma comment(lib, "proteus_d.lib")
        #else
            #pragma comment(lib, "proteus_r.lib")
        #endif
    #endif

#endif//PLATFORM_PC


#endif//__PROTEUS_H
