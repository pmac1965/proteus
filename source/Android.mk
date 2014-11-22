# ----------------------------------------------------------------------------------------------
#
#      Description     - Contains an android .mk file for compiling the proteus engine.
#      Author          - Paul Micheal McNab.
#      Copyright       - Copyright Paul Micheal McNab. All rights reserved.
#
# ----------------------------------------------------------------------------------------------


# ----------------------------------------------------------------------------------------------
#
# NOTES -	The define NO_LOCALECONV was added because android does not fully support locale.h
#               and does not have the localeconv function.
# NOTES -	You must not have code with exceptions or RTTI as android doesn't support it.
#
#  ndk-build NDK_DEBUG=1      - generate debuggable native code.
#  ndk-build V=1              - launch build, displaying build commands.
#  ndk-build -B               - force a complete rebuild.
#  ndk-build -B V=1           - force a complete rebuild and display build commands.
#
# ----------------------------------------------------------------------------------------------


LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)


LOCAL_MODULE := Proteus


ifeq ($(APP_OPTIM),debug)
  DEBUG_FLAGS := -DDEBUG -DNO_LOCALECONV
else
  DEBUG_FLAGS := -DNDEBUG -DNO_LOCALECONV
endif


LOCAL_CFLAGS   += $(DEBUG_FLAGS) -fno-rtti -fno-exceptions


LOCAL_LDLIBS := -ldl -llog -lGLESv1_CM -lstdc++ -lEGL -lGLESv2
LOCAL_C_INCLUDES := D:/engine/freetype2/include


LOCAL_SRC_FILES :=		achievements/prAchievementManager.cpp	\
	achievements/prAchievement_android.cpp	\
	achievements/prLeaderboards.cpp	\
	actor/prActor.cpp	\
	actor/prActorManager.cpp	\
	adverts/prAdvertProvider.cpp	\
	adverts/prAdvertProvider_AdMob.cpp	\
	adverts/prAdvertProvider_Flurry.cpp	\
	adverts/prAdverts.cpp	\
	analytics/prAnalytics.cpp	\
	analytics/prAnalyticsBase.cpp	\
	analytics/prAnalyticsFlurry.cpp	\
	android/prJNIAdverts.cpp	\
	android/prJNIAudio.cpp	\
	android/prJNIInterface.cpp	\
	android/prJNINetwork.cpp	\
	android/prJNIFacebook.cpp	\
	android/prJNITwitter.cpp	\
	android/prJNISleep.cpp	\
	android/prJNIInAppPurchase.cpp	\
	android/prJNIAnalytics.cpp	\
	audio/prOpenALErrors.cpp	\
	audio/prSoundManager.cpp	\
	audio/prSoundManagerShared.cpp	\
	audio/prSoundManager_Android.cpp	\
	audio/prWaves.cpp	\
	Box2D/Collision/b2BroadPhase.cpp	\
	Box2D/Collision/b2CollideCircle.cpp	\
	Box2D/Collision/b2CollideEdge.cpp	\
	Box2D/Collision/b2CollidePolygon.cpp	\
	Box2D/Collision/b2Collision.cpp	\
	Box2D/Collision/b2Distance.cpp	\
	Box2D/Collision/b2DynamicTree.cpp	\
	Box2D/Collision/b2TimeOfImpact.cpp	\
	Box2D/Collision/Shapes/b2ChainShape.cpp	\
	Box2D/Collision/Shapes/b2CircleShape.cpp	\
	Box2D/Collision/Shapes/b2EdgeShape.cpp	\
	Box2D/Collision/Shapes/b2PolygonShape.cpp	\
	Box2D/Common/b2BlockAllocator.cpp	\
	Box2D/Common/b2Draw.cpp	\
	Box2D/Common/b2Math.cpp	\
	Box2D/Common/b2Settings.cpp	\
	Box2D/Common/b2StackAllocator.cpp	\
	Box2D/Common/b2Timer.cpp	\
	Box2D/Dynamics/b2Body.cpp	\
	Box2D/Dynamics/b2ContactManager.cpp	\
	Box2D/Dynamics/b2Fixture.cpp	\
	Box2D/Dynamics/b2Island.cpp	\
	Box2D/Dynamics/b2World.cpp	\
	Box2D/Dynamics/b2WorldCallbacks.cpp	\
	Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp	\
	Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp	\
	Box2D/Dynamics/Contacts/b2CircleContact.cpp	\
	Box2D/Dynamics/Contacts/b2Contact.cpp	\
	Box2D/Dynamics/Contacts/b2ContactSolver.cpp	\
	Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp	\
	Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp	\
	Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp	\
	Box2D/Dynamics/Contacts/b2PolygonContact.cpp	\
	Box2D/Dynamics/Joints/b2DistanceJoint.cpp	\
	Box2D/Dynamics/Joints/b2FrictionJoint.cpp	\
	Box2D/Dynamics/Joints/b2GearJoint.cpp	\
	Box2D/Dynamics/Joints/b2Joint.cpp	\
	Box2D/Dynamics/Joints/b2MotorJoint.cpp	\
	Box2D/Dynamics/Joints/b2MouseJoint.cpp	\
	Box2D/Dynamics/Joints/b2PrismaticJoint.cpp	\
	Box2D/Dynamics/Joints/b2PulleyJoint.cpp	\
	Box2D/Dynamics/Joints/b2RevoluteJoint.cpp	\
	Box2D/Dynamics/Joints/b2RopeJoint.cpp	\
	Box2D/Dynamics/Joints/b2WeldJoint.cpp	\
	Box2D/Dynamics/Joints/b2WheelJoint.cpp	\
	Box2D/Rope/b2Rope.cpp	\
	collision/prLine.cpp	\
	core/prApplication.cpp	\
	core/prApplication_Android.cpp	\
	core/prArgs.cpp	\
	core/prATB.cpp	\
	core/prBitArray.cpp	\
	core/prCore.cpp	\
	core/prCoreSystem.cpp	\
	core/prMessageManager.cpp	\
	core/prRegistry.cpp	\
	core/prResource.cpp	\
	core/prResourceManager.cpp	\
	core/prString.cpp	\
	core/prStringUtil.cpp	\
	core/prVersion.cpp	\
	core/prWindow.cpp	\
	core/prWindow_Android.cpp	\
	debug/prAssert.cpp	\
	debug/prAssert_Android.cpp	\
	debug/prDebug.cpp	\
	debug/prFps.cpp	\
	debug/prFps_Android.cpp	\
	debug/prOnScreenLogger.cpp	\
	debug/prProfileEntry.cpp	\
	debug/prProfileManager.cpp	\
	debug/prTrace.cpp	\
	display/prBackground.cpp	\
	display/prBackgroundManager.cpp	\
	display/prBitmapFont.cpp	\
	display/prCamera.cpp	\
	display/prCameraManager.cpp	\
	display/prColour.cpp	\
	display/prFadeManager.cpp	\
	display/prFixedWidthFont.cpp	\
	display/prLookAt.cpp	\
	display/prOglUtils.cpp	\
	display/prPerspective.cpp	\
	display/prRenderer.cpp	\
	display/prRenderer_GL11.cpp	\
	display/prRenderer_GL20.cpp	\
	display/prSprite.cpp	\
	display/prSpriteAnimation.cpp	\
	display/prSpriteAnimationSequence.cpp	\
	display/prSpriteManager.cpp	\
	display/prTexture.cpp	\
	display/prTrueTypeFont.cpp	\
	file/prFile.cpp	\
	file/prFileManager.cpp	\
	file/prFileShared.cpp	\
	file/prFileSystem.cpp	\
	font/prFontManager.cpp	\
	font/prFontGlyph.cpp	\
	font/prTagParser.cpp	\
	gui/prButton.cpp	\
	gui/prDialog.cpp	\
	gui/prGui.cpp	\
	gui/prWidget.cpp	\
	inAppPurchase/prInAppPurchase.cpp	\
	inAppPurchase/prStore.cpp	\
	inAppPurchase/prStore_android.cpp	\
	input/prAccelerometer.cpp	\
	input/prTouch.cpp	\
	locale/prLanguage.cpp	\
	locale/prLocales.cpp	\
	math/prMathsUtil.cpp	\
	math/prRandom.cpp	\
	math/prRect.cpp	\
	math/prRectF.cpp	\
	math/prVector2.cpp	\
	math/prVector3.cpp	\
	math/prSinCos.cpp	\
	math/prMatrix4.cpp	\
	math/prPlane.cpp	\
	math/prQuaternion.cpp	\
	memory/prMemory.cpp	\
	memory/prSpritePointerPool.cpp	\
	memory/prLinkedHeap.cpp	\
	memory/prStackHeap.cpp	\
	mesh/prAnimation.cpp	\
	mesh/prAnimation_MD2.cpp	\
	mesh/prMesh.cpp	\
	mesh/prMeshLoader.cpp	\
	mesh/prMesh_MD2.cpp	\
	mesh/prMesh_OBJ.cpp	\
	mesh/prNormals_MD2.cpp	\
	multiplayer/prGameSession.cpp	\
	multiplayer/prGameSessionAndroid_BT.cpp	\
	multiplayer/prGameSessionProvider.cpp	\
	multiplayer/prMultiplayer.cpp	\
	multiplayer/prMultiplayerManager.cpp	\
	multiplayer/prMultiplayerProvider.cpp	\
	network/prNetwork.cpp	\
	online/prWeb_android.cpp	\
	particle/prEmitter.cpp	\
	particle/prParticle.cpp	\
	particle/prParticleManager.cpp	\
	particle/prParticleShared.cpp	\
	persistence/prEncryption.cpp	\
	persistence/prSave.cpp	\
	persistence/prSaveBase.cpp	\
	persistence/prSave_android.cpp	\
	script/prLua.cpp	\
	script/prLuaDebug.cpp	\
	social/facebook/prFacebook.cpp	\
	social/facebook/prFacebook_Android.cpp	\
	social/twitter/prTwitter.cpp	\
	social/twitter/prTwitter_Android.cpp	\
	system/prSystem.cpp	\
	thread/prMutex.cpp	\
	thread/prThread.cpp	\
	tinyxml/tinystr.cpp	\
	tinyxml/tinyxml.cpp	\
	tinyxml/tinyxmlerror.cpp	\
	tinyxml/tinyxmlparser.cpp	\
	utf8proc/utf8proc.cpp	\
	util/prFill.cpp	\
	libzip/mkstemp.c	\
	libzip/zip_add.c	\
	libzip/zip_add_dir.c	\
	libzip/zip_close.c	\
	libzip/zip_delete.c	\
	libzip/zip_dirent.c	\
	libzip/zip_entry_free.c	\
	libzip/zip_entry_new.c	\
	libzip/zip_error.c	\
	libzip/zip_error_clear.c	\
	libzip/zip_error_get.c	\
	libzip/zip_error_get_sys_type.c	\
	libzip/zip_error_strerror.c	\
	libzip/zip_error_to_str.c	\
	libzip/zip_err_str.c	\
	libzip/zip_fclose.c	\
	libzip/zip_filerange_crc.c	\
	libzip/zip_file_error_clear.c	\
	libzip/zip_file_error_get.c	\
	libzip/zip_file_get_offset.c	\
	libzip/zip_file_strerror.c	\
	libzip/zip_fopen.c	\
	libzip/zip_fopen_index.c	\
	libzip/zip_fread.c	\
	libzip/zip_free.c	\
	libzip/zip_get_archive_comment.c	\
	libzip/zip_get_archive_flag.c	\
	libzip/zip_get_file_comment.c	\
	libzip/zip_get_name.c	\
	libzip/zip_get_num_files.c	\
	libzip/zip_memdup.c	\
	libzip/zip_name_locate.c	\
	libzip/zip_new.c	\
	libzip/zip_open.c	\
	libzip/zip_rename.c	\
	libzip/zip_replace.c	\
	libzip/zip_set_archive_comment.c	\
	libzip/zip_set_archive_flag.c	\
	libzip/zip_set_file_comment.c	\
	libzip/zip_set_name.c	\
	libzip/zip_source_buffer.c	\
	libzip/zip_source_file.c	\
	libzip/zip_source_filep.c	\
	libzip/zip_source_free.c	\
	libzip/zip_source_function.c	\
	libzip/zip_source_zip.c	\
	libzip/zip_stat.c	\
	libzip/zip_stat_index.c	\
	libzip/zip_stat_init.c	\
	libzip/zip_strerror.c	\
	libzip/zip_unchange.c	\
	libzip/zip_unchange_all.c	\
	libzip/zip_unchange_archive.c	\
	libzip/zip_unchange_data.c	\
	lua/lapi.c	\
	lua/lauxlib.c	\
	lua/lbaselib.c	\
	lua/lbitlib.c	\
	lua/lcode.c	\
	lua/lcorolib.c	\
	lua/lctype.c	\
	lua/ldblib.c	\
	lua/ldebug.c	\
	lua/ldo.c	\
	lua/ldump.c	\
	lua/lfunc.c	\
	lua/lgc.c	\
	lua/linit.c	\
	lua/liolib.c	\
	lua/llex.c	\
	lua/lmathlib.c	\
	lua/lmem.c	\
	lua/loadlib.c	\
	lua/lobject.c	\
	lua/lopcodes.c	\
	lua/loslib.c	\
	lua/lparser.c	\
	lua/lstate.c	\
	lua/lstring.c	\
	lua/lstrlib.c	\
	lua/ltable.c	\
	lua/ltablib.c	\
	lua/ltm.c	\
	lua/lundump.c	\
	lua/lvm.c	\
	lua/lzio.c	\
	zlib/adler32.c	\
	zlib/compress.c	\
	zlib/crc32.c	\
	zlib/deflate.c	\
	zlib/infback.c	\
	zlib/inffast.c	\
	zlib/inflate.c	\
	zlib/inftrees.c	\
	zlib/trees.c	\
	zlib/uncompr.c	\
	zlib/zutil.c

LOCAL_SHARED_LIBRARIES := libopenal
LOCAL_SHARED_LIBRARIES += libfreetype2

include $(BUILD_SHARED_LIBRARY)
