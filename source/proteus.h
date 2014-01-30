/**
* @file       proteus.h
* @brief      Contains the engines includes
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
