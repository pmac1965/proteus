/**
 * @file       prPvr.h
 * @brief      Contains data related to using PVR files.
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
