/**
 * prJNITwitter.cpp
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
#include "prJNITwitter.h"
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
    char *prJNI_MakeTwitterClassName(char *pName, const char *pClassName)
    {
        PRASSERT(pName);
        PRASSERT(pClassName && *pClassName);

        strcpy(pName, "com/redcliffeinteractive/engine/social/");
        strcat(pName, pClassName);

        return pName;
    }


    /// ---------------------------------------------------------------------------
    /// Finds a class.
    /// ---------------------------------------------------------------------------
    jclass prJNI_GetTwitterClass(JNIEnv *env, const char *className, bool isAttached) 
    {
        PRASSERT(env);
        PRASSERT(className && *className);

        char name[256];

        jclass cls = env->FindClass(prJNI_MakeTwitterClassName(name, className));
        if (!cls) 
        {
            // Warn
            prTrace(LogError, "Failed to find class %s", className);
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
/// Opens the tweet sheet
/// ---------------------------------------------------------------------------
void prJNI_ShowTweet(const char *initialText)
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
        jclass cls = prJNI_GetTwitterClass(env, "TwitterManager", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "showTweetSheet", "(Ljava/lang/String;)V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID %s", "showTweetSheet");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Construct a Java string.
        jstring js = env->NewStringUTF(initialText);

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
