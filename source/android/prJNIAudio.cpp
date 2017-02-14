/**
 * prJNIAudio.cpp
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


#if defined(PLATFORM_ANDROID)


#include <jni.h>
#include <string.h>
#include <android/log.h>
#include "prJNIAudio.h"
#include "prJNIInterface.h"
#include "../debug/prDebug.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prDefines.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"


namespace
{
    /// ---------------------------------------------------------------------------
    /// Makes the final audio class name. 
    /// ---------------------------------------------------------------------------
    char *prJNI_MakeAudioClassName(char *pName, const char *pClassName)
    {
        PRASSERT(pName);
        PRASSERT(pClassName && *pClassName);

        strcpy(pName, "engine/proteus/audio/");
        strcat(pName, pClassName);

        return pName;
    }


    /// ---------------------------------------------------------------------------
    /// Finds an audio class.
    /// ---------------------------------------------------------------------------
    jclass prJNI_GetAudioClass(JNIEnv *env, const char *className, bool isAttached) 
    {
        PRASSERT(env);
        PRASSERT(className && *className);

        char name[256];

        jclass cls = env->FindClass(prJNI_MakeAudioClassName(name, className));
        if (!cls) 
        {
            prTrace(LogError, "Failed to find class %s\n", className);

            if (isAttached)
            {
                PRASSERT(prJNI_GetVM());
                prJNI_GetVM()->DetachCurrentThread();
            }
        }

        return cls;
    }
}


/// ---------------------------------------------------------------------------
/// Plays a song.
/// ---------------------------------------------------------------------------
void prJNI_SongPlay(const char *filename)
{
    JavaVM *pJavaVM = prJNI_GetVM();
    PRASSERT(pJavaVM);
    if (pJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!prJNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = prJNI_GetAudioClass(env, "Music", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "songPlay", "(Ljava/lang/String;)V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID %s", "songPlay");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Construct a Java string.
        jstring js = env->NewStringUTF(filename);
        env->CallStaticVoidMethod(cls, method, js);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Stops a song.
/// ---------------------------------------------------------------------------
void prJNI_SongStop()
{
    JavaVM *pJavaVM = prJNI_GetVM();
    PRASSERT(pJavaVM);
    if (pJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!prJNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = prJNI_GetAudioClass(env, "Music", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "songStop", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID %s", "songStop");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sets song volume.
/// ---------------------------------------------------------------------------
void prJNI_SongSetVolume(float volume)
{
    JavaVM *pJavaVM = prJNI_GetVM();
    PRASSERT(pJavaVM);
    if (pJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!prJNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = prJNI_GetAudioClass(env, "Music", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "songSetVolume", "(F)V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID %s", "songSetVolume");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method, volume);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Pauses the currently playing song.
/// ---------------------------------------------------------------------------
void prJNI_SongPause()
{
    JavaVM *pJavaVM = prJNI_GetVM();
    PRASSERT(pJavaVM);
    if (pJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!prJNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = prJNI_GetAudioClass(env, "Music", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "songPause", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID %s", "songPause");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Resumes playing the current song.
/// ---------------------------------------------------------------------------
void prJNI_SongResume()
{
    JavaVM *pJavaVM = prJNI_GetVM();
    PRASSERT(pJavaVM);
    if (pJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!prJNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = prJNI_GetAudioClass(env, "Music", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "songResume", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID %s", "songResume");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


#endif