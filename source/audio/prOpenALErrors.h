// File: prOpenALErrors.h
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


#ifndef __PROPENALERRORS_H
#define __PROPENALERRORS_H


#if defined(PLATFORM_PC) || defined(PLATFORM_IOS) || defined(PLATFORM_BADA) || defined(PLATFORM_ANDROID) || defined(PLATFORM_LINUX)
    // Method: ALC_ErrorCheck
    //      Checks for errors
    int ALC_ErrorCheck(void *device);

    // Method: AL_ErrorCheck
    //      Checks for errors
    int AL_ErrorCheck();

    // Method: ALUT_ErrorCheck
    //      Checks for errors
    int ALUT_ErrorCheck();

    #if defined(_DEBUG) || defined(DEBUG)
        #define AL_ERROR_CHECK()                                                                \
        {                                                                                       \
            int error = AL_ErrorCheck();                                                        \
            if (error != 0)                                                                     \
            {                                                                                   \
                prTrace(LogError, "OpenAL error in '%s' at line %i\n", __FUNCTION__, __LINE__); \
            }                                                                                   \
        }

        #define ALC_ERROR_CHECK(device)                                                         \
        {                                                                                       \
            int error = ALC_ErrorCheck(device);                                                 \
            if (error != 0)                                                                     \
            {                                                                                   \
                prTrace(LogError, "OpenAL error in '%s' at line %i\n", __FUNCTION__, __LINE__); \
            }                                                                                   \
        }

        #define ALUT_ERROR_CHECK()                                                              \
        {                                                                                       \
            int error = ALUT_ErrorCheck();                                                      \
            if (error != 0)                                                                     \
            {                                                                                   \
                prTrace(LogError, "ALUT error in '%s' at line %i\n", __FUNCTION__, __LINE__);   \
            }                                                                                   \
        }

    #else
        #define AL_ERROR_CHECK()
        #define ALC_ERROR_CHECK(device)
        #define ALUT_ERROR_CHECK()

    #endif

#else
    #define AL_ERROR_CHECK()
    #define ALC_ERROR_CHECK(device)
    #define ALUT_ERROR_CHECK()

#endif


#endif//__PROPENALERRORS_H
