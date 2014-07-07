/**
 * prJNIFacebook.cpp
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
#include "prJNIInterface.h"
#include "prJNIFacebook.h"
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prDefines.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"


namespace
{
    /// ---------------------------------------------------------------------------
    /// Makes the final class name. 
    /// ---------------------------------------------------------------------------
    const char *prJNI_MakeFacebookClassName(const char *pClassName)
    {
        static char name[256];

        strcpy(name, "proteus/social/");
        strcat(name, pClassName);

        return name;
    }


    /// ---------------------------------------------------------------------------
    /// Finds a class.
    /// ---------------------------------------------------------------------------
    jclass prJNI_GetFacebookClass(JNIEnv *env, const char *className, bool isAttached) 
    {
        PRASSERT(env);
        PRASSERT(className && *className);

        jclass cls = env->FindClass(prJNI_MakeFacebookClassName(className));
        if (!cls) 
        {
            // Warn
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to find class %s", className);
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
/// Opens a facebook session
/// ---------------------------------------------------------------------------
void prJNI_FacebookOpenSession()
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
        jclass cls = prJNI_GetFacebookClass(env, "Facebook", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "openSession", "()V");
        if (!method)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get method ID %s", "openSession");
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
/// Logs into facebook
/// ---------------------------------------------------------------------------
void prJNI_FacebookLogin()
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
        jclass cls = prJNI_GetFacebookClass(env, "Facebook", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "logIn", "()V");
        if (!method)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get method ID %s", "logIn");
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
/// brag to facebook
/// ---------------------------------------------------------------------------
void prJNI_FacebookBrag()
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
        jclass cls = prJNI_GetFacebookClass(env, "Facebook", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "brag", "()V");
        if (!method)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get method ID %s", "brag");
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
