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
 *
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
            u32 pitch       = FreeImage_GetPitch(dib);
            u32 imageBPP    = FreeImage_GetBPP(dib);
            u32 imageWidth  = FreeImage_GetWidth(dib);
            u32 imageHeight = FreeImage_GetHeight(dib);

            // Calculate raw data size and create buffer
            u8 *rawImage     = new u8[mTextureSize];
            memset(rawImage, 0, mTextureSize);

            // Create the copy image for merging frames
            bool copyBase = false;
            if (mpImageCopy == NULL)
            {
                mpImageCopy = new u8[mTextureSize];
                memset(mpImageCopy, 0, mTextureSize);
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
                                    line[0] = pixel[FI_RGBA_RED];
                                    line[1] = pixel[FI_RGBA_GREEN];
                                    line[2] = pixel[FI_RGBA_BLUE];
                                    line[3] = pixel[FI_RGBA_ALPHA];

                                    if (copyBase)
                                    {
                                        lineCopy[0] = line[0];
                                        lineCopy[1] = line[1];
                                        lineCopy[2] = line[2];
                                        lineCopy[3] = line[3];
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

                s32 count = (mTextureSize / 4);
                while(count > 0)
                {
                    u32 curr = *pCurr++;
                    
                    if (curr) { *pPrev = curr; }
                    
                    pPrev++;
                    count--;
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

            PRSAFE_DELETE(rawImage);
        }
    }

    return result;
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
