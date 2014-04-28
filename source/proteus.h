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


// The config file
#include "prConfig.h"


// External code
#include "Box2D/Box2D.h"
#include "tinyxml/tinyxml.h"
#include "tool/AntTweakBar.h"
#include "zlib/zlib.h"


// Engine includes
#include "achievements/prAchievementManager.h"
#include "achievements/prLeaderboards.h"
#include "actor/prActor.h"
#include "actor/prActorManager.h"
#include "adverts/prAdvertProvider.h"
#include "adverts/prAdvertProvider_AdMob.h"
#include "adverts/prAdvertProvider_iAds.h"
#include "adverts/prAdverts.h"
#include "analytics/prAnalytics.h"
#include "analytics/prAnalyticsBase.h"
#include "analytics/prAnalyticsFlurry.h"
#include "android/prJNIInterface.h"
#include "audio/prOpenALDeviceList.h"
#include "audio/prOpenALErrors.h"
#include "audio/prSoundManager.h"
#include "audio/prSoundManagerShared.h"
#include "audio/prSoundManager_Ios.h"
#include "audio/prSoundManager_PC.h"
#include "audio/prWaves.h"
//#include "audio/external/codec.h"
//#include "audio/external/ogg.h"
//#include "audio/external/os_types.h"
//#include "audio/external/vorbisenc.h"
//#include "audio/external/vorbisfile.h"
#include "collision/prLine.h"
#include "core/prApplication.h"
#include "core/prApplication_Android.h"
#include "core/prApplication_Bada.h"
#include "core/prApplication_IOS.h"
#include "core/prApplication_Linux.h"
#include "core/prApplication_PC.h"
#include "core/prArgs.h"
#include "core/prATB.h"
#include "core/prBitArray.h"
#include "core/prContainers.h"
#include "core/prCore.h"
#include "core/prCoreSystem.h"
#include "core/prDefines.h"
#include "core/prList.h"
#include "core/prMacros.h"
#include "core/prMessage.h"
#include "core/prMessageManager.h"
#include "core/prRegistry.h"
#include "core/prResource.h"
#include "core/prResourceManager.h"
#include "core/prSingleton.h"
#include "core/prString.h"
#include "core/prStringShared.h"
#include "core/prStringUtil.h"
#include "core/prTypes.h"
#include "core/prVersion.h"
#include "core/prVertex.h"
#include "core/prWaypoint.h"
#include "core/prWindow.h"
#include "core/prWindowProcedure.h"
#include "core/prWindow_Android.h"
#include "core/prWindow_Bada.h"
#include "core/prWindow_Ios.h"
#include "core/prWindow_Linux.h"
#include "core/prWindow_PC.h"
#include "debug/prAssert.h"
#include "debug/prConsoleWindow.h"
#include "debug/prDebug.h"
#include "debug/prFps.h"
#include "debug/prFps_PC.h"
#include "debug/prOnScreenLogger.h"
#include "debug/prTrace.h"
#include "display/prBackground.h"
#include "display/prBackgroundManager.h"
#include "display/prBitmapFont.h"
#include "display/prCamera.h"
#include "display/prCameraManager.h"
#include "display/prColour.h"
#include "display/prFadeManager.h"
#include "display/prFixedWidthFont.h"
#include "display/prGLFont.h"
#include "display/prLookAt.h"
#include "display/prOglUtils.h"
#include "display/prPerspective.h"
#include "display/prPvr.h"
#include "display/prRenderer.h"
#include "display/prRenderer_DX9.h"
#include "display/prRenderer_GL11.h"
#include "display/prRenderer_GL20.h"
#include "display/prSprite.h"
#include "display/prSpriteAnimation.h"
#include "display/prSpriteAnimationSequence.h"
#include "display/prSpriteManager.h"
#include "display/prTexture.h"
#include "file/prFile.h"
#include "file/prFileManager.h"
#include "file/prFileShared.h"
#include "file/prFileSystem.h"
#include "gui/prButton.h"
#include "gui/prButtonListener.h"
#include "gui/prGui.h"
#include "gui/prWidget.h"
#include "inAppPurchase/prInAppPurchase.h"
#include "inAppPurchase/prStore.h"
#include "inAppPurchase/prStore_android.h"
#include "inAppPurchase/prStore_bada.h"
#include "inAppPurchase/prStore_ios.h"
#include "inAppPurchase/prStore_pc.h"
#include "inAppPurchase/prTransactionResult.h"
#include "input/prAccelerometer.h"
#include "input/prMouse.h"
#include "input/prTouch.h"
#include "input/prTouchListener.h"
#include "ios/prIos.h"
#include "ios/prIosAudio.h"
#include "linux/prLinux.h"
#include "locale/prLanguage.h"
#include "locale/prLocales.h"
#include "math/prMathsUtil.h"
#include "math/prPoint.h"
#include "math/prRandom.h"
#include "math/prRect.h"
#include "math/prRectF.h"
#include "math/prVector2.h"
#include "math/prVector3.h"
#include "memory/prMemory.h"
#include "memory/prMemoryPool.h"
#include "multiplayer/prGameSession.h"
#include "multiplayer/prGameSessionAndroid_BT.h"
#include "multiplayer/prGameSessionProvider.h"
#include "math/prRect.h"
#include "particle/prEmitter.h"
#include "particle/prParticle.h"
#include "particle/prParticleManager.h"
#include "thread/prMutex.h"
#include "thread/prThread.h"
#include "utf8proc/utf8proc.h"


// Include the PC libraries
#if defined(PLATFORM_PC)

    // OpenGL libs
    #pragma comment(lib, "opengl32.lib")
    #pragma comment(lib, "glu32.lib")

    // DirectX
    #if defined(ALLOW_DIRECTX)
      #pragma comment(lib, "dxguid.lib")
      #pragma comment(lib, "dinput8.lib")
      #pragma comment(lib, "dxerr.lib")
    #endif

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
        #if defined(_DEBUG)
            #pragma comment(lib, "proteus_td.lib")
        #else
            #pragma comment(lib, "proteus_tr.lib")
        #endif
    #else
        #if defined(_DEBUG)
            #pragma comment(lib, "proteus_d.lib")
        #else
            #pragma comment(lib, "proteus_r.lib")
        #endif
    #endif

#endif//PLATFORM_PC


#endif//__PROTEUS_H
