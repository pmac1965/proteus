// File: prThread.h
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
#ifndef __PRTHREAD_H
#define __PRTHREAD_H


#include "../prConfig.h"
#include "../core/prTypes.h"


// PC
#if defined(PLATFORM_PC)
  #include <windows.h>
  #include <process.h>

  #define PRTHREAD_CALLCONV WINAPI
  #define PRTHREAD_RETVAL   DWORD

// Android
#elif defined(PLATFORM_ANDROID)
  #include <pthread.h>

  #define PRTHREAD_CALLCONV
  #define PRTHREAD_RETVAL   void*

// IOS
#elif defined(PLATFORM_IOS)
  #include <pthread.h>

  #define PRTHREAD_CALLCONV
  #define PRTHREAD_RETVAL   void*

#else
#error Platform not supported

#endif


// Type: prThreadFunc
//      A shared thread function type
typedef PRTHREAD_RETVAL (PRTHREAD_CALLCONV *prThreadFunc)(void *);


// Class: prThread
//      A cross platfrom thread class
class prThread
{
public:
    prThread(prThreadFunc pThreadFunction, void *pThreadData, bool suspended);
    ~prThread();

	void Resume();
	void Stop();
	void Sleep();
	void Join();

    //void *Result();

private:
#if defined(PLATFORM_PC)
    HANDLE      mThread;
    DWORD       mThreadID;

#elif defined(PLATFORM_ANDROID)
    pthread_t   mThread;

#endif
};


#endif//__PRTHREAD_H
