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


#LOCAL_LDLIBS := -ldl -llog -lGLESv1_CM -lstdc++ -lopenal
LOCAL_LDLIBS := -ldl -llog -lGLESv1_CM -lstdc++


LOCAL_SRC_FILES :=	actor/prActor.cpp	\
	actor/prActorManager.cpp	\
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
	core/prApplication.cpp	\
	core/prApplication_Android.cpp	\
	core/prApplication_Bada.cpp	\
	core/prApplication_IOS.cpp	\
	core/prApplication_Linux.cpp	\
	core/prApplication_PC.cpp	\
	core/prArgs.cpp	\
	core/prATB.cpp	\
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
	core/prWindowProcedure.cpp	\
	core/prWindow_Android.cpp	\
	core/prWindow_Bada.cpp	\
	core/prWindow_Linux.cpp	\
	core/prWindow_PC.cpp	\
	debug/prAssert.cpp	\
	debug/prConsoleWindow.cpp	\
	debug/prDebug.cpp	\
	debug/prFps.cpp	\
	debug/prFps_PC.cpp	\
	debug/prOnScreenLogger.cpp	\
	debug/prTrace.cpp	\
	display/prBackground.cpp	\
	display/prBackgroundManager.cpp	\
	display/prColour.cpp	\
	display/prFadeManager.cpp	\
	display/prFixedWidthFont.cpp	\
	display/prGLFont.cpp	\
	display/prOglUtils.cpp	\
	display/prRenderer.cpp	\
	display/prRenderer_DX9.cpp	\
	display/prRenderer_GL11.cpp	\
	display/prRenderer_GL20.cpp	\
	display/prSprite.cpp	\
	display/prSpriteAnimation.cpp	\
	display/prSpriteAnimationSequence.cpp	\
	display/prSpriteManager.cpp	\
	display/prTexture.cpp	\
	file/prFile.cpp	\
	file/prFileManager.cpp	\
	file/prFileShared.cpp	\
	input/prMouse.cpp	\
	input/prTouch.cpp	\
	math/prMathsUtil.cpp	\
	math/prRect.cpp	\
	math/prRectF.cpp	\
	math/prVector2.cpp	\
	math/prVector3.cpp	\
	memory/prMemory.cpp	\
	tinyxml/tinystr.cpp	\
	tinyxml/tinyxml.cpp	\
	tinyxml/tinyxmlerror.cpp	\
	tinyxml/tinyxmlparser.cpp	\
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

include $(BUILD_SHARED_LIBRARY)