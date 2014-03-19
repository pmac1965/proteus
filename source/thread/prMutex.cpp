/**
 * prMutex.cpp
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
 *
 */


#include "prMutex.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMutex::prMutex()
{
#if defined(PLATFORM_ANDROID)
    // Init lock.
    pthread_mutex_init(&m_mutex, 0);

#elif defined(PLATFORM_PC)
    // Init lock.
    InitializeCriticalSection(&m_cs);

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif
}


/// ---------------------------------------------------------------------------
/// Dtor
/// ---------------------------------------------------------------------------
prMutex::~prMutex()
{
#if defined(PLATFORM_ANDROID)
    // Destroy lock.
    pthread_mutex_destroy(&m_mutex);

#elif defined(PLATFORM_PC)
    // Destroy lock.
    DeleteCriticalSection(&m_cs);

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif
}


/// ---------------------------------------------------------------------------
/// Locks the mutex
/// ---------------------------------------------------------------------------
void prMutex::Lock()
{
#if defined(PLATFORM_ANDROID)
    // Lock
    pthread_mutex_lock(&m_mutex);

#elif defined(PLATFORM_PC)
    // Lock
    if (!TryEnterCriticalSection(&m_cs))
    {
        EnterCriticalSection(&m_cs);
    }

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif
}


// ----------------------------------------------------------------------------
// Unlocks the mutex
// ----------------------------------------------------------------------------
void prMutex::Unlock()
{
#if defined(PLATFORM_ANDROID)
    // Unlock
    pthread_mutex_unlock(&m_mutex);

#elif defined(PLATFORM_PC)
    // Unlock
    LeaveCriticalSection(&m_cs);

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_IOS) || defined(PLATFORM_LINUX))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif
}
