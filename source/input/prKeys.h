// File: prKeys.h
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


#ifndef __PRKEYS_H
#define __PRKEYS_H


#include "../prConfig.h"


#if defined(PLATFORM_PC)
    // We need these defines
    #include "../core/prDefines.h"


    #define PRKEY_BACKSPACE     8
    #define PRKEY_ENTER         13
    #define PRKEY_ESCAPE        27
    #define PRKEY_TAB           9
    #define PRKEY_A             VK_A
    #define PRKEY_B             VK_B
    #define PRKEY_C             VK_C
    #define PRKEY_D             VK_D
    #define PRKEY_E             VK_E
    #define PRKEY_F             VK_F
    #define PRKEY_G             VK_G
    #define PRKEY_H             VK_H
    #define PRKEY_I             VK_I
    #define PRKEY_J             VK_J
    #define PRKEY_K             VK_K
    #define PRKEY_L             VK_L
    #define PRKEY_M             VK_M
    #define PRKEY_N             VK_N
    #define PRKEY_O             VK_O
    #define PRKEY_P             VK_P
    #define PRKEY_Q             VK_Q
    #define PRKEY_R             VK_R
    #define PRKEY_S             VK_S
    #define PRKEY_T             VK_T
    #define PRKEY_U             VK_U
    #define PRKEY_V             VK_V
    #define PRKEY_W             VK_W
    #define PRKEY_X             VK_X
    #define PRKEY_Y             VK_Y
    #define PRKEY_Z             VK_Z


#endif//PLATFORM_PC


#endif//__PRKEYS_H
