/**
 * @file       prDefines.h
 * @brief      Contains globally used defines that don't fall under a specific category.
 * @copyright  Copyright Paul Michael McNab. All rights reserved.
 *
 *//*
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef __PRDEFINES_H
#define __PRDEFINES_H


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
#define VK_A    65
#define VK_B    66
#define VK_C    67
#define VK_D    68
#define VK_E    69
#define VK_F    70
#define VK_G    71
#define VK_H    72
#define VK_I    73
#define VK_J    74
#define VK_K    75
#define VK_L    76
#define VK_M    77
#define VK_N    78
#define VK_O    79
#define VK_P    80
#define VK_Q    81
#define VK_R    82
#define VK_S    83
#define VK_T    84
#define VK_U    85
#define VK_V    86
#define VK_W    87
#define VK_X    88
#define VK_Y    89
#define VK_Z    90
#define VK_0    48
#define VK_1    49
#define VK_2    50
#define VK_3    51
#define VK_4    52
#define VK_5    53
#define VK_6    54
#define VK_7    55
#define VK_8    56
#define VK_9    57


#endif//__PRDEFINES_H
