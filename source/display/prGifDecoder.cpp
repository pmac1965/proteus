/**
 * prGifDecoder.cpp
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


#include <cstring>
#include <cstdlib>
#include "prGifDecoder.h"
#include "../file/prFile.h"
#include "../core/prCore.h"
#include "../core/prMacros.h"
#include "../core/prResourceManager.h"
#include "../debug/prTrace.h"
#include "../display/prSprite.h"
#include "../display/prTexture.h"
#include "../display/prSpriteManager.h"
#include "../math/prMathsUtil.h"


using namespace Proteus::Math;


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prGifDecoder::prGifDecoder(const char *filename)
{
    PRASSERT(filename && *filename);

    // Init
    mFif            = FIF_UNKNOWN;
    mDib            = NULL;
    mMultiBmp       = NULL;
    mpImage         = NULL;
    mpImageCopy     = NULL;
    mpSprite        = NULL;
    mpTetxure       = NULL;
    mFileSize       = 0;
    mFrameCount     = 0;
    mFrameWidth     = 0;
    mFrameHeight    = 0;
    mFrameCurrent   = GIFDECODER_NOFRAME;
    pRawImage       = NULL;
    pitch           = 0;
    imageBPP        = 0;
    imageWidth      = 0;
    imageHeight     = 0;

    // Read the gif into memory
    prFile *pFile = new prFile(filename);
    if (pFile)
    {
        if (pFile->Open())
        {
            mFileSize = pFile->Size();
            if (mFileSize > 0)
            {
                mpImage = new u8[mFileSize];
                if (mpImage)
                {
                    pFile->Read(mpImage, mFileSize);
                }

                pFile->Close();
            }
            else
            {
                PRPANIC("Gif was empty: %s", filename);
            }
        }
        else
        {
            PRPANIC("Failed to open gif: %s", filename);
        }

        PRSAFE_DELETE(pFile);
    }

    // Get gif details
    if (mpImage && mFileSize > 0)
    {
        // Attach the binary data to a memory stream
        FIMEMORY *hmem = FreeImage_OpenMemory(mpImage, mFileSize);

        // Get the image type. So we know its a gif
        mFif = FreeImage_GetFileTypeFromMemory(hmem, 0);
        if (mFif == FIF_GIF)
        {
            // Open the multipage bitmap stream as read-only
            mMultiBmp = FreeImage_LoadMultiBitmapFromMemory(mFif, hmem, 0);
            if (mMultiBmp)
            {
                // Gets frames
                mFrameCount = FreeImage_GetPageCount(mMultiBmp);


                // Use the first page to get the size
                FIBITMAP *dib = FreeImage_LockPage(mMultiBmp, 0);
                if(dib)
                {
                    mFrameWidth  = FreeImage_GetWidth(dib);
                    mFrameHeight = FreeImage_GetHeight(dib);
                    FreeImage_UnlockPage(mMultiBmp, dib, FALSE);

                    // Set texture width/height and size.
                    mTextureWidth   = mFrameWidth;
                    mTextureHeight  = mFrameHeight;

                    // Generate power of two texture size if required.
                    if (!prIsPowerOf2(mTextureWidth))
                    {
                        mTextureWidth = prNextPowerOf2(mTextureHeight);
                    }
            
                    if (!prIsPowerOf2(mTextureHeight))
                    {
                        mTextureHeight = prNextPowerOf2(mTextureHeight);
                    }

                    // Times four for RGBA
                    mTextureSize = ((mTextureWidth * mTextureHeight) * 4);
                    //prTrace("GIF.TEX: %i, %i, %i\n", mTextureWidth, mTextureHeight, mTextureSize);
                }
            }
        }
        else
        {
            PRPANIC("Image file was not a gif: %s", filename);
        }
    }
    else
    {
        PRWARN("Failed to read gif");
    }
}


/// ---------------------------------------------------------------------------
/// Constructor
/// ---------------------------------------------------------------------------
prGifDecoder::~prGifDecoder()
{
    if (mMultiBmp)
    {
        FreeImage_CloseMultiBitmap(mMultiBmp, 0);
        mMultiBmp = NULL;
    }

    PRSAFE_DELETE(mpImage);
    PRSAFE_DELETE(mpImageCopy);
    PRSAFE_DELETE_ARRAY(pRawImage);
}


/// ---------------------------------------------------------------------------
/// Decodes the current frame into an image which can be drawn
/// ---------------------------------------------------------------------------
bool prGifDecoder::DecodeFrame(u32 frame)
{
    bool result = false;

    if (mpImage && mFileSize > 0)
    {
	    FIBITMAP *dib = FreeImage_LockPage(mMultiBmp, frame);
        if (dib)
        {
            // Get image pitch
            pitch       = FreeImage_GetPitch(dib);
            imageBPP    = FreeImage_GetBPP(dib);
            imageWidth  = FreeImage_GetWidth(dib);
            imageHeight = FreeImage_GetHeight(dib);

            // Calculate raw data size and create buffer
            u8 *rawImage     = new u8[mTextureSize];
            //memset(rawImage, 0, mTextureSize);

            // Create the copy image for merging frames
            bool copyBase = false;
            if (mpImageCopy == NULL)
            {
                mpImageCopy = new u8[mTextureSize];
                //memset(mpImageCopy, 0, mTextureSize);
                copyBase = true;
            }

            // Copy data
            FIBITMAP *bmp = FreeImage_ConvertTo32Bits(dib);
            if (bmp)
            {
                FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);

                switch(type)
                {
                // Convert bitmap
                case FIT_BITMAP:
                    // 8 bit?
                    if (imageBPP == 8)
                    {
                        BYTE *raw  = (BYTE*)malloc(imageHeight * (pitch * 4));          // pitch * 4 == 8 to 32 bits
                        BYTE *bits = raw;
                        if (raw)
                        {
                            FreeImage_ConvertToRawBits(bits, bmp, pitch * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
                            BYTE *image    = rawImage;
                            BYTE *imageCpy = mpImageCopy;

                            u32 offset = (mTextureHeight - imageHeight);

                            // Put at top
                            image    += ((mTextureWidth * 4) * offset);
                            imageCpy += ((mTextureWidth * 4) * offset);

                            for(u32 y = 0; y < imageHeight; y++)
                            {
                                // Set row start
                                BYTE *pixel     = (BYTE*)bits;
                                BYTE *line      = image;
                                BYTE *lineCopy  = imageCpy;

                                for (u32 x = 0; x < imageWidth; x++)
                                {
                                    u32 colour = (pixel[FI_RGBA_RED]        ) |
                                                 (pixel[FI_RGBA_GREEN] << 8 ) |
                                                 (pixel[FI_RGBA_BLUE]  << 16) |
                                                 (pixel[FI_RGBA_ALPHA] << 24);
                                    
                                    *(u32*)line = colour;

                                    if (copyBase)
                                    {
                                        *(u32*)lineCopy = colour;
                                    }

                                    pixel    += 4;
                                    line     += 4;
                                    lineCopy += 4;
                                }

                                // Next line
                                bits += (pitch * 4);

                                // Next row
                                image    += (mTextureWidth * 4);
                                imageCpy += (mTextureWidth * 4);
                            }

                            free(raw);
                        }
                    }
                    else
                    {
                        PRPANIC("Only 8 bit gifs supported at the moment");
                    }
                    break;

                    default:
                        break;
                    }

                FreeImage_Unload(bmp);
            }

            FreeImage_UnlockPage(mMultiBmp, dib, FALSE);
            result = true;

            // Merge?
            if (!copyBase)
            {
                u32 *pPrev = (u32*)mpImageCopy;
                u32 *pCurr = (u32*)rawImage;

                u32 offset = (mTextureHeight - imageHeight);
                
                // Put at top
                pCurr += (mTextureWidth * offset);
                pPrev += (mTextureWidth * offset);

                for(u32 y = 0; y < imageHeight; y++)
                {
                    u32 *row = pCurr;
                    u32 *pre = pPrev;
                    
                    for (u32 x = 0; x < imageWidth; x++)
                {
                        u32 curr = *row++;
                        
                        if (curr) { *pre = curr; }
                    
                        pre++;
                    }
                    
                    pPrev += mTextureWidth;
                    pCurr += mTextureWidth;
                }
            }

            // Load texture
            prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));

            // Release old texture
            if (mpTetxure)
            {
                pRM->Unload(mpTetxure);
                mpTetxure = NULL;
            }

            mpTetxure = pRM->LoadFromRaw<prTexture>("Texture", mpImageCopy, mTextureSize, mTextureWidth, mTextureHeight);
            PRASSERT(mpTetxure);

            // Create the working sprite
            prSpriteManager *pSM = static_cast<prSpriteManager *>(prCoreGetComponent(PRSYSTEM_SPRITEMANAGER));
            if (pSM)
            {
                if (mpSprite)
                {
                    // Delete old sprite
                    pSM->ToolRelease(mpSprite);
                    mpSprite = NULL;
                }

                // Set working
                mpSprite = pSM->ToolCreate(mpTetxure, imageWidth, imageHeight);
                mpSprite->SetFrame(0);
            }

            PRSAFE_DELETE_ARRAY(rawImage);
        }
    }

    return result;
}


/// ---------------------------------------------------------------------------
/// Slower platforms can decode and animate the gif over several frames
/// This function acquires the next frame from the gif
/// ---------------------------------------------------------------------------
void prGifDecoder::PartDecode1(u32 frame)
{
    PRASSERT(mpImageCopy);
    PRASSERT(pRawImage == NULL);

    if (mpImage && mFileSize > 0)
    {
	    FIBITMAP *dib = FreeImage_LockPage(mMultiBmp, frame);
        if (dib)
        {
            // Get image pitch
            pitch       = FreeImage_GetPitch(dib);
            imageBPP    = FreeImage_GetBPP(dib);
            imageWidth  = FreeImage_GetWidth(dib);
            imageHeight = FreeImage_GetHeight(dib);

            // Calculate raw data size and create buffer
            pRawImage = new u8[mTextureSize];

            // Copy data
            FIBITMAP *bmp = FreeImage_ConvertTo32Bits(dib);
            if (bmp)
            {
                FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);

                switch(type)
                {
                // Convert bitmap
                case FIT_BITMAP:
                    // 8 bit?
                    if (imageBPP == 8)
                    {
                        BYTE *raw  = (BYTE*)malloc(imageHeight * (pitch * 4));          // pitch * 4 == 8 to 32 bits
                        BYTE *bits = raw;
                        if (raw)
                        {
                            FreeImage_ConvertToRawBits(bits, bmp, pitch * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
                            BYTE *image    = pRawImage;

                            u32 offset = (mTextureHeight - imageHeight);

                            // Put at top
                            image += ((mTextureWidth << 2) * offset);

                            for(u32 y = 0; y < imageHeight; y++)
                            {
                                // Set row start
                                u32 *pixel = (u32*)bits;
                                u32 *line  = (u32*)image;

                                for (u32 x = 0; x < (imageWidth >> 1); x++)
                                {
                                    u32 colour = *pixel++;
                                    
                                    colour = ((colour & FI_RGBA_RED_MASK)   >> FI_RGBA_RED_SHIFT)          |
                                            (((colour & FI_RGBA_GREEN_MASK) >> FI_RGBA_GREEN_SHIFT) <<  8) |
                                            (((colour & FI_RGBA_BLUE_MASK)  >> FI_RGBA_BLUE_SHIFT)  << 16) |
                                            (((colour & FI_RGBA_ALPHA_MASK) >> FI_RGBA_ALPHA_SHIFT) << 24);

                                    *line++ = colour;

                                    // Again, less loops
                                    colour = *pixel++;
                                    
                                    colour = ((colour & FI_RGBA_RED_MASK)   >> FI_RGBA_RED_SHIFT)          |
                                            (((colour & FI_RGBA_GREEN_MASK) >> FI_RGBA_GREEN_SHIFT) <<  8) |
                                            (((colour & FI_RGBA_BLUE_MASK)  >> FI_RGBA_BLUE_SHIFT)  << 16) |
                                            (((colour & FI_RGBA_ALPHA_MASK) >> FI_RGBA_ALPHA_SHIFT) << 24);
                                    
                                    *line++ = colour;
                                }

                                // Next line
                                bits += (pitch << 2);

                                // Next row
                                image += (mTextureWidth << 2);
                            }

                            free(raw);
                        }
                    }
                    else
                    {
                        PRPANIC("Only 8 bit gifs supported at the moment");
                    }
                    break;

                    default:
                        break;
                    }

                FreeImage_Unload(bmp);
            }

            FreeImage_UnlockPage(mMultiBmp, dib, FALSE);
        }
    }
}


/// ---------------------------------------------------------------------------
/// Slower platforms can decode and animate the gif over several frames
/// This function merges the acquire data with the previous image to
/// create the next animation frame
/// ---------------------------------------------------------------------------
void prGifDecoder::PartDecode2()
{
    // Merge?
    {
        u32 *pPrev = (u32*)mpImageCopy;
        u32 *pCurr = (u32*)pRawImage;

        u32 offset = (mTextureHeight - imageHeight);
        
        PRUNUSED(offset); // Cppcheck says this is unused. Cppcheck is wrong
                
        // Put at top
        pCurr += (mTextureWidth * offset);
        pPrev += (mTextureWidth * offset);

        for(u32 y = 0; y < imageHeight; y++)
        {
            u32 *row = pCurr;
            u32 *pre = pPrev;
                    
            for (u32 x = 0; x < imageWidth; x++)
            {
                u32 curr = *row++;
                        
                if (curr) { *pre = curr; }
                    
                pre++;
            }
                    
            pPrev += mTextureWidth;
            pCurr += mTextureWidth;
        }
    }
}


/// ---------------------------------------------------------------------------
/// Slower platforms can decode and animate the gif over several frames
/// This function creates and uploads the texture
/// ---------------------------------------------------------------------------
void prGifDecoder::PartDecode3()
{
    // Load texture
    prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));

    // Release old texture
    if (mpTetxure)
    {
        pRM->Unload(mpTetxure);
        mpTetxure = NULL;
    }

    mpTetxure = pRM->LoadFromRaw<prTexture>("Texture", mpImageCopy, mTextureSize, mTextureWidth, mTextureHeight);
    PRASSERT(mpTetxure);

    // Create the working sprite
    prSpriteManager *pSM = static_cast<prSpriteManager *>(prCoreGetComponent(PRSYSTEM_SPRITEMANAGER));
    if (pSM)
    {
        if (mpSprite)
        {
            // Delete old sprite
            pSM->ToolRelease(mpSprite);
            mpSprite = NULL;
        }

        // Set working
        mpSprite = pSM->ToolCreate(mpTetxure, imageWidth, imageHeight);
        mpSprite->SetFrame(0);
    }

    PRSAFE_DELETE_ARRAY(pRawImage);
}


/// ---------------------------------------------------------------------------
/// Draws the currently decoded frame
/// ---------------------------------------------------------------------------
void prGifDecoder::Draw(f32 x, f32 y, f32 scale)
{
    if (mpSprite)
    {
        mpSprite->SetScale(scale);
        mpSprite->pos.x = x;
        mpSprite->pos.y = y;
        mpSprite->BatchDraw();
    }
}
