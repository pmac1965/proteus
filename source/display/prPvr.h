/**
 * @file       prPvr.h
 * @brief      Contains data related to using PVR files.
 *
 */
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


#ifndef __PRPVR_H
#define __PRPVR_H


#include "../core/prTypes.h"


/// @struct     prPVRTextureHeader
/// @brief      The PVR header structure
typedef struct prPVRTextureHeader
{
    u32     dwHeaderSize;           ///< Size of the structure 
    u32     dwHeight;               ///< Height of surface to be created 
    u32     dwWidth;                ///< Width of input surface 
    u32     dwMipMapCount;          ///< Number of MIP-map levels requested 
    u32     dwpfFlags;              ///< Pixel format flags 
    u32     dwDataSize;             ///< Size of the compress data 
    u32     dwBitCount;             ///< Number of bits per pixel 
    u32     dwRBitMask;             ///< Mask for red bit 
    u32     dwGBitMask;             ///< Mask for green bits 
    u32     dwBBitMask;             ///< Mask for blue bits 
    u32     dwAlphaBitMask;         ///< Mask for alpha channel 
    u32     dwPVR;                  ///< Should be 'P' 'V' 'R' '!' 
    u32     dwNumSurfs;             ///< Number of slices for volume textures or skyboxes

} prPVRTextureHeader;


#endif//__PRPVR_H
