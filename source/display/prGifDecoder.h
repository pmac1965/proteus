// File: prCamera.h
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


#define GIFDECODER_NOFRAME  0xFFFFFFFF


// Forward declarations
class prSprite;
class prTexture;


// Class: prGifDecoder
//      Decodes and draws a gif animation at runtime.
//      Designed for tutorials, adverts, etc
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

    FREE_IMAGE_FORMAT   mFif;
    FIBITMAP           *mDib;
    FIMULTIBITMAP      *mMultiBmp;
    u8                 *mpImage;
    prSprite           *mpSprite;
    prTexture          *mpTetxure;
    u32                 mImageSize;

    u32                 mFrameCount;
    u32                 mFrameWidth;
    u32                 mFrameHeight;
    u32                 mFrameCurrent;
};


#endif//__PRGIFDECODER_H
