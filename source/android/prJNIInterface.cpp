/**
 * prJNIInterface.cpp
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
#include "prJNIInterface.h"


#if defined(PLATFORM_ANDROID)


#include <jni.h>
#include <string.h>
#include <android/log.h>
#include "../debug/prTrace.h"
#include "../debug/prAssert.h"
#include "../core/prDefines.h"
#include "../core/prStringUtil.h"
#include "../core/prMacros.h"


// Defines.
#define ANDROID_NAME_SIZE       256


// Local data.
namespace
{
    JavaVM *gJavaVM = NULL;
    char    gPackageName  [ANDROID_NAME_SIZE] = { '\0' };
    char    gActivityName [ANDROID_NAME_SIZE] = { '\0' };
    char    gProviderName [ANDROID_NAME_SIZE] = { '\0' };
    char    gAnalyticName [ANDROID_NAME_SIZE] = { '\0' };
}


/// ---------------------------------------------------------------------------
/// This is called by the device. DO NOT CALL
/// ---------------------------------------------------------------------------
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv *env;
    gJavaVM = vm;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK)
    {
        __android_log_print(ANDROID_LOG_ERROR, "Proteus", "JNI_OnLoad failed");
        return -1;
    }
    
    return JNI_VERSION_1_4;
}


/// ---------------------------------------------------------------------------
/// This is called by the device. DO NOT CALL 
/// ---------------------------------------------------------------------------
JNIEXPORT void JNI_OnUnload(JavaVM *vm, void *reserved)
{
    gJavaVM = NULL;
}


/// ---------------------------------------------------------------------------
/// Sets the package name.
/// ---------------------------------------------------------------------------
void prJNI_SetPackageName(const char *package)
{
    PRASSERT(package && *package);
    PRASSERT(prStringLength(package) < (ANDROID_NAME_SIZE - 1));
    prStringCopySafe(gPackageName, package, ANDROID_NAME_SIZE);
}


/// ---------------------------------------------------------------------------
/// Sets the activity name.
/// ---------------------------------------------------------------------------
void prJNI_SetActivityName(const char *activity)
{
    PRASSERT(activity && *activity);
    PRASSERT(prStringLength(activity) < (ANDROID_NAME_SIZE - 1));
    prStringCopySafe(gActivityName, activity, ANDROID_NAME_SIZE);
}


/// ---------------------------------------------------------------------------
/// Sets the provider name.
/// ---------------------------------------------------------------------------
void prJNI_SetProviderName(const char *provider)
{
    PRASSERT(provider && *provider);
    PRASSERT(prStringLength(provider) < (ANDROID_NAME_SIZE - 1));
    prStringCopySafe(gProviderName, provider, ANDROID_NAME_SIZE);
}


/// ---------------------------------------------------------------------------
/// Sets the analytic name.
/// ---------------------------------------------------------------------------
void prJNI_SetAnalyticName(const char *analytic)
{
    PRASSERT(analytic && *analytic);
    PRASSERT(prStringLength(analytic) < (ANDROID_NAME_SIZE - 1));
    prStringCopySafe(gAnalyticName, analytic, ANDROID_NAME_SIZE);
}


/// ---------------------------------------------------------------------------
/// Makes the final class name. 
/// ---------------------------------------------------------------------------
const char *prJNI_MakeFinalClassName(const char *pClassName)
{
    static char name[256];

    strcpy(name, gPackageName);
    strcat(name, "/");
    strcat(name, pClassName);
    __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Final %s", name);

    return name;
}


/// ---------------------------------------------------------------------------
/// Get environment.
/// ---------------------------------------------------------------------------
bool prJNI_GetEnv(JNIEnv **env, bool &isAttached)
{
    int status = gJavaVM->GetEnv((void **)env, JNI_VERSION_1_4);
    if (status < 0)
    {
        // Failed to get JNI environment. Assuming native thread.
        __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to get JNI environment. Assuming native thread.");

        status = gJavaVM->AttachCurrentThread(env, NULL);
        if (status < 0) 
        {
            __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to attach current thread");
            return false;
        }

        isAttached = true;
    }

    return true;
}


/// ---------------------------------------------------------------------------
/// Finds a class.
/// ---------------------------------------------------------------------------
jclass prJNI_GetClass(JNIEnv *env, const char *className, bool isAttached) 
{
    PRASSERT(env);
    PRASSERT(className && *className);

    jclass cls = env->FindClass(prJNI_MakeFinalClassName(className));
    if (!cls) 
    {
        // Warn
        __android_log_print(ANDROID_LOG_ERROR, "Proteus", "Failed to find class %s", className);

        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }

    return cls;
}


/// ---------------------------------------------------------------------------
/// Gets the VM
/// ---------------------------------------------------------------------------
JavaVM *prJNI_GetVM()
{
    return gJavaVM;
}


/// ---------------------------------------------------------------------------
/// Gets the package name
/// ---------------------------------------------------------------------------
const char *prJNI_GetPackageName()
{
    return gPackageName;
}


/// ---------------------------------------------------------------------------
/// Gets the activity name
/// ---------------------------------------------------------------------------
const char *prJNI_GetActivityName()
{
    return gActivityName;
}


#if 0


// ----------------------------------------------------------------------------
// Loads a sound effect.
// ----------------------------------------------------------------------------
int JNI_SFXLoad(const char *filename)
{
    int sid = 0;

    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return sid;

        // Find class
        jclass cls = JNI_GetClass(env, "Audio", isAttached);
        if (!cls)
            return sid;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "SFXLoad", "(Ljava/lang/String;)I");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return sid;
        }

        // Call.
        jstring js = env->NewStringUTF(filename);
        sid = (int)env->CallStaticIntMethod(cls, method, js);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }

    return sid;
}


// ----------------------------------------------------------------------------
// Unsupported as yet by android code.
// ----------------------------------------------------------------------------
int JNI_SFXGetState()
{
    return 0;
}


// ----------------------------------------------------------------------------
// Plays a sound effect.
// ----------------------------------------------------------------------------
int JNI_SFXPlay(int sid, float volume)
{
    int id = -1;

    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return id;

        // Find class
        jclass cls = JNI_GetClass(env, "Audio", isAttached);
        if (!cls)
            return id;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "SFXPlay", "(IF)I");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return id;
        }

        // Call.
        id = env->CallStaticIntMethod(cls, method, sid, volume);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }

    return id;
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_SFXInit()
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, "Audio", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "SFXInit", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_SFXStop(int sid)
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, "Audio", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "SFXStop", "(I)V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method, sid);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_SFXSetVolume(float volume)
{
    UNUSED(volume);
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_OpenURL(const char *url)
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, "Network", isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "OpenURL", "(Ljava/lang/String;)V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        jstring js = env->NewStringUTF(url);
        env->CallStaticVoidMethod(cls, method, js);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool JNI_ShowAchievements()
{
    bool result = false;

    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return result;

        // Find class
        jclass cls = JNI_GetClass(env, gProviderName, isAttached);
        if (!cls)
            return result;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "ShowAchievements", "()Z");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return result;
        }

        // Call.
        result = env->CallStaticBooleanMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }

    return result;
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool JNI_ShowLeaderboards()
{
    bool result = false;

    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return result;

        // Find class
        jclass cls = JNI_GetClass(env, gProviderName, isAttached);
        if (!cls)
            return result;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "ShowLeaderboards", "()Z");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return result;
        }

        // Call.
        result = env->CallStaticBooleanMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }

    return result;
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_ShowDashboard()
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, gProviderName, isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "ShowDashboard", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
bool JNI_ShowPurchaseDialog()
{
    bool result = false;

    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return result;

        // Find class
        jclass cls = JNI_GetClass(env, gProviderName, isAttached);
        if (!cls)
            return result;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "ShowPurchaseDialog", "()Z");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return result;
        }

        // Call.
        result = env->CallStaticBooleanMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }

    return result;
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_SubmitLeaderboardScore(const char *name, int score)
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, gProviderName, isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "Leaderboards_Submit", "(Ljava/lang/String;I)V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        jstring js = env->NewStringUTF(name);
        env->CallStaticVoidMethod(cls, method, js, score);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_AwardAchievement(const char *name)
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, gProviderName, isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "Achievement_Unlock", "(Ljava/lang/String;)V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID\n");
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        jstring js = env->NewStringUTF(name);
        env->CallStaticVoidMethod(cls, method, js);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_AdvertInit()
{
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_AdvertShow()
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, gActivityName, isAttached);
        if (!cls)
            return;

        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "Admob_ShowAd", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID");   	
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_AdvertHide()
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, gActivityName, isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "Admob_HideAd", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID");   	
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_SubmitFlurry(const char *str)
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, gActivityName, isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "Admob_HideAd", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID");   	
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
void JNI_SubmitFlurry(const char *str1, const char *str2)
{
    ASSERT(gJavaVM);
    if (gJavaVM)
    {
        bool    isAttached  = false;
        JNIEnv *env         = NULL;

        // Get environment.
        if (!JNI_GetEnv(&env, isAttached))
            return;

        // Find class
        jclass cls = JNI_GetClass(env, gActivityName, isAttached);
        if (!cls)
            return;
        
        // Find the callBack method ID
        jmethodID method = env->GetStaticMethodID(cls, "Admob_HideAd", "()V");
        if (!method)
        {
            prTrace(LogError, "Failed to get method ID");   	
            if (isAttached)
            {
                gJavaVM->DetachCurrentThread();
            }
            return;
        }

        // Call.
        env->CallStaticVoidMethod(cls, method);

        // And done
        if (isAttached)
        {
            gJavaVM->DetachCurrentThread();
        }
    }
}


#endif
#endif
