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


#pragma once


// NULL
#ifndef NULL
#ifdef __cplusplus
#define NULL                0
#else
#define NULL                ((void*)0)
#endif
#endif


// Lower case NULL
#ifndef null
#define null                NULL
#endif


// FALSE
#ifndef FALSE
#define FALSE               0
#endif


// TRUE
#ifndef TRUE
#define TRUE                1
#endif


// YES
#ifndef YES
#define YES                 TRUE
#endif


// NO
#ifndef NO
#define NO                  FALSE
#endif


// Generic success
#ifndef SUCCESS
#define SUCCESS             TRUE
#endif


// Generic failure
#ifndef FAILURE
#define FAILURE             FALSE
#endif


// On/Off
#define ON                  TRUE
#define OFF                 FALSE


// The missing virtual key defines.
#define VK_A                65
#define VK_B                66
#define VK_C                67
#define VK_D                68
#define VK_E                69
#define VK_F                70
#define VK_G                71
#define VK_H                72
#define VK_I                73
#define VK_J                74
#define VK_K                75
#define VK_L                76
#define VK_M                77
#define VK_N                78
#define VK_O                79
#define VK_P                80
#define VK_Q                81
#define VK_R                82
#define VK_S                83
#define VK_T                84
#define VK_U                85
#define VK_V                86
#define VK_W                87
#define VK_X                88
#define VK_Y                89
#define VK_Z                90
#define VK_0                48
#define VK_1                49
#define VK_2                50
#define VK_3                51
#define VK_4                52
#define VK_5                53
#define VK_6                54
#define VK_7                55
#define VK_8                56
#define VK_9                57
