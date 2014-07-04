// File: prJNIInterface.h
/**
 * Copyright 2014 Paul Michael McNab
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef __PRJNIINTERFACE_H
#define __PRJNIINTERFACE_H


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)


#include <jni.h>


// Function: prJNI_SetPackageName
//      Sets the android package name
void prJNI_SetPackageName(const char *package);

// Function: prJNI_SetActivityName
//      Sets the android activity name
void prJNI_SetActivityName(const char *activity);

// Function: prJNI_SetProviderName
//      Sets the android provider name
void prJNI_SetProviderName(const char *provider);

// Function: prJNI_SetAnalyticName
//      Sets the android analytic name
void prJNI_SetAnalyticName(const char *analytic);

// Function: prJNI_MakeFinalClassName
//      Makes the final class name. 
const char *prJNI_MakeFinalClassName(const char *pClassName);

// Function: prJNI_GetEnv
//      Get java environment.
bool prJNI_GetEnv(JNIEnv **env, bool &isAttached);

// Function: prJNI_GetClass
//      Finds a class.
jclass prJNI_GetClass(JNIEnv *env, const char *className, bool isAttached);

// Function: prJNI_GetVM
//      Gets the VM
JavaVM *prJNI_GetVM();

// Function: prJNI_GetPackageName
//      Gets the package name
const char *prJNI_GetPackageName();

// Function: prJNI_GetActivityName
//      Gets the activity name
const char *prJNI_GetActivityName();


#endif//PLATFORM_ANDROID


#endif//__PRJNIINTERFACE_H
