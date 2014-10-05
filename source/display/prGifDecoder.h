// File: prGifDecoder.h
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


#ifndef __PRGIFDECODER_H
#define __PRGIFDECODER_H


#include "../core/prTypes.h"
#include "../freeImage/FreeImage.h"


// Defines
#define GIFDECODER_NOFRAME  0xFFFFFFFF


// Forward declarations
class prSprite;
class prTexture;


// Class: prGifDecoder
//      Decodes and draws a gif animation at runtime.
//      Designed for tutorials, adverts, etc
//
// Notes:
//      Currently only decodes on 8 bit gifs
class prGifDecoder
{
public:
    // Method: prGifDecoder
    //      Ctor
    prGifDecoder(const char *filename);

    // Method: ~prGifDecoder
    //      Dtor
    ~prGifDecoder();

    // Method: DecodeFrame
    //      Decodes the current frame into an image which can be drawn
    //
    // Parameters:
    //      frame - The frame to decode
    bool DecodeFrame(u32 frame);

    // Method: PartDecode1
    //      Slower platforms can decode and animate the gif over several frames
    //
    // Notes:
    //      This function acquires the next frame from the gif
    void PartDecode1(u32 frame);

    // Method: PartDecode2
    //      Slower platforms can decode and animate the gif over several frames
    //
    // Notes:
    //      This function merges the acquire data with the previous image to
    //      create the next animation frame
    void PartDecode2();

    // Method: PartDecode3
    //      Slower platforms can decode and animate the gif over several frames
    //
    // Notes:
    //      This function creates and uploads the texture
    void PartDecode3();

    // Method: Draw
    //      Draws the currently decoded frame
    //
    // Parameters:
    //      x       - X coordinate
    //      y       - Y coordinate
    //      scale   - Image scale
    //
    // Notes:
    //      Does nothing if <DecodeFrame> has not been called
    //
    // See Also:
    //      <DecodeFrame>
    void Draw(f32 x, f32 y, f32 scale);

    // Method: GetFrameCount
    //      Returns the number of frames in the animation
    u32 GetFrameCount() const { return mFrameCount; }

    // Method: GetFrameWidth
    //      Returns the animations frame width
    u32 GetFrameWidth() const { return mFrameWidth; }

    // Method: GetFrameHeight
    //      Returns the animations frame height
    u32 GetFrameHeight() const { return mFrameHeight; }


private:
    // Stops passing by value and assignment.
    prGifDecoder(const prGifDecoder&);
    const prGifDecoder& operator = (const prGifDecoder&);

    
private:
    FREE_IMAGE_FORMAT   mFif;
    FIBITMAP           *mDib;
    FIMULTIBITMAP      *mMultiBmp;
    u8                 *mpImage;
    u8                 *mpImageCopy;
    prSprite           *mpSprite;
    prTexture          *mpTetxure;
    u32                 mFileSize;

    u32                 mFrameCount;
    u32                 mFrameWidth;
    u32                 mFrameHeight;
    u32                 mFrameCurrent;

    u32                 mTextureWidth;
    u32                 mTextureHeight;
    u32                 mTextureSize;

    u32                 pitch;
    u32                 imageBPP;
    u32                 imageWidth;
    u32                 imageHeight;
    u8                 *pRawImage;
};


#endif//__PRGIFDECODER_H
