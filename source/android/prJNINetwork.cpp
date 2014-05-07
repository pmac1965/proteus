/**
 * prJNINetwork.cpp
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
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prDefines.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"


namespace
{
    /// ---------------------------------------------------------------------------
    /// Makes the final network class name. 
    /// ---------------------------------------------------------------------------
    const char *prJNI_MakeNetworkClassName(const char *pClassName)
    {
        static char name[256];

        strcpy(name, "proteus/network/");
        strcat(name, pClassName);

        return name;
    }


    /// ---------------------------------------------------------------------------
    /// Finds a network class.
    /// ---------------------------------------------------------------------------
    jclass prJNI_GetNetworkClass(JNIEnv *env, const char *className, bool isAttached) 
    {
        PRASSERT(env);
        PRASSERT(className && *className);

        jclass cls = env->FindClass(prJNI_MakeNetworkClassName(className));
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
/// Initialises bluetooth
/// ---------------------------------------------------------------------------
void prJNI_BTInit()
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
        jclass cls = prJNI_GetNetworkClass(env, "Bluetooth", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "connect", "()V");
        if (!method)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get method ID %s", "connect");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call methid
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Sends data via bluetooth.
/// ---------------------------------------------------------------------------
void prJNI_BTSend(unsigned char *bytes, unsigned int arraySize)
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
        jclass cls = prJNI_GetNetworkClass(env, "Bluetooth", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "send", "([B)V");
        if (!method)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get method ID %s", "send");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return;
        }

        jbyteArray bArray = env->NewByteArray(arraySize);

        env->SetByteArrayRegion(bArray, 0, arraySize, (const jbyte *)bytes);

        // Call method
        env->CallStaticVoidMethod(cls, method, bArray);

        env->DeleteLocalRef(bArray);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }
}


/// ---------------------------------------------------------------------------
/// Determines if current machine is the server.
/// ---------------------------------------------------------------------------
bool prJNI_BTIsServer()
{
    bool result = false;

    JavaVM *pJavaVM = prJNI_GetVM();
    PRASSERT(pJavaVM);
    if (pJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!prJNI_GetEnv(&env, isAttached))
            return result;

        // Find class
        jclass cls = prJNI_GetNetworkClass(env, "Bluetooth", isAttached);
        if (!cls)
            return result;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "isServer", "()Z");
        if (!method)
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get method ID %s", "isServer");
            if (isAttached)
            {
                pJavaVM->DetachCurrentThread();
            }
            return result;
        }


        // Call method
        result = env->CallStaticBooleanMethod(cls, method);

        // And done
        if (isAttached)
        {
            pJavaVM->DetachCurrentThread();
        }
    }


    return result;
}


#endif
