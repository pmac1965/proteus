/**
 * prThread.cpp
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


#include "prThread.h"


// http://www.bogotobogo.com/cplusplus/multithreading_win32A.php


// http://www.yolinux.com/TUTORIALS/LinuxTutorialPosixThreads.html


/// ---------------------------------------------------------------------------
/// Ctor
/// NOT using _beginthreadex as docs state that its not allowed on
/// windows runtime for windows 8
/// ---------------------------------------------------------------------------
prThread::prThread(prThreadFunc pThreadFunction, void *pThreadData, bool suspended)
{
#if defined(PLATFORM_PC)

    // Start suspended?
    unsigned int initFlag = suspended ? CREATE_SUSPENDED : 0;

    mThread = CreateThread(NULL, 0, pThreadFunction, pThreadData, initFlag, &mThreadID);

#elif defined(PLATFORM_ANDROID)

    int result = pthread_create(&mThread, NULL, pThreadFunction, pThreadData);
    if (result)
    {
        //
        //switch(result)
        //{
        //case EAGAIN:
        //    break;
        //}
    }

#endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prThread::~prThread()
{
    Stop();
}

    
/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prThread::Resume()
{
#if defined(PLATFORM_PC)

#elif defined(PLATFORM_ANDROID)

#endif
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prThread::Stop()
{
#if defined(PLATFORM_PC)

#elif defined(PLATFORM_ANDROID)

#endif
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prThread::Sleep()
{
#if defined(PLATFORM_PC)

#elif defined(PLATFORM_ANDROID)

#endif
}


/// ---------------------------------------------------------------------------
/// 
/// ---------------------------------------------------------------------------
void prThread::Join()
{
#if defined(PLATFORM_PC)

#elif defined(PLATFORM_ANDROID)

#endif
}
