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
    mpSprite        = NULL;
    mpTetxure       = NULL;
    mImageSize      = 0;
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
            mImageSize = pFile->Size();
            if (mImageSize > 0)
            {
                mpImage = new u8[mImageSize];
                if (mpImage)
                {
                    pFile->Read(mpImage, mImageSize);
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

    if (mpImage && mImageSize > 0)
    {
        // Attach the binary data to a memory stream
        FIMEMORY *hmem = FreeImage_OpenMemory(mpImage, mImageSize);

        // Get the image type. So we know its a gif
		mFif = FreeImage_GetFileTypeFromMemory(hmem, 0);
        if (mFif == FIF_GIF)
        {
	        // Open the multipage bitmap stream as read-only
	        mMultiBmp = FreeImage_LoadMultiBitmapFromMemory(mFif, hmem, 0);//GIF_LOAD256);
            if (mMultiBmp)
            {
                mFrameCount = FreeImage_GetPageCount(mMultiBmp);
                //prTrace("Loaded %s\n", filename);
                //prTrace("Frames %i\n", mFrameCount);

		        // Use the first page to get the size
			    FIBITMAP *dib = FreeImage_LockPage(mMultiBmp, 0);
			    if(dib)
                {
                    mFrameWidth  = FreeImage_GetWidth(dib);
                    mFrameHeight = FreeImage_GetHeight(dib);
                    //prTrace("w %i\n", mFrameWidth);
                    //prTrace("h %i\n", mFrameHeight);
				    FreeImage_UnlockPage(mMultiBmp, dib, FALSE);
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
    }//*/

    /*if (mMultiBmp)
    {
        FreeImage_CloseMultiBitmap(mMultiBmp, 0);
        mMultiBmp = NULL;
    }//*/
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

    //FreeImage_DeInitialise();

    PRSAFE_DELETE(mpImage);
}


/// ---------------------------------------------------------------------------
/// Decodes the current frame into an image which can be drawn
/// ---------------------------------------------------------------------------
bool prGifDecoder::DecodeFrame(u32 frame)
{
    bool result = false;

    if (mpImage && mImageSize > 0)
    {
        // Attach the binary data to a memory stream
        /*FIMEMORY *hmem = FreeImage_OpenMemory(mpImage, mImageSize);

	    // Open the multipage bitmap stream as read-only
	    mMultiBmp = FreeImage_LoadMultiBitmapFromMemory(mFif, hmem, 0);//*/

	    FIBITMAP *dib = FreeImage_LockPage(mMultiBmp, frame);
        if (dib)
        {
            // Get image pitch
            u32 pitch       = FreeImage_GetPitch(dib);
            u32 imageBPP    = FreeImage_GetBPP(dib);
            u32 imageWidth  = FreeImage_GetWidth(dib);
            u32 imageHeight = FreeImage_GetHeight(dib);

            // Calculate raw data size and create buffer
            u32 rawImageSize = ((imageWidth * imageHeight) * 4);
            u8 *rawImage     = new u8[rawImageSize];
            memset(rawImage, 0, rawImageSize);

            // Copy data
            FIBITMAP *bmp = FreeImage_ConvertTo32Bits(dib);
            if (bmp)
            {
                FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);
                switch(type)
                {
                // Convert bitmap
                case FIT_BITMAP:
                    // 32 bit bitmap?
                    if (imageBPP == 32)
                    {
                        BYTE *bits  = FreeImage_GetBits(bmp);
                        BYTE *image = rawImage;
                        for(u32 y = 0; y < imageHeight; y++)
                        {
                            // Set row start
                            BYTE *pixel = (BYTE*)bits;
                            BYTE *line  = image;
                            for (u32 x = 0; x < imageWidth; x++)
                            {
                                line[0] = pixel[FI_RGBA_RED];
                                line[1] = pixel[FI_RGBA_GREEN];
                                line[2] = pixel[FI_RGBA_BLUE];
                                line[3] = pixel[FI_RGBA_ALPHA];
                                pixel += 4;
                                line  += 4;
                            }

                            // Next line
                            bits += pitch;

                            // Next row
                            image += (imageWidth * 4);
                        }
                    }                    
                    // 24 bit?
                    else if (imageBPP == 24)
                    {
                        BYTE *raw  = (BYTE*)malloc(imageHeight * (imageWidth * 4));          // imageWidth * 4 == 24 to 32 bits
                        BYTE *bits = raw;
                        if (raw)
                        {
                            FreeImage_ConvertToRawBits(bits, bmp, imageWidth * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
                            BYTE *image = rawImage;
                            for(u32 y = 0; y < imageHeight; y++)
                            {
                                // Set row start
                                BYTE *pixel = (BYTE*)bits;
                                BYTE *line  = image;

                                for (u32 x = 0; x < imageWidth; x++)
                                {
                                    line[0] = pixel[FI_RGBA_RED];
                                    line[1] = pixel[FI_RGBA_GREEN];
                                    line[2] = pixel[FI_RGBA_BLUE];
                                    line[3] = pixel[FI_RGBA_ALPHA];
                                    pixel += 4;
                                    line  += 4;
                                }

                                // Next line
                                bits += (imageWidth * 4);

                                // Next row
                                image += (imageWidth * 4);
                            }

                            free(raw);
                        }
                    }

                    // 8 bit?
                    else if (imageBPP == 8)
                    {
                        BYTE *raw  = (BYTE*)malloc(imageHeight * (pitch * 4));          // pitch * 4 == 8 to 32 bits
                        BYTE *bits = raw;
                        if (raw)
                        {
                            FreeImage_ConvertToRawBits(bits, bmp, pitch * 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE);
                            BYTE *image = rawImage;
                            for(u32 y = 0; y < imageHeight; y++)
                            {
                                // Set row start
                                BYTE *pixel = (BYTE*)bits;
                                BYTE *line  = image;

                                for (u32 x = 0; x < imageWidth; x++)
                                {
                                    line[0] = pixel[FI_RGBA_RED];
                                    line[1] = pixel[FI_RGBA_GREEN];
                                    line[2] = pixel[FI_RGBA_BLUE];
                                    line[3] = pixel[FI_RGBA_ALPHA];
                                    pixel += 4;
                                    line  += 4;
                                }

                                // Next line
                                bits += (pitch * 4);

                                // Next row
                                image += (imageWidth * 4);
                            }

                            free(raw);
                        }
                    }
                    break;

                    default:
                        break;
                    }

                FreeImage_Unload(bmp);
            }

            FreeImage_UnlockPage(mMultiBmp, dib, FALSE);
            result = true;


            // Load texture
            prResourceManager *pRM = static_cast<prResourceManager *>(prCoreGetComponent(PRSYSTEM_RESOURCEMANAGER));

            // Release old texture
            if (mpTetxure)
            {
                pRM->Unload(mpTetxure);
                mpTetxure = NULL;
            }

            mpTetxure = pRM->LoadFromRaw<prTexture>("Texture", rawImage, rawImageSize, imageWidth, imageHeight);
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
            }

            PRSAFE_DELETE(rawImage);
        }


        /*if (mMultiBmp)
        {
            FreeImage_CloseMultiBitmap(mMultiBmp, 0);
            mMultiBmp = NULL;
        }//*/
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
