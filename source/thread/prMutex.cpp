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
 */


#include "prMutex.h"
#include "../debug/prTrace.h"
#include "../debug/prDebug.h"


/// ---------------------------------------------------------------------------
/// Ctor
/// ---------------------------------------------------------------------------
prMutex::prMutex()
{
#if defined(PLATFORM_ANDROID)
    // Init lock.
    if (pthread_mutex_init(&m_mutex, 0) != 0)
    {
        PRPANIC("Failed to initialise mutex");
    }

#elif defined(PLATFORM_PC)
    // Init lock.
    InitializeCriticalSection(&m_cs);

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC))  
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

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC))  
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
    EnterCriticalSection(&m_cs);

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif
}


// ----------------------------------------------------------------------------
// Trys to lock mutex 
// ----------------------------------------------------------------------------
bool prMutex::TryLock()
{
    bool locked = false;

#if defined(PLATFORM_ANDROID)
    // Lock?
    int result = pthread_mutex_trylock(&m_mutex);
    if (result == 0)
    {
        locked = true;
    }
    else
    {
        prTrace(LogError, "Failed to locked mutex\n");
    }

#elif defined(PLATFORM_PC)
    // Lock?
    if (!TryEnterCriticalSection(&m_cs))
    {
        EnterCriticalSection(&m_cs);
        locked = true;
    }
    else
    {
        prTrace(LogError, "Failed to locked mutex\n");
    }

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif

    return locked;
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

#elif (defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_LINUX) || defined(PLATFORM_MAC))  
    // Allows class to compile for the other platforms.

#else
    #error unsupported platform

#endif
}
