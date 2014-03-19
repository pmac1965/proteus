// File: prMutex.h
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


#ifndef _PRMUTEX_H
#define _PRMUTEX_H


#include "../prConfig.h"


#if defined(PLATFORM_ANDROID)
    #include <pthread.h>

#elif defined(PLATFORM_PC)
    #include <windows.h>

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif


// Class: prMutex
//      A basic mutex class.
class prMutex
{
public:
    // Method: prMutex
    //      Ctor
    prMutex();

    // Method: ~prMutex
    //      Dtor
    ~prMutex();

    // Method: Lock
    //      Locks the mutex
	void Lock();

    // Method: Unlock
    //      Unlocks the mutex.
	void Unlock();

private:

#if defined(PLATFORM_ANDROID)
    pthread_mutex_t     m_mutex;

#elif defined(PLATFORM_PC)
    CRITICAL_SECTION    m_cs;

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif
};


#endif//_PRMUTEX_H
