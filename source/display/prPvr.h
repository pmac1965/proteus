// File: prPvr.h
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


// Typedef: prPVRTextureHeader
//      The PVR header structure
typedef struct prPVRTextureHeader
{
    Proteus::Core::u32     dwHeaderSize;           // Size of the structure 
    Proteus::Core::u32     dwHeight;               // Height of surface to be created 
    Proteus::Core::u32     dwWidth;                // Width of input surface 
    Proteus::Core::u32     dwMipMapCount;          // Number of MIP-map levels requested 
    Proteus::Core::u32     dwpfFlags;              // Pixel format flags 
    Proteus::Core::u32     dwDataSize;             // Size of the compress data 
    Proteus::Core::u32     dwBitCount;             // Number of bits per pixel 
    Proteus::Core::u32     dwRBitMask;             // Mask for red bit 
    Proteus::Core::u32     dwGBitMask;             // Mask for green bits 
    Proteus::Core::u32     dwBBitMask;             // Mask for blue bits 
    Proteus::Core::u32     dwAlphaBitMask;         // Mask for alpha channel 
    Proteus::Core::u32     dwPVR;                  // Should be 'P' 'V' 'R' '!' 
    Proteus::Core::u32     dwNumSurfs;             // Number of slices for volume textures or skyboxes

} prPVRTextureHeader;


#endif//__PRPVR_H
