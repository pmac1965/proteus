/**
 * prJNIAnalytics.cpp
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
#include "prJNIAnalytics.h"
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
    const char *prJNI_MakeAnalyticsClassName(const char *pClassName)
    {
        static char name[256];

        strcpy(name, "proteus/analytics/");
        strcat(name, pClassName);

        return name;
    }


    /// ---------------------------------------------------------------------------
    /// Finds a class.
    /// ---------------------------------------------------------------------------
    jclass prJNI_GetAnalyticsClass(JNIEnv *env, const char *className, bool isAttached) 
    {
        PRASSERT(env);
        PRASSERT(className && *className);

        jclass cls = env->FindClass(prJNI_MakeAnalyticsClassName(className));
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
/// Add an analytics message
/// ---------------------------------------------------------------------------
void prJNI_Submit(const char *message)
{
    JavaVM *pJavaVM = prJNI_GetVM();
    PRASSERT(pJavaVM);
    if (pJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = nullptr;

        // Get environment.
        if (!prJNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = prJNI_GetAnalyticsClass(env, "Flurry", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "submit", "(Ljava/lang/String;)V");
        if (!method)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get method ID %s", "submit");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Construct a Java string.
        jstring js = env->NewStringUTF(message);

        // Call.
        env->CallStaticVoidMethod(cls, method, js);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


#endif
