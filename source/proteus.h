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


#pragma once


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
#include "actor/prActorStateMachine.h"


// Platform specific includes
#if defined(PLATFORM_ANDROID)
#include "android/prJNIAudio.h"
#include "android/prJNIInterface.h"
#include "android/prJNINetwork.h"
#endif


// Includes
#include "adverts/prAdvertProvider.h"
#include "adverts/prAdvertProvider_AdMob.h"
#include "adverts/prAdvertProvider_iAds.h"
#include "adverts/prAdvertProvider_Flurry.h"
#include "adverts/prAdverts.h"
#include "analytics/prAnalytics.h"
#include "analytics/prAnalyticsBase.h"
#include "analytics/prAnalyticsFlurry.h"
#include "audio/prSoundManager.h"
#include "audio/prSoundManagerShared.h"
#include "collision/prLine.h"
#include "core/prApplication.h"
#include "core/prApplication_Android.h"
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
#include "core/prSettings.h"
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
#include "core/prWindow_Ios.h"
#include "core/prWindow_Linux.h"
#include "core/prWindow_PC.h"
#include "debug/prAlert.h"
#include "debug/prAssert.h"
#include "debug/prConsoleWindow.h"
#include "debug/prDebug.h"
#include "debug/prException.h"
#include "debug/prFps.h"
#include "debug/prOnScreenLogger.h"
#include "debug/prProfileEntry.h"
#include "debug/prProfileManager.h"
#include "debug/prTrace.h"
#include "display/prBackground.h"
#include "display/prBackgroundLayer.h"
#include "display/prBackgroundManager.h"
#include "display/prBitmapFont.h"
#include "display/prCamera.h"
#include "display/prCameraManager.h"
#include "display/prColour.h"
#include "display/prFadeManager.h"
#include "display/prFixedWidthFont.h"
#include "display/prGifDecoder.h"
#include "display/prGLFont.h"
#include "display/prLookAt.h"
#include "display/prOglUtils.h"
#include "display/prPerspective.h"
#include "display/prPvr.h"
#include "display/prRenderer.h"
#include "display/prRenderer_GL11.h"
#include "display/prRenderer_GL3.h"
#include "display/prShader.h"
#include "display/prSprite.h"
#include "display/prSpriteAnimation.h"
#include "display/prSpriteAnimationSequence.h"
#include "display/prSpriteManager.h"
#include "display/prTexture.h"
#include "display/prTrueTypeFont.h"
#include "editor/prEditor.h"
#include "file/prFile.h"
#include "file/prFileManager.h"
#include "file/prFileShared.h"
#include "file/prFileSystem.h"
#include "font/prFontManager.h"
#include "gui/prButton.h"
#include "gui/prButtonListener.h"
#include "gui/prDialog.h"
#include "gui/prDialogListener.h"
#include "gui/prGui.h"
#include "gui/prMenu.h"
#include "gui/prMenuItem.h"
#include "gui/prMenuStrip.h"
#include "gui/prPane.h"
#include "gui/prWidget.h"
#include "inAppPurchase/prInAppPurchase.h"
#include "inAppPurchase/prStore.h"
//#include "inAppPurchase/prStore_android.h"
//#include "inAppPurchase/prStore_ios.h"
//#include "inAppPurchase/prStore_linux.h"
//#include "inAppPurchase/prStore_mac.h"
//#include "inAppPurchase/prStore_pc.h"
#include "inAppPurchase/prTransactionResult.h"
#include "input/prAccelerometer.h"
#include "input/prKeyboard.h"
#include "input/prKeys.h"
#include "input/prMouse.h"
#include "input/prTouch.h"
#include "input/prTouchListener.h"
//#include "ios/prBluetooth.h"
//#include "ios/prGameCenter.h"
//#include "ios/prIos.h"
//#include "ios/prIosAudio.h"
#include "linux/prLinux.h"
#include "linux/prLinuxInput.h"
#include "locale/prLanguage.h"
#include "locale/prLocales.h"
#include "math/MersenneTwister.h"
#include "math/prFixedPoint.h"
#include "math/prMathsUtil.h"
#include "math/prMatrix4.h"
#include "math/prPlane.h"
#include "math/prPoint.h"
#include "math/prQuaternion.h"
#include "math/prRandom.h"
#include "math/prRect.h"
//#include "math/prRectF.h"
#include "math/prSinCos.h"
#include "math/prVector2.h"
#include "math/prVector3.h"
#include "memory/prMemory.h"
#include "memory/prLinkedHeap.h"
#include "memory/prMemoryPool.h"
#include "memory/prSpritePointerPool.h"
#include "memory/prStackHeap.h"
#include "mesh/prAnimation.h"
#include "mesh/prAnimation_MD2.h"
#include "mesh/prMD2.h"
#include "mesh/prMesh.h"
#include "mesh/prMeshLoader.h"
#include "mesh/prMesh_MD2.h"
#include "mesh/prMesh_OBJ.h"
#include "mesh/prNormals_MD2.h"
#include "multiplayer/prGameSession.h"
#include "multiplayer/prGameSessionAndroid_BT.h"
#include "multiplayer/prGameSessionIos_BT.h"
#include "multiplayer/prGameSessionProvider.h"
#include "multiplayer/prMultiplayer.h"
#include "multiplayer/prMultiplayerManager.h"
#include "multiplayer/prMultiplayerProvider.h"
#include "network/prNetwork.h"
#include "online/prWeb.h"
#include "particle/prEmitter.h"
#include "particle/prParticle.h"
#include "particle/prParticleManager.h"
#include "particle/prParticleShared.h"
#include "persistence/prEncryption.h"
#include "persistence/prSave.h"
#include "persistence/prSaveBase.h"
#include "persistence/prSave_android.h"
#include "persistence/prSave_ios.h"
#include "persistence/prSave_linux.h"
#include "persistence/prSave_mac.h"
#include "persistence/prSave_pc.h"
#include "script/prLua.h"
#include "script/prLuaDebug.h"
#include "social/facebook/prFacebook.h"
//#include "social/facebook/prFacebookBase.h"
//#include "social/facebook/prFacebook_Android.h"
//#include "social/facebook/prFacebook_ios.h"
#include "social/twitter/prTwitter.h"
//#include "social/twitter/prTwitterBase.h"
//#include "social/twitter/prTwitter_Android.h"
//#include "social/twitter/prTwitter_ios.h"
#include "steam/prSteamManager.h"
#include "system/prSystem.h"
#include "thread/prMutex.h"
#include "thread/prThread.h"
#include "utf8proc/utf8proc.h"
#include "util/prUtility_PC.h"
#include "util/prDictionarySearch.h"


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

    // Freetype
    #if defined(ALLOW_FREETYPE)
        #if (defined(_DEBUG) || defined(DEBUG))
            #pragma comment(lib, "freetyped.lib")
        #else
            #pragma comment(lib, "freetype.lib")
        #endif
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
        #if (defined(_DEBUG) || defined(DEBUG))
            #pragma comment(lib, "proteus_td.lib")
        #else
            #pragma comment(lib, "proteus_tr.lib")
        #endif
    #else
        #if (defined(_DEBUG) || defined(DEBUG))
            #pragma comment(lib, "proteus_d.lib")
        #else
            #pragma comment(lib, "proteus_r.lib")
        #endif
    #endif

    // Steam
    #if defined(ALLOW_STEAM)
        #pragma comment(lib, "steam_api.lib")
    #endif

    // GLEW
    #if defined(ALLOW_GLEW)
		// The GLEW library has changed,  library/dll combo or static library now.
		#if defined(STATIC_GLEW)
			#pragma comment(lib, "glew32s.lib")
        #else
            #pragma comment(lib, "glew32.lib")
        #endif
    #endif

#endif//PLATFORM_PC
